// The basic Rollergames game

const byte RG_OutholeSwitch = 9;											// number of the outhole switch
const byte RG_BallThroughSwitches[3] = {13,12,11};		// ball through switches from the plunger lane to the outhole
const byte RG_PlungerLaneSwitch = 20;
const byte RG_ACselectRelay = 12;											// solenoid number of the A/C select relay - set it to 0 if the game doesn't have one
const byte RG_OutholeKicker = 1;											// solenoid number of the outhole kicker
const byte RG_ShooterLaneFeeder = 2;									// solenoid number of the shooter lane feeder
const byte RG_InstalledBalls = 3;											// number of balls installed in the game
const byte RG_SearchCoils[15] = {1,4,6,8,13,15,16,17,18,19,20,21,22,14,0}; // coils to fire when the ball watchdog timer runs out - has to end with a zero
const unsigned int RG_SolTimes[32] = {30,30,50,50,50,50,30,999,100,999,999,999,50,50,50,50,50,999,50,50,50,999,0,0,100,100,100,100,100,100,100,100}; // Activation times for solenoids

const byte RG_defaults[64] = {0,0,0,0,0,0,0,0,		 		// game default settings
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0};

struct SettingTopic RG_setList[5] = {{"  GOD   MODE  ",HandleBoolSetting,0,0,0}, // defines the game specific settings
		{" RESET  HIGH  ",RG_ResetHighScores,0,0,0},
		{"RESTOREDEFAULT",RestoreDefaults,0,0,0},
		{"  EXIT SETTNGS",ExitSettings,0,0,0},
		{"",NULL,0,0,0}};

																		// Duration..11111110...22222111...33322222...43333333...44444444...55555554...66666555
																		// Duration..65432109...43210987...21098765...09876543...87654321...65432109...43210987
const struct LampPat RG_AttractPat1[57] ={{250,0b00000001,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000010,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000100,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00001000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00010000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00100000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b01000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b10000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000001,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000010,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000100,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00001000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00010000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00100000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b01000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b10000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000001,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000100,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00001000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00010000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00100000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b10000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000001,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000100,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00001000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00010000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00100000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b10000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000001,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000100,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00010000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00100000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b10000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000001,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000010,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000100,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00010000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00100000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b10000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000001},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000010},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000100},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00010000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00100000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b01000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b10000000},
																					{0,0,0,0,0,0,0,0}};

const struct LampPat RG_AttractPat2[57] ={{250,0b00000001,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000010,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000100,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00001000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00010000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00100000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b01000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b10000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000001,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000010,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000100,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00001000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00010000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00100000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b01000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b10000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000001,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000100,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00001000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00010000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00100000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b10000000,0b00000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000001,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000100,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00001000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00010000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00100000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b10000000,0b00000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000001,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000100,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00010000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00100000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b10000000,0b00000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000001,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000010,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000100,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00010000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00100000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b10000000,0b00000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000001},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000010},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000100},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00010000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00100000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b01000000},
																					{250,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b10000000},
																					{0,0,0,0,0,0,0,0}};

const struct LampFlow RG_AttractFlow[3] = {{3,RG_AttractPat1},{10,RG_AttractPat2},{0,0}};

struct GameDef RG_GameDefinition = {
		RG_setList,																				// GameSettingsList
		(byte*)RG_defaults,																// GameDefaultsPointer
		"RG_SET.BIN",																			// GameSettingsFileName
		"RG_SCORE.BIN",																		// HighScoresFileName
		RG_AttractMode,																		// AttractMode
		RG_SolTimes};																			// Default activation times of solenoids

void RG_init() {
	ACselectRelay = RG_ACselectRelay;										// assign the number of the A/C select relay
	GameDefinition = RG_GameDefinition;}								// read the game specific settings and highscores

void RG_AttractMode() {                               // Attract Mode
	DispRow1 = DisplayUpper;
	DispRow2 = DisplayLower;
	WriteUpper("MY NEW GAME     ");
	LampPattern = LampColumns;													// point to the standard lamp array
	TurnOnLamp(53);
	AddBlinkLamp(54, 250);
	Switch_Pressed = RG_TestSW;
	Switch_Released = DummyProcess;}

//	digitalWrite(VolumePin,HIGH);                       // set volume to zero
//	LampPattern = NoLamps;

//	AppByte2 = 0;
//	LampReturn = RG_AttractLampCycle;
//	ActivateTimer(1000, 0, RG_AttractLampCycle);
//	RG_AttractDisplayCycle(0);}

void RG_AttractLampCycle(byte Event) {                // play multiple lamp pattern series
	UNUSED(Event);
	PatPointer = RG_AttractFlow[AppByte2].FlowPat;      // set the pointer to the current series
	FlowRepeat = RG_AttractFlow[AppByte2].Repeat;       // set the repetitions
	AppByte2++;                                         // increase counter
	if (!RG_AttractFlow[AppByte2].Repeat) {             // repetitions of next series = 0?
		AppByte2 = 0;}                                    // reset counter
	ShowLampPatterns(0);}                               // call the player

void RG_AttractDisplayCycle(byte Step) {
	RG_CheckForLockedBalls(0);
	switch (Step) {
	case 0:
		WriteUpper2("APC BASE CODE   ");
		ActivateTimer(50, 0, ScrollUpper);
		WriteLower2("                ");
		ActivateTimer(1000, 0, ScrollLower2);
		if (NoPlayers) {																	// if there were no games before skip the next step
			Step++;}
		else {
			Step = 2;}
		break;
	case 1:
		WriteUpper2("                ");                  // erase display
		WriteLower2("                ");
		for (i=1; i<=NoPlayers; i++) {                  	// display the points of all active players
			ShowNumber(8*i-1, Points[i]);}
		ActivateTimer(50, 0, ScrollUpper);
		ActivateTimer(900, 0, ScrollLower2);
		Step++;
		break;
	case 2:																							// Show highscores
		WriteUpper2("1>              ");
		WriteLower2("2>              ");
		for (i=0; i<3; i++) {
			*(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[i]-32)*2];
			*(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[i]-32)*2+1];
			*(DisplayLower2+8+2*i) = DispPattern2[(HallOfFame.Initials[3+i]-32)*2];
			*(DisplayLower2+8+2*i+1) = DispPattern2[(HallOfFame.Initials[3+i]-32)*2+1];}
		ShowNumber(15, HallOfFame.Scores[0]);
		ShowNumber(31, HallOfFame.Scores[1]);
		ActivateTimer(50, 0, ScrollUpper);
		ActivateTimer(900, 0, ScrollLower2);
		Step++;
		break;
	case 3:
		WriteUpper2("3>              ");
		WriteLower2("4>              ");
		for (i=0; i<3; i++) {
			*(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2];
			*(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2+1];
			*(DisplayLower2+8+2*i) = DispPattern2[(HallOfFame.Initials[9+i]-32)*2];
			*(DisplayLower2+8+2*i+1) = DispPattern2[(HallOfFame.Initials[9+i]-32)*2+1];}
		ShowNumber(15, HallOfFame.Scores[2]);
		ShowNumber(31, HallOfFame.Scores[3]);
		ActivateTimer(50, 0, ScrollUpper);
		ActivateTimer(900, 0, ScrollLower2);
		Step = 0;}
	ActivateTimer(4000, Step, RG_AttractDisplayCycle);}

void RG_AttractModeSW(byte Button) {                  // Attract Mode switch behaviour
	switch (Button) {
	case 8:                                             // high score reset
		digitalWrite(Blanking, LOW);                      // invoke the blanking
		break;
	case 20:                                            // outhole
		ActivateTimer(200, 0, RG_CheckForLockedBalls);    // check again in 200ms
		break;
	case 72:                                            // Service Mode
		BlinkScore(0);
    KillAllTimers();
		BallWatchdogTimer = 0;
		CheckReleaseTimer = 0;
    ByteBuffer3 = 0;
    LampPattern = NoLamps;                            // Turn off all lamps
    ReleaseAllSolenoids();
    if (!QuerySwitch(73)) {														// Up/Down switch pressed?
      WriteUpper("  TEST  MODE    ");
      WriteLower("                ");
      AppByte = 0;
      ActivateTimer(1000, 0, RG_Testmode);}
    else {
      Settings_Enter();}
		break;
	case 3:																							// start game
		if (RG_CountBallsInTrunk() == RG_InstalledBalls || (RG_CountBallsInTrunk() == RG_InstalledBalls-1 && QuerySwitch(RG_PlungerLaneSwitch))) { // Ball missing?
			Switch_Pressed = DummyProcess;                  // Switches do nothing
      KillAllTimers();
      ByteBuffer3 = 0;
			if (APC_settings[Volume]) {                     // system set to digital volume control?
				analogWrite(VolumePin,255-APC_settings[Volume]);} // adjust PWM to volume setting
			else {
				digitalWrite(VolumePin,HIGH);}                // turn off the digital volume control
			for (i=0; i< 8; i++) {													// turn off all lamps
				LampColumns[i] = 0;}
			LampPattern = LampColumns;
			NoPlayers = 0;
			WriteUpper("                ");
			WriteLower("                ");
			Ball = 1;
			RG_AddPlayer();
			Player = 1;
			ExBalls = 0;
			Bonus = 1;
			BonusMultiplier = 1;
			InLock = 0;
			Multiballs = 1;
			for (i=1; i < 5; i++) {
				LockedBalls[i] = 0;
				Points[i] = 0;}
			RG_NewBall(3);                                  // release a new ball (3 expected balls in the trunk)
			ActivateSolenoid(0, 23);                        // enable flipper fingers
			ActivateSolenoid(0, 24);}}}

void RG_AddPlayer() {
	if ((NoPlayers < 4) && (Ball == 1)) {               // if actual number of players < 4
		NoPlayers++;                                      // add a player
		Points[NoPlayers] = 0;                            // delete the points of the new player
		ShowPoints(NoPlayers);}}                          // and show them

void RG_CheckForLockedBalls(byte Event) {             // check if balls are locked and release them
	UNUSED(Event);
	if (QuerySwitch(RG_OutholeSwitch)) {                     // for the outhole
		ActA_BankSol(RG_OutholeKicker);}
}																											// add the locks of your game here

void RG_NewBall(byte Balls) {                         // release ball (Event = expected balls on ramp)
	ShowAllPoints(0);
	if (APC_settings[DisplayType] < 2) {               	// credit display present?
		*(DisplayUpper+16) = LeftCredit[32 + 2 * Ball];} 	// show current ball in left credit
	BlinkScore(1);																			// start score blinking
	Switch_Released = RG_CheckShooterLaneSwitch;
	if (!QuerySwitch(RG_PlungerLaneSwitch)) {
		ActA_BankSol(RG_ShooterLaneFeeder);               // release ball
		Switch_Pressed = RG_BallReleaseCheck;             // set switch check to enter game
		CheckReleaseTimer = ActivateTimer(5000, Balls-1, RG_CheckReleasedBall);} // start release watchdog
	else {
		Switch_Pressed = RG_ResetBallWatchdog;}}

void RG_CheckShooterLaneSwitch(byte Switch) {
	if (Switch == RG_PlungerLaneSwitch) {               // shooter lane switch released?
		Switch_Released = DummyProcess;
		if (!BallWatchdogTimer) {
			BallWatchdogTimer = ActivateTimer(30000, 0, RG_SearchBall);}}}

void RG_BallReleaseCheck(byte Switch) {               // handle switches during ball release
	if (Switch > 15) { 																	// edit this to be true only for playfield switches
		if (CheckReleaseTimer) {
			KillTimer(CheckReleaseTimer);
			CheckReleaseTimer = 0;}                   			// stop watchdog
		Switch_Pressed = RG_ResetBallWatchdog;
		if (Switch == RG_PlungerLaneSwitch) {							// ball is in the shooter lane
			Switch_Released = RG_CheckShooterLaneSwitch;}		// set mode to register when ball is shot
		else {
			if (!BallWatchdogTimer) {
				BallWatchdogTimer = ActivateTimer(30000, 0, RG_SearchBall);}}} // set switch mode to game
	RG_GameMain(Switch);}                               // process current switch

void RG_ResetBallWatchdog(byte Switch) {              // handle switches during ball release
	if (Switch > 15) { 																	// edit this to be true only for playfield switches
		if (BallWatchdogTimer) {
			KillTimer(BallWatchdogTimer);}                  // stop watchdog
		BallWatchdogTimer = ActivateTimer(30000, 0, RG_SearchBall);}
	RG_GameMain(Switch);}                               // process current switch

void RG_SearchBall(byte Counter) {										// ball watchdog timer has run out
	BallWatchdogTimer = 0;
	if (QuerySwitch(RG_OutholeSwitch)) {
		BlockOuthole = false;
		ActivateTimer(1000, 0, RG_ClearOuthole);}
	else {
		if (QuerySwitch(RG_PlungerLaneSwitch)) {					// if ball is waiting to be launched
			BallWatchdogTimer = ActivateTimer(30000, 0, RG_SearchBall);}	// restart watchdog
		else {  																					// if ball is really missing
			byte c = RG_CountBallsInTrunk();								// recount all balls
			if (c == RG_InstalledBalls) {										// found all balls in trunk?
				if (BlockOuthole) {														// is the outhole blocked
					RG_BallEnd(0);}															// then it was probably a ball loss gone wrong
				else {
					ActivateTimer(1000, RG_InstalledBalls, RG_NewBall);}}	// otherwise try it with a new ball
			else {
				byte c2 = 0;																	// counted balls in lock
									// count balls in lock here with 5 being a warning when the switch states don't add up
				if (c == 5) {																	// balls have not settled yet
					WriteUpper("  LOCK  STUCK   ");
					BallWatchdogTimer = ActivateTimer(1000, 0, RG_SearchBall);} // and try again in 1s
				else {
					if (c2 > InLock) {													// more locked balls found than expected?
						RG_HandleLock(0);													// lock them
						BallWatchdogTimer = ActivateTimer(30000, 0, RG_SearchBall);}
					else {
						WriteUpper("  BALL  SEARCH  ");
						ActivateSolenoid(0, RG_SearchCoils[Counter]); // fire coil to get ball free
						Counter++;
						if (!RG_SearchCoils[Counter]) {						// all coils fired?
							Counter = 0;}														// start again
						BallWatchdogTimer = ActivateTimer(1000, Counter, RG_SearchBall);}}}}}} // come again in 1s if no switch is activated

byte RG_CountBallsInTrunk() {
	byte Balls = 0;
	for (i=0; i<RG_InstalledBalls; i++) {               // check how many balls are on the ball ramp
		if (QuerySwitch(*(RG_BallThroughSwitches+i))) {
			if (Balls < i) {
				return 5;}                                    // send warning
			Balls++;}}
	return Balls;}

void RG_CheckReleasedBall(byte Balls) {               // ball release watchdog
	CheckReleaseTimer = 0;
	BlinkScore(0); 																			// stop blinking to show messages
	WriteUpper("WAITINGFORBALL  ");                       // indicate a problem
	WriteLower("                ");
	if (Balls == 10) {																	// indicating a previous trunk error
		WriteUpper("                ");
		WriteLower("                ");
		ShowAllPoints(0);
		BlinkScore(1);
		ActA_BankSol(RG_ShooterLaneFeeder);}
	byte c = RG_CountBallsInTrunk();
	if (c == Balls) {                             			// expected number of balls in trunk
		WriteUpper("  BALL MISSING  ");
		if (QuerySwitch(RG_OutholeSwitch)) {                   // outhole switch still active?
			ActA_BankSol(RG_OutholeKicker);}}								// shove the ball into the trunk
	else {																							//
		if (c == 5) {																			// balls not settled
			WriteLower(" TRUNK  ERROR   ");
			Balls = 10;}
		else {
			if ((c > Balls) || !c) {												// more balls in trunk than expected or none at all
				WriteUpper("                ");
				WriteLower("                ");
				ShowAllPoints(0);
				BlinkScore(1);
				ActA_BankSol(RG_ShooterLaneFeeder);}}}        // release again
	CheckReleaseTimer = ActivateTimer(5000, Balls, RG_CheckReleasedBall);}

void RG_GameMain(byte Event) {                        // game switch events
	switch (Event) {
	case 1:                                             // plumb bolt tilt
	case 2:                                             // ball roll tilt
	case 7:                                             // slam tilt
		break;
	case RG_OutholeSwitch:
		ActivateTimer(200, 0, RG_ClearOuthole);           // check again in 200ms
		break;
	case 46:                                            // playfield tilt
		WriteUpper(" TILT  WARNING  ");
		ActivateTimer(3000, 0, ShowAllPoints);
		break;
	case 3:                                             // credit button
		RG_AddPlayer();
		break;

	}}

void RG_ClearOuthole(byte Event) {
	UNUSED(Event);
	if (QuerySwitch(RG_OutholeSwitch)) {                     // outhole switch still active?
		if (!BlockOuthole && !C_BankActive) {							// outhole blocked?
			BlockOuthole = true;														// block outhole until this ball has been processed
			ActivateSolenoid(30, RG_OutholeKicker);         // put ball in trunk
			ActivateTimer(2000, 0, RG_BallEnd);}
		else {
			ActivateTimer(2000, 0, RG_ClearOuthole);}}}			// come back in 2s if outhole is blocked

void RG_HandleLock(byte Balls) {
			// do something with your lock
}

void RG_BallEnd(byte Event) {
	byte BallsInTrunk = RG_CountBallsInTrunk();
	if ((BallsInTrunk == 5)||(BallsInTrunk < RG_InstalledBalls+1-Multiballs-InLock)) {
		InLock = 0;
//		if (Multiballs == 1) {
//			for (i=0; i<3; i++) {                         	// Count your locked balls here
//				if (Switch[41+i]) {
//					InLock++;}}}
		WriteLower(" BALL   ERROR   ");
		if (QuerySwitch(RG_OutholeSwitch)) {                   // ball still in outhole?
			ActA_BankSol(RG_OutholeKicker);                 // make the coil a bit stronger
			ActivateTimer(2000, Event, RG_BallEnd);}        // and come back in 2s
		else {
			if (Event < 11) {                               // have I been here already?
				Event++;
				ActivateTimer(1000, Event, RG_BallEnd);}      // if not try again in 1s
			else {                                          // ball may be still in outhole
				BlockOuthole = false;
				Event = 0;
				RG_ClearOuthole(0);}}}
	else {
		switch (Multiballs) {
		case 3:                                           // goto 2 ball multiball
			Multiballs = 2;
			if (BallsInTrunk != 1) {                        // not 1 ball in trunk
				ActivateTimer(1000, 0, RG_BallEnd);}          // check again later
			else {
				BlockOuthole = false;}												// remove outhole block
			break;
		case 2:                                           // end multiball
			Multiballs = 1;
			if (BallsInTrunk == 3) {                        // 3 balls in trunk?
				ActivateTimer(1000, 0, RG_BallEnd);}
			else {
				BlockOuthole = false;}												// remove outhole block
			break;
		case 1:                                           // end of ball
			if (BallsInTrunk + InLock != RG_InstalledBalls) {
				WriteUpper(" COUNT  ERROR   ");
				InLock = 0;
//				for (i=0; i<3; i++) {                       // check how many balls are on the ball ramp
//					if (Switch[41+i]) {
//						InLock++;}}
				ActivateTimer(1000, 0, RG_BallEnd);}
			else {
				LockedBalls[Player] = 0;
				BlinkScore(0);																// stop score blinking
				RG_BallEnd2(BallsInTrunk);										// add bonus count here and start BallEnd2 afterwards
			}}}}

void RG_BallEnd2(byte Balls) {
	if (BallWatchdogTimer) {
		KillTimer(BallWatchdogTimer);
		BallWatchdogTimer = 0;}
	if (ExBalls) {                                			// Player has extra balls
		ExBalls--;
		ActivateTimer(1000, AppByte, RG_NewBall);
		BlockOuthole = false;}														// remove outhole block
	else {                                        			// Player has no extra balls
		if ((Points[Player] > HallOfFame.Scores[3]) && (Ball == APC_settings[NofBalls])) { // last ball & high score?
			Switch_Pressed = DummyProcess;                                // Switches do nothing
			RG_CheckHighScore(Player);}
		else {
			RG_BallEnd3(Balls);}}}

void RG_BallEnd3(byte Balls) {
	BlockOuthole = false;																// remove outhole block
	if (Player < NoPlayers) {                 					// last player?
		Player++;
		ActivateTimer(1000, Balls, RG_NewBall);}
	else {
		if (Ball < APC_settings[NofBalls]) {
			Player = 1;
			Ball++;
			ActivateTimer(1000, Balls, RG_NewBall);}
		else {																						// game end
			ReleaseSolenoid(23);                  					// disable flipper fingers
			ReleaseSolenoid(24);
			RG_CheckForLockedBalls(0);
			GameDefinition.AttractMode();}}}

void RG_ResetHighScores(bool change) {								// delete the high scores file
	if (change) {																				// if the start button has been pressed
		if (SDfound) {
			SD.remove(GameDefinition.HighScoresFileName);
			struct HighScores HScBuffer;										// create new high score table
			HallOfFame = HScBuffer;													// copy it to the hall of fame
			WriteLower(" SCORES DONE    ");}
		else {
			WriteLower(" SCORES NO SD   ");}}
	else {
		WriteLower(" SCORES         ");}}

void RG_CheckHighScore(byte Player) {

}

// Test mode

void RG_Testmode(byte Select) {
	Switch_Pressed = RG_Testmode;
	switch(AppByte) {																		// which testmode?
	case 0:																							// display test
		switch(Select) {																	// switch events
		case 0:																						// init (not triggered by switch)
			*(DisplayLower) = 0;														// clear credit display
			*(DisplayLower+16) = 0;
			*(DisplayUpper) = 0;
			*(DisplayUpper+16) = 0;
			WriteUpper("DISPLAY TEST    ");
			WriteLower("                ");
			AppByte2 = 0;
			break;
		case 3:																						// credit button
			WriteUpper("0000000000000000");
			WriteLower("0000000000000000");
			AppByte2 = ActivateTimer(1000, 32, RG_DisplayCycle);
			break;
		case 72:																					// advance button
			if (AppByte2) {
				KillTimer(AppByte2);
				AppByte2 = 0;}
			else {
				AppByte++;}
			RG_Testmode(0);}
		break;
		case 1:																						// switch edges test
			switch(Select) {																// switch events
			case 0:																					// init (not triggered by switch)
				AppByte2 = 0;
				WriteUpper(" SWITCH EDGES   ");
				WriteLower("                ");
				break;
			case 72:																				// advance button
				if (AppByte2) {
					AppByte2 = 0;}
				else {
					AppByte++;}
				RG_Testmode(0);
				break;
			case 3:																					// credit button
				if (!AppByte2) {
					WriteUpper(" LATEST EDGES   ");
					AppByte2 = 1;
					break;}
			default:																				// all other switches
				for (i=1; i<24; i++) {                        // move all characters in the lower display row 4 chars to the left
					DisplayLower[i] = DisplayLower[i+8];}
				*(DisplayLower+30) = DispPattern2[32 + 2 * (Select % 10)]; // and insert the switch number to the right of the row
				*(DisplayLower+31) = DispPattern2[33 + 2 * (Select % 10)];
				*(DisplayLower+28) = DispPattern2[32 + 2 * (Select - (Select % 10)) / 10];
				*(DisplayLower+29) = DispPattern2[33 + 2 * (Select - (Select % 10)) / 10];}
			break;
			case 2:																					// solenoid test
				switch(Select) {															// switch events
				case 0:																				// init (not triggered by switch)
					WriteUpper("  COIL  TEST    ");
					WriteLower("                ");
					AppByte2 = 0;
					break;
				case 3:
					WriteUpper(" FIRINGCOIL NO  ");
					AppBool = false;
					AppByte2 = ActivateTimer(1000, 1, RG_FireSolenoids);
					break;
				case 72:
					if (AppByte2) {
						KillTimer(AppByte2);
						AppByte2 = 0;}
					else {
						AppByte++;}
					RG_Testmode(0);}
				break;
				case 3:																				// single lamp test
					switch(Select) {														// switch events
					case 0:																			// init (not triggered by switch)
						WriteUpper(" SINGLE LAMP    ");
						WriteLower("                ");
						AppByte2 = 0;
						for (i=0; i<8; i++){            					// erase lamp matrix
							LampColumns[i] = 0;}
						LampPattern = LampColumns;                // and show it
						break;
					case 3:
						WriteUpper(" ACTUAL LAMP    ");
						AppByte2 = ActivateTimer(1000, 1, RG_ShowLamp);
						break;
					case 72:
						LampPattern = NoLamps;
						if (AppByte2) {
							KillTimer(AppByte2);
							AppByte2 = 0;}
						else {
							AppByte++;}
						RG_Testmode(0);}
					break;
					case 4:																			// all lamps test
						switch(Select) {													// switch events
						case 0:																		// init (not triggered by switch)
							WriteUpper("  ALL   LAMPS   ");
							WriteLower("                ");
							AppByte2 = 0;
							break;
						case 3:
							WriteUpper("FLASHNG LAMPS   ");
							AppByte2 = ActivateTimer(1000, 1, RG_ShowAllLamps);
							break;
						case 72:
							LampPattern = NoLamps;
							if (AppByte2) {
								KillTimer(AppByte2);
								AppByte2 = 0;}
							else {
								AppByte++;}
							RG_Testmode(0);}
						break;
						case 5:																		// all music test
							switch(Select) {												// switch events
							case 0:																	// init (not triggered by switch)
								WriteUpper(" MUSIC  TEST    ");
								WriteLower("                ");
								AppByte2 = 0;
								break;
							case 3:
								WriteUpper("PLAYING MUSIC   ");
                if (APC_settings[Volume]) {           // system set to digital volume control?
                  analogWrite(VolumePin,255-APC_settings[Volume]);} // adjust PWM to volume setting
								AfterMusic = RG_NextTestSound;
								AppByte2 = 1;
								PlayMusic(50, (char*) TestSounds[0]);
								break;
							case 72:
								AfterMusic = 0;
                digitalWrite(VolumePin,HIGH);         // set volume to zero
								StopPlayingMusic();
								if (AppByte2) {
									AppByte2 = 0;}
								else {
									GameDefinition.AttractMode();
                  return;}
								RG_Testmode(0);}
							break;
}}

void RG_ShowLamp(byte CurrentLamp) {                  // cycle all solenoids
	if (QuerySwitch(73)) {															// Up/Down switch pressed?
		*(DisplayLower+30) = DispPattern2[32 + 2 * (CurrentLamp % 10)]; // and show the actual solenoid number
		*(DisplayLower+31) = DispPattern2[33 + 2 * (CurrentLamp % 10)];
		*(DisplayLower+28) = DispPattern2[32 + 2 * (CurrentLamp - (CurrentLamp % 10)) / 10];
		*(DisplayLower+29) = DispPattern2[33 + 2 * (CurrentLamp - (CurrentLamp % 10)) / 10];
		TurnOnLamp(CurrentLamp);                         	// turn on lamp
		if (CurrentLamp > 1) {                            // and turn off the previous one
			TurnOffLamp(CurrentLamp-1);}
		else {
			TurnOffLamp(LampMax);}
		CurrentLamp++;                                    // increase the lamp counter
		if (CurrentLamp == LampMax+1) {                   // maximum reached?
			CurrentLamp = 1;}}                              // then start again
	AppByte2 = ActivateTimer(1000, CurrentLamp, RG_ShowLamp);} // come back in one second

void RG_ShowAllLamps(byte State) {                    // Flash all lamps
	if (State) {                                     		// if all lamps are on
		LampPattern = NoLamps;                            // turn them off
		State = 0;}
	else {                                              // or the other way around
		LampPattern = AllLamps;
		State = 1;}
	AppByte2 = ActivateTimer(500, State, RG_ShowAllLamps);}  // come back in 500ms

void RG_FireSolenoids(byte Solenoid) {                // cycle all solenoids
	if (AppBool) {																			// if C bank solenoid
		ActC_BankSol(Solenoid);
		*(DisplayLower+30) = DispPattern2[('C'-32)*2];		// show the C
		*(DisplayLower+31) = DispPattern2[('C'-32)*2+1];
		if (QuerySwitch(73)) {														// Up/Down switch pressed?
			AppBool = false;
			Solenoid++;}}
	else {																							// if A bank solenoid
		*(DisplayLower+28) = DispPattern2[32 + 2 * (Solenoid % 10)]; // show the actual solenoid number
		*(DisplayLower+29) = DispPattern2[33 + 2 * (Solenoid % 10)];
		*(DisplayLower+26) = DispPattern2[32 + 2 * (Solenoid - (Solenoid % 10)) / 10];
		*(DisplayLower+27) = DispPattern2[33 + 2 * (Solenoid - (Solenoid % 10)) / 10];

		if (Solenoid == 11 || Solenoid == 12 || Solenoid == 13 || Solenoid == 14 || Solenoid == 9 || Solenoid == 10 || Solenoid == 18) {	// is it a relay or a #1251 flasher?
			ActivateSolenoid(999, Solenoid);}								// then the duration must be specified
		else {
			ActivateSolenoid(0, Solenoid);}                 // activate the solenoid
		if (Solenoid < 9) {
			*(DisplayLower+30) = DispPattern2[('A'-32)*2];	// show the A
			*(DisplayLower+31) = DispPattern2[('A'-32)*2+1];
			if (QuerySwitch(73)) {													// Up/Down switch pressed?
				AppBool = true;}}
		else {
			*(DisplayLower+30) = DispPattern2[(' '-32)*2];	// delete the C
			*(DisplayLower+31) = DispPattern2[(' '-32)*2+1];
			if (QuerySwitch(73)) {													// Up/Down switch pressed?
				Solenoid++;                                   // increase the solenoid counter
				if (Solenoid == 22) {                         // maximum reached?
					Solenoid = 1;}}}}                           // then start again
	AppByte2 = ActivateTimer(1000, Solenoid, RG_FireSolenoids);}   // come back in one second

void RG_DisplayCycle(byte CharNo) {                   // Display cycle test
	if (QuerySwitch(73)) {                         			// cycle only if Up/Down switch is not pressed
		if (CharNo == 116) {                             	// if the last character is reached
			CharNo = 32;}                                 	// start from the beginning
		else {
			if (CharNo == 50) {                            	// reached the gap between numbers and characters?
				CharNo = 66;}
			else {
				CharNo = CharNo+2;}}                       		// otherwise show next character
		for (i=0; i<16; i++) {                            // use for all alpha digits
			if ((APC_settings[DisplayType] != 3) && ((i==0) || (i==8))) {
				DisplayUpper[2*i] = LeftCredit[CharNo];
				DisplayUpper[2*i+1] = LeftCredit[CharNo+1];
				DisplayLower[2*i] = RightCredit[CharNo];
				DisplayLower[2*i+1] = RightCredit[CharNo+1];}
			else {
				DisplayUpper[2*i] = DispPattern1[CharNo];
				DisplayUpper[2*i+1] = DispPattern1[CharNo+1];
				DisplayLower[2*i] = DispPattern2[CharNo];
				DisplayLower[2*i+1] = DispPattern2[CharNo+1];}}}
	AppByte2 = ActivateTimer(500, CharNo, RG_DisplayCycle);}   // restart timer

void RG_NextTestSound() {
	if (QuerySwitch(73)) {															// Up/Down switch pressed?
		AppByte++;}
	if (!TestSounds[AppByte][0]) {
		AppByte = 0;}
	*(DisplayLower+30) = DispPattern2[32 + 2 * ((AppByte+1) % 10)]; // show the actual solenoid number
	*(DisplayLower+31) = DispPattern2[33 + 2 * ((AppByte+1) % 10)];
	*(DisplayLower+28) = DispPattern2[32 + 2 * ((AppByte+1) - ((AppByte+1) % 10)) / 10];
	*(DisplayLower+29) = DispPattern2[33 + 2 * ((AppByte+1) - ((AppByte+1) % 10)) / 10];
	PlayMusic(50, (char*) TestSounds[AppByte]);}
