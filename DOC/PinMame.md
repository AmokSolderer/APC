# PinMame Sound

This page is to guide you through the process of enabling the APC to generate the original sound and music for your pinball machine. 
If you're lucky then your machine is listed below which means that someone else has done the work for you. If not then I'm afraid that you have to read the rest of this page also.

# Available Sounds

At the moment the list of available sound packages is still quite short, but we hope that some of you will do this for their machines also. Of course it would be great if you'd share your sound package with the rest of us. Up to now we haven't found some server space to store the files on, so send me a PM via Pinside or Flippertreff (see the feedback section on the main page) if you want to have one of the sound packages listed below.

|System|Game| Sound File URL|Comments|
|--|--|--|--|
|4|Flash| URL available on request| |
|6|Firepower| Ask Matiou in the Pinside forum | PinMameExceptions also done by Matiou |
|7|Barracora| URL available on request| |
|7|Black Knight| URL available on request| |
|7|Jungle Lord| URL available on request| Works great. Sound samples are also good. |
|7|Pharaoh| Ask Grangeomatic in the Pinside forum| PinMameExceptions also done by Grangeomatic |
|9|Comet|URL available on request| Works great. Sound samples are also good. |
|11a|Pinbot| URL available on request| Some PinMame sounds are quite bad, e.g. visor opening and closing. May be someone can sample them from the orignal HW.|
|11a|F-14 Tomcat| Ask Snux in the Pinside forum | PinMameExceptions also done by Snux |
|11c|Rollergames| URL available on request| There're problem with this game. For some reason PinMame restarts random music tracks all 5 seconds. The issue doesn't seem to affect the Windows version of PinMame. Please contact me if you have any idea what the root cause might be. |

# What needs to be done

The fact that you're reading this probably means that your machine is not yet supported. So let's first summarize what you have to do to change that:

* Get the sounds. This can be done from the internet, PinMame or by self recording.
* Adjust the amplitude, DC offset and sampling rate if necessary. You need a mono WAV file with 44.1KHz sampling rate
* Ensure the filenames are correct to help the APC find the correct audio file
* Convert them to the APC sound file format
* Put them on the SD card of the APC
* Do the necessary PinMameExceptions to incorporate the special functions of the respective audio board

Click on the generation of your game to get to the corresponding how-to page:

[System 3 - 7 sound how-to](https://github.com/AmokSolderer/APC/blob/V01.00/DOC/PinMameSound_3_7.md)

System 9 sound how-to

[System 11 sound how-to](https://github.com/AmokSolderer/APC/blob/V01.00/DOC/PinMameSound_11.md)
