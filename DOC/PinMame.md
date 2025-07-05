# PinMame Sound

This page is to guide you through the process of enabling the APC to generate the original sound and music for your pinball machine. 
If you're lucky then your machine is listed below which means that someone else has done the work for you. If not then I'm afraid that you have to read the rest of this page also.

# Available Sounds

At the moment the list of available sound packages is still quite short, but we hope that some of you will do this for their machines also. Of course it would be great if you'd share your sound package with the rest of us.  
You can download the audio files from the links in the table below. Thanks to hotsoupkilz who is hosting them.

|System|Game| Sound Files |Comments|
|--|--|--|--|
|3|Disco Fever| in work | PinMameExceptions might still need some finetuning |
|4|Flash| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Flash-sounds.zip)| Ball saver and alternate music mode implemented for this machine |
|6|Time Warp| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Time-Warp-sounds.zip) | Ball saver and alternate music mode implemented for this machine |
|6|Gorgar| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Gorgar-sounds.zip) | Ball saver and alternate music mode implemented for this machine |
|6|Firepower| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Firepower-sounds.zip) thanks to Matiou | PinMameExceptions also done by Matiou / Ball saver and alternate music mode implemented for this machine / [Throttle value](https://github.com/AmokSolderer/APC/blob/master/DOC/RunGame.md#emulation-speed) needs to be changed to 110|
|6|Alien Poker| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Alien-Poker-sounds.zip) | Ball saver and alternate music mode implemented for this machine |
|7|Barracora| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Barracora-sounds.zip) | Ball saver and alternate music mode implemented for this machine |
|7|Black Knight| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Black-Knight-APC-sounds.zip)| Including the multiballsound for the APC Black Knight game |
|7|Jungle Lord| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Jungle-Lord-sounds.zip)| Ball saver and alternate music mode implemented for this machine |
|7|Pharaoh| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Pharaoh-sounds.zip)| PinMameExceptions done by Grangeomatic / Ball saver and alternate music mode implemented for this machine|
|9|Comet|[available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Comet-sounds.zip)| Ball saver and alternate music mode implemented for this machine |
|11a|Pinbot| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Pinbot-sounds.zip)| Ball saver implemented for this machine |
|11a|F-14 Tomcat| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/F14-Tomcat-sounds.zip) thanks to Snux | PinMameExceptions also done by Snux / Ball saver implemented for this machine|
|11b|Space Station| [available](https://storage.googleapis.com/hotsoup-public/APC/sound-packages/Space-Station-sounds.zip)| Ball saver implemented for this machine |
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
