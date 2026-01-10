# If things don't work

## APC doesn't boot

On some System 9 and 11 machines the APC doesn't boot sometimes. Usually it is enough to powercycle your machine or to press the reset button of the Arduino DUE to make it work.  
If it happens frequently then you could swap C14 from a 10µF to a 22µF capacitor. Boards fabricated after [December 2023](https://github.com/AmokSolderer/APC/blob/master/DOC/Changes.md#hw-1) should already have this fix.

## The debug mode

There's a main debug mode available which can be activated in the [System Settings](https://github.com/AmokSolderer/APC/blob/V01.04/DOC/Settings.md#system-settings) or by changing

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
