# System 3 - 6

These machines only have one sound channel which means they can only play one sound at a time. This is also valid for later games featuring some kind of permanent background sound - due to the limitation to one channel this sound needs to be suspended for as long as a foreground sound is played.

The communication between CPU and audio board is 5 bit wide, which leads to 32 possible sound commands (0x00 - 0x1f).  
Not all of these commands have to be sounds, up to now we know of two control commands which seem to be common for System 3 - 6 audio boards:

|Command / Hex|Comment|
|--|--|
|0c|Stop Sound|
|1f|Not a real sound command, but used to reset the data bus to the audio board between commands|

Some of these games are featuring what I call a sound series. This means that if a certain sound number is called multiple times, a different version of this sound is played (usually with a higher tune). For the APC these sound file names have to be named like 0_0a_001, where the leading zero is selecting sound channel 0 (System 3 - 6 games use only this channel), 0a being the sound number in hex format and 001 the tune of this sound. The latter number is counted up for all tunes available for this sound. 

# System 7

The audio boards haven't changed much between System 3 - 6 and System 7. This means everything written above is also valid for System 7 with two exceptions:  
For some reason PinMame adds 32 (hex 20) to each audio command when it's a System 7 game. Hence, the audio commands are in the range from 0x20 to 0x3f and not from 0x00 to 0x1f as for the previous generations.  
This changes the 'Stop Sound' Command to 0x2c. The second exception is that 0x7f is sent for the bus initialization, but we don't care that much about this as the APC needs no initialization and we therefore ignore this command anyway.  
Find the special command table for System 7 below:

|Command / Hex|Comment|
|--|--|
|2c|Stop Sound|
|7f|Not a real sound command, but used to reset the data bus to the audio board between commands|

## Sound file preparation

If your game features one or more of the above mentioned sound series then extracting the sounds from PinMame will be quite cumbersome as every tune of a series has to be recorded and preprocessed.  
Luckily all sounds for these machines are available on the internet and only the naming has to be adapted.

In order to find out the correct sound names you have to install PinMame32 for Windows.  
After PinMame is running press F4 to enter the 'Sound Command Mode'. To play a sound you have to enter  7fXX with XX being the sound number in hex format and press the space bar. Try to find a match for each previously downloaded WAV file. If you found one, rename the WAV file to 0_XX.wav with XX being the sound number you just found out. If you found a sound series rename their tunes to 0_XX_001 and increase the 001 for each tune.  
This could be performed with an automatic renaming tool ( eg. Bulk Rename Utility https://www.bulkrenameutility.co.uk).

The big advantage of the downmloaded files is that they usually don't need any preprocessing (unlike those derived from PinMame), so you can directly convert them.

## Audio file conversion

Put all renamed WAV files in one folder and execute the [AudioSaveFolder.pl]https://github.com/AmokSolderer/APC/blob/V01.00/DOC/UsefulSWtools.md tool.  
The resulting .snd files need to be copied to the APC SD-CARD. Please format your card before with a special SD-card formatting tool like the one from the SD Association (sdcard.org).

If you start your game now you should already hear most of the sounds, but everything out of the ordinary like sound series and so on won't work correctly. We need to tell the APC about special sound sommands and how to deal with them. 

## PinMameExceptions for audio

The APC features a machine specific exception handling, which means that you can manipulate your game even though it is running in PinMame. To enable this for your machine you have to add a game specific section to the PinMameExceptions.ino file and recompile the SW.
You can manipulate sound, lamp, switch, display and solenoid commands. Some of these expections are necessary to make your machine work correctly while others are simply improvements or moderate rule changes.

In this case we're using these exceptions to tell the APC what to do when a certain audio command is issued by PinMame. I'm using the Jungle Lord as an example here because it incorporates everything these audio boards can do AFAIK.  
Note that the only difference between System 3 - 6 and System 7 is that PinMame adds 32 (hex 20) to each audio command when it's a System 7 game. Hence, the audio commands are in the range from 0x20 to 0x3f and not from 0x00 to 0x1f as for the previous generations.  
However, the basic setup is the same for every System 3 - 7 game.

### Set up of PinMameExceptions for the Jungle Lord

First of all we need to generate a Jungle Lord specific code section to handle all the required exceptions. There's a template named

    byte EX_DummyProcess(byte Type, byte Command)

in PinMameExceptions.ino you could use as a start. So let's create a copy of this and rename it to

    byte EX_JungleLord(byte Type, byte Command)

In order for the system to use this code section, we have to add it to EX_Init which is at the ende of PinMameExceptions.ino and determines which code is used for which machine. The games are being distinguished by their [PinMame game number](https://github.com/AmokSolderer/APC/blob/master/DOC/lisyminigames.csv). Hence we have to add a case 20 for the Jungle Lord.

    void EX_Init(byte GameNumber) {
      switch(GameNumber) {
      case 4:                                             // Disco Fever
    //SolRecycleTime[5-1] = 250;                        // set recycle time for eject hole to prevent double kicking
    PinMameException = EX_Fever;                      // use exception rules for Flash
    break;
      case 6:                                             // Flash
    SolRecycleTime[5-1] = 250;                        // set recycle time for eject hole to prevent double kicking
    PinMameException = EX_Flash;                      // use exception rules for Flash
    break;
      case 16:                                            // Firepower
    PinMameException = EX_Firepower;                  // use exception rules for Firepower
    break;
      case 20:                                            // Jungle Lord
    EX_EjectSolenoid = 2;                             // specify eject coil for improved ball release
    PinMameException = EX_JungleLord;                 // use exception rules for Jungle Lord
    break;
      case 21:                                            // Pharaoh
    PinMameException = EX_Pharaoh;                    // use exception rules for Pharaoh
    break;

and so on.

All games not listet here do not have exceptions defined yet, so the exception pointer just points to a dummy process which only plays the standard sounds, but knows no exceptions.
The definition of EX_EjectSolenoid is only there because I also want to improve the ball release of System 7 machines. The way it works is handled on the general [PinMameExceptions page](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameExceptions.md), but let's skip it for now as it has nothing to do with the sound.

As System7 just uses one sound channel, all sound exceptions have to be put into the SoundCommandCh1 case of our EX_JungleLord program. For a System11 game you'd also have to prepare exceptions for SoundCommandCh2 as these games use both sound channels.

### Random sounds

The first exception is the 0x26 sound command which triggers one of four random spoken phrases. In the exception handler this looks like this:

    case SoundCommandCh1:                               // sound commands for channel 1
      if (Command == 38){                               // sound command 0x26 - start game
        char FileName[13] = "0_26_000.snd";             // generate base filename
        FileName[7] = 48 + random(4) + 1;               // change the counter according to random number
        PlaySound(52, (char*) FileName);}               // play the corresponding sound file

The APC expects the corresponding sound files to be named 0_26_00X.snd with the X being one for the first file, two for the second and so on.
First we generate the base filename "0_26_000.snd", then we change the 8th character of this string to a random number between 1 and 4 and play the sound file with a priority of 52.

### Looping sound series

The next special sound command of the Jungle Lord is 0x2d. This is a looping sound series, which means it starts again with the first tune after the last has been played. The corresponding code is:

    else if (Command == 45){                          // sound command 0x2d - multiball start - sound series
      if (SoundSeries[1] < 31)                        // this sound has 31 tunes
        SoundSeries[1]++;                             // every call of this sound proceeds with next tune
      else
        SoundSeries[1] = 1;                           // start all over again
      char FileName[13] = "0_2d_000.snd";             // generate base filename
      FileName[7] = 48 + (SoundSeries[1] % 10);       // change the 7th character of filename according to current tune
      FileName[6] = 48 + (SoundSeries[1] % 100) / 10; // the same with the 6th character
      PlaySound(51, (char*) FileName);}               // play the sound

For this we need an additional variable SoundSeries which stores the number of the tune currently being played. This variable has to be defined as static byte at the beginning of our EX_JungleLord.
At first it is checked whether the last tune of this series is currently being played. If yes then the tune number is set back to one otherwise it is increased by one. After that the base filename is generated, the new tune number is written into it and the sound is played.

### The bachground sound

Sound command 0x2a is also a sound series, so it's treated very similarly.

    else if (Command == 42){                          // sound command 0x2a - background sound - sound series
      SoundSeries[1] = 0;                             // reset the multiball start sound
      if (SoundSeries[0] < 29)                        // BG sound has 29 tunes
        SoundSeries[0]++;                             // every call of this sound proceeds with the next tune
      char FileName[13] = "0_2a_000.snd";             // generate base filename
      FileName[7] = 48 + (SoundSeries[0] % 10);       // change the 7th character of filename according to current tune
      FileName[6] = 48 + (SoundSeries[0] % 100) / 10; // the same with the 6th character
      for (byte i=0; i<12; i++) {                     // store the name of this sound
        USB_RepeatSound[i] = FileName[i];}
      QueueNextSound(USB_RepeatSound);                // select this sound to be repeated
      PlaySound(51, (char*) FileName);}               // play the sound

One difference is that this sound series is not a looping one which means the tune counter is not reset to one, but stays at the highest value until it is reset by the stop sound command 0x2c. Furthermore this command resets the tune of the 0x2d sound series (SoundSeries[1] = 0;).  
However, the major difference is that 0x2a is the background sound which can be interrupted by other sounds, but will continue afterwards.  
In the APC SW the Aftersound pointer can be used for this. This pointer can be set to a routine which is called automatically when a sound has run out. That's why the name of the current tune is always stored in the USB_RepeatSound variable. Then QueueNextSound is called which takes the filename USB_RepeatSound points to as an argument. It sets the AfterSound pointer to a routine which will play the filename stored in USB_RepeatSound when the current sound file is running out. This ensures that whenever the background sound is being interrupted by another sound, it is immediately being restarted as soon as this sound has run out. Setting AfterSound = 0 will disable this mechanism.

### Defining the basic special commands

At last we have to implement the two basic special commands every system 3 - 7 game has. These are the sound stop command (0x0c for Sys 3 - 6 and 0x2c for Sys 7) and the bus initialization (0x1f for Sys 3 - 6 and 0x7f for Sys 7). 
Let's do the stop command first:

    else if (Command == 44) {                         // sound command 0x2c - stop sound
      AfterSound = 0;
      SoundSeries[0] = 0;                             // Reset BG sound
      SoundSeries[1] = 0;                             // reset the multiball start sound
      StopPlayingSound();}

This command sets AfterSound = 0 which will prevent the BG sound from being restarted. Then it resets both sound series counters and stops the current playback.

Another basic command every System 3 - 7 game has is the bus initialization (0x1f for Sys 3 - 6 and 0x7f for Sys7). This command is only needed when an old audio board is used, so we can just ignore it for the APC.

    if (Command == 127) { }                           // ignore sound command 0x7f - audio bus init - not relevant for APC sound

### Handle ordinary sounds
    
Up to now we have only implemented the sounds that are somehow special for the Jungle Lord, but the majority of the sounds are ordinary ones. That means we need a default handler for all the sound numbers that have not been covered by the exceptions above.

    else {                                            // standard sound
      char FileName[9] = "0_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        PlaySound(51, (char*) FileName);}}
    return(0);                                        // return number not relevant for sounds

The filenames of these sounds just consist of the channel number, an underscore and the sound number in hex followed by ".snd". There is a routine called

    byte USB_GenerateFilename(byte Channel, byte Sound, char* FileName)

which adds the hex code to a given filename and handles the display messages if the audio debug mode is active. It returns a 1 in case the soundfiles does exist and a 0 if it doesn't. Is is therefore only necessary to play the sound when the return value has been 1.

The return marks the end of the SoundCommandCh1 case.

### The result

The final result is shown below. It contains the PinMameExceptions needed to make the audio for Jungle Lord work.

    byte EX_JungleLord(byte Type, byte Command){
      static byte SoundSeries[2];                         // buffer to handle pre system11 sound series
      switch(Type){
      case SoundCommandCh1:                               // sound commands for channel 1
        if (Command == 127) { }                           // ignore sound command 0x7f - audio bus init - not relevant for APC sound
        else if (Command == 38){                          // sound command 0x26 - start game
          char FileName[13] = "0_26_000.snd";             // generate base filename
          FileName[7] = 48 + random(4) + 1;               // change the counter according to random number
          PlaySound(52, (char*) FileName);}               // play the corresponding sound file
        else if (Command == 42){                          // sound command 0x2a - background sound - sound series
          SoundSeries[1] = 0;                             // reset the multiball start sound
          if (SoundSeries[0] < 29)                        // BG sound has 29 tunes
            SoundSeries[0]++;                             // every call of this sound proceeds with the next tune
          char FileName[13] = "0_2a_000.snd";             // generate base filename
          FileName[7] = 48 + (SoundSeries[0] % 10);       // change the 7th character of filename according to current tune
          FileName[6] = 48 + (SoundSeries[0] % 100) / 10; // the same with the 6th character
          for (byte i=0; i<12; i++) {                     // store the name of this sound
            USB_RepeatSound[i] = FileName[i];}
          QueueNextSound(USB_RepeatSound);                // select this sound to be repeated
          PlaySound(51, (char*) FileName);}               // play the sound
        else if (Command == 44) {                         // sound command 0x2c - stop sound
          AfterSound = 0;
          SoundSeries[0] = 0;                             // Reset BG sound
          SoundSeries[1] = 0;                             // reset the multiball start sound
          StopPlayingSound();}
        else if (Command == 45){                          // sound command 0x2d - multiball start - sound series
          if (SoundSeries[1] < 31)                        // this sound has 31 tunes
            SoundSeries[1]++;                             // every call of this sound proceeds with next tune
          else
            SoundSeries[1] = 1;                           // start all over again
          char FileName[13] = "0_2d_000.snd";             // generate base filename
          FileName[7] = 48 + (SoundSeries[1] % 10);       // change the 7th character of filename according to current tune
          FileName[6] = 48 + (SoundSeries[1] % 100) / 10; // the same with the 6th character
          PlaySound(51, (char*) FileName);}               // play the sound
        else {                                            // standard sound
          char FileName[9] = "0_00.snd";                  // handle standard sound
          if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
            PlaySound(51, (char*) FileName);}}
        return(0);                                        // return number not relevant for sounds
          default:
        return(0);}}                                      // no exception rule found for this type so proceed as normal

The actual EX_JungleLord in PinMameExceptions.ino looks slightly different as it features some additional improvements, but these have nothing to do with audio. However, you can more infos about that on the general [PinMameExceptions page](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameExceptions.md).
