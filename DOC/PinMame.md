# The APC and PinMame

Bringing PinMame and the APC together was only possible with the help of the [Lisy project](https://lisy.dev).  
To set it up you have to get a Raspberry Pi (all Zero and Pi3 variants are supported) and install the [Lisy image](https://lisy.dev/apc.html) on the SD card of your Pi.  
The APC's 'Active Game' setting has to be set to 'Remote Control' (which is the default setting) to make it communicate with the Pi.

If you have an APC 3 board you can just plug the Pi on the J1 connector of the APC directly. As the APC is set to USB communication by default, you have to change the 'Connect Type' setting in the 'System Settings' to 'On board'. Take a look at the [settings page](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md) for a desription of the settings. Connect an SD-card to P8 of the APC for the settings to be stored permanently.  
It might be necessary to power cycle the system as the synchonization between the APC and Lisy is still under development.

If you have an APC 2.x board or you just want a portable Lisy system you can use the [Lisy_Mini](https://lisy.dev/lisy_mini.html) board which controls the APC via USB.

Read the [Controlling Lisy](https://github.com/AmokSolderer/APC/blob/master/DOC/LisyDebug.md) page to learn how to make incremental updates or use the Lisy Debug functions.

We have started our PinMame experiments with my old trusty Pinbot which is running quite well as you can see in the following video:

[Lisy, APC and PinMame](https://youtu.be/cXrh-XPqCKw)

PinMame support is still in it's early stages. As you can see the Pinbot is already working quite well, but it's not yet perfect. The main challenges when adding support for more machines are sound related. This has to do with the way we did the sound implementation which offers a lot more than just playing the original sounds and music, but requires a bit more work to do so.

For most of the game generations you could also install the original audio board. In this case you cannot do any sound related changes of course. For System 3 - 6, the sound board is controlled by some reserved solenoid drivers, so it will work out of the box. System 7 and System 11c need an adapter for connecting the audio board to the HW extension interface of the APC. System 9, 11, 11a and 11b are having some sound related circuitry on the CPU, so their audio boards cannot be used any more.

## The Sounds

Our current setup is not using the Raspberry Pi to generate the PinMame sounds. Instead all sound and music files have to be created once and stored on the SD card of the APC. Of course this requires some work, but we think the benefits are worth it.

First of all, the Pi uses the Unix version of PinMame which has severe sound quality issues while the audio quality of the APC is very good. But there're some more advantages in having all sound files on the SD card. One is that you can use the same files for PinMame, MPF or when programming the APC natively. This may not sound like a big deal, but it gives you the opportunity to run your game in PinMame and introduce certain changes by using the [APC API](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf). A description of this can be found on the [Howto page](https://github.com/AmokSolderer/APC/tree/master/DOC/PinMame_howto.md).

A good example is a local fellow who wants his System 3 'Disco Fever' to play songs like 'Saturday Night Fever' additionally to the normal sounds of the machine. We still have to add PinMame support for System 3, but once we have it adding the music should be an easy task. The commands of the APC API can be used to start the music when the game starts and to stop it when it ends. You could even identify certain trigger points to switch to another song and so on.

To make this possible, it is necessary that PinMame and the APC can play sounds simultaneously which would normally require an additional HW sound mixer which mixes both sound channels together. In order to avoid this we decided to let the APC do the complete sound handling with PinMame just telling him which sound to play when.

The drawback of this is that you have to extract the music files from PinMame. Furthermore we have to understand how the audio boards work and emulate their behaviour. The first task is easy and if we find some server space to store the files somewhere in the internet then this work must only be done once for each game. For System 3 - 9 games I expect this to be easy anyway, as they have a very limited sound performance. You could use the Audio Debug Mode which is explained later to find out which sounds to extract.

At the moment the list of available sound packages is still quite short, but we hope that some of you will do this for their machines also. Of course it would be great if you'd share you sound package with the rest of us. Up to now we haven't found some server space to store the files on, so send me a PM via Pinside or Flippertreff (see the feedback section on the main page) if you want to have one of the sound packages listed below.

|System|Game| Sound File URL|Comments|
|--|--|--|--|
|6|Firepower| Ask Matiou in the Pinside forum | PinMameExceptions also done by Matiou |
|7|Black Knight| URL available on request| |
|7|Jungle Lord| URL available on request| Works great. Sound samples are also good. |
|7|Pharaoh| Ask Grangeomatic in the Pinside forum| PinMameExceptions also done by Grangeomatic |
|11a|Pinbot| URL available on request| Some PinMame sounds are quite bad, e.g. visor opening and closing. May be someone can sample them from the orignal HW.|
|11a|F-14 Tomcat| Ask Snux in the Pinside forum | PinMameExceptions also done by Snux |
|11c|Rollergames| URL available on request| There're problem with this game. For some reason PinMame restarts random music tracks all 5 seconds. The issue doesn't seem to affect the Windows version of PinMame. Please contact me if you have any idea what the root cause might be. |

## Audio boards

Understanding the audio boards is the second task at hand. Again I expect this to be easy for games prior to System 11, but it took me roughly a week before I was satified with the sounds of my Pinbot. Of course I started from scratch, so it's going to be easier for the next one doing this, but System 11 features various audio boards and I don't know how much they differ from the Pinbot's. For this reason I'm going to maintain a second list here where all PinMame relevant information about the various systems are listed.  
Alas, the sound commands seem to be unique for every machine which requires a slightly different handling of sound commands depending of which machine is selected. The APC SW features a machine specific exception handling which allows you to implement these special audio commands for your machine. A description of this can be found on the [Howto page](https://github.com/AmokSolderer/APC/tree/master/DOC/PinMame_howto.md).

### System 7

Up to now we know standard commands which seem to be identical for System7 audio boards:

|Command / Hex|Comment|
|--|--|
|2c|Stop Sound|
|7f|Not a real sound command, but used to reset the data bus to the audio board between commands|

These boards are featuring what I call a sound series. This means that if a certain sound number is called multiple times, a different version of this sound is played (usually with a higher tune). For the APC these sound file names have to be like 0_2a_001, where the leading zero is selecting sound channel 0 (System 7 games use only this channel), 2a being the sound number and 001 the tune of this sound. The latter number is counted up for all tunes available for this sound.  
One of these sound series is usually the BG sound. This sound can be interrupted by other sounds but continues afterwards. As the numbers of these sounds are different from game to game they have to be set as an exception for every game. 

### System 11

The first System11 machines had two independent audio circuits, each containing a processor and sound EPROMS. One of these circuits was located on the main CPU board while the other was on a seperate audio board.  
In case of the Pinbot, PinMame distinguishes these boards by using two audio prefixes which can be selected by 00 and 01 in PinMame's 'Sound Command Mode'. Both of these prefixes offer different sound commands. Our current status of what we know about these sound commands is listed below. There're still lots of gaps, but we didn't want to spend too much effort in investigating these boards. Again, we see this more as a community task and there're probably plenty of people out there with a lot more knowledge about this stuff than we have.

So if you have additional information to fill these gaps please tell us and we're going to include it.

|Prefix|Command / Hex|Comment|
|--|--|--|
|00|00|Stop Sound|
|00|55|Unknown command, currently ignored|
|00|69|Seems to be a sound as PinMame plays one when this command is entered. But this sound is played at the start of multiball where it is completely misplaced, therefore it is currently ignored|
|00|aa|Unknown command, currently ignored|
|00|ff|Unknown command, currently ignored|
|01|00|Stop Music|
|01|01 - 08|Music tracks, which are probably looped - looping not yet implemented|
|01|1d - 30|Unknown commands, currently ignored|
|01|40 - 42|Transistions and endings for looped music|
|01|4f - 59|Unknown commands, currently ignored|
|01|7f|Stop sounds of prefix 01|
|01|6X|Music volume setting 0x60 is full volume 0x64 is almost nothing|
|01|aa|Unknown command, currently ignored|
|01|ff|Unknown command, currently ignored|

Prefix 01 plays music and sounds simultaneously. It looks like sound numbers below 0x80 are for music and the rest is for sounds. As the APC has only two independent sound channels, only the music of prefix 01 is played on the APC's music channel while the sounds are being forwarded to the sound channel.

Somewhere in System11B all audio circuitry was removed from the CPU board and the functionality was added to the audio board. For these games only the prefix 01 is used by PinMame.

## How to make it work

So much for the theory. Let's proceed to the [Howto page](https://github.com/AmokSolderer/APC/tree/master/DOC/PinMame_howto.md).
