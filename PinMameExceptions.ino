#define SoundCommandCh1 0
#define SoundCommandCh2 1
#define SwitchActCommand 2
#define SwitchRelCommand 3
#define SolenoidActCommand 4
#define SolenoidRelCommand 5
#define LampOnCommand 6
#define LampOffCommand 7
#define WriteToDisplay0 8
#define WriteToDisplay1 9
#define WriteToDisplay2 10
#define WriteToDisplay3 11
#define WriteToDisplay4 12
#define USB_Option1 5                                 // an option for own PinMame exceptions
#define LED_red 6                                     // to change the color of the LED GI
#define LED_green 7
#define LED_blue 8
#define USB_BGmusic 9                                 // to select an own BG music
#define USB_CustomText 10                             // to select a custom text to be shown during attract mode

byte USB_ChangedSwitches[64];                         // moved here from USBcontrol
const byte PME_GIallOn[4] = {255, 255, 255, 255};     // all GI LEDs on
const byte PME_GIon[4] = {255, 255, 255, 251};        // all GI LEDs on except of one
const byte PME_GIoff[4] = {0, 0, 0, 0};               // all GI LEDs off
const byte PME_GI1[4] = {255, 0, 0, 0};               // patterns for GI flashers
const byte PME_GI2[4] = {0, 255, 0, 0};
const byte PME_GI3[4] = {0, 0, 255, 0};
const byte PME_GI4[4] = {0, 0, 0, 255};

byte USB_SerialBuffer[128];                           // received command arguments
char USB_RepeatSound[13];                             // name of the sound file to be repeated
byte EX_EjectSolenoid;                                // eject coil for improved ball release
byte EX_CustomText[28];                               // stores the custom text read from SD card

                            //LED number..00000000....11111110....22222111....33322222...43333333...44444444...55555554...66666555
                              // Color....12345678....65432109....43210987....21098765...09876543...87654321...65432109...43210987
const byte GI_Pattern[21] = {255, 0, 0, 0b01001001, 0b10010010, 0b00100100, 0b01001001,
                             0, 255, 0, 0b10010010, 0b00100100, 0b01001001, 0b10010010,
                             0, 0, 255, 0b00100100, 0b01001001, 0b10010010, 0b00100100};  // GI LEDs patterns
const uint16_t GI_Duration[4] = {30, 30, 30, 0};


void EX_BallRelease(byte State) {                     // repeat ball eject in case the ball got stuck
  static byte Timer;                                  // stores the timer number
  switch (State) {                                    // determines what to do
  case 0:                                             // kill the timer
    if (Timer) {                                      // if a timer is active
      KillTimer(Timer);                               // kill it
      Timer = 0;}                                     // and set Timer = 0 to indicate that no timer is active
    break;
  case 1:                                             // start a timer
    if (!Timer) {                                     // if no timer is active
      Timer = ActivateTimer(3000, 2, EX_BallRelease);}  // start one for 3s with 2 as the argument
    break;
  case 2:                                             // timer has run out
    ActivateSolenoid(40, EX_EjectSolenoid);           // activate the shooter lane feeder again
    Timer = ActivateTimer(3000, 2, EX_BallRelease);}} // and restart the timer

void EX_StepperMotor(byte State) {                    // control stepper motor
  static byte MainTimer = 0;                          // number of the main timer
  static unsigned int RefTime;                        // stores the start value of the main timer
  static unsigned int PWMtime;                        // stores the PWM time for the active cycle
  switch(State) {                                     // state machine
  case 0:                                             // procedure is called from solenoid exception
    if (!MainTimer) {                                 // initial call, stepper motor not yet running
      RefTime = 60;                                   // set timeout to 60ms
      MainTimer = ActivateTimer(RefTime, 1, EX_StepperMotor);}  // start main timer
    else {                                            // not the first solenoid call from PinMame as main timer is already running
      PWMtime = RefTime - TimerValue[MainTimer];      // calculate time difference between PinMame's solenoid pulses
      KillTimer(MainTimer);                           // cancel timeout
      RefTime = PWMtime + 10;                         // set new timeout value based on last PWM cycle time
      MainTimer = ActivateTimer(RefTime, 1, EX_StepperMotor); // restart main timer
      ActivateSolenoid(PWMtime/2, 14);                // generate PWM pulse on first solenoid
      ActivateTimer(PWMtime/4, 2, EX_StepperMotor);}  // come back to generate pulse on second solenoid
    break;
  case 1:                                             // timeout has occurred as no pulses from PinMame have been detected recently
    MainTimer = 0;                                    // indicate that main timer is not active
    ActivateSolenoid(PWMtime/2, 14);                  // generate last step on the solenoids anyway
    ActivateTimer(PWMtime/4, 2, EX_StepperMotor);
    break;
  case 2:                                             // generate pulse on second solenoid
    ActivateSolenoid(PWMtime/2, 15);
    break;}}

void EX_FakeSwitchSequence(byte State) {              // sends a sequence of fake switches to PinMame
  switch (State) {
  case 1:
    USB_SwitchHandler(3);                             // report switch 3 activated
    ActivateTimer(100, 2, EX_FakeSwitchSequence);     // wait 100ms and come back
    break;
  case 2:
    USB_ReleasedSwitches(3);                          // report switch 3 released
    ActivateTimer(1000, 3, EX_FakeSwitchSequence);
    break;
  case 3:
    break;}}

byte EX_FakeSwitches(byte Type, byte Command){        // use this to start sending fake switches to PinMame
  switch (Type) {
//  case SoundCommandCh1:                               // sound commands for channel 1
//    char FileName[9] = "0_00.snd";                    // handle standard sound
//    if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
//      PlaySound(51, (char*) FileName);}
//    return(0);                                        // return number not relevant for sounds
//  case SoundCommandCh2:                               // sound commands for channel 2
//    char FileName[9] = "1_00.snd";                    // handle standard music
//    if (USB_GenerateFilename(2, Command, FileName)) { // create filename and check whether file is present
//      PlayMusic(51, (char*) FileName);}
//    return(0);                                        // return number not relevant for sounds
  case SwitchActCommand:                              // activated switches
    Serial.print("Sw ");                              // report their activation
    Serial.println(Command);
    if (Command == 65) {                              // start sequence with special solenoid switch 1
      EX_FakeSwitchSequence(1);                       // start fake sequence
      return(1);}                                     // but don't tell PinMame
    return(0);                                        // all other switches are reported
  case SolenoidActCommand:                            // activate solenoids
    if (Command != 14 && Command != 15){              // for all solenoids except for 14 and 15
      Serial.print("Sol ");                           // report their activation
      Serial.println(Command);    }                   // works only when debug mode is enabled
    return(0);                                        // solenoid will be activated
  default:
    return(0);}}

                                // game specific exceptions

byte EX_DummyProcess(byte Type, byte Command) {       // plays just the standard sounds
  if (Type == SoundCommandCh1) {                      // sound commands for channel 1
    char FileName[9] = "0_00.snd";                    // handle standard sound
    if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
      PlaySound(51, (char*) FileName);}}
  else if (Type == SoundCommandCh2) {                 // sound commands for channel 2
    char FileName[9] = "1_00.snd";                    // handle standard music
    if (USB_GenerateFilename(2, Command, FileName)) { // create filename and check whether file is present
      PlayMusic(51, (char*) FileName);}}
  return(0);}                                         // no exception rule found for this type so proceed as normal

byte EX_Firepower(byte Type, byte Command){           // thanks to Matiou for sending me this code
  static byte SoundSeries[5] = {0, 0, 0, 0, 0};       // buffer to handle pre system11 sound series
  static byte PlayingMultiballSound = 0;
  const byte PlayCombinedSoundForMultiball = 1;       // 1-> play one sound "1, 2, 3". 2-> play 3 sounds "1", "2", "3".

  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
	  if (Command == 33 ||                              // ignore sound command 0x21, 0x27, 0x7f,0xff
			  Command == 39 ||
			  Command == 127 ||
			  Command == 255) { }
	  else if (Command == 108) {                        // sound command 0x6c - stop all sounds and reset series
		  AfterSound = 0;
		  SoundSeries[0] = 0;
		  SoundSeries[1] = 0;
		  SoundSeries[2] = 0;
		  SoundSeries[3] = 0;
		  SoundSeries[4] = 0;
		  StopPlayingSound();}
	  else {
		  if (Command > 127) {
			  Command &= 127;}
		  if (Command == 100){                         // 0x64 End game - random speech
			  char FileName[13] = "0_64_000.snd";             // generate base filename
			  FileName[7] = 48 + random(9) + 1;               // change the counter according to random number
			  PlaySound(52, (char*) FileName);}               // play the corresponding sound file
		  else if (Command == 103){                         // 0x67 Fire one/two/three series (multiball!)
			  // code for individual sounds
			  if (PlayCombinedSoundForMultiball == 0) {
				  PlayingMultiballSound = 1;                    // remember we're in a multiball start session
				  if (SoundSeries[0] < 3)                       // this sound has 3 tunes
					  SoundSeries[0]++;                           // every call of this sound proceeds with next tune
				  else                                          //
					  SoundSeries[0] = 1;                         // start all over again
				  char FileName[13] = "0_67_000.snd";           // generate base filename
				  FileName[7] = 48 + (SoundSeries[0] % 10);     // change the 7th character of filename according to current tune
				  PlaySound(51, (char*) FileName);}             // play the sound
			  else {                                          // code for combined sounds (not standard but works better)
				  if (PlayingMultiballSound == 0) {
					  PlayingMultiballSound = 1;
					  char FileName[13] = "0_67_004.snd"; // this wav is combined version from 67_001 to 67_003
					  PlaySound(51, (char*) FileName);}}}
		  else if (Command == 105){                         // 0x69 Bonus
			  if (SoundSeries[1] < 146)                       // this sound has 146 tunes
				  SoundSeries[1]++;                             // every call of this sound proceeds with next tune
			  char FileName[13] = "0_69_000.snd";             // generate base filename
			  FileName[7] = 48 + (SoundSeries[1] % 10);       // change the 7th character of filename according to current tune
			  FileName[6] = 48 + (SoundSeries[1] % 100) / 10; // the same with the 6th character
			  FileName[5] = 48 + (SoundSeries[1] / 100);      // the same with the 5th character
			  PlaySound(51, (char*) FileName);}               // play the sound
		  else if (Command == 106) {                        // 0x6a Whirlling background
			  if (SoundSeries[2] < 29 )                       // this sound has 29 tunes
				  SoundSeries[2]++;                             // every call of this sound proceeds with next tune
			  char FileName[13] = "0_6a_000.snd";             // generate base filename
			  FileName[7] = 48 + (SoundSeries[2] % 10);       // change the 7th character of filename according to current tune
			  FileName[6] = 48 + (SoundSeries[2] % 100) / 10; // the same with the 6th character
			  PlaySound(51, (char*) FileName);}               // play the sound
		  else if (Command == 109) {                        // 0x6d Spinner
			  if (SoundSeries[3] < 31 )                       // this sound has 31 tunes
				  SoundSeries[3]++;                             // every call of this sound proceeds with next tune
			  char FileName[13] = "0_6d_000.snd";             // generate base filename
			  FileName[7] = 48 + (SoundSeries[3] % 10);       // change the 7th character of filename according to current tune
			  FileName[6] = 48 + (SoundSeries[3] % 100) / 10; // the same with the 6th character
			  PlaySound(51, (char*) FileName);}               // play the sound
		  else if (Command == 110) {                        // 0x6e Background // repeated
			  PlayingMultiballSound = 0;                      // if the background plays, we're not in a multiball start session
			  if (SoundSeries[4] < 31 )                       // this sound has 31 tunes
				  SoundSeries[4]++;                             // every call of this sound proceeds with next tune
			  char FileName[13] = "0_6e_000.snd";             // generate base filename
			  FileName[7] = 48 + (SoundSeries[4] % 10);       // change the 7th character of filename according to current tune
			  FileName[6] = 48 + (SoundSeries[4] % 100) / 10; // the same with the 6th character
			  for (byte i=0; i<12; i++) {                     // store the name of this sound
				  USB_RepeatSound[i] = FileName[i];}
			  QueueNextSound(USB_RepeatSound);                // select this sound to be repeated
			  PlaySound(51, (char*) FileName);}               // play the sound
		  else if ((Command == 104 || Command == 107 || Command == 60 || Command == 63)      // ignore these sounds at beginning of multiball
				  && PlayingMultiballSound == 1) { }
		  else {                                            // standard sound
			  char FileName[9] = "0_00.snd";                  // handle standard sound
			  if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
				  PlaySound(51, (char*) FileName);}}}
	  return(0);                                        // return number not relevant for sounds
  default:
	  return(0);}}                                      // no exception rule found for this type so proceed as normal

void EX_JL_LaneChange(byte Mode) {
  EX_JungleLord(Mode, 0);}

byte EX_JungleLord(byte Type, byte Command){
  static byte SoundSeries[2];                         // buffer to handle pre system11 sound series
  static byte LordModeTimer = 0;											// stores the timer number for the optional LORD lane change
  static byte MultiballTimer = 0;                     // stores the timer number for the optional LORD lane change multiball detect timer
  static byte PMlamp;                                 // stores the PinMame state of the LORD lamps
  static byte LampMov;                                // record the LORD lane change movements
  static bool LaneChangeDone = 0;                     // disable the magna save switches after the mini ball has activated a switch
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
  case SwitchActCommand:                              // activated switches
    if (!LaneChangeDone && LordModeTimer && Command > 12 && Command < 17) {  // LORD lane change mode active and one of the LORD switches triggered?
      LaneChangeDone = true;                          // stop lane change
      Command = Command - LampMov;                    // apply lane change moves to switch number
      if (Command < 13) {
        Command = Command + 4;}
      switch(Command) {                               // store corresponding PinMame lamp
      case 13:
        PMlamp = PMlamp | 8;
        break;
      case 14:
        PMlamp = PMlamp | 4;
        break;
      case 15:
        PMlamp = PMlamp | 2;
        break;
      case 16:
        PMlamp = PMlamp | 1;
        break;}
      byte i = 0;                                     // add switch number to list of changed switches
      while (USB_ChangedSwitches[i] && (i<63)) {
        i++;}
      USB_ChangedSwitches[i] = Command | 128;         // send manipulated switch number to PinMame
      return(1);}                                     // don't report the original switch number to PinMame
    else if (Command == 43) {                         // ball successfully ejected
      EX_BallRelease(0);}                             // stop ball release timer
    else if (Command == 49) {                         // right magnet button
      if (LordModeTimer) {                            // mini playfield active?
        if (!LaneChangeDone) {                        // mini ball has not already triggered a switch
          LampMov++;                                  // store right move of lamps
          if (LampMov > 3) {
            LampMov = 0;}
          bool Buffer = QueryLamp(16);                // move all LORD lamps to the right
          for (byte i=0; i<3; i++) {
            if (QueryLamp(15-i)) {
              TurnOnLamp(16-i);}
            else {
              TurnOffLamp(16-i);}}
          if (Buffer) {
            TurnOnLamp(13);}
          else {
            TurnOffLamp(13);}
          return(1);}}
      else {
        if (QueryLamp(8) && QueryLamp(2)) {           // right magnet and ball in play lamp lit?
          ActivateSolenoid(0, 22);}}}                 // activate right magnet
    else if (Command == 50) {                         // left magnet button
      if (LordModeTimer) {                            // mini playfield active?
        if (!LaneChangeDone) {                        // mini ball has not already triggered a switch
          if (LampMov) {                              // store left move of lamps
            LampMov--;}
          else {
            LampMov = 3;}
          bool Buffer = QueryLamp(13);                // move all LORD lamps to the left
          for (byte i=0; i<3; i++) {
            if (QueryLamp(14+i)) {
              TurnOnLamp(13+i);}
            else {
              TurnOffLamp(13+i);}}
          if (Buffer) {
            TurnOnLamp(16);}
          else {
            TurnOffLamp(16);}
          return(1);}}
      else {
        if (QueryLamp(39) && QueryLamp(2)) {          // left magnet and ball in play lamp lit?
          ActivateSolenoid(0, 21);}}}                 // activate left magnet
    return(0);                                        // all switches are reported to PinMame
  case SwitchRelCommand:                              // deactivated switches
    if (LordModeTimer && Command > 12 && Command < 17) {  // LORD lane change mode active and one of the LORD switches triggered?
      Command = Command - LampMov;                    // apply lane change moves to switch number
      if (Command < 13) {
        Command = Command + 4;}
      byte i = 0;                                     // add switch number to list of changed switches
      while (USB_ChangedSwitches[i] && (i<63)) {
        i++;}
      USB_ChangedSwitches[i] = Command;               // send manipulated switch number to PinMame
      return(1);}
    if (Command == 49){                               // right magnet button
      ReleaseSolenoid(22);}                           // turn off right magnet
    else if (Command == 50) {                         // left magnet button
      ReleaseSolenoid(21);}                           // turn off left magnet
    return(0);                                        // all switches are reported to PinMame
  case SolenoidActCommand:                            // activated solenoids
    if (Command == 7) {                               // lower eject hole
      ActivateSolenoid(15, 7);                        // activate with less strength
      return(1);}                                     // ignore PinMame's on command
    if (Command == EX_EjectSolenoid){                 // ball eject coil
      if (QueryLamp(2)) {                             // ball in play lamp lit?
        EX_BallRelease(1);}}                          // start ball release timer
    return(0);                                        // solenoid will be activated
  case LampOnCommand:																	// activated lamps
    if (game_settings[USB_Option1]) {                 // Lane change option active?
      if (Command == 24 && (QueryLamp(2) || MultiballTimer)) {  // Mini playfield illumination turn on
        if (LordModeTimer) {                          // LordModeTimer already active
          KillTimer(LordModeTimer);}                  // stop it
        else {                                        // fresh start of Lord Mode
          LaneChangeDone = false;
          LampMov = 0;                                // reset lane change movements
          PMlamp = 0;                                 // read current status of LORD lamps
          for (byte i=0; i<4; i++) {
            PMlamp = PMlamp<<1;
            PMlamp = PMlamp | QueryLamp(13+i);}}
        LordModeTimer = ActivateTimer(1000, 20, EX_JL_LaneChange);}  // (re-) start timer
      if (Command == 7) {                             // detect multiball by Multi-Ball timer lamp
        if (MultiballTimer) {                         // timer already running?
          KillTimer(MultiballTimer);}                 // kill and
        MultiballTimer = ActivateTimer(1000, 21, EX_JL_LaneChange);}  // restart it
      else if (LordModeTimer) {                       // mini playfield active?
        if (Command > 12 && Command < 17) {           // apply lane change movements to lamp number
          Command = Command + LampMov;
          if (Command > 16) {
            Command = Command - 4;}
          TurnOnLamp(Command);                        // turn on the shifted lamp number
          return(1);}}}                               // ignore the original lamp command from PinMame
    return(0);                                        // apply lamp command from PinMame
  case LampOffCommand:                                // deactivated lamps
    if (LordModeTimer) {                              // mini playfield active?
      if (Command > 12 && Command < 17) {             // apply lane change movements to lamp number
        Command = Command + LampMov;
        if (Command > 16) {
          Command = Command - 4;}
        TurnOffLamp(Command);                         // turn off the shifted lamp number
        return(1);}}                                  // ignore the original lamp command from PinMame
    return(0);                                        // report lamp command to PinMame
  case 20:                                            // LordModeTimer has run out
    if (QueryLamp(24)) {                              // mini playfield illumination lamp
      LordModeTimer = ActivateTimer(1000, 20, EX_JL_LaneChange);}  // (re-) start timer
    else {                                            // mini playfield not active any more
      LordModeTimer = 0;                              // end lane change mode
      if (PMlamp == 15) {                             // all lamps set?
        PMlamp = 0;}                                  // turn them off
      for (byte i=0; i<4; i++) {                      // restore lamps according to PinMame lamp state
        if (PMlamp & 1) {
          TurnOnLamp(16-i);}
        else {
          TurnOffLamp(16-i);}
        PMlamp = PMlamp>>1;}}
    return(0);
  case 21:
    MultiballTimer = 0;                               // Multiball-Timer lamp is not flashing any more
    return(0);
  default:
    return(0);}}                                      // no exception rule found for this type so proceed as normal

byte EX_Pharaoh(byte Type, byte Command){             // thanks to Grangeomatic for sending me this code
  static byte SoundSeries;                            // buffer to handle pre system11 sound series
  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
    if (Command == 127) { }                           // ignore sound command 0x7f - audio bus init - not relevant for APC sound
    else if (Command == 37) {                         // sound command 0x25 - random speech
      char FileName[13] = "0_25_000.snd";             // generate base filename
      FileName[7] = 48 + random(4) + 1;               // change the counter according to random number
      PlaySound(52, (char*) FileName);}               // play the corresponding sound file
    else if (Command == 42) {                         // sound command 0x2a - random speech
      char FileName[13] = "0_2a_000.snd";             // generate base filename
      FileName[7] = 48 + random(2) + 1;               // change the counter according to random number
      PlaySound(52, (char*) FileName);}               // play the corresponding sound file
    else if (Command == 44) {                         // sound command 0x2c - stop sound
      AfterSound = 0;                                 // disable auto restart of BG sound
      SoundSeries = 0;                                // Reset BG sound
      StopPlayingSound();}
    else if (Command == 45) {                         // sound command 0x2d - background sound - sound series
      if (SoundSeries < 31)                           // sound series has 31 different tunes
        SoundSeries++;                                // switch to the next tune when sound command is called again
      char FileName[13] = "0_2d_000.snd";             // generate base filename
      FileName[7] = 48 + (SoundSeries % 10);          // change the 7th character of filename according to current tune
      FileName[6] = 48 + (SoundSeries % 100) / 10;    // the same with the 6th character
      for (byte i=0; i<12; i++) {                     // store filename to be repeated
        USB_RepeatSound[i] = FileName[i];}
      QueueNextSound(USB_RepeatSound);                // set this filename to be started by PlayNextSound
      PlaySound(51, (char*) FileName);}
    else if (Command == 48) {                         // sound command 0x30 - random speech
      char FileName[13] = "0_30_000.snd";             // generate base filename
      FileName[7] = 48 + random(2) + 1;               // change the counter according to random number
      PlaySound(52, (char*) FileName);}               // play the corresponding sound file
    else if (Command == 49) {                         // sound command 0x31 - random speech
      char FileName[13] = "0_31_000.snd";             // generate base filename
      FileName[7] = 48 + random(2) + 1;               // change the counter according to random number
      PlaySound(52, (char*) FileName);}               // play the corresponding sound file
    else if (Command == 55) {                         // sound command 0x37 - random speech
      char FileName[13] = "0_37_000.snd";             // generate base filename
      FileName[7] = 48 + random(2) + 1;               // change the counter according to random number
      PlaySound(52, (char*) FileName);}               // play the corresponding sound file
    else if (Command == 58) {                         // sound command 0x3a - random speech
      char FileName[13] = "0_3a_000.snd";             // generate base filename
      FileName[7] = 48 + random(4) + 1;               // change the counter according to random number
      PlaySound(52, (char*) FileName);}               // play the corresponding sound file
    else {                                            // standard sound
      char FileName[9] = "0_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        PlaySound(51, (char*) FileName);}}
    return(0);                                        // return number not relevant for sounds
  default:
    return(0);}}                                      // no exception rule found for this type so proceed as normal

byte EX_Barracora(byte Type, byte Command){
  static byte SoundSeries[2];                         // buffer to handle pre system11 sound series
  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
    if (Command == 127) { }                           // ignore sound command 0x7f - audio bus init - not relevant for APC sound
    else if (Command == 44) {                         // sound command 0x2c - stop sound
      AfterSound = 0;
      SoundSeries[0] = 0;                             // Reset BG sound
      SoundSeries[1] = 0;                             // reset the multiball start sound
      StopPlayingSound();}
    else if (Command == 45){                          // sound command 0x2d - sound series
      if (SoundSeries[0] < 31) {                      // this sound has 31 tunes
        SoundSeries[0]++;}                            // every call of this sound proceeds with next tune
      char FileName[13] = "0_2d_000.snd";             // generate base filename
      FileName[7] = 48 + (SoundSeries[0] % 10);       // change the 7th character of filename according to current tune
      FileName[6] = 48 + (SoundSeries[0] % 100) / 10; // the same with the 6th character
      for (byte i=0; i<12; i++) {                     // prepare the filename
        USB_RepeatSound[i] = FileName[i];}
      QueueNextSound(USB_RepeatSound);                // sound is being auto repeated
      PlaySound(51, (char*) FileName);}               // play the sound
    else if (Command == 46) {                         // sound command 0x2e - background sound - sound series
      SoundSeries[0] = 0;
      if (SoundSeries[1] < 29) {                      // this sound has 29 tunes
        SoundSeries[1]++;}
      char FileName[13] = "0_2e_000.snd";
      FileName[7] = 48 + (SoundSeries[1] % 10);
      FileName[6] = 48 + (SoundSeries[1] % 100) / 10;
      for (byte i=0; i<12; i++) {                     // prepare the filename
        USB_RepeatSound[i] = FileName[i];}
      QueueNextSound(USB_RepeatSound);                // sound is being auto repeated
      PlaySound(51, (char*) FileName);}
    else {                                            // standard sound
      char FileName[9] = "0_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        PlaySound(51, (char*) FileName);}}
    return(0);                                        // return number not relevant for sounds
  case SwitchActCommand:                              // activated switches
    if (Command == 16) {                              // ball successfully ejected
      EX_BallRelease(0);}                             // stop ball release timer
    return(0);                                        // all switches are reported to PinMame
  case SolenoidActCommand:                            // activated solenoids
    if (Command == EX_EjectSolenoid){                 // ball eject coil
      if (QueryLamp(2)) {                             // ball in play lamp lit?
        EX_BallRelease(1);}}                          // start ball release timer
    return(0);                                        // solenoid will be activated
  default:
    return(0);}}                                      // no exception rule found for this type so proceed as normal

byte EX_BlackKnight(byte Type, byte Command){
  static byte SoundSeries[3];                         // buffer to handle pre system11 sound series
  static byte LastCh1Sound;                           // preSys11: stores the number of the last sound that has been played on Ch1
  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
    if (Command == 127) { }                           // ignore sound command 0x7f - audio bus init - not relevant for APC sound
    else if (Command == 48) {                         // sound command 0x30
      if (QuerySolenoid(11)) {                        // GI off?
        PlaySound(152, "0_30_001.snd");}}             // play multiball ball release sequence
    else if (Command == 56) {                         // sound command 0x38
      if (QuerySolenoid(11)) {                        // GI off?
        if (LastCh1Sound != 56) {                     // ignore all subsequent 0x38 commands
          AfterSound = 0;
          LastCh1Sound = Command;                     // buffer sound number
          PlaySound(51, "0_38_001.snd");}}}           // play multiball start sequence
    else if (Command == 43) {                         // sound command 0x2b - start game
      char FileName[13] = "0_2b_000.snd";             // generate base filename
      FileName[7] = 48 + random(5) + 1;               // change the counter according to random number
      PlaySound(52, (char*) FileName);}               // play the corresponding sound file
    else if (Command == 45) {                         // sound command 0x2d - activated spinner - sound series
      if (SoundSeries[0] != 45) {
        SoundSeries[0] = 45;
        SoundSeries[1] = 0;}
      SoundSeries[1]++;
      char FileName[13] = "0_2d_000.snd";
      FileName[7] = 48 + (SoundSeries[1] % 10);
      FileName[6] = 48 + (SoundSeries[1] % 100) / 10;
      FileName[5] = 48 + SoundSeries[1] / 100;
      LastCh1Sound = Command;                         // buffer sound number
      PlaySound(51, (char*) FileName);}
    else if (Command == 44) {                         // sound command 0x2c - stop sound
      AfterSound = 0;
      SoundSeries[0] = 0;                             // Reset last sound series number
      SoundSeries[1] = 0;                             // reset the multiball start sound
      SoundSeries[2] = 0;                             // Reset BG sound
      StopPlayingSound();}
    else if (Command == 46) {                         // sound command 0x2e - background sound - sound series
      SoundSeries[0] = 0;
      if (SoundSeries[2] < 29)
        SoundSeries[2]++;
      char FileName[13] = "0_2e_000.snd";
      FileName[7] = 48 + (SoundSeries[2] % 10);
      FileName[6] = 48 + (SoundSeries[2] % 100) / 10;
      FileName[5] = 48 + SoundSeries[2] / 100;
      for (byte i=0; i<12; i++) {
        USB_RepeatSound[i] = FileName[i];}
      QueueNextSound(USB_RepeatSound);
      LastCh1Sound = Command;                         // buffer sound number
      PlaySound(51, (char*) FileName);}
    else if (Command == 52) {                         // sound command 0x34 - bonus count
      AfterSound = 0;
      if (!QueryLamp(49) && !QueryLamp(57) && !QueryLamp(61)) { // only bonus lamp 1 lit?
        PlaySound(51, "0_34_002.snd");}
      else if (LastCh1Sound != 52) {
        LastCh1Sound = Command;                       // buffer sound number
        SoundSeries[2] = 0;                           // Reset BG sound
        PlaySound(51, "0_34_001.snd");}}
    else if (Command == 58) {                         // sound command 0x3a
      PlaySound(152, "0_3a.snd");}                    // play multiball ball release sequence
    else {                                            // standard sound
      LastCh1Sound = Command;                         // buffer sound number
      char FileName[9] = "0_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        PlaySound(51, (char*) FileName);}}
    return(0);
  case SwitchActCommand:                              // activated switches
    if (Command == 45) {                              // ball successfully ejected
      EX_BallRelease(0);}                             // stop ball release timer
    else if (Command == 9) {                          // right magnet button
      if (QueryLamp(9) && QueryLamp(2)) {             // right magnet and ball in play lamp lit?
        ActivateSolenoid(0, 9);}}                     // activate right magnet
    else if (Command == 10) {                         // left magnet button
      if (QueryLamp(10) && QueryLamp(2)) {            // left magnet and ball in play lamp lit?
        ActivateSolenoid(0, 10);}}                    // activate left magnet
    return(0);                                        // all switches are reported to PinMame
  case SolenoidActCommand:                            // activated solenoids
    if (Command == EX_EjectSolenoid){                 // ball eject coil
      if (QueryLamp(2)) {                             // ball in play lamp lit?
        EX_BallRelease(1);}}                          // start ball release timer
    return(0);                                        // return number not relevant for sounds
  default:
    return(0);}}

void EX_AttractLEDeffects(byte State) {               // call with State = 1, stop with State = 0
  static byte Timer = 0;
  static byte Red = 0;
  static byte Green = 0;
  static byte Blue = 0;
  switch (State) {
  case 0:                                             // terminate
    if (Timer) {
      KillTimer(Timer);
      Timer = 0;}
    break;
  case 1:                                             // initialize
    LEDinit();
    LEDsetColorMode(2);                               // to immediately apply the selected color to the GI
    LEDpattern = PME_GIallOn;                         // and turn on GI
    Timer = ActivateTimer(20, 2, EX_AttractLEDeffects);
    break;
  case 2:                                             // increase green
    LEDsetColor(Green*10, Red*10, Blue*10);           // set GI color
    if (Green < 25) {
      Green++;
      Timer = ActivateTimer(20, 2, EX_AttractLEDeffects);}
    else {
      Timer = ActivateTimer(20, 3, EX_AttractLEDeffects);}
    break;
  case 3:                                             // decrease red
    LEDsetColor(Green*10, Red*10, Blue*10);           // set GI color
    if (Red) {
      Red--;
      Timer = ActivateTimer(20, 3, EX_AttractLEDeffects);}
    else {
      Timer = ActivateTimer(20, 4, EX_AttractLEDeffects);}
    break;
  case 4:                                             // increase blue
    LEDsetColor(Green*10, Red*10, Blue*10);           // set GI color
    if (Blue < 25) {
      Blue++;
      Timer = ActivateTimer(20, 4, EX_AttractLEDeffects);}
    else {
      Timer = ActivateTimer(20, 5, EX_AttractLEDeffects);}
    break;
  case 5:                                             // decrease green
    LEDsetColor(Green*10, Red*10, Blue*10);           // set GI color
    if (Green) {
      Green--;
      Timer = ActivateTimer(20, 5, EX_AttractLEDeffects);}
    else {
      Timer = ActivateTimer(20, 6, EX_AttractLEDeffects);}
    break;
  case 6:                                             // increase red
    LEDsetColor(Green*10, Red*10, Blue*10);           // set GI color
    if (Red < 25) {
      Red++;
      Timer = ActivateTimer(20, 6, EX_AttractLEDeffects);}
    else {
      Timer = ActivateTimer(20, 7, EX_AttractLEDeffects);}
    break;
  case 7:                                             // decrease blue
    LEDsetColor(Green*10, Red*10, Blue*10);           // set GI color
    if (Blue) {
      Blue--;
      Timer = ActivateTimer(20, 7, EX_AttractLEDeffects);}
    else {
      Timer = ActivateTimer(20, 2, EX_AttractLEDeffects);}
    break;}}

void EX_AttractLEDeffects2(byte State) {              // call with State = 1
  switch(State) {
  case 0:                                             // after the colors have been set
    LEDreturn = 0;
    LEDsetColorMode(4);                               // freeze the LEDs
    LEDhandling(6, 102);                              // write 102 to the command buffer
    LEDhandling(7, 1);
    break;
  case 1:
    LEDsetColorMode(2);
    LEDpointer = GI_Pattern;
    LEDpatDuration = GI_Duration;
    LEDreturn = EX_AttractLEDeffects2;                // come back after the colors are set
    ShowLEDpatterns(1);
    break;
  case 2:                                             // back to GI color
    LEDsetColor(game_settings[LED_green], game_settings[LED_red], game_settings[LED_blue]); // set GI color
    LEDsetColorMode(2);
    break;}}

byte EX_Comet(byte Type, byte Command) {
  static byte LastSwitch;                             // stores the number of the last activated switch
  static byte BlindPinmame;                           // hide switches from PinMame while active
  static bool BlockDisplay;                           // to block PinMame's messages while the custom text is shown
  static byte Timer = 0;
  switch(Type) {
  case SoundCommandCh1:                               // sound commands for channel 1
    IntBuffer = 0;
    if (!Command || Command > 254) {                  // sound command 0x00 and 0xff -> stop sound
      AfterMusic = 0;
      StopPlayingMusic();
      StopPlayingSound();}
    else if (Command == 11 || Command == 254) { }     // ignore sound commands 0x0b and 0xfe
    else if (Command == 47) {                         // play BG music
      if (game_settings[USB_BGmusic]) {
        PlayMusic(150, "MUSIC.BIN");                  // play special music
        QueueNextMusic("MUSIC.BIN");}
      else {
        PlayMusic(150, "0_2f.snd");                   // play default music
        QueueNextMusic("0_2f.snd");}}                 // track is looping so queue it also
    else {                                            // handle standard sound
      if (Command == 9) {
        MusicVolume = 4;}                             // reduce music volume
      if (Command == 241) {
        RestoreMusicVolumeAfterSound(25);}            // and restore it
      char FileName[9] = "0_00.snd";
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        PlaySound(51, (char*) FileName);}}
    return(0);                                        // no exception rule found for this type so proceed as normal
  case SwitchActCommand:                              // activated switches
    if (Command == 3) {                               // credit switch
      EX_AttractLEDeffects(0);                        // and stop GI animations
      BlockDisplay = false;                           // don't show the custom text
      if (game_settings[USB_CustomText]) {
        WriteUpper("              ");
        WriteLower("              ");}
      LEDhandling(6, 103);                            // write 103 to stop the GI animation
      LEDhandling(7, 1);
      LEDsetColorMode(0);                             // turn off unwanted LEDs
      LEDpattern = PME_GIon;                          // apply GI pattern
      ActivateTimer(40, 2, EX_AttractLEDeffects2);}   // wait a cycle to apply GI color
    if (BlindPinmame) {                               // hide switches from PinMame
      if (Command == 31 || Command == 32 || Command == 44) {
        return(1);}                                   // hide these switches from PinMame
      if (Command == 45) {                            // outhole switch?
        if (Timer) {                                  // timer still running?
          KillTimer(Timer);                           // stop it
          Timer = 0;}
        ActivateSolenoid(40, 1);                      // kick ball into plunger lane
        RemoveBlinkLamp(60);                          // stop blinking the extra ball lamps
        RemoveBlinkLamp(5);
        if (BlindPinmame > 1) {                       // extra ball lamps were on before?
          TurnOnLamp(60);
          TurnOnLamp(5);}
        BlindPinmame = 0;                             // and don't fool PinMame any longer
        return(1);}}                                  // hide this switch from PinMame
    else {                                            // normal mode
      if ((Command == 44 && (game_settings[USB_Option1] & 2)) || (Command == 31 && (game_settings[USB_Option1] & 1))) { // Ball Saver for outlanes active?
        if ((LastSwitch > 32) && (LastSwitch < 43)) { // ball dropped from the bumpers directly into the outlane?
          if (QueryLamp(60)) {                        // extra ball lamp lit?
            BlindPinmame = 2;}
          else {                                      // extra ball lamp not lit
            BlindPinmame = 1;}
          AddBlinkLamp(60, 150);                      // blink extra ball lamps
          AddBlinkLamp(5, 150);
          Timer = ActivateTimer(5000, 1, EX_Comet2);  // wait 5s for the ball to reach the outhole
          return(1);}}                                // hide this switch from PinMame
      else {                                          // switches are reported normally
        LastSwitch = Command;}}
    return(0);
  case SolenoidActCommand:
    if (Command == 11) {                              // GI relais?
      LEDsetColorMode(0);
      LEDsetColor(255, 255, 255);
      LEDpattern = PME_GIoff;}                        // turn off GI
    else if (QuerySolenoid(11)) {                     // use GI flash effects
      switch(Command) {
      case 7:
        LEDpattern = PME_GI1;
        break;
      case 8:
        LEDpattern = PME_GI2;
        break;
      case 9:
        LEDpattern = PME_GI3;
        break;
      case 10:
        LEDpattern = PME_GI4;
        break;}}
    return(0);
  case SolenoidRelCommand:
    if (Command == 11) {                              // GI relais?
      LEDsetColor(game_settings[LED_green], game_settings[LED_red], game_settings[LED_blue]); // set GI color
      LEDpattern = PME_GIon;}                         // turn on GI
    return(0);
  case LampOnCommand:
    if (Command == 1) {                               // count the blinking of the game over lamp
      IntBuffer++;                                    // increase counter
      switch(IntBuffer) {
      case 500:                                       // play a GI effect
        EX_AttractLEDeffects(1);
        if (game_settings[USB_CustomText]) {
          BlockDisplay = true;                        // take control of the displays
          WriteUpper((char*) EX_CustomText);
          WriteLower((char*) EX_CustomText + 14);}
        break;
      case 700:
        EX_AttractLEDeffects(0);                      // and stop it
        BlockDisplay = false;                         // give display control back to PinMame
        LEDhandling(6, 103);                          // write 103 to stop the GI animation
        LEDhandling(7, 1);
        LEDsetColorMode(0);                           // turn off unwanted LEDs
        LEDpattern = PME_GIon;                        // apply GI pattern
        ActivateTimer(40, 2, EX_AttractLEDeffects2);  // wait a cycle to apply GI color
        break;
      case 1100:
        EX_AttractLEDeffects2(1);                     // play another GI effect
        if (game_settings[USB_CustomText]) {
          BlockDisplay = true;                        // take control of the displays
          WriteUpper((char*) EX_CustomText);
          WriteLower((char*) EX_CustomText + 14);}
        break;
      case 1300:                                      // and stop it
        LEDhandling(6, 103);                          // write 103 to stop the GI animation
        LEDhandling(7, 1);
        LEDsetColorMode(0);                           // turn off unwanted LEDs
        LEDpattern = PME_GIon;                        // apply GI pattern
        BlockDisplay = false;                         // give display control back to PinMame
        ActivateTimer(40, 2, EX_AttractLEDeffects2);}}  // wait a cycle to apply GI color
    return(0);
  case WriteToDisplay0:
  case WriteToDisplay1:
  case WriteToDisplay2:
  case WriteToDisplay3:
  case WriteToDisplay4:
    if (BlockDisplay) {                               // displays are blocked while the custom text is shown
      return(1);}
    return(0);
  case 50:                                            // timer of ball saver has run out
    Timer = 0;
    if (BlindPinmame) {                               // ball saver still active?
      RemoveBlinkLamp(60);                            // stop blinking the extra ball lamps
      RemoveBlinkLamp(5);
      if (BlindPinmame > 1) {                         // extra ball lamps were on before?
        TurnOnLamp(60);
        TurnOnLamp(5);}
      BlindPinmame = 0;}                              // and don't fool PinMame any longer
    return(0);
  default:
    return(0);}}                                      // switch will also be reported to PinMame.

void EX_Comet2(byte Dummy) {                          // to be called by timer from EX_Comet
  UNUSED(Dummy);
  EX_Comet(50, 0);}

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

byte EX_F14Tomcat(byte Type, byte Command){           // Exceptions code for Tomcat, thanks to Snux for sending me this code
  switch(Type){
  case SwitchActCommand:                              // Check for switches
    if (Command > 64 && Command < 72) {               // If special solenoid switch
      if (QuerySolenoid(24))                          // Check if the flippers are active
        return(0);                                    // If they are active then permit the switch
      else
        return(1);}                                   // otherwise block it, standard Sys11 behaviour
    else
      return (0);
  case SoundCommandCh1:                               // sound commands for channel 1
    if (!Command){                                    // sound command 0x00 - stop sound
      AfterSound = 0;
      StopPlayingSound();}
    else if (Command == 85) { }                       // ignore sound command 0x55
    else if (Command == 23) { }                       // ignore
    else if (Command == 170) { }                      // ignore
    else if (Command == 255) { }                      // ignore sound command 0xff
    else if (Command == 206) {                        // ball waiting in shooter lane
      PlaySound(50, "0_CE.snd");                      // start sound
      QueueNextSound("0_CF.snd");}                    // loop that high pitch noise
    else {                                            // proceed with standard sound handling
      char FileName[9] = "0_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        AfterSound = 0;                               // needed this to shut the looping sound up.
        StopPlayingSound();
        if (Command < 128) {                          // play speech with a higher priority
          PlaySound(50, (char*) FileName);}
        else {
          PlaySound(51, (char*) FileName);}}}
    return(0);                                        // return number not relevant for sounds
  case SoundCommandCh2:                               // sound commands for channel 2
    if (!Command) {                                   // sound command 0x00 - stop music
      AfterMusic = 0;
      StopPlayingMusic();
      MusicVolume=0; }                                // reset music volume in case it was made quieter earlier
    else if (Command == 127) {                        // sound command 0x7f - stop sound
      AfterSound = 0;
      StopPlayingSound();}
    else if (Command > 7 && Command < 80){ }          // ignore unknown sound commands 0x1d to 0x30
    else if (Command > 84 && Command < 96) { }        // ignore unknown sound commands 0x4f to 0x59
    else if (Command == 170) { }                      // ignore unknown sound command 0xaa
    else if (Command == 123) { }                      // ignore unknown 0x7b during multiball start
    else if (Command == 255) { }                      // ignore unknown sound command 0xff
    else if (Command > 95 && Command < 112) {         // music volume command 0x6X
      MusicVolume = (Command - 96) / 4;}
    else if (Command == 1) {                          // music track 1
      PlayMusic(50, "1_01.snd");                      // play music track
      QueueNextMusic("1_01.snd");}                    // track is looping so queue it also
    else if (Command == 2) {                          // music track 2
      PlayMusic(50, "1_02.snd");                      // play non looping part of music track
      QueueNextMusic("1_02.snd");}                    // queue looping part as next music to be played
    else if (Command == 6) {                          // music track 6
      PlayMusic(50, "1_06.snd");                      // play non looping part of music track
      QueueNextMusic("1_06L.snd");}                   // queue looping part as next music to be played
    else {
      char FileName[9] = "1_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(2, Command, FileName)) { // create filename and check whether file is present
        if (Command < 8  || Command == 128) {         // play only music on the music channel except 128 which is multiball alert
          AfterMusic = 0;                             // stop looping music
          PlayMusic(50, (char*) FileName);}           // play on the music channel
        else {
          PlaySound(50, (char*) FileName);}}}         // play on the sound channel
    return(0);                                        // return number not relevant for sounds
  default:                                            // use default treatment for undefined types
    return(0);}}                                      // no exception rule found for this type so proceed as normal

byte EX_Rollergames(byte Type, byte Command){
  static byte LastMusic;
  switch(Type){
  case SoundCommandCh2:                               // sound commands for channel 1
    if (!Command){                                    // sound command 0x00 - stop sound
      AfterSound = 0;
      StopPlayingSound();
      AfterMusic = 0;
      StopPlayingMusic();}
    else if (Command == 32 || Command == 64) { }      // unknown commands
    else if (Command > 95 && Command < 100) {         // music volume command 0x6X
      MusicVolume = Command - 96;}
    else if (Command == 1) {                          // music track 1
      if (LastMusic != 1) {
        LastMusic = 1;
        PlayMusic(50, "1_01.snd");                    // play non looping part of music track 1
        QueueNextMusic("1_01L.snd");}}                // queue looping part as next music to be played
    else if (Command == 3 || Command == 65) {         // music track 3 identical to 0x41
      if (LastMusic != 3) {
        LastMusic = 3;
        PlayMusic(50, "1_03.snd");                    // play non looping part of music track
        QueueNextMusic("1_03L.snd");}}                // queue looping part as next music to be played
    else if (Command == 4) {                          // music track 0x04
      if (LastMusic != 4) {
        LastMusic = 4;
        PlayMusic(50, "1_04.snd");                    // play music track
        QueueNextMusic("1_04.snd");}}                 // track is looping so queue it also
    else if (Command == 6) {                          // music track 6 Multiball start
      if (LastMusic != 6) {
        LastMusic = 6;
        PlayMusic(50, "1_06.snd");                    // play non looping part of music track
        QueueNextMusic("1_06L.snd");}}                // queue looping part as next music to be played
    else if (Command == 7) {                          // music track 0x07
      if (LastMusic != 7) {
        LastMusic = 7;
        PlayMusic(50, "1_07.snd");                    // play music track
        QueueNextMusic("1_07.snd");}}                 // track is looping so queue it also
    else if (Command == 8) {                          // music track 8 Multiball lock
      if (LastMusic != 8) {
        LastMusic = 8;
        PlayMusic(50, "1_08.snd");                    // play non looping part of music track
        QueueNextMusic("1_08L.snd");} }               // queue looping part as next music to be played
    else if (Command == 9 || Command == 66) {         // music track 9 identical to 0x42
      if (LastMusic != 9) {
        LastMusic = 9;
        PlayMusic(50, "1_09.snd");                    // play non looping part of music track
        QueueNextMusic("1_09L.snd");}}                // queue looping part as next music to be played
    else if (Command == 11) {                         // music track 0x0b
      if (LastMusic != 11) {
        LastMusic = 11;
        PlayMusic(50, "1_0bL.snd");                   // play music track
        QueueNextMusic("1_0bL.snd");}}                // track is looping so queue it also
    else if (Command == 12) {                         // music track 0x0c
      if (LastMusic != 12) {
        LastMusic = 12;
        PlayMusic(50, "1_0c.snd");                    // play non looping part of music track
        QueueNextMusic("1_0cL.snd");}}                // queue looping part as next music to be played
    else if (Command == 13) {                         // music track 0x0d
      if (LastMusic != 13) {
        LastMusic = 13;
        PlayMusic(50, "1_0d.snd");                    // play non looping part of music track
        QueueNextMusic("1_0dL.snd");}}                // queue looping part as next music to be played
    else if (Command == 14) {                         // music track 0x0e
      if (LastMusic != 14) {
        LastMusic = 14;
        PlayMusic(50, "1_0e.snd");                    // play non looping part of music track
        QueueNextMusic("1_0eL.snd");}}                // queue looping part as next music to be played
    else if (Command == 15) {                         // music track 0x0f
      if (LastMusic != 15) {
        LastMusic = 15;
        PlayMusic(50, "1_0f.snd");                    // play music track
        QueueNextMusic("1_0bL.snd");}}                // queue looping part as next music to be played
    else if (Command == 67) {                         // music track 0x43
      if (LastMusic != 67) {
        LastMusic = 67;
        PlayMusic(50, "1_43.snd");                    // play non looping part of music track
        QueueNextMusic("1_43L.snd");}}                // queue looping part as next music to be played
    else if (Command == 68) {                         // music track 0x44
      if (LastMusic != 68) {
        LastMusic = 68;
        PlayMusic(50, "1_44.snd");                    // play non looping part of music track
        QueueNextMusic("1_43L.snd");}}                // queue looping part as next music to be played
    else if (Command == 69) {                         // music track 0x45
      if (LastMusic != 69) {
        LastMusic = 69;
        PlayMusic(50, "1_45.snd");                    // play non looping part of music track
        QueueNextMusic("1_09L.snd");}}                // queue looping part as next music to be played
    else if (Command == 75) {                         // music track 0x4b
      if (LastMusic != 75) {
        LastMusic = 75;
        PlayMusic(50, "1_4b.snd");                    // play non looping part of music track
        QueueNextMusic("1_08L.snd");}}                // queue looping part as next music to be played
    else if (Command == 76) {                         // music track 0x4c
      if (LastMusic != 76) {
        LastMusic = 76;
        PlayMusic(50, "1_0dL.snd");                   // play non looping part of music track
        QueueNextMusic("1_0dL.snd");}}                // queue looping part as next music to be played
    else {                                            // standard sound
      if (Command == 78) {                            // 0x4e ends music track
        StopPlayingMusic();}
      char FileName[9] = "1_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(2, Command, FileName)) { // create filename and check whether file is present
        PlaySound(51, (char*) FileName);}}
    return(0);
  default:                                            // use default treatment for undefined types
    return(0);}}

byte EX_BlockAll(byte Type, byte Command) {
  UNUSED(Type);
  UNUSED(Command);
  return(1);}

byte EX_Blank(byte Type, byte Command){               // use this as a template and an example of how to add your own exceptions
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
  case SwitchActCommand:                              // activated switches
    if (Command == 43) {                              // handle the activation of switch 43
      // enter your special handling for switch 43 here
    }
    return(0);                                        // switch will also be reported to PinMame. Use return(1) to hide the activation from PinMame
  case SwitchRelCommand:                              // released switches
    if (Command == 43){                               // handle the deactivation of switch 43
      // enter your special handling for switch 43 here
    }
    return(0);                                        // switch will also be reported to PinMame. Use return(1) to hide the deactivation from PinMame
  case SolenoidActCommand:                            // activate solenoids
    if (Command == 2){                                // handle the activation of solenoid 2
      // enter your special handling for activating solenoid 2 here
    }
    return(0);                                        // solenoid will be activated. Use return(1) to suppress this
  case SolenoidRelCommand:                            // deactivate solenoids
    if (Command == 2){                                // handle the deactivation of solenoid 2
      // enter your special handling for deactivating solenoid 2 here
    }
    return(0);                                        // solenoid will be deactivated. Use return(1) to suppress this
  case LampOnCommand:                                 // turn on lamp
    if (Command == 2){                                // handle the turn on of lamp 2
      // enter your special handling for activating lamp 2 here
    }
    return(0);                                        // lamp will be turn on. Use return(1) to suppress this
  case LampOffCommand:                                // turn off lamp
    if (Command == 2){                                // handle the turn off of lamp 2
      // enter your special handling for turning off lamp 2 here
    }
    return(0);                                        // lamp will be turn off. Use return(1) to suppress this
  case WriteToDisplay0:                               // write to display 0 (Credit)
    if (USB_SerialBuffer[0] == 1) {                   // if the first digit is zero
      // enter your special handling for display 0 here
    }
    return(0);                                        // display content will be written. Use return(1) to suppress this
  default:                                            // use default treatment for undefined types
    return(0);}}

void EX_Init(byte GameNumber) {
  switch(GameNumber) {
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
  case 25:                                            // Barracora
    EX_EjectSolenoid = 12;                            // specify eject coil for improved ball release
    PinMameException = EX_Barracora;                  // use exception rules for Barracora
    break;
  case 34:                                            // Black Knight
    EX_EjectSolenoid = 6;                             // specify eject coil for improved ball release
    PinMameException = EX_BlackKnight;                // use exception rules for Jungle Lord
    break;
  case 39:                                            // Comet
    if (APC_settings[LEDsetting]) {                   // LEDs used?
      LEDsetColor(game_settings[LED_green], game_settings[LED_red], game_settings[LED_blue]); // set GI color
      LEDpattern = PME_GIon;}                         // and turn on GI
    if (SDfound) {                                    // SD card found?
      File CustomTxt = SD.open("Custom.txt");
      if (CustomTxt) {                                // custom text found?
        CustomTxt.read(&EX_CustomText, 28);}          // read text
      CustomTxt.close();}
    PinMameException = EX_Comet;                      // use exception rules for Comet
    break;
  case 43:                                            // Pinbot
    PinMameException = EX_Pinbot;                     // use exception rules for Pinbot
    break;
  case 44:                                            // F-14 Tomcat
    PinMameException = EX_F14Tomcat;                  // use exception rules for Tomcat
    break;
  case 67:                                            // Rollergames
    PinMameException = EX_Rollergames;                // use exception rules for Rollergames
    break;
  case 71:                                            // Riverboat Gambler
    PinMameException = EX_FakeSwitches;               // use exception rules for Rollergames
    break;
  default:
    PinMameException = EX_DummyProcess;}}

