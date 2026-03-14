# If things don't work

## APC doesn't boot

On some System 9 and 11 machines the APC doesn't boot sometimes. Usually it is enough to powercycle your machine or to press the reset button of the Arduino DUE to make it work.  
If it happens frequently then you could swap C14 from a 10µF to a 22µF capacitor. Boards fabricated after [December 2023](https://github.com/AmokSolderer/APC/blob/master/DOC/Changes.md#hw-1) should already have this fix.

## The debug mode

There's a main debug mode available which can be activated in the [System Settings](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md#system-settings) or by changing

    #define DebugState 0    // set this value to 2 and remove SD card for USB debug log

at the top of the APC.ino file. In the latter case you have to recompile the SW afterwards and upload it again to the Arduino. If greater that zero, this definition will overrule the selected value in the System Settings.

A setting different from zero will activate a serial interface at the USB (programming) port of the DUE. You can use the serial monitor of your Arduino IDE or any serial terminal program to display the messages.  
Note that you cannot control your APC via USB (e.g. with MPF) while the USB debug mode is enabled. It might also slow down your system, so only turn it on for as long as it's needed. 

* A setting of 1 will display critical error messages and halt the system afterwards, but you're free to use this interface from your own code with e.g. the Serial.print command

* Setting this to 2 is useful if you have pure numerical displays and you want to see the settings menus or error messages in full text. After startup the APC will then list all settings in full text. If you enter the settings, then the APC will show the name and value of the current setting, just like you'd see them on an alphanumerical display. Also file not found errors for audio files will show up here.

## Sound stuttering

If your sound has unregular lags and stutters sometimes then it's most probably an issue with your SD card.
 
The first thing to try is to format your card with a special SD-card formatting tool like the one from the SD Association (sdcard.org) which can improve the time your SD card needs to open a file. This can be crucial especially for pre System11 machines as their sound sequences usually consist of lot's of single sounds. Each of these sounds requires a new file on the card to be opened and if your card cannot open the files fast enough than the sounds just start to stutter.

If this doesn't work then try a different SD card. I had several SD cards of the same brand, but for some reason one of these cards just produced sound lags on my Pinbot, even though I had it formatted with the formatting tool. After I replaced the card with one of it's siblings everything worked fine.

However, some sound sequences might just be too challenging for your SD cards.  
The solution is to record the whole sequence into one single sound file and to play this with a high priority. This requires only one file to be opened for the whole sequence and the high priority blocks all subsequent sound commands from PinMame as long as the file is being played.

## Damaged Arduino clone

If you're using a chinese clone of the Arduino DUE which just stopped working and draws a huge current instead, then it's most probably a blown buck converter IC.  
You can check this by supplying your DUE on the bench (without the APC board) with 5V. Then check the temperature of the marked IC.

![Buck](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Buck.jpg)

If the IC gets hot then it's damaged. The good thing is that you don't need this IC when using the DUE with the APC, so you can just remove it and your system should run fine again.

## PinMame game doesn't start

If the displays are working, but the game doesn't start, then the LEDs on the APC board can help you to find the problem.  
The meaning of the LEDs is:

|Color|Meaning|
|--|--|
|Yellow|Lisy has been started on the Raspi and is now trying to contact the Arduino DUE|
|Green|Communication between Raspi and DUE has been established|
|Red|Communication has failed|

* After the machine has been switched on, all LEDs are off while the Raspi is booting. If you have alphanumeric displays they should show 'Booting Lisy'. If they show 'USB control instead, read the 'red LED' part below. 
* When the Raspi has finished booting, the yellow LED will be lit to show that it is trying to reach the DUE. If the yellow LED does not light up, then something is wrong with your Raspberry Pi. There is an LED on the Pi which will flicker while the Pi is reading data from the SD card (e.g. while booting). If this LED stays dark, check your SD card and be sure to have Lisy installed correctly. Note that [Lisy Embedded](https://lisy.dev/lisy_embedded.html) will only work on the correct hardware.
* If the green LED is lit, the communication works. If the game still doesn't work correctly, check the [settings for PinMame use](https://github.com/AmokSolderer/APC/blob/master/DOC/RunGame.md#pinmame) like the PinMame game number.
* A red LED shows that the communication has failed. Check that 'Active game' (setting 1  of the System settings) is set to 'Remote Control' and 'Connect Type' (setting 4) is set to 'On board'. This will select the Raspi located on the APC board for communication.  

## Fixing a dead APC

If neither switches, lamps, solenoids nor displays work any more, you might have a damaged data bus on your APC. In this case the green LED will still be lit after startup when you're in 'Remote Control' game mode and have a Raspberry Pi on board, but except of the sound nothing will work. Pre System11 displays will probably show all zeros and BK2K style displays will look like this:

![DeadAPC](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/DeadAPC.jpg)

If you have none of the display types mentioned above, they'll just be dead and you have to use a logic probe or an oscilloscope to check the display strobe signals (Sys11_1J1 and Sys11_1J2) for activity. If those signals are pulsing, you should check the pins (12 to 19) of U2. If they're static (like in the picture) your data bus is dead.  
You should now check the input signals (pins 2 to 9) of U2. However, these are 3.3V signals so be sure that your logic probe can handle those or use an oscilloscope. If the input signals are also static you should check your Arduino DUE, otherwise you can locate the problem as follows:  
The APC uses an 8 bit data bus to control most of the HW. If this bus is dead then this is most probably caused because one of the latches addressed by this bus is damaged and shorting the signals of the bus to 5V or GND.  
Finding the damaged latch can be tricky as they're all connected in parallel, but the most likely candidate is the switch driver latch U23. This latch is providing the signals of the switch row and can therefore be damaged by high voltages being shorted to the switch matrix.  
You can remove the IC quite easily with two soldering irons. If the APC comes to live again with the IC removed, you have found your problem, but be sure to fix the short in your switch matrix before installing a new IC.

