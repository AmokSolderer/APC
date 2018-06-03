// Rules for the Pinbot pinball machine

bool OpenVisor = false;																// visor is being opened if true
bool CloseVisor = false;															// visor is being closed if true
bool PB_DropWait = false;															// ignore drop target switches when true
bool PB_DropRamp = false;															// ramp needs to be dropped when possible
bool PB_EnergyActive = false;													// score energy active?
bool PB_SkillShot = false;														// is the skill shot active?
byte PB_ChestMode = 0;																// current status of the chest and visor
byte PB_DropTimer = 0;																// number of the drop target timer
byte PB_ChestLightsTimer = 0;													// number of the timer controlling the chest lamp sequencing
byte PB_Planet[5];																		// reached planets for all players
byte PB_ExBallsLit = 0;																// no of lanes lit for extra ball
byte PB_SkillMultiplier = 1;													// Multiplier for the skill shot value
byte DropBlinkLamp = 0;																// number of the lamp currently blinking

const unsigned int PB_SolTimes[32] = {50,30,30,70,30,200,30,30,500,500,999,999,0,0,500,500,50,500,50,50,50,50,0,0,50,500,500,500,500,500,500,500}; // Activation times for solenoids (last 8 are C bank)
const byte PB_BallSearchCoils[9] = {3,4,5,17,19,22,6,20,21}; // coils to fire when the ball watchdog timer runs out
const byte ChestRows[11][5] = {{28,36,44,52,60},{28,29,30,31,32},{36,37,38,39,40},{44,45,46,47,48},{52,53,54,55,56},{60,61,62,63,64},
																{32,40,48,56,64},{31,39,47,55,63},{30,38,46,54,62},{29,37,45,53,61},{28,36,44,52,60}};

struct SettingTopic PB_setList[6] = {{"DROP TG TIME  ",HandleNumSetting,0,3,20},
		{" REACH PLANET ",HandleNumSetting,0,1,9},
		{" ENERGY TIMER ",HandleNumSetting,0,1,90},
    {"RESTOREDEFAULT",RestoreDefaults,0,0,0},
    {"  EXIT SETTNGS",ExitSettings,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

																	 //  DurationXX0910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364
const struct LampPat PB_AttractPat1[5] = {{150,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														           		{150,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
																					{150,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1},
																					{150,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1},
																					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

const struct LampPat PB_AttractPat2[5] = {{150,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0},
 																					{150,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0},
																					{150,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1},
																					{150,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1},
																					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

const struct LampPat PB_AttractPat3[4] = {{150,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0},
 																					{150,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
																					{150,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
																					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

const struct LampFlow PB_AttractFlow[4] = {{1,PB_AttractPat1},{10,PB_AttractPat2},{1,PB_AttractPat3},{0,0}};

const bool PB_ChestPatterns[4][25] =  {{1,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,0,1},
																			 {1,1,1,0,0,0,0,1,0,1,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0},
																			 {0,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,0,1},
																			 {0,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,1,0,1,0}};

bool PB_ChestLamp[25];

const byte PB_DropTime = 0;														// drop target down time setting
const byte PB_ReachPlanet = 1;												// target planet setting
const byte PB_EnergyTimer = 2;												// energy timer setting

const byte PB_defaults[64] = {5,6,15,0,0,0,0,0,		 		// game default settings
											  			0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0};

const struct GameDef PB_GameDefinition = {
		PB_setList,																				// GameSettingsList
		(byte*)PB_defaults,																// GameDefaultsPointer
		"PB_SET.APC",																			// GameSettingsFileName
		"PBSCORES.APC",																		// HighScoresFileName
		PB_AttractMode,																		// AttractMode
		PB_SolTimes};																			// Default activation times of solenoids

void PB_init() {
	GameDefinition = PB_GameDefinition;}								// read the game specific settings and highscores

void PB_AttractMode() {
	DispRow1 = DisplayUpper;
	DispRow2 = DisplayLower;
	Switch_Pressed = PB_AttractModeSW;
	Switch_Released = DummyProcess;
  AddBlinkLamp(1, 150);                               // blink Game Over lamp
  AppByte2 = 0;
  LampReturn = PB_AttractLampCycle;
  PB_AttractLampCycle(0);
  PB_AttractDisplayCycle(0);}

void PB_AttractDisplayCycle(byte Event) {
  PB_CheckForLockedBalls(0);
  switch (Event) {
    case 0:
      WriteUpper2("THE APC       ");
      ActivateTimer(50, 1, ScrollUpper);
      ActivateTimer(900, 1, PB_AttractScroll);
      WriteLower2("              ");
      ActivateTimer(1400, 1, ScrollLower);
      if (NoPlayers) {																// was there a previous game?
        Event++;}																			// proceed to case 1 next time
      else {																					// no previous games since power on
        Event = 2;}																		// skip case 1
      break;
    case 1:																						// show points of previous game
    	WriteUpper2("              ");                  // erase display
    	WriteLower2("              ");
    	  for (i=1; i<=NoPlayers; i++) {                // display the points of all active players
    	  	ShowNumber(8*i-1, Points[i]);}
    	ActivateTimer(50, 1, ScrollUpper);
    	ActivateTimer(900, 1, ScrollLower);
      Event++;
      break;
    case 2:																						// Show highscores
      WriteUpper2("1>     2>     ");
      WriteLower2("              ");
      for (i=0; i<3; i++) {
        *(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[i]-32)*2];
        *(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[i]-32)*2+1];
        *(DisplayUpper2+24+2*i) = DispPattern1[(HallOfFame.Initials[3+i]-32)*2];
        *(DisplayUpper2+24+2*i+1) = DispPattern1[(HallOfFame.Initials[3+i]-32)*2+1];}
      ShowNumber(23, HallOfFame.Scores[0]);
      ShowNumber(31, HallOfFame.Scores[1]);
      ActivateTimer(50, 1, ScrollUpper);
      ActivateTimer(900, 1, ScrollLower2);
      Event++;
      break;
    case 3:
      WriteUpper2("3>     4>     ");
      WriteLower2("              ");
      for (i=0; i<3; i++) {
        *(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2];
        *(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2+1];
        *(DisplayUpper2+24+2*i) = DispPattern1[(HallOfFame.Initials[9+i]-32)*2];
        *(DisplayUpper2+24+2*i+1) = DispPattern1[(HallOfFame.Initials[9+i]-32)*2+1];}
      ShowNumber(23, HallOfFame.Scores[2]);
      ShowNumber(31, HallOfFame.Scores[3]);
      ActivateTimer(50, 1, ScrollUpper);
      ActivateTimer(900, 1, ScrollLower2);
      Event = 0;}
    ActivateTimer(4000, Event, PB_AttractDisplayCycle);}

void PB_AttractScroll(byte Dummy) {
  WriteUpper2("PINBOT        ");
  AddScrollUpper(1);}

void PB_AttractLampCycle(byte Event) {                // play multiple lamp pattern series
  PatPointer = PB_AttractFlow[AppByte2].FlowPat;      // set the pointer to the current series
  FlowRepeat = PB_AttractFlow[AppByte2].Repeat;       // set the repetitions
  AppByte2++;                                         // increase counter
  if (!PB_AttractFlow[AppByte2].Repeat) {             // repetitions of next series = 0?
    AppByte2 = 0;}                                    // reset counter
  ShowLampPatterns(0);}                               // call the player

void PB_AttractModeSW(byte Select) {
	switch(Select) {
	case 3:																							// credit button
		RemoveBlinkLamp(1);																// stop the blinking of the game over lamp
    KillAllTimers();
    StrobeLightsTimer = 0;
    ByteBuffer3 = 0;
		for (i=0; i< LampMax+1; i++) {
			Lamp[i] = false;}
		LampPattern = Lamp;
		Lamp[3] = true;                                 	// turn on Ball in Play lamp
		NoPlayers = 0;
		WriteUpper("              ");
		WriteLower("              ");
		Ball = 1;
		PB_AddPlayer();
		for (i=1;i<5;i++) {																// reset reached planets
			PB_Planet[i] = 0;}
    InLock = 0;
		Player = 1;
		ExBalls = 0;
    Multiballs = 1;
		Bonus = 1;
		BonusMultiplier = 1;
		if (Switch[49] || Switch[50] || Switch[51]) {			// any drop target down?
			ActA_BankSol(0, 4);}														// reset it
		if (!Switch[44]) {																// ramp in up state?
			ActA_BankSol(0, 6);}														// put it down
		PB_NewBall(2);                                    // release a new ball (2 expected balls in the trunk)
		PB_ChestMode = 1;
		PB_LightChestRows(0); 								// TO BE CHANGED LATER
		ActivateSolenoid(0, 23);                        	// enable flipper fingers
		ActivateSolenoid(0, 24);
		break;
	case 8:																							// high score reset
		digitalWrite(Blanking, HIGH);                     // invoke the blanking
		break;
	case 46:
		if (CloseVisor) {
			CloseVisor = false;
			ReleaseSolenoid(13);}
		break;
	case 47:
		if (OpenVisor) {
			OpenVisor = false;
			ReleaseSolenoid(13);}
		break;
	case 72:
		Switch_Pressed = DummyProcess;
//		if (ByteBuffer3) {																// ShowLampPattern running?
//			KillTimer(ByteBuffer3);													// stop it
    RemoveBlinkLamp(1);                                // stop the blinking of the game over lamp
    KillAllTimers();
    StrobeLightsTimer = 0;
		ByteBuffer3 = 0;
		LampPattern = NoLamps;                          	// Turn off all lamps
   // BlinkScoreTimer
		ReleaseAllSolenoids();
		if (digitalRead(UpDown)) {
			WriteUpper("  TEST  MODE  ");
			WriteLower("              ");
			AppByte = 0;
			ActivateTimer(1000, 0, PB_Testmode);}
		else {
			Settings_Enter();}
		break;
	}}

void PB_CheckForLockedBalls(byte Dummy) {             // check if balls are locked and release them
	if (Switch[16]) {                                   // for the outhole
		ActA_BankSol(0, 1);}
	if (Switch[38]) {                                   // for the single eject hole
		ActA_BankSol(0, 3);}
	if (Switch[25] || Switch[26]) {                     // for the eyes
		if (Switch[47]) {																	// visor is open
			if (Switch[25]) {																// left eye
				ActA_BankSol(0, 7);}													// eject ball
			else {																					// right eye
				ActA_BankSol(0, 8);}}													// eject ball
		else {																						// visor is not open
			ActivateSolenoid(0, 13);												// activate visor motor
			ActivateTimer(2000, 0, PB_OpenVisor);}}					// ignore the visor switch for 2 seconds
	else {																							// no balls in lock
		if (!Switch[46]) {																// visor not closed
			ActivateSolenoid(0, 13);												// activate visor motor
			ActivateTimer(2000, 0, PB_CloseVisor);}}}				// ignore the visor switch for 2 seconds

void PB_AddPlayer() {
  if ((NoPlayers < 4) && (Ball == 1)) {               // if actual number of players < 4
    NoPlayers++;                                      // add a player
    Points[NoPlayers] = 0;                            // delete the points of the new player
    ShowPoints(NoPlayers);}}                          // and show them
    
void PB_NewBall(byte Balls) {                         // release ball (Event = expected balls on ramp)
	ShowAllPoints(0);
	//ShowBonus();
	*(DisplayUpper+16) = LeftCredit[32 + 2 * Ball]; 		// show current ball in left credit
	//*(DisplayUpper+17) = LeftCredit[33 + 2 * Ball];
	if (!BlinkScoreTimer) {
		BlinkScoreTimer = ActivateTimer(1000, 1, BlinkScore);}
  DropBlinkLamp = 41;
	PB_CycleDropLights(1);															// start the blinking drop target lights
	PB_SkillShot = true;																// the first shot is a skill shot
	if (PB_Planet[Player] < game_settings[PB_ReachPlanet]) {	// target planet not reached yet?
		AddBlinkLamp(18+game_settings[PB_ReachPlanet],100);}		// let target planet blink
	if (!Switch[20]) {																	// ball not yet in shooter lane?
		Switch_Released = DummyProcess;
		ActA_BankSol(0, 2);                          			// release ball
		Switch_Pressed = PB_BallReleaseCheck;             // set switch check to enter game
		CheckReleaseTimer = ActivateTimer(5000, Balls-1, PB_CheckReleasedBall);} // start release watchdog
	else {																							// ball already in shooter lane
		Switch_Released = PB_CheckShooterLaneSwitch;			//	wait for switch 20 to be released
		Switch_Pressed = PB_ResetBallWatchdog;}}

void PB_CheckShooterLaneSwitch(byte Switch) {
  if (Switch == 20) {                                 // shooter lane switch released?
    Switch_Released = DummyProcess;
    if (!BallWatchdogTimer) {
      BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}}}

void PB_ResetBallWatchdog(byte Switch) {              // handle switches during ball release
	if ((Switch > 11)&&(Switch != 17)&&(Switch != 18)&&(Switch != 19)&&(Switch != 44)&&(Switch != 46)&&(Switch != 47)) { // playfield switch activated?
		if (BallWatchdogTimer) {
			KillTimer(BallWatchdogTimer);}                  // stop watchdog
		if (PB_DropRamp&&(Switch != 45)&&(Switch != 49)&&(Switch != 50)&&(Switch != 51)) { // switch not close to the ramp?
			PB_DropRamp = false;														// clear request
			ActA_BankSol(0, 6);}														// drop ramp
		if (PB_SkillShot) {																// is this a skill shot?
			if (Switch != 20) {															// no bouncing of the shooter lane switch?
				PB_SkillShot = false;}												// the next shot is not a skill shot any more
			switch (Switch) {																// was a skill shot target hit
			case 22:
				Points[Player] += 20000 * PB_SkillMultiplier;
				ShowPoints(Player);
				break;
			case 23:
				Points[Player] += 100000 * PB_SkillMultiplier;
				ShowPoints(Player);
				break;
			case 24:
				Points[Player] += 5000 * PB_SkillMultiplier;
				ShowPoints(Player);
				break;}}
		BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}
	PB_GameMain(Switch);}                               // process current switch

void PB_BallReleaseCheck(byte Switch) {               // handle switches during ball release
	if ((Switch > 11)&&(Switch != 17)&&(Switch != 18)&&(Switch != 19)&&(Switch != 44)&&(Switch != 46)&&(Switch != 47)) { // playfield switch activated?
		if (CheckReleaseTimer) {
			KillTimer(CheckReleaseTimer);
			CheckReleaseTimer = 0;}                   			// stop watchdog
		Switch_Pressed = PB_ResetBallWatchdog;
		if (Switch == 20) {																// ball is in the shooter lane
			Switch_Released = PB_CheckShooterLaneSwitch;}		// set mode to register when ball is shot
		else {
			if (!BallWatchdogTimer) {
				BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}}} // set switch mode to game
  PB_GameMain(Switch);}

void PB_CheckReleasedBall(byte Balls) {               // ball release watchdog
  if (BlinkScoreTimer) {															// stop blinking to show messages
    KillTimer(BlinkScoreTimer);
    BlinkScoreTimer = 0;}
  WriteUpper("WAITINGFORBALL");                       // indicate a problem
  WriteLower("              ");
  if (Balls == 10) {																	// indicating a previous trunk error
    WriteUpper("              ");
    WriteLower("              ");
    ShowAllPoints(0);
    BlinkScoreTimer = ActivateTimer(1000, 1, BlinkScore);
    ActA_BankSol(0, 2);}
  AppByte = PB_CountBallsInTrunk();
  if (AppByte == Balls) {                             // expected number of balls in trunk
    WriteUpper("  BALL MISSING");
    if (Switch[16]) {                                 // outhole switch still active?
      ActA_BankSol(0, 1);}}														// shove the ball into the trunk
  else {																							//
    if (AppByte == 5) {																// balls not settled
      WriteLower(" TRUNK  ERROR ");
      Balls = 10;}
    else {
      if (AppByte > Balls) {													// more balls in trunk than expected
        WriteUpper("              ");
        WriteLower("              ");
        ShowAllPoints(0);
        BlinkScoreTimer = ActivateTimer(1000, 1, BlinkScore);
        ActA_BankSol(0, 2);}}}                    		// release again
  CheckReleaseTimer = ActivateTimer(5000, Balls, PB_CheckReleasedBall);}

byte PB_CountBallsInTrunk() {
  byte Balls = 0;
  for (i=0; i<2; i++) {                               // check how many balls are on the ball ramp
    if (Switch[17+i]) {
      if (Balls < i) {
        return 5;}                                    // send warning
      Balls++;}}
  return Balls;}

void PB_SearchBall(byte Counter) {										// ball watchdog timer has run out
  BallWatchdogTimer = 0;
  if (Switch[16]) {																		// ball in outhole?
    BlockOuthole = false;
    ActivateTimer(1000, 0, PB_ClearOuthole);}
  else {
    if (Switch[20]) {																	// if ball is waiting to be launched
      BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}	// restart watchdog
    else {  																					// if ball is really missing
      AppByte = PB_CountBallsInTrunk();								// recount all balls
      if (AppByte == 2) {															// found 2 balls in trunk?
      	if (BlockOuthole) {														// is the outhole blocked
      		PB_BallEnd(0);}															// then it was probably a ball loss gone wrong
      	else {
      		ActivateTimer(1000, 2, PB_NewBall);}}				// otherwise try it with a new ball
      else {
      	AppByte2 = 0;
      	for (i=0; i<2; i++) {                         // count balls in lock
      		if (Switch[25+i]) {
      			if (AppByte2 < i) {
      				AppByte = 5;}                           // set warning flag
      			AppByte2++;}}
      	if (AppByte == 5) {														// balls have not settled yet
      		WriteUpper("  BALL  STUCK ");
      		BallWatchdogTimer = ActivateTimer(1000, 0, PB_SearchBall);} // and try again in 1s
      	else {
      		if (AppByte2 > InLock) {										// more locked balls found than expected?
      			PB_HandleLock(0);													// lock them
      			BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}
      		else {
      			WriteUpper("  BALL  SEARCH");
      			ActivateSolenoid(0, PB_BallSearchCoils[Counter]); // fire coil to get ball free
      			Counter++;
      			if (Counter == 9) {												// all coils fired?
      				Counter = 0;}														// start again
      			if (Switch[46] && !CloseVisor) {					// visor closed?
      				ActivateSolenoid(0, 13);								// open it enough to deactivate switch 46
      				ActivateTimer(2000, 0, PB_CloseVisor);}	// and prepare to close it again
      			BallWatchdogTimer = ActivateTimer(1000, Counter, PB_SearchBall);}}}}}} // come again in 1s if no switch is activated

void PB_OpenVisor(byte Dummy) {
	OpenVisor = true;}																	// set flag to stop visor motor when open

void PB_CloseVisor(byte Dummy) {
	CloseVisor = true;}																	// set flag to stop visor motor when closed

void PB_ClearOuthole(byte Event) {
  if (Switch[16]) {                                   // outhole switch still active?
    if (!BlockOuthole) {															// outhole blocked?
      BlockOuthole = true;														// block outhole until this ball has been processed
      ActA_BankSol(0, 1);                         		// put ball in trunk
      ActivateTimer(2000, 0, PB_BallEnd);}}
    else {
      ActivateTimer(2000, 0, PB_ClearOuthole);}}			// come back in 2s if outhole is blocked

void PB_GameMain(byte Switch) {
	switch(Switch) {
	case 8:																							// high score reset
		digitalWrite(Blanking, HIGH);                     // invoke the blanking
		break;
	case 16:
    ActivateTimer(200, 0, PB_ClearOuthole);           // check again in 200ms
    break;
	case 25:
	case 26:
		ActivateTimer(2000, 0, PB_HandleLock);            // handle locked balls after 1s
		break;
	case 28:																						// chest
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
		if (PB_ChestMode > 0) {														// chest switches active?
			if (PB_ChestMode < 10) {												// visor can be opened with one row / column hit
				if (Switch == PB_ChestMode) {									// correct row / column hit?
					OpenVisor = true;
					ActivateSolenoid(0, 13);}										// open visor
				else {																				// incorrect row / column hit
					if (PB_ChestLightsTimer) {
						KillTimer(PB_ChestLightsTimer);						// disable timer to change row / column blinking
						PB_ChestLightsTimer = 0;}
					for (i=0; i<5; i++) {												// turn off blinking row / column
						RemoveBlinkLamp(ChestRows[PB_ChestMode-1][i]);}
					PB_ChestMode = 15;
					PB_ChestLightsTimer = ActivateTimer(200, 0, PB_LightChestPattern);}
			}
		}

		break;
	case 45:																						// score energy switch
		if (PB_EnergyActive) {
																	// score energy value
			PB_EnergyOff(0);}
		break;
	case 46:
		if (CloseVisor) {
			CloseVisor = false;
			ReleaseSolenoid(13);}
		break;
	case 47:
		if (OpenVisor) {
			OpenVisor = false;
			ReleaseSolenoid(13);}
		break;
	case 49:																						// drop targets
	case 50:
	case 51:
    if (!PB_DropWait) {
      PB_DropWait = true;
      Points[Player] += Multiballs * 1000;
      ShowPoints(Player);
      ActivateTimer(200, Switch, PB_HandleDropTargets);}
		break;
	case 65:																						// lower jet bumper
		ActivateSolenoid(0, 17);
		break;
	case 67:																						// left jet bumper
		ActivateSolenoid(0, 19);
		break;
	case 68:																						// left slingshot
		ActivateSolenoid(0, 20);
		break;
	case 69:																						// right slingshot
		ActivateSolenoid(0, 21);
		break;
  case 70:                                            // upper jet bumper
    ActivateSolenoid(0, 22);
    break;
	}
}

void PB_LightChestRows(byte Dummy) {									// and columns
	//AddBlinkLamp(ChestRows[PB_ChestMode][0], 100);
	for (i=0; i<5; i++) {
		RemoveBlinkLamp(ChestRows[PB_ChestMode-1][i]);}
	for (i=0; i<5; i++) {
		AddBlinkLamp(ChestRows[PB_ChestMode][i], 75);}
	PB_ChestMode++;
	if (PB_ChestMode > 10) {
		PB_ChestMode = 1;}
	PB_ChestLightsTimer = ActivateTimer(1000, 0, PB_LightChestRows);}

void PB_LightChestPattern(byte step) {
	byte x = 0;
	byte y = 0;
	for (x=0; x<5; x++) {
		for (y=0; y<5; y++) {
			if (!PB_ChestLamp[28+8*x+y]) {
				Lamp[28+8*x+y] = PB_ChestPatterns[step][5*y+x];}}}
	step++;
	if (step == 4) {
		step = 0;}
	PB_ChestLightsTimer = ActivateTimer(200, step, PB_LightChestPattern);}

void PB_HandleLock(byte Dummy) {
	;
}

void PB_HandleDropTargets(byte Target) {
	PB_DropWait = false;																// stop ignoring drop target switches
	if (Switch[49] && Switch[50] && Switch[51]) {				// all targets down
		if (PB_DropTimer) {																// any targets down before?
			KillTimer(PB_DropTimer);												// turn off timer
			PB_DropTimer = 0;
			DropBlinkLamp = 41;
			PB_CycleDropLights(1);													// start the blinking drop target lights
			RemoveBlinkLamp(17);}														// stop blinking of timer lamp
		Points[Player] += Multiballs * 25000;
		PB_Planet[Player]++;															// player has reached next planet
		ActA_BankSol(0, 4);																// reset drop targets
		if (PB_Planet[Player] > 10) {											// sun already reached before?
			PB_Planet[Player] = 10;}												// set it back to the sun
		else {
			if 	(PB_Planet[Player] == 10) {									//  10 = Sun
				PB_ExBallsLit++;}
			else {
				if (PB_Planet[Player] == game_settings[PB_ReachPlanet]) { // target planet reached
					PB_ExBallsLit++;
					RemoveBlinkLamp(18+game_settings[PB_ReachPlanet]);}	// stop blinking
				Lamp[PB_Planet[Player]+18] = true;}}}
	else {
		if (!PB_DropTimer) {															// first target hit
			if (Target-8 == DropBlinkLamp) {								// blinking target hit?
				ActA_BankSol(0, 5);														// raise ramp
				AddBlinkLamp(34, 500);												// blink energy lamp
				PB_EnergyActive = true;												// energy value on
				ActivateTimer(game_settings[PB_EnergyTimer]*1000, 0, PB_EnergyOff);}
			PB_CycleDropLights(0);													// stop blinking of drop target lights
			AddBlinkLamp(17, 500);													// blink drop target timer lamp
			PB_DropTimer = ActivateTimer(game_settings[PB_DropTime]*1000, 0, PB_ResetDropTargets);}}}

void PB_ResetDropTargets(byte Dummy) {
	RemoveBlinkLamp(17);																// stop drop target timer lamp
  DropBlinkLamp = 41;
  PB_DropTimer = 0;
	PB_CycleDropLights(1);															// start the blinking drop target lights
	ActA_BankSol(0, 4);}																// reset drop targets

void PB_EnergyOff(byte Dummy) {
	if (PB_EnergyActive) {
		RemoveBlinkLamp(34);															// stop blinking of energy lamp
		PB_EnergyActive = false;													// energy value off
		PB_DropRamp = true;}}															// ramp needs to be dropped

void PB_CycleDropLights(byte State) {
	if (State) {
		if (DropBlinkLamp) {															// blink lamp active?
			if (DropBlinkLamp == 43) {											// last lamp blinking?
				AddBlinkLamp(41, 100);												// start again with the first one
				RemoveBlinkLamp(43);													// remove the current one
				DropBlinkLamp = 41;}													// reset the number of the currently blinking lamp
			else {																					// not the last one
				AddBlinkLamp(DropBlinkLamp+1, 100);						// start the next one
				RemoveBlinkLamp(DropBlinkLamp);								// remove the current one
				DropBlinkLamp++;}															// increase number of currently blinking lamp
			ActivateTimer(3000, 1, PB_CycleDropLights);}}
	else {
		if (DropBlinkLamp) {															// blink lamp active?
			RemoveBlinkLamp(DropBlinkLamp);
			DropBlinkLamp = 0;}}}

void PB_BallEnd(byte Event) {													// ball has been kicked into trunk
	AppByte = PB_CountBallsInTrunk();
	if ((AppByte == 5)||(AppByte < 3-Multiballs-InLock)) {
		InLock = 0;
		if (Multiballs == 1) {
			for (i=0; i<2; i++) {                         	// check how many balls are on the ball ramp
				if (Switch[41+i]) {
					InLock++;}}}
		WriteLower(" BALL   ERROR ");
		if (Event < 11) {                                	// have I been here already?
			Event++;
			ActivateTimer(1000, Event, PB_BallEnd);}        // if not try again in 1s
		else {                                          	// ball may be still in outhole
			BlockOuthole = false;
			Event = 0;
			PB_ClearOuthole(0);}}
	else {
		if (Multiballs == 2) {														// multiball running?
			Multiballs = 1;																	// turn it off
			if (AppByte == 2) {															// 2 balls detected in the trunk
				ActivateTimer(1000, 0, PB_BallEnd);}					// come back and check again
			else {
				BlockOuthole = false;}}												// remove outhole block
		else {
			LockedBalls[Player] = 0;
			//BonusToAdd = Bonus;
			//BonusCountTime = 20;
			//CountBonus(AppByte);
			PB_CycleDropLights(0);
			if (BlinkScoreTimer) {
				KillTimer(BlinkScoreTimer);
				BlinkScoreTimer = 0;}
			if (BallWatchdogTimer) {
				KillTimer(BallWatchdogTimer);
				BallWatchdogTimer = 0;}
			BlockOuthole = false;														// remove outhole block
			if (ExBalls) {                                	// Player has extra balls
				AddBlinkLamp(33, 250);                      	// Let the extra ball lamp blink
				ExBalls--;
				if (PB_SkillMultiplier < 10) {
					PB_SkillMultiplier++;
					//					if (PB_SkillMultiplier == 10) {
					//						Super Skill Shot
					//					}
				}
				//ActivateTimer(2000, 0, AfterExBallRelease);
				ActivateTimer(1000, AppByte, PB_NewBall);}		// remove outhole block
			else {                                        	// Player has no extra balls
				PB_SkillMultiplier = 1;
				if ((Points[Player] > HallOfFame.Scores[3]) && (Ball == APC_settings[NofBalls])) { // last ball & high score?
					Switch_Pressed = DummyProcess;              // Switches do nothing
					CheckHighScore(Player);}
				else {
					if (Player < NoPlayers) {                 	// last player?
						Player++;
						ActivateTimer(1000, AppByte, PB_NewBall);}
					else {
						if (Ball < APC_settings[NofBalls]) {			// last ball?
							Player = 1;															// not yet
							Ball++;
							ActivateTimer(1000, AppByte, PB_NewBall);}
						else {																		// game end
							ReleaseSolenoid(23);                  	// disable flipper fingers
							ReleaseSolenoid(24);
							//PB_CheckForLockedBalls(0);
							Lamp[3] = false;                      	// turn off Ball in Play lamp
							GameDefinition.AttractMode();}}}}}}}

void PB_Testmode(byte Select) {
	Switch_Pressed = PB_Testmode;
	switch(AppByte) {																		// which testmode?
	case 0:																					// display test
		switch(Select) {																	// switch events
		case 0:																						// init (not triggered by switch)
			*(DisplayLower) = 0;														// clear credit display
			*(DisplayLower+16) = 0;
			*(DisplayUpper) = 0;
			*(DisplayUpper+16) = 0;
			WriteUpper("DISPLAY TEST  ");
			WriteLower("              ");
			AppByte2 = 0;
			break;
		case 3:																						// credit button
			WriteUpper("0000000000000000");
			WriteLower("0000000000000000");
			AppByte2 = ActivateTimer(1000, 32, PB_DisplayCycle);
			break;
		case 72:																					// advance button
			if (AppByte2) {
				KillTimer(AppByte2);
				AppByte2 = 0;}
			else {
				AppByte++;}
			PB_Testmode(0);}
		break;
	case 1:																				// switch edges test
		switch(Select) {																	// switch events
		case 0:																						// init (not triggered by switch)
			AppByte2 = 0;
			WriteUpper(" SWITCHEDGES  ");
			WriteLower("              ");
			break;
		case 72:																					// advance button
			if (AppByte2) {
				AppByte2 = 0;}
			else {
				AppByte++;}
			PB_Testmode(0);
			break;
		case 3:																						// credit button
			if (!AppByte2) {
				WriteUpper(" LATESTEDGES  ");
				AppByte2 = 1;
				break;}
		default:																					// all other switches
			ConvertToBCD(Select);                         	// convert the switch number to BCD
			for (i=1; i<24; i++) {                        	// move all characters in the lower display row 4 chars to the left
				DisplayLower[i] = DisplayLower[i+8];}
			*(DisplayLower+28) = DispPattern2[32 + 2 * ByteBuffer2]; // and insert the two result digits to the right of the row
			*(DisplayLower+29) = DispPattern2[33 + 2 * ByteBuffer2];
			*(DisplayLower+30) = DispPattern2[32 + 2 * ByteBuffer];
			*(DisplayLower+31) = DispPattern2[33 + 2 * ByteBuffer];}
		break;
	case 2:																				// solenoid test
		switch(Select) {																	// switch events
		case 0:																						// init (not triggered by switch)
			WriteUpper("  COIL  TEST  ");
			WriteLower("              ");
			AppByte2 = 0;
			break;
		case 3:
			WriteUpper(" FIRINGCOIL NO");
			AppBool = false;
			AppByte2 = ActivateTimer(1000, 1, PB_FireSolenoids);
			break;
		case 72:
			if (AppByte2) {
				KillTimer(AppByte2);
				AppByte2 = 0;}
			else {
				AppByte++;}
			PB_Testmode(0);}
		break;
	case 3:																							// single lamp test
		switch(Select) {																	// switch events
		case 0:																						// init (not triggered by switch)
			WriteUpper(" SINGLE LAMP  ");
			WriteLower("              ");
			AppByte2 = 0;
      for (i=0; i<(LampMax+1); i++){                  // erase lamp matrix
        Lamp[i] = false;}
      LampPattern = Lamp;                             // and show it
			break;
		case 3:
			WriteUpper(" ACTUAL LAMP  ");
			AppByte2 = ActivateTimer(1000, 1, PB_ShowLamp);
			break;
		case 72:
			LampPattern = NoLamps;
			if (AppByte2) {
				KillTimer(AppByte2);
				AppByte2 = 0;}
			else {
				AppByte++;}
			PB_Testmode(0);}
		break;
	case 4:																							// all lamps test
		switch(Select) {																	// switch events
		case 0:																						// init (not triggered by switch)
			WriteUpper("  ALL   LAMPS ");
			WriteLower("              ");
			AppByte2 = 0;
			break;
		case 3:
			WriteUpper("FLASHNG LAMPS ");
			AppByte2 = ActivateTimer(1000, 1, PB_ShowAllLamps);
			break;
		case 72:
			LampPattern = NoLamps;
			if (AppByte2) {
				KillTimer(AppByte2);
				AppByte2 = 0;}
			else {
				AppByte++;}
			PB_Testmode(0);}
		break;
	case 5:																							// all lamps test
		switch(Select) {																	// switch events
		case 0:																						// init (not triggered by switch)
			WriteUpper(" MUSIC  TEST  ");
			WriteLower("              ");
			AppByte2 = 0;
			break;
		case 3:
			WriteUpper("PLAYING MUSIC ");
			AfterMusic = PB_NextTestSound;
			AppByte2 = 1;
			PlayMusic((char*) TestSounds[0]);
			break;
		case 72:
	    AfterMusic = 0;
			StopPlayingMusic();
			if (AppByte2) {
				AppByte2 = 0;}
			else {
				AppByte++;}
			PB_Testmode(0);}
		break;
	case 6:																							// visor test
		switch(Select) {																	// switch events
		case 0:																						// init (not triggered by switch)
			WriteUpper(" VISOR  TEST  ");
			WriteLower("              ");
			AppByte2 = 0;
			AppBool = false;
			break;
		case 3:
			AppByte2 = 1;
			if (AppBool) {
				if (Switch[46]) {
					AppBool = false;
					PB_Testmode(3);}
				else {
					WriteUpper("CLOSING VISOR ");
					ActivateSolenoid(0, 13);}}
			else {
				if (Switch[47]) {
					AppBool = true;
					PB_Testmode(3);}
				else {
					WriteUpper("OPENING VISOR ");
					ActivateSolenoid(0, 13);}}
			break;
		case 46:
			if (AppBool) {
				ReleaseSolenoid(13);
				AppBool = false;
				WriteUpper(" VISOR CLOSED ");}
			break;
		case 47:
			if (!AppBool) {
				ReleaseSolenoid(13);
				AppBool = true;
				WriteUpper(" VISOR  OPEN  ");}
			break;
		case 72:
			ReleaseSolenoid(13);
			if (AppByte2) {
				AppByte2 = 0;}
			else {
				// AppByte++;}
				GameDefinition.AttractMode();
				return;}
			PB_Testmode(0);
		break;
	}}}

void PB_ShowLamp(byte CurrentLamp) {                  // cycle all solenoids
  if (!digitalRead(UpDown)) {
    ConvertToBCD(CurrentLamp);                        // convert the actual solenoid number
    *(DisplayLower+28) = DispPattern2[32 + 2 * ByteBuffer2]; // and show it
    *(DisplayLower+29) = DispPattern2[33 + 2 * ByteBuffer2];
    *(DisplayLower+30) = DispPattern2[32 + 2 * ByteBuffer];
    *(DisplayLower+31) = DispPattern2[33 + 2 * ByteBuffer];
    Lamp[CurrentLamp] = true;                         // turn on lamp
    if (CurrentLamp > 1) {                            // and turn off the previous one
      Lamp[CurrentLamp-1] = false;}
    else {
      Lamp[LampMax] = false;}
    CurrentLamp++;                                    // increase the lamp counter
    if (CurrentLamp == LampMax+1) {                   // maximum reached?
    	CurrentLamp = 1;}}                              // then start again
  AppByte2 = ActivateTimer(1000, CurrentLamp, PB_ShowLamp);} // come back in one second

void PB_ShowAllLamps(byte State) {                    // Flash all lamps
  if (State) {                                     		// if all lamps are on
    LampPattern = NoLamps;                            // turn them off
    State = 0;}
  else {                                              // or the other way around
    LampPattern = AllLamps;
    State = 1;}
  AppByte2 = ActivateTimer(500, State, PB_ShowAllLamps);}  // come back in 500ms

void PB_FireSolenoids(byte Solenoid) {                // cycle all solenoids
	if (AppBool) {																			// if C bank solenoid
		ActC_BankSol(Solenoid);
		*(DisplayLower+30) = DispPattern2[('C'-32)*2];		// show the C
		*(DisplayLower+31) = DispPattern2[('C'-32)*2+1];
		if (!digitalRead(UpDown)) {
			AppBool = false;
			Solenoid++;}}
	else {																							// if A bank solenoid
		ConvertToBCD(Solenoid);                           // convert the actual solenoid number
		*(DisplayLower+26) = DispPattern2[32 + 2 * ByteBuffer2]; // and show it
		*(DisplayLower+27) = DispPattern2[33 + 2 * ByteBuffer2];
		*(DisplayLower+28) = DispPattern2[32 + 2 * ByteBuffer];
		*(DisplayLower+29) = DispPattern2[33 + 2 * ByteBuffer];
    if (Solenoid == 13 || Solenoid == 14) {						// is it the visor motor or the AC relay?
      ActivateSolenoid(999, Solenoid);}								// then the duration must be specified
    else {
		  ActivateSolenoid(0, Solenoid);}                 // activate the solenoid
		if (Solenoid < 9) {
			*(DisplayLower+30) = DispPattern2[('A'-32)*2];	// show the A
			*(DisplayLower+31) = DispPattern2[('A'-32)*2+1];
			if (!digitalRead(UpDown)) {
				AppBool = true;}}
		else {
			*(DisplayLower+30) = DispPattern2[(' '-32)*2];	// delete the C
			*(DisplayLower+31) = DispPattern2[(' '-32)*2+1];
			if (!digitalRead(UpDown)) {
				Solenoid++;                                   // increase the solenoid counter
				if (Solenoid == 22) {                         // maximum reached?
					Solenoid = 1;}}}}                           // then start again
	AppByte2 = ActivateTimer(1000, Solenoid, PB_FireSolenoids);}   // come back in one second

void PB_DisplayCycle(byte CharNo) {                   // Display cycle test
  if (!digitalRead(UpDown)) {                         // cycle only if Up/Down switch is not pressed
    if (CharNo == 116) {                             	// if the last character is reached
    	CharNo = 32;}                                 	// start from the beginning
    else {
      if (CharNo == 50) {                            	// reached the gap between numbers and characters?
      	CharNo = 66;}
      else {
      	CharNo = CharNo+2;}}                       		// otherwise show next character
    for (i=0; i<16; i++) {                            // use for all alpha digits
      if ((i==0) || (i==8)) {
        DisplayUpper[2*i] = LeftCredit[CharNo];
        DisplayUpper[2*i+1] = LeftCredit[CharNo+1];
        DisplayLower[2*i] = RightCredit[CharNo];
        DisplayLower[2*i+1] = RightCredit[CharNo+1];}
      else {
        DisplayUpper[2*i] = DispPattern1[CharNo];
        DisplayUpper[2*i+1] = DispPattern1[CharNo+1];
        DisplayLower[2*i] = DispPattern2[CharNo];
        DisplayLower[2*i+1] = DispPattern2[CharNo+1];}}}
  AppByte2 = ActivateTimer(500, CharNo, PB_DisplayCycle);}   // restart timer

void PB_NextTestSound() {
  if (!digitalRead(UpDown)) {
	  AppByte++;}
	if (!TestSounds[AppByte][0]) {
		AppByte = 0;}
  ConvertToBCD(AppByte+1);                           	// convert the actual solenoid number
  *(DisplayLower+28) = DispPattern2[32 + 2 * ByteBuffer2]; // and show it
  *(DisplayLower+29) = DispPattern2[33 + 2 * ByteBuffer2];
  *(DisplayLower+30) = DispPattern2[32 + 2 * ByteBuffer];
  *(DisplayLower+31) = DispPattern2[33 + 2 * ByteBuffer];
	PlayMusic((char*) TestSounds[AppByte]);}


