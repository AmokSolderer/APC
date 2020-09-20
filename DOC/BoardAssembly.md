The assembly of this board should be done in steps, as this makes it much easier to find faulty soldering points. I have prepared pictures of the location of the components of each step on the top and bottom side of the board as well as in the schematics main page. When a sub-block is marked for assembly in the main page please refer to the [APC schematics](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/APC_schematics.pdf) to see the schematic of this sub-block.

For the assembly it makes quite a difference whether you have a professionally manufactures board with conducting vias or a self etched board. The latter requires to solder vias and component pins on every side they are connected. This is not only cumbersome (for example when connectors pins have connections on the top layer) but also prone to mistakes when you miss a top level connection because it is hidden by an IC.

In some cases a self etched board even requires the assembly of certain components even though they are not required for the current step yet, but they are needed as vias. I have pointed out these situations in my description of the corresponding assembly steps.

# Preparation

If you have a self etched board you have to drill a lot of holes first. I have generated a [drilling map](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Layout/APC-PTH-drl_map.pdf) to help you choose the right drill for every hole (of course the 0.81mm holes can be drilled with 0.8mm). After that you should prepare all vias first. There are 18 of them which can easily be found as they're the only 0.6mm drilling points in the drilling map. However one of them can be skipped as it just connects a ground plate on the bottom side with no connection to any pins.

Those who have a professionally manufactured board can skip the preparation and proceed to step 1.

# Step 1

Step 1 covers the assembly of the socket for the Arduino Due, as well as the SD card connector, the audio block and the power connector:

[Top level schematic of step 1](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC_schem_step1.pdf)

[Bottom layer components of step 1](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-bot_step1.pdf)

[Top layer components of step 1](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-top_step1.pdf)

Take care of the polarity of capacitor C14. The positive terminal of this capacitor is marked with a wide line - it must be soldered on the board pad that's been marked with a small circle.

The only reason I included the audio block into step 1 is that it includes a lot of small SMD components on the top layer which are easier to solder as long as the power connector is not assembled yet.

There is a special test software to check the board after step 1. For this you need to use the [AudioSave.pl](https://github.com/AmokSolderer/APC/wiki/Useful-software-tools) tool to prepare two audio files, one named MUSIC.BIN which should be several minutes long and one just a few seconds long named SOUND.BIN. Store them on the SD Card and 
upload [APC_SoundCheck](https://github.com/AmokSolderer/APC/tree/master/DOC/Software/APC_SoundCheck) into the DUE.
For this program you should use the manual volume control (the old turning knob) on connector 10J4 for System 7 which is 1J16 for System11.

After the system has started you should hear Music.bin playing and on top of that Sound.bin repeated every 10 seconds. In case of problems you can check the serial interface for messages, but you have to set it to 115200 baud first.

# Step 2

Step 2 contains the drivers for the data bus, for the blanking signal and all display related stuff:

[Top level schematic of step 2](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC_schem_step2.pdf)

[Bottom layer components of step 2](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-bot_step2.pdf)

[Top layer components of step 2](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-top_step2.pdf)

This step is slightly different for people with self etched boards (without conducting vias). This is because U1 (marked with magenta color) is not needed in this step yet, but some of its solder pads are used as vias for the data bus driver U2. That means, if you have a board without conducting vias you should decide now whether you want to assemble the hardware extensions connector (step 6). If you do want to have it, you should already assemble U1, otherwise you can manually build vias at those pads of U1 having wires leading to U2.

There's one additional topic to know about using different kinds of displays. For some reason Williams changed the polarity of the display segment signals for the latest System11 displays (those having two rows with 16 alphanumeric characters each). This means that with these boards the segment signal has to be on _high_ level for the corresponding segment _not_ to be lit. As the APC disables the outputs of the latches generating the display signals during the update cycle, the corresponding signals are pulled to the _off_ level by the resistor networks RR2 - RR5. I have therefore put the jumper JP1 at the supply pins of these resistor networks. For displays with 2x 16 alphanumeric characters pin2 of this jumper must be shorted to +5V, for all other kinds of displays it must be shorted to GND.

Remember to check for supply shorts before putting the board into your machine.

If you have used APC_SoundCheck after step 1, you have to upload the normal APC software now to see any display messages. However, the messages may still be not 100% OK in case the currently selected game doesn't match to your kind of display, but it should be sufficient to check whether it works or not. 

# Step 3

This step is about the switches:

[Top level schematic of step 3](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC_schem_step3.pdf)

[Bottom layer components of step 3](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-bot_step3.pdf)

[Top layer components of step 3](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-top_step3.pdf)

The later System11 machines do not use special solenoids any more, which means that connector 1J18 is not used.

For testing press the Advance button while Up/Down is in down position to enter the test mode. Now press Up/Down to bring it up again, proceed to the 'Switch Edges' test by pressing Advance and enter the test with the game start button. For every switch being closed its number will be shown in the display. Be sure to try switches from all rows and columns. 

# Step 4

Lamps are next:

[Top level schematic of step 4](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC_schem_step4.pdf)

[Bottom layer components of step 4](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-bot_step4.pdf)

[Top layer components of step 4](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-top_step4.pdf)

Remember to check for supply shorts before putting the board into your machine. I recommend not to connect all lamp related connectors at first, except of _Lamp GND_. Then supply _Lamp B+_ with 5V instead of 18 and use an oscilloscope or a logic probe together with a 100 ohms pull-down resistor to check all pins of the _Lamp Strobe_ connector for the strobe pulses. Do not proceed if one pin is stuck on as this can damage your light bulbs.

When this test has been passed you can connect all connectors and use the lamp test to check whether all lamps are working.

# Step 5

Solenoid drivers:

[Top level schematic of step 5](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC_schem_step5.pdf)

[Bottom layer components of step 5](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-bot_step5.pdf)

[Top layer components of step 5](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-top_step5.pdf)

Remember to check for supply shorts before putting the board into your machine. Again I recommend not to connect any solenoid related connector except of _Solenoid GND_ at first. Use your logic probe instead to check all solenoid driver pins first. They should all be high-Z (no lamp lit on the logic probe). Do not proceed if one of the pins is stuck low (driver to GND is permanently on) as this will fry your coil. 

# Step 6

This step is optional. You only need to do it when you have a Whirlwind (or another machine using the Sound Overlay Board C-13287) or you want to use the hardware extension interface:

[Top level schematic of step 6](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC_schem_step6.pdf)

[Bottom layer components of step 6](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-bot_step6.pdf)

[Top layer components of step 6](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC-top_step6.pdf)

***