# System 9

The audio generation circuits of System 9 machines are basically the same as of System 7. The main difference seems to be that System 9 boards use larger EPROMs and can therefore store more sound data. Hence, the data bus is not limited to 5 bit any more but uses the whole 8 bit which makes 256 different sound commands possible.  
Furthermore these machine feature some kind of crude background music. As these board still have just one audio channel, this 'music' must be interrupted as long as another sound is played.

This documentation uses Comet as an example as APC sound files and PinMameExceptions are available for this machine. I expect the other System 9 machines to work in a similar way.

Comet uses the following audio commands which are not sounds:

|Command / Hex|Comment|
|--|--|
|00|Stop Sound/Music|
|0b, fe|Unknown command -> ignored|
|2f|Background music|
|ff|Stop Sound/Music|

## Sound files

There're multiple ways to obtain the original sounds for System 9 machines. Some can be found on the internet, directly recorded from your pinball machine or you can extract them from PinMame. What ever you do, the result should be a mono WAV file with 44.1KHz sampling rate and reasonable amplitude.  
If you find the files on the internet you can proceed to the [Audio file conversion](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameSound_9.md#audio-file-conversion) section, as you don't have to extract them from PinMame.

## Automatic sound file preparation

There're several methods to automatically extract WAV files from PinMame and batch process them to work with the APC. Using this, it just takes me 2 - 3 hours to generate the files for an early System11 machine  
My way of doing this is described [here](https://github.com/AmokSolderer/APC/blob/master/DOC/SoundAuto.md). Since I'm using Linux, things might be a bit different for Windows users, as they additionally have to adjust the sample rate for example.  
Mokopin (from the Flippertreff forum) has also written some [Instructions for extracting sound files](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameSounds.md) which explain the automatic extraction of sound files and the use of Audacity for Windows users.

## Manual sound file preparation

This decribes the [manual way of sound file preparation](https://github.com/AmokSolderer/APC/blob/master/DOC/SoundManual.md). I'd recommend to try the automatic way first, because it's much faster. However, the manual way explains what needs to be done and it can be used as a fallback solution, if the automatic way fails.

## Audio file conversion

Put all preprocessed WAV files in one folder, check that the names are correct and execute the [AudioSaveFolder.pl](https://github.com/AmokSolderer/APC/blob/master/DOC/UsefulSWtools.md) tool.  
The resulting .snd files must be copied to the APC's SD card. Please format your card before with a special SD card formatting tool from the SD Association (sdcard.org) to improve the card's access times.

If you start your game now you should already hear most of the sounds, but everything out of the ordinary like correct music handling and so on won't work correctly. We need to tell the APC about these special sound sommands and how to deal with them. 

## PinMameExceptions for audio

The APC features a machine specific exception handling, which means that you can manipulate your game even though it is running in PinMame. The exceptions are written in C and are using the commands of the APC SW framework. More information about the framework can be found in [the tutorial](https://github.com/AmokSolderer/APC/blob/master/DOC/GameCodeTutorial.md) and the [APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf).

To enable PinMameExceptions for your machine you have to add a game specific section to the PinMameExceptions.ino file and recompile the SW.
You can manipulate sound, lamp, switch, display and solenoid commands. Some of these expections are necessary to make your machine work correctly, but you can also do improvements or moderate rule changes.

In the case at hand we're using these exceptions to tell the APC what to do when a certain audio command is issued by PinMame. I'm using the Comet as an example here.

### Setting up PinMameExceptions for the Comet

The following steps have been done by me to make my Comet work correctly. That means you can find the code which is described here in PinMameExceptions.ino.

There's a template named

    byte EX_Blank(byte Type, byte Command)

in PinMameExceptions.ino to be used as a starting point. So let's create a copy of this and rename it to

    byte EX_Comet(byte Type, byte Command)

In order for the system to use this code section, we have to add it to EX_Init which is at the end of PinMameExceptions.ino and determines which code is used for which machine. The games are being distinguished by their [PinMame game number](https://github.com/AmokSolderer/APC/blob/master/DOC/lisyminigames.csv). Hence we have to add a case 39 for the Comet.

    void EX_Init(byte GameNumber) {
      switch(GameNumber) {
      case 20:                                            // Jungle Lord
        EX_EjectSolenoid = 2;                             // specify eject coil for improved ball release
        PinMameException = EX_JungleLord;                 // use exception rules for Jungle Lord
        break;
        break;
      case 39:                                            // Comet
        PinMameException = EX_Comet;                      // use exception rules for Comet
        break;
      case 43:                                            // Pinbot
        PinMameException = EX_Pinbot;                     // use exception rules for Pinbot
        break;
      case 44:                                            // F-14 Tomcat
        PinMameException = EX_F14Tomcat;                  // use exception rules for Tomcat
        break;
      default:
        PinMameException = EX_DummyProcess;}}

All games not listet here do not have exceptions defined yet, so the exception pointer just points to a dummy process which only plays the standard sounds, but knows no exceptions.

### Handling ordinary sounds
     
As System9 just uses one sound channel, all sound exceptions have to be put into the SoundCommandCh1 case of our EX_Comet program. Hence you can delete all cases except of SoundCommandCh1 and the default case. The result should look like this:

    byte EX_Comet(byte Type, byte Command){               // use this as a template and an example of how to add your own exceptions
      switch(Type){                                       // usually just a few exception cases are needed, just delete the rest
      case SoundCommandCh1:                               // sound commands for channel 1
        if (Command == 38){                               // sound command 0x26
          // enter your special sound command 0x26 here
        }
        else {
          char FileName[9] = "0_00.snd";                  // handle standard sound
          if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
            PlaySound(51, (char*) FileName);}}
        return(0);                                        // return number not relevant for sounds
      default:                                            // use default treatment for undefined types
        return(0);}}

What we have now is a default handler for all the sound numbers that have no exceptions defined.  
The filenames of these sounds just consist of the channel number, an underscore and the sound number in hex followed by ".snd". There is a routine called

    byte USB_GenerateFilename(byte Channel, byte Sound, char* FileName)

which adds the hex code to a given filename and handles the display messages if the audio debug mode is active. It returns a 1 in case the soundfile does exist and a 0 if it doesn't. Is is therefore only necessary to play the sound when the return value has been 1.

In EX_Blank the 'if' for sound command 38 is just meant as an example to make it clear where exceptions have to be added, but for the Comet we can change this to incorporate the Sound Stop commands (00 and 0xff):

    byte EX_Comet(byte Type, byte Command) {
      switch(Type){
      case SoundCommandCh1:                               // sound commands for channel 1
        if (!Command || Command > 254) {                  // sound command 0x00 and 0xff -> stop sound
          AfterMusic = 0;
          StopPlayingMusic();
          StopPlayingSound();}
        else {                                            // handle standard sound
          char FileName[9] = "0_00.snd";
          if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
            PlaySound(51, (char*) FileName);}}
        return(0);                                        // return number not relevant for sounds
      default:
        return(0);}}
    
### Background music

Comet features a simple background music which is looping all over again. If you want it easy, you could just put some repetitions of the music in one file. This will work in most cases, but if you wait long enough the music will just run out.
In order to prevent this from happening, an exception rule must handle the looping.

    else if (Command == 47) {                         // play BG music
      PlayMusic(50, "0_2f.snd");
      QueueNextMusic("0_2f.snd");}                    // track is looping so queue it also

When audio command 47 is issued by PinMame then the music is played and the QueueNextMusic command is used to queue the file also for looping. As long as the AfterMusic variable is not set to zero, the looping part will be repeated automatically.

Due to the restriction to one audio channel of the old audio boards, this music had to be suspended every time another sound was played.  
Note that the code snippet above is using the PlayMusic and QueueNextMusic commands to play this file on the music channel of the APC which means that the music is running independently from the sounds.  
We can therefore choose whether we want to mute the music for as long as another sound is played or whether we want to play the music continuously in the background like System11 machines are doing.  
In this case I decided for the latter and only muted the music for one special sound sequence which starts with sound number 9 and ends with number 0xf1 (dec 241). The APC handles the volume of the music by the MusicVolume variable. A value of zero means full volume and every count up cuts the amplitude by half. Hence, the corresponding code looks like this:

      if (Command == 9) {
        MusicVolume = 4;}                             // reduce music volume
      if (Command == 241) {                           // sound 0xf1
        RestoreMusicVolumeAfterSound(25);}            // and restore it

### Dealing with unknown commands

There're still several commands we don't know the meaning of. If you find out what they do please drop us a note, but for the time being we can just ignore them (which worked quite well so far). So for every command that doesn't produce a sound in PinMame, just add a line like 

    else if (Command == 11 || Command == 254) { }     // ignore sound commands 0x0b and 0xfe

to prevent them from causing 'File not found' messages on your display.

### The result

The final result is shown below. It contains the PinMameExceptions needed to make the audio for Comet work.

    byte EX_Comet(byte Type, byte Command) {
      switch(Type){
      case SoundCommandCh1:                               // sound commands for channel 1
        if (!Command || Command > 254) {                  // sound command 0x00 and 0xff -> stop sound
          AfterMusic = 0;
          StopPlayingMusic();
          StopPlayingSound();}
        else if (Command == 11 || Command == 254) { }     // ignore sound commands 0x0b and 0xfe
        else if (Command == 47) {                         // play BG music
          PlayMusic(50, "0_2f.snd");
          QueueNextMusic("0_2f.snd");}                    // track is looping so queue it also
        else {                                            // handle standard sound
          if (Command == 9) {
            MusicVolume = 4;}                             // reduce music volume
          if (Command == 241) {                           // sound 0xf1
            RestoreMusicVolumeAfterSound(25);}            // and restore it
          char FileName[9] = "0_00.snd";
          if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
            PlaySound(51, (char*) FileName);}}
        return(0);                                        // return number not relevant for sounds
      default:
        return(0);}}                                      // no exception rule found for this type so proceed as normal
    
### Finding out which sounds are still missing

After you have extracted most of the files, there'll be the point when only a few files are still missing and you need to find out their numbers. Of course you could still scan the Lisy log for unknown sound numbers, but in this stage it might be easier to use the 'Audio Debug Mode' of the APC.  
You can activate this mode in the [game settings](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md#game-settings-in-remote-control-mode).

In Audio Debug mode the lower display(s) are used for audio information. The Player 3 display (or the left part of the lower display for BK2K type displays) shows information for sound prefix 00 and the Player 4 display (right part of lower display for BK2K) does the same for prefix 01. If the requested sound is found on the SD card, it's hex number is shown in the left side of the corresponding display and the sound is played normally. If the sound file is missing it's hex number is shown on the right side of the corresponding display which makes it easy to find missing sound files.  
As the pre System11 displays cannot show letters, the corresponding sound numbers are shown in decimal values when this kind of display is selected.

By that you can just play your game and only if the number of a missing sound file pops up on the right side of your display you note it down and add it later.

### Changing the background music

Most machines of this era have a quite basic background music. Of course you could just replace the audio file on the SD-card, but you could also do a simple change in the PinMameExceptions to make it selectable. A general [game setting](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md#game-settings-in-remote-control-mode) to select the new background music file is already present, so you don't have to bother with changing the settings menu.

The code below is for the Comet, but the principle is the same for all System 9 machines.  
In case of the Comet, the command for the background sound is 47. If this command is received, we have to check whether the setting for background music is selected or not. If yes the file MUSIC.snd is played on the music channel and also queued for looping. If background music is not selected in the settings, the normal BG music is played as usual.

    else if (Command == 47) {                         // play BG music
      if (game_settings[USB_BGmusic]) {               // use MUSIC.SND instead of BG sound
        PlayMusic(50, "MUSIC.snd");                   // play music track
        QueueNextMusic("MUSIC.snd");}                 // and loop it
      else {
        PlayMusic(50, "0_2f.snd");
        QueueNextMusic("0_2f.snd");}}                 // track is looping so queue it also
        
## Sound problems

Most sound problems like lags and stuttering are caused by the performance of the SD card. Take a look at the [If things don't work](https://github.com/AmokSolderer/APC/blob/master/DOC/Problems.md) section for more.
