# System 11

The first System11 machines had two independent audio circuits, each containing a processor and sound EPROMS. One of these circuits was located on the main CPU board while the other was on a seperate audio board. PinMame distinguishes these boards by using two audio prefixes which can be selected by 00 and 01 in PinMame's 'Sound Command Mode'. Both of these prefixes offer different sound commands.
The table below shows the special commands of the Pinbot audio system that we know about. There might still be some gaps, but it's enough to make the Pinbot work.  
So if you have additional information to fill these gaps please tell us and we're going to include it.

For this documentation I expect the other System 11 machines to work in a similar way.

|Prefix|Command / Hex|Comment|
|--|--|--|
|00|00|Stop Sound|
|01|00|Stop Music|
|01|01 - 08|Looped music tracks, some of them have an intro before the looping part begins|
|01|40 - 42|Transistions and endings for looped music|
|01|7f|Stop sounds of prefix 01|
|01|6X|Music volume setting 0x60 is full volume 0x64 is almost nothing|

## Sound file preparation

There're multiple ways to obtain the original sounds for System 11 machines. Some can be found on the internet, directly recorded from your pinball machine or you can extract them from PinMame. What ever you do, the result should be a mono WAV file with 44.1KHz sampling rate and reasonable amplitude.  
If you find the files on the internet you can proceed to the [Audio file conversion](https://github.com/AmokSolderer/APC/blob/V01.00/DOC/PinMameSound_11.md#audio-file-conversion) section, as you don't have to extract them from PinMame.

The method decribed here will be the the manual way, there're also more automatic solutions which might save some time. I've never tried them myself, but Mokopin (from the Flippertreff forum) has written some [Instructions for extracting sound files](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameSounds.md) which explain the automatic extraction of sound files and the use of Audacity in more detail.

### Which sounds are required?

System 11 boards can handle 8 bit sound commands which leads to a possible number of 256 sounds per board minus some control commands, but only a part of these commands are actually used by a game.  
With this manual method we have to extract and preprocess every sound by hand, so we try to keep the number of sounds as low as possible by just extracting those which are actually requested by PinMame.

When you start from scratch you should play your game with Lisy being in Sound Debug Mode. Please read the [Controlling Lisy page](https://github.com/AmokSolderer/APC/blob/master/DOC/LisyDebug.md) to learn how to do this.

I'd recommend to do the Williams sound and music test while PinMame is running. This will already give you most if not all of the music numbers and some sounds to start with.

When you're done press the shutdown switch SW1 on your APC board to make it exit the emulation and store the log file on the SD card. The file will be located on the Pi's SD card in the folder lisy/lisy_m/debug.

In lisy_m_debug.txt playing a sound looks like

    [461.372965][0.000064] LISY_W sound_handler: board:0 0x79 (121)
    [461.372985][0.000020] play soundindex 121 on board 0 
    [461.373005][0.000020] USB_write(3 bytes): 0x32 0x01 0x79

In this case sound 0x79 of prefix (board) 0 shall be played which means you have to record the sound command 0079 in PinMame.

### Finding out which sounds are still missing

After you have extracted most of the files, there'll be the point when only a few files are still missing and you need to find out their numbers. Of course you could still scan the Lisy log for unknown sound numbers, but in this stage it might be easier to use the 'Audio Debug Mode' of the APC.  
You can activate this mode in the [game settings](https://github.com/AmokSolderer/APC/blob/V01.00/DOC/Settings.md#game-settings-in-remote-control-mode).

In Audio Debug mode the lower display(s) are used for audio information. The Player 3 display (or the left part of the lower display for BK2K type displays) shows information for sound prefix 00 and the Player 4 display (right part of lower display for BK2K) does the same for prefix 01. If the requested sound is found on the SD card, it's hex number is shown in the left side of the corresponding display and the sound is played normally. If the sound file is missing it's hex number is shown on the right side of the corresponding display which makes it easy to find missing sound files.  
As the pre System11 displays cannot show letters, the corresponding sound numbers are shown in decimal values when this kind of display is selected.

The above mentioned Lisy sound debug log and the Audio Debug Mode of the APC only tell you which sound numbers are needed for your game. Now it's time to extract the corresponding sound files.

### Manual extraction from PinMame

For this to work you have to install PinMame32 for Windows as the Unix version has severe sound issues.

After PinMame is running, press F4 to enter the 'Sound Command Mode' then press DEL for the Manual / Command Mode.

The following steps have to be repeated for every sound you want to record. That means you have to do it for every sound number you've derived from the Lisy sound debug log or the Audio Debug Mode of the APC as described above.  
Enter the prefix and the hexadecimal sound number. Then press F5 to start the recording, SPACE to play the sound and F5 again to stop recording. The sound can be found in PinMame's 'wave' folder.  
The sound command 00 (with the corresponding prefix) stops the currently playing sound / music.  
Remember to rename the files immediately to the correct names to avoid confusion.

### Preprocessing the WAV files with Audacity

For some reason the PinMame sounds have a sampling rate of 48KHz and must therefore be converted to the normal sampling rate of 44.1KHz. You can use a free audio editor like Audacity (https://www.audacityteam.org) to adjust the start and end time of your sound sample as well as the sampling rate.  
Furthermore, the volume of the sound and music files generated by PinMame differs quite a lot, which means you should also use Audacity to adjust those levels accordingly.

![Audacity](https://github.com/AmokSolderer/APC/blob/V01.00/DOC/PICS/Audacity.png)

This is the manual way to preprocess a file recorded from PinMame.

* Open the file with Audacity
* Press the button for the Selection Tool (1) and select the part of the sound you want to use
* Delete the rest (2)
* Click inside the cyan box next to the waveform (3) to select the whole track
* Select the 'Tracks -> Sampling rate' menu and set the sampling rate to 44100Hz
* If you have a stereo track, use the 'Tracks -> Mix -> Mix Stereo Down to Mono' menu to get mono
* Select the 'Effect -> Normalize' menu to open the window with the normalizing options
* Select 'Remove DC offset' and 'Normalize peak amplitude to' 0dB.
* Select the 'File -> Export -> Export Selected Audio...' and save it with the 'Signed 16-bit PCM' encoding

Most of this process can be automatized as explained by Mokopin in his [Instructions for extracting sound files](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameSounds.md).

## Audio file conversion

Put all preprocessed WAV files in one folder, check that the names are correct and execute the [AudioSaveFolder.pl](https://github.com/AmokSolderer/APC/blob/V01.00/DOC/UsefulSWtools.md) tool.  
The resulting .snd files must be copied to the APC's SD card. Please format your card before with a special SD card formatting tool from the SD Association (sdcard.org) to improve the card's access times.

If you start your game now you should already hear most of the sounds, but everything out of the ordinary like correct music handling and so on won't work correctly. We need to tell the APC about these special sound sommands and how to deal with them. 

## PinMameExceptions for audio

The APC features a machine specific exception handling, which means that you can manipulate your game even though it is running in PinMame. To enable this for your machine you have to add a game specific section to the PinMameExceptions.ino file and recompile the SW.
You can manipulate sound, lamp, switch, display and solenoid commands. Some of these expections are necessary to make your machine work correctly, but you can also do improvements or moderate rule changes.

In the case at hand we're using these exceptions to tell the APC what to do when a certain audio command is issued by PinMame. I'm using the Pinbot as an example here.

### Setting up PinMameExceptions for the Pinbot

The following steps have been done by me to make my Pinbot work correctly. That means you can find the code which is described here in PinMameExceptions.ino.

There's a template named

    byte EX_Blank(byte Type, byte Command)

in PinMameExceptions.ino to be used as a starting point. So let's create a copy of this and rename it to

    byte EX_Pinbot(byte Type, byte Command)

In order for the system to use this code section, we have to add it to EX_Init which is at the end of PinMameExceptions.ino and determines which code is used for which machine. The games are being distinguished by their [PinMame game number](https://github.com/AmokSolderer/APC/blob/master/DOC/lisyminigames.csv). Hence we have to add a case 43 for the Pinbot.

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
    
As Pinbot uses two audio boards, we need to define a SoundCommandCh1 and a SoundCommandCh2 case in our EX_Pinbot program. Hence you can delete all cases except of these two and the default case. The result should look like this:

    byte EX_Pinbot(byte Type, byte Command){               // use this as a template and an example of how to add your own exceptions
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
      case SoundCommandCh2:                               // sound commands for channel 2
        if (Command == 38){                               // sound command 0x26
          // enter your special sound command 0x26 here
        }
        else {
          char FileName[9] = "1_00.snd";                  // handle standard music
          if (USB_GenerateFilename(2, Command, FileName)) { // create filename and check whether file is present
            PlayMusic(51, (char*) FileName);}}
        return(0);                                        // return number not relevant for sounds
      default:                                            // use default treatment for undefined types
        return(0);}}
    
For System11c games with only one audio board you can remove the SoundCommandCh1 case also.  
What we have now is a default handler for all the sound numbers that have no exceptions defined. The filenames of these sounds just consist of the channel number (prefix), an underscore and the sound number in hex followed by ".snd". There is a routine called

    byte USB_GenerateFilename(byte Channel, byte Sound, char* FileName)

which adds the hex code to a given filename and handles the display messages if the audio debug mode is active. It returns a 1 in case the soundfile does exist and a 0 if it doesn't. Is is therefore only necessary to play the sound when the return value has been 1.

In EX_Blank the 'if' for sound command 38 is just meant as an example to make it clear where exceptions have to be added, but for the Pinbot we can change this to incorporate the Sound Stop command (00) for each channel:

    byte EX_Pinbot(byte Type, byte Command){               // use this as a template and an example of how to add your own exceptions
      switch(Type){                                       // usually just a few exception cases are needed, just delete the rest
        if (!Command){                                    // sound command 0x00 - stop sound
          AfterSound = 0;
          StopPlayingSound();}
        else {
          char FileName[9] = "0_00.snd";                  // handle standard sound
          if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
            PlaySound(51, (char*) FileName);}}
        return(0);                                        // return number not relevant for sounds
      case SoundCommandCh2:                               // sound commands for channel 2
        if (!Command) {                                   // sound command 0x00 - stop music
          AfterMusic = 0;
          StopPlayingMusic();}
        else {
          char FileName[9] = "1_00.snd";                  // handle standard music
          if (USB_GenerateFilename(2, Command, FileName)) { // create filename and check whether file is present
            PlayMusic(51, (char*) FileName);}}
        return(0);                                        // return number not relevant for sounds
      default:                                            // use default treatment for undefined types
        return(0);}}

Note, that the APC refers to it's two channels as the Sound and Music channel and the commands are named accordingly. Check the [APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) for more information.

### Handling music

Most of the music scores of System11 machines consist of an intro and a looping part which is repeated all over. If you want it easy, you could just put the intro and some repetitions of the looping part in one file. This will work in most cases, but if you wait long enough the music will just run out.
In order to prevent this from happening, the intro and the looping part must be in separate files and an exception rule must handle the sequencing.

Let's use music track 4 from Pinbot as an example. In the SoundCommandCh2 case the following code handles track4:

    else if (Command == 4) {                          // music track 4
      PlayMusic(50, "1_04.snd");                      // play non looping part of music track
      QueueNextMusic("1_04L.snd");}                   // queue looping part as next music to be played

When audio channel 2 receives command 4 then the intro 1_04.snd is played first and the QueueNextMusic command is used to queue the looping part 1_04L.snd to be played after the intro has run out. As long as the AfterMusic variable is not set to zero, the looping part will be repeated automatically.

Such a rule needs to be present for each music score your machine features.

Note that if you're machine is using vocals as part of the music then these have to be mixed into the music. The reason for this is explained [here](https://github.com/AmokSolderer/APC/blob/V01.00/DOC/PinMameSound_11.md#special-cases).

### Implementing music volume

System11 machines are reducing the volume of their music track when certain sounds are played. This is done by sending the 0x6X command with the prefix of the music board. 0x60 is the code for full volume and 0x61 and so on for less volume. The granularity might differ for the various System11 generations, so you might want to adapt this for your game. However, for Pinbot 0x64 is already almost inaudible.  
The APC handles the volume of the music by the MusicVolume variable. A value of zero means full volume and every count up cuts the amplitude by half. Hence, the corresponding code looks like this:

    else if (Command > 95 && Command < 100) {         // music volume command 0x6X
      MusicVolume = Command - 96;}

### Dealing with priorities

Normally a sound is stopped by another sound being requested for the same channel. This is fine for the majority of the sounds, but sounds like speech are more important than others and we don't want the APC to cut them. Hence, we have to define priorities for those sounds which are more (or less) important than others.  
In Pinbot's case all sound numbers higher than 128 requested with prefix 0 are callouts and are therefore called with a priority of 51 instead of the 50 which is used for the rest:

    else {                                            // proceed with standard sound handling
      char FileName[9] = "0_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        if (Command < 128) {                          // play speech with a higher priority
          PlaySound(50, (char*) FileName);}
        else {
          PlaySound(51, (char*) FileName);}}}

Check the [APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) for more information about priorities.

### Dealing with unknown commands

There're still several commands we don't know the meaning of. If you find out what they do please drop us a note, but for the time being we can just ignore them (which worked quite well so far). So for every command that doesn't produce a sound in PinMame, just add a line like 

    else if (Command > 29 && Command < 48) { }        // ignore unknown sound commands 0x1d to 0x30

to prevent them from causing 'File not found' messages on your display.

### The result

The final result is shown below. It contains the PinMameExceptions needed to make the audio for Pinbot work.

    byte EX_Pinbot(byte Type, byte Command){
      switch(Type){
      case SoundCommandCh1:                               // sound commands for channel 1
        if (!Command){                                    // sound command 0x00 - stop sound
          AfterSound = 0;
          StopPlayingSound();}
        else if (Command == 85) { }                       // ignore sound command 0x55
        else if (Command == 105) { }                      // ignore strange misplaced sound during multiball
        else if (Command == 170) { }                      // ignore sound command 0xaa
        else if (Command == 255) { }                      // ignore sound command 0xff
        else {                                            // proceed with standard sound handling
          char FileName[9] = "0_00.snd";                  // handle standard sound
          if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
            if (Command < 128) {                          // play speech with a higher priority
              PlaySound(50, (char*) FileName);}
            else {
              PlaySound(51, (char*) FileName);}}}
        return(0);                                        // return number not relevant for sounds
      case SoundCommandCh2:                               // sound commands for channel 2
        if (!Command) {                                   // sound command 0x00 - stop music
          AfterMusic = 0;
          StopPlayingMusic();}
        else if (Command == 127) {                        // sound command 0x7f - stop sound
          AfterSound = 0;
          StopPlayingSound();}
        else if (Command > 29 && Command < 48) { }        // ignore unknown sound commands 0x1d to 0x30
        else if (Command > 79 && Command < 89) { }        // ignore unknown sound commands 0x4f to 0x59
        else if (Command > 95 && Command < 100) {         // music volume command 0x6X
          MusicVolume = Command - 96;}
        else if (Command == 170) { }                      // ignore unknown sound command 0xaa
        else if (Command == 255) { }                      // ignore unknown sound command 0xff
        else if (Command == 1) {                          // music track 1
          PlayMusic(50, "1_01L.snd");                     // play music track
          QueueNextMusic("1_01L.snd");}                   // track is looping so queue it also
        else if (Command == 2) {                          // music track 2
          PlayMusic(50, "1_02.snd");                      // play non looping part of music track
          QueueNextMusic("1_02L.snd");}                   // queue looping part as next music to be played
        else if (Command == 3) {                          // music track 3
          PlayMusic(50, "1_03L.snd");                     // play music track
          QueueNextMusic("1_03L.snd");}                   // track is looping so queue it also
        else if (Command == 4) {                          // music track 4
          PlayMusic(50, "1_04.snd");                      // play non looping part of music track
          QueueNextMusic("1_04L.snd");}                   // queue looping part as next music to be played
        else if (Command == 5) {                          // music track 5
          PlayMusic(50, "1_05.snd");                      // play non looping part of music track
          QueueNextMusic("1_04L.snd");}                   // queue looping part as next music to be played
        else if (Command == 6) {                          // music track 6
          PlayMusic(50, "1_06.snd");                      // play non looping part of music track
          QueueNextMusic("1_06L.snd");}                   // queue looping part as next music to be played
        else if (Command == 8) {                          // music track 8
          PlayMusic(50, "1_08.snd");                      // play non looping part of music track
          QueueNextMusic("1_01L.snd");}                   // queue looping part as next music to be played
        else if (Command == 10) {                         // music track 0xa
          PlayMusic(50, "1_0a.snd");                      // play non looping part of music track
          QueueNextMusic("1_02L.snd");}                   // queue looping part as next music to be played
        else if (Command == 65) {                         // music track 0x41
          PlayMusic(50, "1_01L.snd");                     // play non looping part of music track
          QueueNextMusic("1_01L.snd");}                   // queue looping part as next music to be played
        else if (Command == 66) {                         // music track 0x42
          PlayMusic(50, "1_0a.snd");                      // play non looping part of music track
          QueueNextMusic("1_02L.snd");}                   // queue looping part as next music to be played
        else if (Command == 148) {                        // music track 0x94
          PlayMusic(50, "1_94.snd");                      // play non looping part of music track
          QueueNextMusic("1_94L.snd");}                   // queue looping part as next music to be played
        else {
          char FileName[9] = "1_00.snd";                  // handle standard sound
          if (USB_GenerateFilename(2, Command, FileName)) { // create filename and check whether file is present
            if (Command < 128) {                          // play only music on the music channel
              AfterMusic = 0;                             // stop looping music
              PlayMusic(50, (char*) FileName);}           // play on the music channel
            else {
              PlaySound(50, (char*) FileName);}}}         // play on the sound channel
        return(0);                                        // return number not relevant for sounds
      default:                                            // use default treatment for undefined types
        return(0);}}                                      // no exception rule found for this type so proceed as normal
    

## Sound problems

Most sound problems like lags and stuttering are caused by the performance of the SD card. Take a look at the [If things don't work](https://github.com/AmokSolderer/APC/blob/master/DOC/Problems.md) section for more.

## Special cases 

On the old audio boards each specific kind of sound was generated by special circuitry. For example music was done with synthesizer ICs, speech with special DACs and so on. In total this leads to up to five different, but specialized audio channels which were mixed together eventually.  
Like all modern audio systems, the APC works differently. It has two channels, but doesn't need special audio ICs as every sound can be produced by each channel. In most cases this works quite well, but in some cases we have to adjust the sound files to prevent some sounds from getting lost.

A good example is the music of later System11 games which also include vocals. As the old sound board would produce the music and the voices with different circuits, PinMame will send one command for the music and own commands for each part of the vocals. This will lead to a conflict when additional sounds have to be played simultaneously as the APC has just one channel for sounds. On the other hand, the APC doesn't need a seperation between music and vocals, so we can just use Audacity to mix the vocals into the music file.
