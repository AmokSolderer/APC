# The APC and PinMame
Bringing PinMame and the APC together was only possible with the help of the Lisy project. In the APC case we're using the Lisy_Mini board, but Lisy offers much more especially for Bally and Gottlieb fans. Please visit the [Lisy homepage](https://lisy.dev) for more.

We have started our PinMame experiments with my old trusty Pinbot which is running quite well as you can see in the following video:

[Lisy, APC and PinMame](https://youtu.be/cXrh-XPqCKw)

PinMame support is still in it's early stages. As you can see the Pinbot is already working quite well, but it's not yet perfect. The main challenges when adding support for more machines are sound related. This has to do with the way we did the sound implementation which offers a lot more than just playing the original sounds and music, but requires a bit more work to do so. 

## The Sounds

Our current setup is not using the Raspberry Pi to generate the PinMame sounds. Instead all sound and music files have to be created once and stored on the SD card of the APC. Of course this requires some work, but we think the benefits are worth it.

First of all, the Pi uses the Unix version of PinMame which has severe sound quality issues while the audio quality of the APC is very good. But there're some more advantages in having all sound files on the SD card. One is that you can use the same files for PinMame, MPF or when programming the APC natively. This may not sound like a big deal, but it gives you the opportunity to run your game in PinMame and introduce certain changes by using the [APC API](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf)
.

A good example is a local fellow who wants his System 3 'Disco Fever' to play songs like 'Saturday Night Fever' additionally to the normal sounds of the machine. We still have to add PinMame support for System 3, but once we have it adding the music should be an easy task. The commands of the APC API can be used to start the music when the game starts and to stop it when it ends. You could even identify certain trigger points to switch to another song and so on.

To make this possible, it is necessary that PinMame and the APC can play sounds simultaneously and to avoid an additional HW sound mixer which mixes both sound channels together, we decided to let the APC do the complete sound handling with PinMame just telling him which sound to play when.

The drawback of this is that you have to extract the music files from PinMame. Furthermore we have to understand how the audio boards work and emulate their behaviour. The first task is easy assuming we find a place to store the files somewhere in the internet, because then this work must only be done once for each game. For System 3 - 9 games I expect this to be easy anyway, as they have a very limited sound performance. You could use the Audio Debug Mode which is explained later to find out which sounds to extract.

At the moment the list of available sound packages has only one entry, but we see the extraction of the sound files as a community task. Of course it would be great if you'd share you sound package with the rest of us. 

|System|Game|URL|Comments|
|--|--|--|--|
|11|Pinbot| URL| Some PinMame sounds are quite bad, e.g. visor opening and closing. May be someone can sample them from the orignal HW. No looping implemented yet - if you wait long enough, some sounds will just run out|

## Audio boards

Understanding the audio boards is the second task at hand. Again I expect this to be easy for games prior to System 11, but it took me roughly a week before I was satified with the sounds of my Pinbot. Of course I started from scratch, so it's going to be easier for the next one doing this, but System 11 features various audio boards and I don't know how much they differ from the Pinbot's. For this reason I'm going to maintain a second list here where all PinMame relevant information about the various systems are listed.

### System 11

In case of the Pinbot, PinMame distinguishes two audio prefixes which can be selected by 00 and 01 in PinMame's 'Sound Command Mode'. These prefixes might fit to the two sound boards that are present in the Pinbot with one being on the CPU board and one extra board. Both of these prefixes offer different sound commands. Our current status of what we know about these sound commands is listed below. There're still lots of gaps, but we didn't want to spend too much effort in investigating these boards. We see this more as a community task and there's probably plenty of people out there with a lot more knowledge about this stuff than we have.

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

## Recording the sounds

If you want to get the original sounds for your machine, you have to install PinMame for Windows as the Unix version has severe sound issues.

After PinMame is running press F4 to enter the 'Sound Command Mode' and enter the prefix and the hexadecimal sound number of the sound you want to record. Then press F5 to start the recording, SPACE to play the sound and F5 again to stop recording. The sound can be found in PinMame's 'wave' folder.

Like all sounds to be played by the APC the WAV file has to be processed with my Audio Data Converter. You can find additional information about this tool in the [Useful Software Tools](https://github.com/AmokSolderer/APC/wiki/Useful-software-tools) Wiki.

In order for the APC to find the right sound, all sounds have to be named with the prefix (only one digit), underscore, the hexadecimal number of the sound and .snd as the extension. For example, the name of sound 0xf1 of prefix 00 would be 0_f1.snd. All files need to be placed in the root folder of APC's SD card.

### Which sounds are required?

When you start from scratch you should play your game with Lisy being in Debug Mode. Set DIP 7 and the sound jumper to make it log only sound related commands.

When you're done playing, press the shutdown switch on your Lisy_Mini board to make it exit the emulation and store the log file to the SD card. The file will be located on the PI's SD card in the folder lisy/lisy_m/debug.

In lisy_m_debug.txt playing a sound looks like

    [461.372965][0.000064] LISY_W sound_handler: board:0 0x79 (121)
    [461.372985][0.000020] play soundindex 121 on board 0 
    [461.373005][0.000020] USB_write(3 bytes): 0x32 0x01 0x79

In this case sound 0x79 of prefix (board) 0 shall be played which means you have to record the sound command 0079 in PinMame, convert it and write it to the APC's SD card as 0_79.snd.

### Finding out which sounds are still missing

After you have extracted most of the files, there'll be the point when only a few files are still missing and you need to find out their numbers. Of course you could still scan the Lisy log for unknown sound numbers, but in this stage it might be easier to use the 'Audio Debug Mode' of the APC.

You can activate this mode in the game settings. To do this you have to press Advance for more than a second to enter the settings. Use Advance to change from system to game settings and press the start button to enter those. Use Advance to proceed to the 'Debug Mode' setting, then change it to 'AUDIO' by pressing the start button. Go to 'Exit Settings' by pressing Advance and the Start button to confirm.

In Audio Debug mode the lower display(s) are used for audio information. The Player 3 display (or the left part of the lower display for BK2K type displays) shows information for sound prefix 00 and the Player 4 display (right part of lower display for BK2K) does the same for prefix 01. If the requested sound is found on the SD card, it's hex number is shown in the left side of the corresponding display and the sound is played normally. If the sound file is missing it's hex number is shown on the right side of the corresponding display which makes it easy to find missing sound files.