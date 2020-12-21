# PinMame Howto

## Getting started

For using Lisy you need an additional Raspberry Pi. At the moment all zero and model 3 type Pi's are supported.  
Download the [Lisy software](https://lisy.dev/software.html) and install it to the SD card of your Raspberry Pi. 
In order to run the code for your game, you need the PinMame game numer of your game which can be found [here](https://github.com/AmokSolderer/APC/blob/master/DOC/lisyminigames.csv).

### APC 3 with Raspberry Pi on board

Plug the Pi into connector J1 on the APC. Then power up your APC, enter the 'System Settings' and select 'Remote Control' as 'Active Game' and 'On Board' as 'Connect Type'.  
Now enter the 'Game Settings' and set 'PinMame Game' to the number of your game. 

As the synchronization of Lisy and the APC is not yet finalized it may now necessary to power-cycle your game.  
During booting the display should show 'Booting Lisy'. After the booting has finished the yellow LED on the APC will be turned on, followed by the green one when the connection to the APC has been established. Now the name of the game should appear in the displays with a countdown after which the game emulation starts.

### Using the Lisy_Mini board

Power up your APC, enter the 'System Settings' and select 'Remote Control' as 'Active Game' and 'USB' as 'Connect Type'. Set the S2 DIP switches on the Lisy_Mini board to your game number. 

Connect the power connector K8 of your Lisy_Mini board to the 5V supply of your pinball machine. (You can also supply Lisy_Mini by the micro USB port of the Pi. In this case you need a 2A capable power supply and you should power up Lisy after the APC and connect the USB cable then.)

Start your pinball with Lisy_Mini and the APC. The displays of your pinball machines should show 'USB control' until the connection between Lisy and the APC has been established. 

## Recording the sounds

If you want to get the original sounds for your machine, you have to install PinMame for Windows as the Unix version has severe sound issues.

After PinMame is running press F4 to enter the 'Sound Command Mode' and enter the prefix and the hexadecimal sound number of the sound you want to record. Then press F5 to start the recording, SPACE to play the sound and F5 again to stop recording. The sound can be found in PinMame's 'wave' folder.

Like all sounds to be played by the APC the WAV file has to be processed with my Audio Data Converter. You can find additional information about this tool in the [Useful Software Tools](https://github.com/AmokSolderer/APC/wiki/Useful-software-tools) Wiki.

In order for the APC to find the right sound, all sounds have to be named with the prefix (only one digit), underscore, the hexadecimal number of the sound and .snd as the extension. For example, the name of sound 0xf1 of prefix 00 would be 0_f1.snd. All files need to be placed in the root folder of APC's SD card.

### Which sounds are required?

When you start from scratch you should play your game with Lisy being in Debug Mode. Set DIP 7 and the sound jumper to make it log only sound related commands.

When you're done playing, press the shutdown switch on your Lisy_Mini board to make it exit the emulation and store the log file to the SD card. The file will be located on the Pi's SD card in the folder lisy/lisy_m/debug.

In lisy_m_debug.txt playing a sound looks like

    [461.372965][0.000064] LISY_W sound_handler: board:0 0x79 (121)
    [461.372985][0.000020] play soundindex 121 on board 0 
    [461.373005][0.000020] USB_write(3 bytes): 0x32 0x01 0x79

In this case sound 0x79 of prefix (board) 0 shall be played which means you have to record the sound command 0079 in PinMame, convert it and write it to the APC's SD card as 0_79.snd.

### Finding out which sounds are still missing

After you have extracted most of the files, there'll be the point when only a few files are still missing and you need to find out their numbers. Of course you could still scan the Lisy log for unknown sound numbers, but in this stage it might be easier to use the 'Audio Debug Mode' of the APC.

You can activate this mode in the game settings. To do this you have to press Advance for more than a second to enter the settings. Use Advance to change from system to game settings and press the start button to enter those. Use Advance to proceed to the 'Debug Mode' setting, then change it to 'AUDIO' by pressing the start button. Go to 'Exit Settings' by pressing Advance and the Start button to confirm.

In Audio Debug mode the lower display(s) are used for audio information. The Player 3 display (or the left part of the lower display for BK2K type displays) shows information for sound prefix 00 and the Player 4 display (right part of lower display for BK2K) does the same for prefix 01. If the requested sound is found on the SD card, it's hex number is shown in the left side of the corresponding display and the sound is played normally. If the sound file is missing it's hex number is shown on the right side of the corresponding display which makes it easy to find missing sound files.