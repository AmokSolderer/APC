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

#define USB_BallSave 5                                // optional ball saver
#define USB_BallSaveTime 6                            // activation time for the optional ball saver
#define USB_BGmusic 7                                 // to select an own BG music

#define SwOuthole 0                                   // number of the outhole switch
#define SwTrunk1 1                                    // number of the 1st trunk switch (active if one ball is in the trunk) - set to 0 if machine has only one ball (-> no trunk)
#define SwTrunk2 2                                    // number of the 2nd trunk switch (active if a second ball is in the trunk)
#define SwTrunk3 3                                    // number of the 3rd trunk switch (active if a third ball is in the trunk) - set to 0 if machine has only two balls
#define SwTrunk4 4                                    // number of the 4th trunk switch (active if a fourth ball is in the trunk) - set to 0 if machine has only three balls
#define SwLeftOutlane 5                               // number of the left outlane switch
#define SwRightOutlane 6                              // number of the right outlane switch
#define SolACrelay 7                                  // number of the AC relay solenoid (set to 0 if machine has no AC relay)
#define SolLeftKickback 8                             // number of the left kickback solenoid (set to 0 if machine has no left kickback)
#define SolRightKickback 9                            // number of the right kickback solenoid (set to 0 if machine has no right kickback)
#define SolOuthole 10                                 // number of the outhole kicker solenoid
#define SolShooterLn 11                               // number of the shooter lane feeder solenoid (set to 0 if machine has no shooter lane feeder (only one ball)
#define LampExBall 12                                 // number of the extra ball lamp (on the playfield) which is supposed to blink when ball saver is active

const byte EX_SpaceStationProperties[13] = {          // machine properties for ball saver
    10,                                               // number of the outhole switch
    11,                                               // number of the 1st trunk switch (active if one ball is in the trunk) - set to 0 if machine has only one ball (-> no trunk)
    12,                                               // number of the 2nd trunk switch (active if a second ball is in the trunk)
    13,                                               // number of the 3rd trunk switch (active if a third ball is in the trunk) - set to 0 if machine has only two balls
    0,                                                // number of the 4th trunk switch (active if a fourth ball is in the trunk) - set to 0 if machine has only three balls
    17,                                               // number of the left outlane switch
    32,                                               // number of the right outlane switch
    12,                                               // number of the AC relay solenoid (set to 0 if machine has no AC relay)
    13,                                               // number of the left kickback solenoid (set to 0 if machine has no left kickback)
    0,                                                // number of the right kickback solenoid (set to 0 if machine has no right kickback)
    1,                                                // number of the outhole kicker solenoid
    2,                                                // number of the shooter lane feeder solenoid (set to 0 if machine has no shooter lane feeder (only one ball)
    42};                                              // number of the extra ball lamp (on the playfield) which is supposed to blink when ball saver is active

const byte EX_AlienPokerProperties[13] = {
    9, 0, 0, 0, 0, 11, 32, 0, 0, 0, 1, 0, 1};

const byte EX_BarracoraProperties[13] = {
    12, 15, 14, 13, 0, 23, 24, 0, 0, 0, 11, 12, 26};

const byte EX_BlackKnightProperties[13] = {
    20, 17, 18, 19, 0, 11, 12, 0, 0, 0, 1, 6, 47};

const byte EX_CometProperties[13] = {
    45, 0, 0, 0, 0, 32, 31, 0, 0, 0, 1, 0, 60};

const byte EX_DiscoFeverProperties[13] = {
    22, 0, 0, 0, 0, 25, 19, 0, 0, 0, 1, 0, 33};

const byte EX_FirepowerProperties[13] = {
    9, 57, 58, 51, 0, 10, 44, 0, 7, 0, 1, 8, 1};

const byte EX_FlashProperties[13] = {
    48, 0, 0, 0, 0, 43, 46, 0, 0, 0, 1, 0, 16};

const byte EX_F14Properties[13] = {
    10, 11, 12, 13, 14, 61, 62, 14, 13, 0, 1, 2, 16};

const byte EX_HighSpeedProperties[13] = {
    9, 12, 11, 10, 0, 31, 32, 0, 14, 0, 1, 2, 3};

const byte EX_JungleLordProperties[13] = {
    42, 43, 9, 10, 0, 22, 23, 0, 0, 0, 1, 2, 27};

const byte EX_PharaohProperties[13] = {
    36, 39, 38, 0, 0, 15, 16, 0, 0, 0, 1, 2, 8};

const byte EX_PinbotProperties[13] = {
    16, 17, 18, 0, 0, 12, 15, 14, 0, 0, 1, 2, 33};

const byte EX_RollerGamesProperties[13] = {
    9, 13, 12, 11, 0, 40, 19, 12, 20, 0, 1, 2, 21};

const byte EX_TimeWarpProperties[13] = {
    9, 0, 0, 0, 0, 40, 39, 0, 0, 0, 1, 0, 38};

const byte *EX_Machine;                               // machine specific settings (optional)
byte USB_SerialBuffer[128];                           // received command arguments
char USB_RepeatSound[13];                             // name of the sound file to be repeated
byte EX_EjectSolenoid;                                // eject coil for improved ball release

byte EX_CountBallsInTrunk() {
  byte Count = 0;
  for (byte i=0; i<4; i++) {
    if (EX_Machine[SwTrunk1+i] && QuerySwitch(EX_Machine[SwTrunk1+i])) {
      Count++;}}
  return(Count);}

byte EX_BallSaver(byte Type, byte Command){
  static byte EX_IgnoreOuthole = 0;                   // to ignore the outhole switch while the ball is being kicked out
  static bool EX_BallSaveActive = 0;                  // hide switches from PinMame while active
  static byte EX_BallSaveTimer = 0;                   // Timer used by the ball saver
  static bool EX_BallSaveMonitor = false;             // monitor AC relay while ball saver is operating the coils
  static bool EX_BallSaveExBallLamp = false;          // PinMame state of the extra ball lamp
  static bool EX_BallSaveACstate;                     // store the state of the AC relay while ball saver is operating the coils
  static byte BallsInTrunk;
  switch(Type){
  case SwitchActCommand:                              // activated switches
    if (EX_BallSaveMonitor) {
      if (Command == EX_Machine[SwTrunk1] || Command == EX_Machine[SwTrunk2] || Command == EX_Machine[SwTrunk3]) { // hide trunk switches while ball saver is active
        return (1);}}
    if (EX_BallSaveActive) {                          // hide switches from PinMame
      if (Command == EX_Machine[SwLeftOutlane] || Command == EX_Machine[SwRightOutlane]) { // We hide the switches of the outlanes and trunk
        if (EX_Machine[SolLeftKickback] && Command == EX_Machine[SwLeftOutlane]) { // use the left kickback if present
          ActivateSolenoid(40, EX_Machine[SolLeftKickback]);}
        else if (EX_Machine[SolRightKickback] && Command == EX_Machine[SwRightOutlane]) { // use the right kickback if present
          ActivateSolenoid(40, EX_Machine[SolRightKickback]);}
        return(1);}                                   // hide these switches from PinMame
      else if (Command == EX_Machine[SwOuthole]) {    // Outhole switch : the ball is in the outhole.
        AppByte = 0;                                  // reset outhole kicks
        if (!EX_IgnoreOuthole) {
          if (EX_BallSaveTimer) {                     // timer still running?
            KillTimer(EX_BallSaveTimer);              // stop it
            EX_BallSaveTimer = 0;}
          EX_BallSaveMonitor = true;
          EX_IgnoreOuthole = true;                    // ignore switch bouncing when the ball is kicked out
          if (!EX_Machine[SolACrelay] || !QuerySolenoid(EX_Machine[SolACrelay])) { // AC present or not active?
            EX_BallSaveACstate = false;
            ActivateTimer(1000, 46, EX_BallSaver2);}
          else {                                      // AC needs to be switched
            EX_BallSaveACstate = true;
            if (!SolBuffer[0]) {                      // none of solenoids 1 - 8 active?
              EX_BallSaver(45, 0);}
            else {
              SolBuffer[0] = 0;                       // quick way to release
              ReleaseSolenoid(1);                     // solenoids 1 - 8
              ActivateTimer(40, 45, EX_BallSaver2);}}}
        return(1);}}                                  // hide this switch from PinMame
    return(0);                                        // report switch to PinMame
  case SwitchRelCommand:
    if (EX_BallSaveMonitor) {
      if (Command == EX_Machine[SwTrunk1] || Command == EX_Machine[SwTrunk2] || Command == EX_Machine[SwTrunk3]) { // hide trunk switches while ball saver is active
        return (1);}}
    if (EX_BallSaveActive) {                          // hide switches from PinMame
      if (Command == EX_Machine[SwLeftOutlane] || Command == EX_Machine[SwRightOutlane]) { // We hide the switches of the outlanes and trunk
        return(1);}}                                  // hide these switches from PinMame
    return(0);
  case SolenoidActCommand:
    if (((EX_Machine[SolShooterLn] && Command == EX_Machine[SolShooterLn]) || (!EX_Machine[SolShooterLn] && Command == EX_Machine[SolOuthole])) && QuerySolenoid(24)) { // Use the shooter lane coil to start ball saver. If none present use outhole coil.
      if (!EX_Machine[SolACrelay] || !QuerySolenoid(EX_Machine[SolACrelay])) {
        if (EX_BallSaveTimer) {                       // Activate the timer
          KillTimer(EX_BallSaveTimer);}
        EX_BallSaveActive = true;
        AddBlinkLamp(EX_Machine[LampExBall], 150);    // start blinking of Shoot again lamps (Shoot again on backglass, Drive again on playfield)
        EX_BallSaveTimer = ActivateTimer(game_settings[USB_BallSaveTime] * 1000, 50, EX_BallSaver2);}}
    else if (EX_Machine[SolACrelay]) {                // machine has an AC relay?
      if (Command == EX_Machine[SolACrelay] && EX_BallSaveMonitor) {  // AC relay
        EX_BallSaveACstate = true;
        return(1);}
      if (Command < 9 && EX_BallSaveACstate) {        // PinMame requesting the AC relay to be set
        return(1);}}                                  // ignore the solenoid command
    return(0);
  case SolenoidRelCommand:                            // released switches
    if(EX_Machine[SolACrelay] && Command == EX_Machine[SolACrelay] && EX_BallSaveMonitor) {
      EX_BallSaveACstate = false;
      return(1);}                                     // hide these switches from PinMame
    else if (Command == 25) {                         // disable ball saver when game is over (flipper relay turned off)
      if (EX_BallSaveTimer) {                         // timer still running?
        KillTimer(EX_BallSaveTimer);}                 // stop it
      EX_BallSaver(50, 0);}
    return(0);
  case LampOnCommand:
    if (Command == EX_Machine[LampExBall]) {
      EX_BallSaveExBallLamp = true;
      if (EX_BallSaveActive) {
        return(1);}}
    return(0);
  case LampOffCommand:
    if (Command == EX_Machine[LampExBall]) {
      EX_BallSaveExBallLamp = false;
      if (EX_BallSaveActive) {
        return(1);}}
    return(0);
  case 45:                                            // Turn Off AC relay
    ReleaseSolenoid(EX_Machine[SolACrelay]);
    ActivateTimer(1000, 46, EX_BallSaver2);
    return(1);
  case 46:
    if (QuerySwitch(EX_Machine[SwOuthole])) {         // ball still in outhole?
      if (EX_Machine[SwTrunk1]) {                     // does machine have more than one ball?
        BallsInTrunk = EX_CountBallsInTrunk();
        AppByte = 0;
        ActivateTimer(1500, 47, EX_BallSaver2);}
      else {
        ActivateTimer(200, 50, EX_BallSaver2);}
      ActivateSolenoid(40, EX_Machine[SolOuthole]);}  // kick ball into trunk (solenoid 1 outhole)
    else {                                            // ball not in outhole
      if (AppByte < 10) {                             // already tried 10 times?
        AppByte++;
        ActivateTimer(300, 46, EX_BallSaver2);}
      else {                                          // something's wrong
        if (EX_BallSaveTimer) {                       // timer still running?
          KillTimer(EX_BallSaveTimer);}               // stop it
        EX_BallSaver(50, 0);}}                        // end ball saver cycle
    return(1);
  case 47:                                            // timer after ball has been kicked to trunk run out
  {byte CountTrunk = EX_CountBallsInTrunk();
  if (CountTrunk == BallsInTrunk + 1) {               // ball in the trunk?
    ActivateSolenoid(30, EX_Machine[SolShooterLn]);   // kick ball into shooter lane
    if (!EX_BallSaveACstate) {                        // correct AC state?
      ActivateTimer(40, 50, EX_BallSaver2);}          // end cycle
    else {
      ActivateTimer(40, 48, EX_BallSaver2);}}         // switch on AC relay
  else {                                              // wrong number of balls in trunk
    if (AppByte) {                                    // additional waiting time has already passed
      if (CountTrunk < BallsInTrunk + 1) {            // less balls in trunk than expected
        if (QuerySwitch(EX_Machine[SwOuthole])) {     // ball still in outhole?
          AppByte = 0;
          ActivateTimer(100, 46, EX_BallSaver2);}}    // try again
      else {                                          // more balls in trunk than expected
        USB_SwitchHandler(EX_Machine[SwTrunk1 + BallsInTrunk]); // report one drained ball to PinMame
        BallsInTrunk++;
        ActivateTimer(1000, 47, EX_BallSaver2);}}     // come back to handle second ball
    else {                                            // wait and try again
      AppByte = 1;
      ActivateTimer(1000, 47, EX_BallSaver2);}}       // come back in 1s
  return(1);}
  case 48:                                            // fix state of AC relay
    if (EX_BallSaveACstate) {
      ActivateSolenoid(0, EX_Machine[SolACrelay]);}   // turn on AC relay
    ActivateTimer(40, 50, EX_BallSaver2);
    return(1);
  case 50:                                            // Stop the timer of Shoot Again after 20 seconds
    EX_BallSaveTimer = 0;
    EX_IgnoreOuthole = false;
    EX_BallSaveMonitor = false;
    EX_BallSaveACstate = false;                       // stop fumbling with the AC relay
    RemoveBlinkLamp(EX_Machine[LampExBall]);          // stop blinking the extra ball lamps
    if (EX_BallSaveExBallLamp) {                      // restore the state of the Extra Ball lamps
      TurnOnLamp(EX_Machine[LampExBall]);}            // Activate lamps Shoot Again (backglass) and Drive Again (playfield)
    else {
      TurnOffLamp(EX_Machine[LampExBall]);}
    EX_BallSaveActive = 0;
    return(1);
  default:                                            // use default treatment for undefined types
    return(0);}}                                      // no exception rule found for this type so proceed as normal

void EX_BallSaver2(byte Selector) {                   // to be called by timer from EX_BallSaver
  EX_BallSaver(Selector, 0);}

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

byte EX_DiscoFever(byte Type, byte Command){
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
  switch(Type){
  case SolenoidActCommand:
    if (Command == 2) {                               // ball release
      ActivateSolenoid(80, 2);                        // FEV drop target reset
      return(1);}
    else if (Command == 3) {                          // ER drop target reset
      ActivateSolenoid(60, 3);                        // Fix to increase the strength of the ball release
      return(1);}
    return(0);
  case SolenoidRelCommand:
    if (Command == 2 || Command == 3) {               // ignore turn-off commands for drop target solenoids
      return(1);}                                     // ignore it
    return(0);
  default:
    return(0);}}

byte EX_Flash(byte Type, byte Command){
  static byte SoundSeries[3];                         // buffer to handle pre system11 sound series
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
  switch(Type){
  case SolenoidActCommand:
    if (Command == 1) {                               // ball release
      ActivateSolenoid(40, 1);                        // Fix to increase the strength of the ball release
      return(1);}
    else if (Command == 3) {                          // reset of the lower two of the five drop targets
      ActivateSolenoid(40, 3);                        // Fix to increase the strength of the ball release
      return(1);}
    return(0);
  case SolenoidRelCommand:
    if (Command == 1 || Command == 3) {               // ball release or the reset of the lower two of the five drop targets
      return(1);}                                     // ignore it
    return(0);
  case SoundCommandCh1:                               // sound commands for channel 1
    if (Command == 31) { }                            // ignore sound command 0x1f - audio bus init - not relevant for APC sound / also ignore 0xff whatever it is
    else if (Command == 12) {                         // sound command 0x0c - stop sound
      AfterSound = 0;
      SoundSeries[0] = 0;
      SoundSeries[1] = 0;                             // reset the multiball start sound
      SoundSeries[2] = 0;                             // Reset BG sound
      StopPlayingSound();
      StopPlayingMusic();}
    else if (Command == 10){                          // sound command 0x0a - sound series
      if (SoundSeries[0] < 80) {                      // this sound has 80 pitches
        SoundSeries[0]++;}                            // every call of this sound proceeds with next pitch
      char FileName[13] = "0_0a_000.snd";             // generate base filename
      FileName[7] = 48 + (SoundSeries[0] % 10);       // change the 7th character of filename according to current pitch
      FileName[6] = 48 + (SoundSeries[0] % 100) / 10; // the same with the 6th character
      PlaySound(51, (char*) FileName);}               // play the sound
    else if (Command == 11) {                         // sound command 0x0b - sound series
      if (SoundSeries[1] < 200) {                     // this sound has 200 pitches
        SoundSeries[1]++;}
      char FileName[13] = "0_0b_000.snd";
      FileName[7] = 48 + (SoundSeries[1] % 10);
      FileName[6] = 48 + (SoundSeries[1] % 100) / 10;
      FileName[5] = 48 + (SoundSeries[1] / 100);      // the same with the 5th character
      PlaySound(51, (char*) FileName);}
    else if (Command == 13) {                         // sound command 0x0d - background sound - sound series
      SoundSeries[0] = 0;
      if (game_settings[USB_BGmusic]) {               // use MUSIC.SND instead of BG sound
        if (!SoundSeries[2]) {                        // ignore the pitches
          PlayMusic(50, "MUSIC.snd");                 // play music track
          QueueNextMusic("MUSIC.snd");                // and loop it
          SoundSeries[2] = 1;}}
      else {
        if (SoundSeries[2] < 25) {                    // this sound has 25 pitches
          SoundSeries[2]++;}
        char FileName[13] = "0_0e_000.snd";
        FileName[7] = 48 + (SoundSeries[2] % 10);
        FileName[6] = 48 + (SoundSeries[2] % 100) / 10;
        for (byte i=0; i<12; i++) {                   // prepare the filename
          USB_RepeatSound[i] = FileName[i];}
        QueueNextSound(USB_RepeatSound);              // select this sound to be repeated
        PlaySound(51, (char*) FileName);}}
    else if (Command == 14) {                         // sound command 0x0e - background sound - sound series
      SoundSeries[2] = 0;
      char FileName[13] = "0_0e_001.snd";
      for (byte i=0; i<12; i++) {                     // prepare the filename
        USB_RepeatSound[i] = FileName[i];}
      QueueNextSound(USB_RepeatSound);                // select this sound to be repeated
      PlaySound(51, (char*) FileName);}
    else if (Command == 15) {                         // sound command 0x0f - start game
      char FileName[13] = "0_0f_000.snd";             // generate base filename
      FileName[7] = 48 + random(8) + 1;               // change the counter according to random number
      PlaySound(51, (char*) FileName);}               // play the corresponding sound file
    else {                                            // standard sound
      char FileName[9] = "0_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        PlaySound(51, (char*) FileName);}}
    return(0);                                        // return number not relevant for sounds
  default:
    return(0);}}

byte EX_TimeWarp(byte Type, byte Command){
  static byte SoundSeries;                            // buffer to handle pre system11 sound series
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
    if (Command == 31) { }                            // ignore sound command 0x1f - audio bus init - not relevant for APC sound / also ignore 0xff whatever it is
    else if (Command == 12) {                         // sound command 0x0c - stop sound
      AfterSound = 0;
      SoundSeries = 0;                                // Reset BG sound
      StopPlayingMusic();
      StopPlayingSound();}
    else if (Command == 13) {                         // sound command 0x0d - increase pitch of background sound
      if (!game_settings[USB_BGmusic]) {              // use MUSIC.SND instead of BG sound
        if (SoundSeries < 25 )                        // this sound has 31 pitches
          SoundSeries++;                              // every call of this sound proceeds with next pitch
        char FileName[13] = "0_0e_000.snd";
        FileName[7] = 48 + (SoundSeries % 10);        // change the 7th character of filename according to current pitch
        FileName[6] = 48 + (SoundSeries % 100) / 10;  // the same with the 6th character
        for (byte i=0; i<12; i++) {                   // prepare the filename
          USB_RepeatSound[i] = FileName[i];}
        QueueNextSound(USB_RepeatSound);              // select this sound to be repeated
        PlaySound(51, (char*) FileName);}}
    else if (Command == 14) {                         // sound command 0x0e - background sound - sound series
      if (game_settings[USB_BGmusic]) {               // use MUSIC.SND instead of BG sound
        PlayMusic(50, "MUSIC.snd");                   // play music track
        QueueNextMusic("MUSIC.snd");}                 // and loop it
      else {
        SoundSeries = 1;
        PlaySound(51, "0_0e_001.snd");                // play BG sound
        QueueNextSound("0_0e_001.snd");}}             // select this sound to be repeated
    else {                                            // standard sound
      char FileName[9] = "0_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        PlaySound(51, (char*) FileName);}}
    return(0);                                        // return number not relevant for sounds
  case SolenoidActCommand:
    if (Command == 6) {                               // the 5 bank bottom drop target reset solenoid needs to be stronger
      ActivateSolenoid(80, 6);                        // drop target reset
      return(1);}
    return(0);
  case SolenoidRelCommand:
    if (Command == 6) {                               // ignore turn-off commands for drop target solenoids
      return(1);}                                     // ignore it
    return(0);
  default:
    return(0);}}

byte EX_Firepower(byte Type, byte Command){           // thanks to Matiou for sending me this code
  static byte SoundSeries[5] = {0, 0, 0, 0, 0};       // buffer to handle pre system11 sound series
  static byte PlayingMultiballSound = 0;
  const byte PlayCombinedSoundForMultiball = 1;       // 1-> play one sound "1, 2, 3". 2-> play 3 sounds "1", "2", "3".
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
    if (Command == 31) { }                            // ignore sound command 0x1f
    else if (Command == 12) {                         // sound command 0x0c - stop all sounds and reset series
      AfterSound = 0;
      SoundSeries[0] = 0;
      SoundSeries[1] = 0;
      SoundSeries[2] = 0;
      SoundSeries[3] = 0;
      SoundSeries[4] = 0;
      StopPlayingSound();
      StopPlayingMusic();}
    else {
      if (Command == 4){                              // 0x04 End game - random speech
        char FileName[13] = "0_04_000.snd";           // generate base filename
        FileName[7] = 48 + random(9) + 1;             // change the counter according to random number
        PlaySound(52, (char*) FileName);}             // play the corresponding sound file
      else if (Command == 7){                         // 0x07 Fire one/two/three series (multiball!)
        // code for individual sounds
        if (PlayCombinedSoundForMultiball == 0) {
          PlayingMultiballSound = 1;                  // remember we're in a multiball start session
          if (SoundSeries[0] < 3)                     // this sound has 3 pitches
            SoundSeries[0]++;                         // every call of this sound proceeds with next pitch
          else                                        //
            SoundSeries[0] = 1;                       // start all over again
          char FileName[13] = "0_07_000.snd";         // generate base filename
          FileName[7] = 48 + (SoundSeries[0] % 10);   // change the 7th character of filename according to current pitch
          PlaySound(51, (char*) FileName);}           // play the sound
        else {                                        // code for combined sounds (not standard but works better)
          if (PlayingMultiballSound == 0) {
            PlayingMultiballSound = 1;
            char FileName[13] = "0_07_004.snd"; // this wav is combined version from 67_001 to 67_003
            PlaySound(51, (char*) FileName);}}}
      else if (Command == 9){                         // 0x09 Bonus
        if (SoundSeries[1] < 146)                     // this sound has 146 pitches
          SoundSeries[1]++;                           // every call of this sound proceeds with next pitch
        char FileName[13] = "0_09_000.snd";           // generate base filename
        FileName[7] = 48 + (SoundSeries[1] % 10);     // change the 7th character of filename according to current pitch
        FileName[6] = 48 + (SoundSeries[1] % 100) / 10; // the same with the 6th character
        FileName[5] = 48 + (SoundSeries[1] / 100);    // the same with the 5th character
        PlaySound(51, (char*) FileName);}             // play the sound
      else if (Command == 10) {                       // 0x0a Whirling background
        if (SoundSeries[2] < 29 )                     // this sound has 29 pitches
          SoundSeries[2]++;                           // every call of this sound proceeds with next pitch
        char FileName[13] = "0_0a_000.snd";           // generate base filename
        FileName[7] = 48 + (SoundSeries[2] % 10);     // change the 7th character of filename according to current pitch
        FileName[6] = 48 + (SoundSeries[2] % 100) / 10; // the same with the 6th character
        PlaySound(51, (char*) FileName);}             // play the sound
      else if (Command == 13) {                       // 0x0d Spinner
        if (SoundSeries[3] < 31 )                     // this sound has 31 pitches
          SoundSeries[3]++;                           // every call of this sound proceeds with next pitch
        char FileName[13] = "0_0d_000.snd";           // generate base filename
        FileName[7] = 48 + (SoundSeries[3] % 10);     // change the 7th character of filename according to current pitch
        FileName[6] = 48 + (SoundSeries[3] % 100) / 10; // the same with the 6th character
        PlaySound(51, (char*) FileName);}             // play the sound
      else if (Command == 14) {                       // 0x0e Background // repeated
        PlayingMultiballSound = 0;                    // if the background plays, we're not in a multiball start session
        if (game_settings[USB_BGmusic]) {             // use MUSIC.SND instead of BG sound
          if (!SoundSeries[4]) {                      // don't restart if next pitch is requested
            PlayMusic(50, "MUSIC.snd");               // play music track
            QueueNextMusic("MUSIC.snd");              // and loop it
            SoundSeries[4] = 1;}}
        else {                                        // normal BG sound
          if (SoundSeries[4] < 31 )                   // this sound has 31 pitches
            SoundSeries[4]++;                         // every call of this sound proceeds with next pitch
          char FileName[13] = "0_0e_000.snd";         // generate base filename
          FileName[7] = 48 + (SoundSeries[4] % 10);   // change the 7th character of filename according to current pitch
          FileName[6] = 48 + (SoundSeries[4] % 100) / 10; // the same with the 6th character
          for (byte i=0; i<12; i++) {                 // store the name of this sound
            USB_RepeatSound[i] = FileName[i];}
          QueueNextSound(USB_RepeatSound);            // select this sound to be repeated
          PlaySound(51, (char*) FileName);}}          // play the sound
      else if ((Command == 8 || Command == 11)        // ignore these sounds at beginning of multiball
          && PlayingMultiballSound == 1) { }
      else {                                          // standard sound
        char FileName[9] = "0_00.snd";                // handle standard sound
        if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
          PlaySound(51, (char*) FileName);}}}
    return(0);                                        // return number not relevant for sounds
  default:
    return(0);}}                                      // no exception rule found for this type so proceed as normal

byte EX_AlienPoker(byte Type, byte Command){
  static byte SoundSeries[3] = {0, 0, 0};             // buffer to handle pre system11 sound series
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
    if (Command == 31) { }                            // ignore sound command 0x1f
    else if (Command == 12) {                         // sound command 0x0c - stop all sounds and reset series
      AfterSound = 0;
      SoundSeries[0] = 0;
      SoundSeries[1] = 0;
      SoundSeries[2] = 0;
      StopPlayingSound();
      StopPlayingMusic();}
    else if (Command == 10) {                         // sound series
      if (SoundSeries[0] < 29 )                       // this sound has 29 pitches
        SoundSeries[0]++;                             // every call of this sound proceeds with next pitch
      char FileName[13] = "0_0a_000.snd";             // generate base filename
      FileName[7] = 48 + (SoundSeries[0] % 10);       // change the 7th character of filename according to current pitch
      FileName[6] = 48 + (SoundSeries[0] % 100) / 10; // the same with the 6th character
      PlaySound(51, (char*) FileName);}               // play the sound
    else if (Command == 13) {                         // sound series
      if (SoundSeries[1] < 31 )                       // this sound has 31 pitches
        SoundSeries[1]++;                             // every call of this sound proceeds with next pitch
      char FileName[13] = "0_0d_000.snd";             // generate base filename
      FileName[7] = 48 + (SoundSeries[1] % 10);       // change the 7th character of filename according to current pitch
      FileName[6] = 48 + (SoundSeries[1] % 100) / 10; // the same with the 6th character
      PlaySound(51, (char*) FileName);}               // play the sound
    else if (Command == 14) {                         // 0x0e Background sound series - repeated
      SoundSeries[0] = 0;
      SoundSeries[1] = 0;
      if (game_settings[USB_BGmusic]) {               // use MUSIC.SND instead of BG sound
        if (!SoundSeries[2]) {                        // don't restart if next pitch is requested
          PlayMusic(50, "MUSIC.snd");                 // play music track
          QueueNextMusic("MUSIC.snd");                // and loop it
          SoundSeries[2] = 1;}}
      else {
        if (SoundSeries[2] < 36 )                     // this sound has 36 pitches
          SoundSeries[2]++;                           // every call of this sound proceeds with next pitch
        char FileName[13] = "0_0e_000.snd";           // generate base filename
        FileName[7] = 48 + (SoundSeries[2] % 10);     // change the 7th character of filename according to current pitch
        FileName[6] = 48 + (SoundSeries[2] % 100) / 10; // the same with the 6th character
        for (byte i=0; i<12; i++) {                   // store the name of this sound
          USB_RepeatSound[i] = FileName[i];}
        QueueNextSound(USB_RepeatSound);              // select this sound to be repeated
        PlaySound(51, (char*) FileName);}}            // play the sound
    else if (Command == 23){                          // sound command 0x17 - game over random phrase
      char FileName[13] = "0_17_000.snd";             // generate base filename
      FileName[7] = 48 + random(6) + 1;               // change the counter according to random number
      PlaySound(52, (char*) FileName);}               // play the corresponding sound file
    else {                                            // standard sound
      char FileName[9] = "0_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        PlaySound(51, (char*) FileName);}}
    return(0);                                        // return number not relevant for sounds
  default:
    return(0);}}                                      // no exception rule found for this type so proceed as normal

byte EX_JungleLord(byte Type, byte Command){
  static byte SoundSeries[2];                         // buffer to handle pre system11 sound series
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
    if (Command == 127) { }                           // ignore sound command 0x7f - audio bus init - not relevant for APC sound
    else if (Command == 38){                          // sound command 0x26 - start game
      char FileName[13] = "0_26_000.snd";             // generate base filename
      FileName[7] = 48 + random(4) + 1;               // change the counter according to random number
      PlaySound(52, (char*) FileName);}               // play the corresponding sound file
    else if (Command == 42){                          // sound command 0x2a - background sound - sound series
      SoundSeries[1] = 0;                             // reset the multiball start sound
      if (game_settings[USB_BGmusic]) {               // use MUSIC.SND instead of BG sound
        if (!SoundSeries[0]) {                        // don't restart if next pitch is requested
          PlayMusic(50, "MUSIC.snd");                 // play music track
          QueueNextMusic("MUSIC.snd");                // and loop it
          SoundSeries[0] = 1;}}
      else {
        if (SoundSeries[0] < 29)                      // BG sound has 29 pitches
          SoundSeries[0]++;                           // every call of this sound proceeds with the next pitch
        char FileName[13] = "0_2a_000.snd";           // generate base filename
        FileName[7] = 48 + (SoundSeries[0] % 10);     // change the 7th character of filename according to current pitch
        FileName[6] = 48 + (SoundSeries[0] % 100) / 10; // the same with the 6th character
        for (byte i=0; i<12; i++) {                   // store the name of this sound
          USB_RepeatSound[i] = FileName[i];}
        QueueNextSound(USB_RepeatSound);              // select this sound to be repeated
        PlaySound(51, (char*) FileName);}}            // play the sound
    else if (Command == 44) {                         // sound command 0x2c - stop sound
      AfterSound = 0;
      SoundSeries[0] = 0;                             // Reset BG sound
      SoundSeries[1] = 0;                             // reset the multiball start sound
      StopPlayingSound();
      StopPlayingMusic();}
    else if (Command == 45){                          // sound command 0x2d - multiball start - sound series
      if (SoundSeries[1] < 31)                        // this sound has 31 pitches
        SoundSeries[1]++;                             // every call of this sound proceeds with next pitch
      else
        SoundSeries[1] = 1;                           // start all over again
      char FileName[13] = "0_2d_000.snd";             // generate base filename
      FileName[7] = 48 + (SoundSeries[1] % 10);       // change the 7th character of filename according to current pitch
      FileName[6] = 48 + (SoundSeries[1] % 100) / 10; // the same with the 6th character
      PlaySound(51, (char*) FileName);}               // play the sound
    else {                                            // standard sound
      char FileName[9] = "0_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
        PlaySound(51, (char*) FileName);}}
    return(0);                                        // return number not relevant for sounds
  case SwitchActCommand:                              // activated switches
    if (Command == 43) {                              // ball successfully ejected
      EX_BallRelease(0);}                             // stop ball release timer
    else if (Command == 49) {                         // right magnet button
      if (QueryLamp(8) && QueryLamp(2)) {             // right magnet and ball in play lamp lit?
        ActivateSolenoid(0, 22);}}                    // activate right magnet
    else if (Command == 50) {                         // left magnet button
      if (QueryLamp(39) && QueryLamp(2)) {            // left magnet and ball in play lamp lit?
        ActivateSolenoid(0, 21);}}                    // activate left magnet
    return(0);                                        // all switches are reported to PinMame
  case SwitchRelCommand:                              // deactivated switches
    if (Command == 49){                               // right magnet button
      ReleaseSolenoid(22);}                           // turn off right magnet
    else if (Command == 50) {                         // left magnet button
      ReleaseSolenoid(21);}                           // turn off left magnet
    return(0);                                        // all switches are reported to PinMame
  case SolenoidActCommand:                            // activated solenoids
    if (Command == EX_EjectSolenoid){                 // ball eject coil
      if (QueryLamp(2)) {                             // ball in play lamp lit?
        EX_BallRelease(1);}}                          // start ball release timer
    return(0);                                        // solenoid will be activated
  default:
    return(0);}}                                      // no exception rule found for this type so proceed as normal

byte EX_Pharaoh(byte Type, byte Command){             // thanks to Grangeomatic for sending me this code
  static byte SoundSeries;                            // buffer to handle pre system11 sound series
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
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
      StopPlayingMusic();
      StopPlayingSound();}
    else if (Command == 45) {                         // sound command 0x2d - background sound - sound series
      if (game_settings[USB_BGmusic]) {               // use MUSIC.SND instead of BG sound
        if (!SoundSeries) {                           // don't restart if next pitch is requested
          PlayMusic(50, "MUSIC.snd");                 // play music track
          QueueNextMusic("MUSIC.snd");                // and loop it
          SoundSeries = 1;}}
      else {
        if (SoundSeries < 32)                         // sound series has 32 different pitches
          SoundSeries++;                              // switch to the next pitch when sound command is called again
        char FileName[13] = "0_2d_000.snd";           // generate base filename
        FileName[7] = 48 + (SoundSeries % 10);        // change the 7th character of filename according to current pitch
        FileName[6] = 48 + (SoundSeries % 100) / 10;  // the same with the 6th character
        for (byte i=0; i<12; i++) {                   // store filename to be repeated
          USB_RepeatSound[i] = FileName[i];}
        QueueNextSound(USB_RepeatSound);              // set this filename to be started by PlayNextSound
        PlaySound(51, (char*) FileName);}}
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
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
    if (Command == 127) { }                           // ignore sound command 0x7f - audio bus init - not relevant for APC sound
    else if (Command == 44) {                         // sound command 0x2c - stop sound
      AfterSound = 0;
      SoundSeries[0] = 0;                             // Reset BG sound
      SoundSeries[1] = 0;                             // reset the multiball start sound
      StopPlayingSound();
      StopPlayingMusic();}
    else if (Command == 45){                          // sound command 0x2d - sound series
      if (game_settings[USB_BGmusic]) {               // use MUSIC.SND instead of BG sound
        if (!SoundSeries[0]) {                        // don't restart if next pitch is requested
          PlayMusic(50, "MUSIC.snd");                 // play music track
          QueueNextMusic("MUSIC.snd");                // and loop it
          SoundSeries[0] = 1;}}
      else {
        if (SoundSeries[0] < 32) {                    // this sound has 32 pitches
          SoundSeries[0]++;}                          // every call of this sound proceeds with next pitch
        char FileName[13] = "0_2d_000.snd";           // generate base filename
        FileName[7] = 48 + (SoundSeries[0] % 10);     // change the 7th character of filename according to current pitch
        FileName[6] = 48 + (SoundSeries[0] % 100) / 10; // the same with the 6th character
        for (byte i=0; i<12; i++) {                   // prepare the filename
          USB_RepeatSound[i] = FileName[i];}
        QueueNextSound(USB_RepeatSound);              // sound is being auto repeated
        PlaySound(51, (char*) FileName);}}            // play the sound
    else if (Command == 46) {                         // sound command 0x2e - background sound - sound series
      SoundSeries[0] = 0;
      if (SoundSeries[1] < 30) {                      // this sound has 30 pitches
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
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
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
      StopPlayingSound();
      StopPlayingMusic();}
    else if (Command == 46) {                         // sound command 0x2e - background sound - sound series
      SoundSeries[0] = 0;
      if (game_settings[USB_BGmusic]) {               // use MUSIC.SND instead of BG sound
        if (!SoundSeries[2]) {                        // don't restart if next pitch is requested
          PlayMusic(50, "MUSIC.snd");                 // play music track
          QueueNextMusic("MUSIC.snd");                // and loop it
          SoundSeries[2] = 1;}}
      else {
        if (SoundSeries[2] < 29)
          SoundSeries[2]++;
        char FileName[13] = "0_2e_000.snd";
        FileName[7] = 48 + (SoundSeries[2] % 10);
        FileName[6] = 48 + (SoundSeries[2] % 100) / 10;
        FileName[5] = 48 + SoundSeries[2] / 100;
        for (byte i=0; i<12; i++) {
          USB_RepeatSound[i] = FileName[i];}
        QueueNextSound(USB_RepeatSound);
        LastCh1Sound = Command;                       // buffer sound number
        PlaySound(51, (char*) FileName);}}
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

byte EX_Comet(byte Type, byte Command) {
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
    if (!Command || Command > 254) {                  // sound command 0x00 and 0xff -> stop sound
      AfterMusic = 0;
      StopPlayingMusic();
      StopPlayingSound();}
    else if (Command == 11 || Command == 254) { }     // ignore sound commands 0x0b and 0xfe
    else if (Command == 47) {                         // play BG music
      if (game_settings[USB_BGmusic]) {               // use MUSIC.SND instead of BG sound
        PlayMusic(50, "MUSIC.snd");                   // play music track
        QueueNextMusic("MUSIC.snd");}                 // and loop it
      else {
        PlayMusic(50, "0_2f.snd");
        QueueNextMusic("0_2f.snd");}}                 // track is looping so queue it also
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

byte EX_HighSpeed(byte Type, byte Command) {          // plays just the standard sounds
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
  if (Type == SoundCommandCh1) {                      // sound commands for channel 1
    char FileName[9] = "0_00.snd";                    // handle standard sound
    if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
      PlaySound(51, (char*) FileName);}}
  else if (Type == SoundCommandCh2) {                 // sound commands for channel 2
    char FileName[9] = "1_00.snd";                    // handle standard music
    if (USB_GenerateFilename(2, Command, FileName)) { // create filename and check whether file is present
      PlayMusic(51, (char*) FileName);}}
  return(0);}                                         // no exception rule found for this type so proceed as normal

byte EX_Pinbot(byte Type, byte Command){
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
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
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
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
      RestoreMusicVolume(100);                        // reset music volume in case it was made quieter earlier
      StopPlayingMusic();}
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
      PlayMusic(50, "1_01.snd");                      // play non looping part of music track
      QueueNextMusic("1_01.snd");}                    // queue looping part as next music to be played
    else if (Command == 3) {                          // music track 3
      PlayMusic(50, "1_03.snd");                      // play non looping part of music track
      QueueNextMusic("1_03L.snd");}                   // queue looping part as next music to be played
    else if (Command == 4) {                          // music track 2
      PlayMusic(50, "1_04L.snd");}                    // play non looping part of music track
    else if (Command == 6) {                          // music track 6
      PlayMusic(50, "1_06.snd");                      // play non looping part of music track
      QueueNextMusic("1_03L.snd");}                   // queue looping part as next music to be played
    else if (Command == 7) {                          // music track 7
      PlayMusic(50, "1_07.snd");                      // play non looping part of music track
      QueueNextMusic("1_07L.snd");}                   // queue looping part as next music to be played
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

byte EX_SpaceStation(byte Type, byte Command){
  static byte LastMusic;
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
  switch(Type){
  case SoundCommandCh1:                               // sound commands for channel 1
    if (!Command){                                    // sound command 0x00 - stop sound
      AfterSound = 0;
      StopPlayingSound();}
    else if (Command == 52) { }                       // ignore sound command 0x34
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
      LastMusic = Command;                            // store number of last music track
      RestoreMusicVolume(100);
      StopPlayingMusic();}
    else if (Command == 66) { }                       // ignore sound command 0x42
    else if (Command == 85) { }                       // ignore sound command 0x55
    else if (Command > 95 && Command < 104) {         // music volume command 0x6X
      MusicVolume = Command - 96;}
    else if (Command == 170) { }                      // ignore unknown sound command 0xaa
    else if (Command == 255) { }                      // ignore unknown sound command 0xff
    else if (Command == 1) {                          // music track 1
      LastMusic = Command;                            // store number of last music track
      PlayMusic(50, "1_01L.snd");                     // play music track
      QueueNextMusic("1_01L.snd");}                   // track is looping so queue it also
    else if (Command == 2) {                          // music track 2
      LastMusic = Command;                            // store number of last music track
      PlayMusic(50, "1_02.snd");                      // play non looping part of music track
      QueueNextMusic("1_02L.snd");}                   // queue looping part as next music to be played
    else if (Command == 3) {                          // music track 3
      LastMusic = Command;                            // store number of last music track
      PlayMusic(50, "1_03L.snd");                     // play music track
      QueueNextMusic("1_03L.snd");}                   // track is looping so queue it also
    else if (Command == 4) {                          // music track 4
      LastMusic = Command;                            // store number of last music track
      PlayMusic(50, "1_04.snd");                      // play non looping part of music track
      QueueNextMusic("1_04L.snd");}                   // queue looping part as next music to be played
    else if (Command == 5) {                          // music track 5
      LastMusic = Command;                            // store number of last music track
      PlayMusic(50, "1_05.snd");                      // play music track
      QueueNextMusic("1_05L.snd");}                   // queue looping part as next music to be played
    else if (Command == 6) {                          // music track 6
      LastMusic = Command;                            // store number of last music track
      PlayMusic(50, "1_06.snd");                      // play non looping part of music track
      QueueNextMusic("1_06L.snd");}                   // queue looping part as next music to be played
    else if (Command == 7) {                          // music track 7
      LastMusic = Command;                            // store number of last music track
      AfterMusic = 0;                                 // no looping part
      PlayMusic(50, "1_07.snd");}                     // play music track
    else if (Command == 11) {                         // music track 0x0b
      LastMusic = Command;                            // store number of last music track
      AfterMusic = 0;                                 // no looping part
      PlayMusic(50, "1_0b.snd");}                     // play music track
    else if (Command == 12) {                         // music track 0x0c
      LastMusic = Command;                            // store number of last music track
      AfterMusic = 0;                                 // no looping part
      PlaySound(60, "mball.snd");}                    // play multiball intro and block everything else
    else if (Command == 13) {                         // music track 0x0d
      LastMusic = Command;                            // store number of last music track
      PlayMusic(50, "1_0d.snd");                      // play non looping part of music track
      QueueNextMusic("1_0dL.snd");}                   // queue looping part as next music to be played
    else if (Command == 64) {                         // music ending 0x40
      if (LastMusic != 64) {                          // avoid double play
        LastMusic = Command;                          // store number of last music track
        AfterMusic = 0;                               // disable looping
        PlayMusic(50, "1_40.snd");}}                  // play ending
    else if (Command == 65) {                         // music ending 0x41
      LastMusic = Command;                            // store number of last music track
      PlayMusic(50, "1_01.snd");                      // play non looping part of music track
      QueueNextMusic("1_01L.snd");}                   // queue looping part as next music to be played
    else if (Command == 66) {                         // music command 0x42
      LastMusic = Command;                            // store number of last music track
      PlayMusic(50, "1_03.snd");                      // play non looping part of music track
      QueueNextMusic("1_03L.snd");}                   // queue looping part as next music to be played
    else if (Command == 67) {                         // music ending 0x43
      LastMusic = Command;                            // store number of last music track
      PlayMusic(50, "1_04.snd");                      // play non looping part of music track
      QueueNextMusic("1_04L.snd");}                   // queue looping part as next music to be played
    else if (Command == 130) {
      if (LastMusic) {                                // dont play it when no music is being played
        PlaySound(50, "1_82.snd");}
      else {
        return(0);}}
    else if (Command == 144) {                        // sound 0x90
      PlaySound(49, "1_90.snd");}                     // play with lower prio
    else if (Command == 149) {                        // sound 0x95
      PlayMusic(50, "1_95.snd");}                     // play on music channel
    else if (Command == 158) {                        // sound 0x9e
      PlayMusic(50, "1_9e.snd");}                     // play on music channel
    else if (Command == 165) {                        // sound 0xa5
      PlayMusic(50, "1_a5.snd");}                     // play on music channel
    else {
      char FileName[9] = "1_00.snd";                  // handle standard sound
      if (USB_GenerateFilename(2, Command, FileName)) { // create filename and check whether file is present
        PlaySound(50, (char*) FileName);}}            // play on the sound channel
    return(0);                                        // return number not relevant for sounds
  case SolenoidActCommand:
    if (!QuerySolenoid(12) && (Command == 3 || Command == 4 || Command == 6)) { // protect high power solenoids from over turn on
      ActivatePrioTimer(40, Command, ReleaseSolenoid);}
    else if (Command == 8 || Command == 13 || Command == 17) {
      ActivatePrioTimer(40, Command, ReleaseSolenoid);}
    return(0);
  default:                                            // use default treatment for undefined types
    return(0);}}                                      // no exception rule found for this type so proceed as normal

byte EX_Rollergames(byte Type, byte Command){
  static byte LastMusic;
  if (game_settings[USB_BallSave]) {                  // ball saver set to active?
    if (EX_BallSaver(Type, Command)) {                // include ball saver
      return(1);}}                                    // omit command if ball saver says so
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
  case 4:                                             // Disco Fever
    //SolRecycleTime[5-1] = 250;                        // set recycle time for eject hole to prevent double kicking
    PinMameException = EX_DiscoFever;                 // use exception rules for Flash
    EX_Machine = EX_DiscoFeverProperties;             // machine properties for ball saver
    break;
  case 6:                                             // Flash
    SolRecycleTime[5-1] = 250;                        // set recycle time for eject hole to prevent double kicking
    PinMameException = EX_Flash;                      // use exception rules for Flash
    EX_Machine = EX_FlashProperties;                  // machine properties for ball saver
    break;
  case 10:                                            // Time Warp
    PinMameException = EX_TimeWarp;                   // use exception rules for Time Warp
    EX_Machine = EX_TimeWarpProperties;               // machine properties for ball saver
    break;
  case 16:                                            // Firepower
    PinMameException = EX_Firepower;                  // use exception rules for Firepower
    EX_Machine = EX_FirepowerProperties;              // machine properties for ball saver
    break;
  case 18:                                            // Alien Poker
    PinMameException = EX_AlienPoker;                 // use exception rules for Alien Poker
    EX_Machine = EX_AlienPokerProperties;             // machine properties for ball saver
    break;
  case 20:                                            // Jungle Lord
    EX_EjectSolenoid = 2;                             // specify eject coil for improved ball release
    PinMameException = EX_JungleLord;                 // use exception rules for Jungle Lord
    EX_Machine = EX_JungleLordProperties;             // machine properties for ball saver
    break;
  case 21:                                            // Pharaoh
    PinMameException = EX_Pharaoh;                    // use exception rules for Pharaoh
    EX_Machine = EX_PharaohProperties;                // machine properties for ball saver
    break;
  case 25:                                            // Barracora
    EX_EjectSolenoid = 12;                            // specify eject coil for improved ball release
    PinMameException = EX_Barracora;                  // use exception rules for Barracora
    EX_Machine = EX_BarracoraProperties;              // machine properties for ball saver
    break;
  case 34:                                            // Black Knight
    EX_EjectSolenoid = 6;                             // specify eject coil for improved ball release
    PinMameException = EX_BlackKnight;                // use exception rules for Jungle Lord
    EX_Machine = EX_BlackKnightProperties;            // machine properties for ball saver
    break;
  case 39:                                            // Comet
    PinMameException = EX_Comet;                      // use exception rules for Comet
    EX_Machine = EX_CometProperties;                  // machine properties for ball saver
    break;
  case 40:                                            // High Speed
    PinMameException = EX_HighSpeed;                  // use exception rules for High Speed
    EX_Machine = EX_HighSpeedProperties;              // machine properties for ball saver
    break;
  case 43:                                            // Pinbot
    PinMameException = EX_Pinbot;                     // use exception rules for Pinbot
    EX_Machine = EX_PinbotProperties;                 // machine properties for ball saver
    break;
  case 44:                                            // F-14 Tomcat
    PinMameException = EX_F14Tomcat;                  // use exception rules for Tomcat
    EX_Machine = EX_F14Properties;                    // machine properties for ball saver
    break;
  case 48:                                            // Space Station
    PinMameException = EX_SpaceStation;               // use exception rules for Space Station
    EX_Machine = EX_SpaceStationProperties;           // machine properties for ball saver
    break;
  case 67:                                            // Rollergames
    PinMameException = EX_Rollergames;                // use exception rules for Rollergames
    EX_Machine = EX_RollerGamesProperties;            // machine properties for ball saver
    break;
  case 71:                                            // Riverboat Gambler
    PinMameException = EX_FakeSwitches;               // use exception rules for Rollergames
    break;
  default:
    PinMameException = EX_DummyProcess;}}

