# PinMame Sound

This page is to guide you through the process of enabling the APC to generate the original sound and music for your pinball machine. 
If you're lucky then your machine is listed below which means that someone else has done the work for you. If not then I'm afraid that you have to read the rest of this page also.

# Available Sounds

At the moment the list of available sound packages is still quite short, but we hope that some of you will do this for their machines also. Of course it would be great if you'd share your sound package with the rest of us.  
If the sound files for your machine are marked as 'available', the corresponding ZIP file can be downloaded [here](https://drive.google.com/drive/folders/1NDCfrecSSClfyiHb7cRy5wmoFfd4UPNM?usp=sharing). Most of these ZIP files also contain the settings files to make the corresponding machine work.__
There's also a MUSIC.snd file available which is played during the music test of the Basecode and while the volume setting is selected.

|System|Game| Sound Files |Comments|
|--|--|--|--|
|3|Disco Fever| available | Only files for one audio board mode available. Ball saver and alternate music mode implemented for this machine |
|4|Flash| available | Ball saver and alternate music mode implemented for this machine |
|6|Tri Zone| available | Ball saver and alternate music mode implemented for this machine |
|6|Time Warp| available | Ball saver and alternate music mode implemented for this machine |
|6|Gorgar| available | Ball saver and alternate music mode implemented for this machine |
|6|Firepower| available thanks to Matiou | PinMameExceptions also done by Matiou / Ball saver and alternate music mode implemented for this machine / [Throttle value](https://github.com/AmokSolderer/APC/blob/master/DOC/RunGame.md#emulation-speed) needs to be changed to 110|
|6|Alien Poker| available | Ball saver and alternate music mode implemented for this machine |
|7|Barracora| available | Ball saver and alternate music mode implemented for this machine |
|7|Black Knight| available | Including the multiballsound for the APC Black Knight game |
|7|Jungle Lord| available | Ball saver and alternate music mode implemented for this machine |
|7|Pharaoh| available | PinMameExceptions done by Grangeomatic / Ball saver and alternate music mode implemented for this machine|
|9|Comet| available | Ball saver and alternate music mode implemented for this machine |
|11|High Speed| available thanks to jejepinside |Ball saver implemented for this machine / PinMameExceptions still need to be implemented|
|11a|Pinbot| available | Ball saver implemented for this machine |
|11a|F-14 Tomcat| available thanks to Snux | PinMameExceptions also done by Snux / Ball saver implemented for this machine / [Throttle value](https://github.com/AmokSolderer/APC/blob/master/DOC/RunGame.md#emulation-speed) needs to be changed to 110|
|11b|Space Station| available | Ball saver implemented for this machine |
|11b|Whirlwind| available | Ball saver implemented for this machine / Sounds still need some finetuning / The Sound Overlay Solenoid Board needs to be connected to the HW_ext interface (P11/Sys11_J21)|
|11c|Rollergames| available | There're problem with this game. For some reason PinMame restarts random music tracks all 5 seconds. This probably need to be fixed in the PinMameExceptions |

# What needs to be done

The fact that you're reading this probably means that your machine is not yet supported. So let's first summarize what you have to do to change that:

* Get the sounds. This can be done from the internet, PinMame or by self recording.
* Adjust the amplitude, DC offset and sampling rate if necessary. You need a mono WAV file with 44.1KHz sampling rate
* Ensure the filenames are correct to help the APC find the correct audio file
* Convert them to the APC sound file format
* Put them on the SD card of the APC
* Do the necessary PinMameExceptions to incorporate the special functions of the respective audio board

Click on the generation of your game to get to the corresponding how-to page:

[System 3 - 7 sound how-to](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameSound_3_7.md)

[System 9 sound how-to](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameSound_9.md)

[System 11 sound how-to](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameSound_11.md)
