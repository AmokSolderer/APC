# PinMame Sound

This page is to guide you through the process of enabling the APC to generate the original sound and music for your pinball machine. 
If you're lucky then your machine is listed below which means that someone else has done the work for you. If not then I'm afraid that you have to read the rest of this page also.

# Available Sounds

At the moment the list of available sound packages is still quite short, but we hope that some of you will do this for their machines also. Of course it would be great if you'd share your sound package with the rest of us. Up to now we haven't found some server space to store the files on, so send me a PM via Pinside or Flippertreff (see the feedback section on the main page) if you want to have one of the sound packages listed below.

|System|Game| Sound Files |Comments|
|--|--|--|--|
|3|Disco Fever| available on request| PinMameExceptions might still need some finetuning |
|4|Flash| available on request| Ball saver and alternate music mode implemented for this machine |
|6|Time Warp| available on request | Ball saver and alternate music mode implemented for this machine |
|6|Gorgar| available on request | Ball saver and alternate music mode implemented for this machine |
|6|Firepower| available on request thanks to Matiou | PinMameExceptions also done by Matiou / Ball saver and alternate music mode implemented for this machine / [Throttle value](https://github.com/AmokSolderer/APC/blob/V01.03/DOC/RunGame.md#Emulation speed) needs to be changed to 110|
|6|Alien Poker| available on request | Ball saver and alternate music mode implemented for this machine |
|7|Barracora| available on request | Ball saver and alternate music mode implemented for this machine |
|7|Black Knight| available on request| Ball saver and alternate music mode implemented for this machine |
|7|Jungle Lord| available on request| Ball saver and alternate music mode implemented for this machine |
|7|Pharaoh| available on request| PinMameExceptions done by Grangeomatic / Ball saver and alternate music mode implemented for this machine|
|9|Comet|available on request| Ball saver and alternate music mode implemented for this machine |
|11a|Pinbot| available on request| Ball saver implemented for this machine |
|11a|F-14 Tomcat| available on request thanks to Snux | PinMameExceptions also done by Snux / Ball saver implemented for this machine|
|11b|Space Station| available on request| Ball saver implemented for this machine |
|11c|Rollergames| available on request| There're problem with this game. For some reason PinMame restarts random music tracks all 5 seconds. The issue doesn't seem to affect the Windows version of PinMame. Please contact me if you have any idea what the root cause might be. |

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
