// Rules for the Pinbot pinball machine

bool PB_OpenVisorFlag = false;												// visor is being opened if true
bool PB_CloseVisorFlag = false;											  // visor is being closed if true
bool PB_DropWait = false;															// ignore drop target switches when true
bool PB_DropRamp = false;															// ramp needs to be dropped when possible
bool PB_EnergyActive = false;													// score energy active?
bool PB_SkillShot = false;														// is the skill shot active?
bool PB_EjectIgnore = false;                          // ignore the hole switch while the ball is in the hole
bool PB_IgnoreLock = false;                           // ignore the lock switches to cope with switch bouncing
byte PB_ChestMode = 0;																// current status of the chest and visor
uint16_t PB_SolarValue = 100;                         // current solar value / 1000
byte PB_SolarValueTimer = 0;                          // number of the timer for the solar value shot
byte PB_DropTimer = 0;																// number of the drop target timer
byte PB_ChestLightsTimer = 0;													// number of the timer controlling the chest lamp sequencing
byte PB_DropLightsTimer = 0;                          // number of the timer controlling the drop target light sequencing
byte PB_SkillMultiplier = 0;													// Multiplier for the skill shot value
byte PB_DropBlinkLamp = 0;														// number of the lamp currently blinking
byte PB_LitChestLamps = 0;														// amount of lit chest lamps
byte PB_ChestLamp[4][5];															// status of the chest lamps for each player / one column per byte
byte PB_Chest_Status[5];                              // number of visor openings for this player / > 100 means visor is actually open
byte PB_Planet[5];                                    // reached planets for all players
byte PB_ExBallsLit[5];                                // no of lanes lit for extra ball
byte PB_EjectMode[5];                                 // current mode of the eject hole
byte PB_EnergyValue[5];                              	// energy value for current player (value = byte*2000)
byte PB_LampsToLight = 2;															// number of lamps to light when chest is hit
byte *PB_FlashSequence;                               // pointer to the current flash lamp sequence
byte *PB_ChestPatterns;                               // pointer to the current chest lamp pattern
uint16_t PB_ChestPatternCounter = 0;                  // counter for the current chest lamp pattern to be shown

const unsigned int PB_SolTimes[32] = {50,20,30,70,50,200,30,30,100,100,999,999,0,0,100,100,50,100,50,50,50,50,0,0,50,100,100,100,100,100,100,100}; // Activation times for solenoids (last 8 are C bank)
const byte PB_BallSearchCoils[9] = {3,4,5,17,19,22,6,20,21}; // coils to fire when the ball watchdog timer runs out
const byte PB_OpenVisorSeq[45] = {30,2,30,2,30,2,5,3,5,7,5,8,5,5,5,4,5,2,2,5,3,5,7,5,8,5,5,5,4,5,2,2,5,3,5,7,5,8,5,5,5,4,10,2,0};
const byte PB_ChestRows[11][5] = {{28,36,44,52,60},{28,29,30,31,32},{36,37,38,39,40},{44,45,46,47,48},{52,53,54,55,56},{60,61,62,63,64},
																{32,40,48,56,64},{31,39,47,55,63},{30,38,46,54,62},{29,37,45,53,61},{28,36,44,52,60}};
const byte PB_ExBallLamps[4] = {49, 50, 58, 57};                          

struct SettingTopic PB_setList[7] = {{"DROP TG TIME  ",HandleNumSetting,0,3,20}, // TODO switch it to const struct
		{" REACH PLANET ",HandleNumSetting,0,1,9},
		{" ENERGY TIMER ",HandleNumSetting,0,1,90},
    {" RESET  HIGH  ",PB_ResetHighScores,0,0,0},
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

                                      //  DurationXX0910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364
const struct LampPat PB_OpenVisorPat[23] = {{100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1},
                                            {100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},                                                                                                                            
                                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
                                      
const struct LampFlow PB_AttractFlow[4] = {{1,PB_AttractPat1},{10,PB_AttractPat2},{1,PB_AttractPat3},{0,0}};

const byte PB_RandomChestPat[25] =  {15,0b10011,0b00100,0b01100,0b10101,0b00101,
																		 15,0b11100,0b00101,0b10011,0b10100,0b10010,
																		 15,0b00101,0b11010,0b10110,0b01011,0b01101,
																		 15,0b01010,0b11011,0b01001,0b01010,0b11010,0};

const byte PB_WalkingLines[199] = {15,0b01000,0b01000,0b01000,0b01000,0b01000,
                                   15,0b00100,0b00100,0b00100,0b00100,0b00100,
                                   15,0b00010,0b00010,0b00010,0b00010,0b00010,
                                   15,0b00001,0b00001,0b00001,0b00001,0b00001,
                                   15,0b00001,0b00010,0b00010,0b00100,0b01000,
                                   15,0b00001,0b00010,0b00100,0b01000,0b10000,
                                   15,0b00001,0b00110,0b11000,0b00000,0b00000,
                                   15,0b11111,0b00000,0b00000,0b00000,0b00000,
                                   15,0b00000,0b11111,0b00000,0b00000,0b00000,
                                   15,0b00000,0b00000,0b11111,0b00000,0b00000,
                                   15,0b00000,0b00000,0b00000,0b11111,0b00000,
                                   15,0b00000,0b00000,0b00000,0b00000,0b11111,                                  
                                   15,0b00000,0b00000,0b11000,0b00110,0b00001,
                                   15,0b10000,0b01000,0b00100,0b00010,0b00001,
                                   15,0b00100,0b00100,0b00010,0b00010,0b00001,
                                   15,0b00001,0b00001,0b00001,0b00001,0b00001,
                                   15,0b00010,0b00010,0b00010,0b00010,0b00010,
                                   15,0b00100,0b00100,0b00100,0b00100,0b00100,
                                   15,0b01000,0b01000,0b01000,0b01000,0b01000,
                                   15,0b10000,0b10000,0b10000,0b10000,0b10000,
                                   15,0b00100,0b00100,0b01000,0b01000,0b10000,                                   
                                   15,0b00001,0b00010,0b00100,0b01000,0b10000,
                                   15,0b00000,0b00000,0b00110,0b01100,0b10000,
                                   15,0b00000,0b00000,0b00000,0b00000,0b11111,
                                   15,0b00000,0b00000,0b00000,0b11111,0b00000,
                                   15,0b00000,0b00000,0b11111,0b00000,0b00000,
                                   15,0b00000,0b11111,0b00000,0b00000,0b00000,
                                   15,0b11111,0b00000,0b00000,0b00000,0b00000,
                                   15,0b10000,0b01100,0b00110,0b00000,0b00000,
                                   15,0b10000,0b01000,0b00100,0b00010,0b00001,
                                   15,0b10000,0b01000,0b01000,0b00100,0b00100,
                                   15,0b10000,0b10000,0b10000,0b10000,0b10000,0};                                   

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
    // StrobeLightsTimer = 0;
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
		for (i=1;i<5;i++) {																// for all players
      PB_Chest_Status[i] = 0;                         // reset the number of number of visor openings
      PB_ResetPlayersChestLamps(i);                   // reset the chest lamps
      PB_EnergyValue[i] = 25;                        // reset the energy value to 50K
      PB_ExBallsLit[i] = 0;                           // reset the lit extra balls
      PB_EjectMode[i] = 0;                            // reset the mode of the eject hole
			PB_Planet[i] = 0;}                              // reset reached planets
    PB_LitChestLamps = 0;
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
		ActivateSolenoid(0, 23);                        	// enable flipper fingers
		ActivateSolenoid(0, 24);
		break;
	case 8:																							// high score reset
		digitalWrite(Blanking, LOW);                     // invoke the blanking
		break;
	case 46:
		if (PB_CloseVisorFlag) {
			PB_CloseVisorFlag = false;
			ReleaseSolenoid(13);}
		break;
	case 47:
		if (PB_OpenVisorFlag) {
			PB_OpenVisorFlag = false;
			ReleaseSolenoid(13);}
		break;
	case 72:
		Switch_Pressed = DummyProcess;
//		if (ByteBuffer3) {																// ShowLampPattern running?
//			KillTimer(ByteBuffer3);													// stop it
    RemoveBlinkLamp(1);                                // stop the blinking of the game over lamp
    KillAllTimers();
    // StrobeLightsTimer = 0;
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
	BonusMultiplier = 1;                                // reset bonus multiplier
  for (i=0; i<4; i++) {                               // turn off the corresponding lamps
    Lamp[9+i] = false;}
  PB_SkillMultiplier = 0;                             // reset skill shot multiplier
	*(DisplayUpper+16) = LeftCredit[32 + 2 * Ball]; 		// show current ball in left credit
	//*(DisplayUpper+17) = LeftCredit[33 + 2 * Ball];
	if (!BlinkScoreTimer) {
		BlinkScoreTimer = ActivateTimer(1000, 1, BlinkScore);}
  PB_ClearChest();                                    // turn off chest lamps
  PB_CountLitChestLamps();                            // check how many chest lamps are lit
  if ((PB_Chest_Status[Player] > 100)) {              // > 100 means visor has to be open
    PB_Chest_Status[Player] = PB_Chest_Status[Player] - 100; // use it as a counter for opened visors
    PB_LampsToLight = 1;
    PB_ChestMode = 0;                                 // indicate an open visor
    if (!Switch[47]) {                                // visor not already open?
      PB_OpenVisorFlag = true;
      ActivateSolenoid(0, 13);}                       // open visor
    PB_ChestPatterns = (byte*)PB_WalkingLines;        // set chest lamps pattern
    PB_ChestLightsTimer = ActivateTimer(500, 0, PB_StartChestPattern);} // start player
  else {
    if (!Switch[46]) {                                // visor not already closed?
      PB_CloseVisorFlag = true;
      ActivateSolenoid(0, 13);}                       // close visor    
    if (PB_LitChestLamps) {                           // chest lamps lit?
      PB_ChestMode = 12;                              // indicate that the visor can not be opened with one hit      
      PB_ChestPatterns = (byte*)PB_RandomChestPat;    // set chest lamps pattern
      PB_ChestLightsTimer = ActivateTimer(500, 0, PB_StartChestPattern);} // start player
    else {
      PB_ChestMode = 1;                               // indicate that the visor can be opened with one hit
      PB_ChestLightHandler(0);}}                      // start moving rows / columns
  if (PB_Chest_Status[Player] > 2) {                  // visor has been open more than 2 times
    PB_LampsToLight = 1;}                             // TODO change according to difficulty setting
  else {
    PB_LampsToLight = 2;}
  if (PB_EjectMode[Player] < 5) {
    for (i=0; i<PB_EjectMode[Player]; i++) {
      Lamp[13+i] = true;}}
  else {
    for (i=0; i<(PB_EjectMode[Player]-5); i++) {
      Lamp[13+i] = true;}
    AddBlinkLamp(PB_EjectMode[Player]+8, 100);}
  PB_DropBlinkLamp = 41;
	PB_CycleDropLights(1);															// start the blinking drop target lights
	if (PB_Planet[Player] < game_settings[PB_ReachPlanet]) {	// target planet not reached yet?
		AddBlinkLamp(18+game_settings[PB_ReachPlanet],100);}		// let target planet blink
  PB_GiveBall(Balls);}

void PB_GiveBall(byte Balls) {
  if (PB_SkillMultiplier < 10) {
    PB_SkillMultiplier++;
    if (PB_SkillMultiplier == 10) {
      // super skill shot
    }}
    WriteUpper2(" VORTEX   X   ");
    WriteLower2("              ");
    ShowNumber(15, PB_SkillMultiplier);               // show multiplier
    ShowMessage(3);
  PB_SkillShot = true;                                // the first shot is a skill shot  
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
      byte c = 0;
			switch (Switch) {																// was a skill shot target hit
			case 22:
        c = 20;
				break;
			case 23:
				c = 100;
				break;
			case 24:
				c = 5;
				break;}
      WriteUpper2(" VORTEX   X   ");
      WriteLower2("              ");
      ShowNumber(31, c * PB_SkillMultiplier * 1000);                       // show skill shot points
      ShowNumber(15, PB_SkillMultiplier);             // and multiplier
      ShowMessage(3);
      Points[Player] += c * 1000 * PB_SkillMultiplier;
      ShowPoints(Player);} 
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
      if (AppByte == 5) {                             // balls have not settled yet
        WriteUpper("  BALL  STUCK ");
        BallWatchdogTimer = ActivateTimer(1000, 0, PB_SearchBall);} // and try again in 1s
      else {
        if (AppByte == 2) {														// found 2 balls in trunk?
        	if (BlockOuthole) {													// is the outhole blocked
        		PB_BallEnd(0);}														// then it was probably a ball loss gone wrong
        	else {
        		ActivateTimer(1000, 2, PB_NewBall);}}			// otherwise try it with a new ball
        else {
        	AppByte2 = 0;
        	for (i=0; i<2; i++) {                       // count balls in lock
        		if (Switch[25+i]) {
        			AppByte2++;}}
      		if (AppByte2 > InLock) {										// more locked balls found than expected?
      			PB_HandleLock(0);													// lock them
      			BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}
      		else {
      			WriteUpper("  BALL  SEARCH");
      			ActivateSolenoid(0, PB_BallSearchCoils[Counter]); // fire coil to get ball free
            if (PB_BallSearchCoils[Counter] == 5) {   // ramp raise?
              PB_DropRamp = true;}                    // set flag to drop ramp
            if (PB_BallSearchCoils[Counter] == 6) {   // ramp down?
              PB_DropRamp = false;}                   // clear flag to drop ramp               
      			Counter++;
      			if (Counter == 9) {												// all coils fired?
      				Counter = 0;}														// start again                        
      			if (Switch[46] && !SolenoidStatus(13)) {	// visor closed and motor not active?
      				ActivateSolenoid(0, 13);								// open it enough to deactivate switch 46
      				ActivateTimer(2000, 0, PB_CloseVisor);}	// and prepare to close it again
      			BallWatchdogTimer = ActivateTimer(1000, Counter, PB_SearchBall);}}}}}} // come again in 1s if no switch is activated

void PB_OpenVisor(byte Dummy) {
	PB_OpenVisorFlag = true;}																	// set flag to stop visor motor when open

void PB_CloseVisor(byte Dummy) {
	PB_CloseVisorFlag = true;}																	// set flag to stop visor motor when closed

void PB_ClearOuthole(byte Event) {
  if (Switch[16]) {                                   // outhole switch still active?
    if (!BlockOuthole) {															// outhole blocked?
      BlockOuthole = true;														// block outhole until this ball has been processed
      ActA_BankSol(30, 1);                         		// put ball in trunk
      ActivateTimer(2000, 0, PB_BallEnd);}}
    else {
      ActivateTimer(2000, 0, PB_ClearOuthole);}}			// come back in 2s if outhole is blocked

void PB_GameMain(byte Switch) {
	switch(Switch) {
  case 1:
  case 2:
  case 7:
  case 9:
    WriteUpper(" TILT  WARNING");
    ActivateTimer(3000, 0, ShowAllPoints);
    break;
  case 3:
    PB_AddPlayer();
    break;
  case 4:
  case 5:
  case 6:
    WriteUpper(" FREE   GAME  ");
    ActivateTimer(3000, 0, ShowAllPoints);
    break;
	case 8:																							// high score reset
		digitalWrite(Blanking, LOW);                      // invoke the blanking
		break;
  case 10:                                            // left lane change
    PB_MoveExBallLamps(0);
    break;
  case 11:                                            // right lane change
    PB_MoveExBallLamps(1);
    break;
  case 12:                                            // left outlane
    PB_AddBonus(3);
    if (Lamp[49]) {
      Lamp[49] = false;
      PB_ExBallsLit[Player]--;
      PB_GiveExBall();}
    break;
  case 13:                                            // left inlane
    PB_AddBonus(1);
    Lamp[18] = true;
    if (Lamp[50]) {
      Lamp[50] = false;
      PB_ExBallsLit[Player]--;
      PB_GiveExBall();}
    break;    
  case 14:                                            // right inlane
    PB_AddBonus(1);
    if (Lamp[58]) {
      Lamp[58] = false;
      PB_ExBallsLit[Player]--;
      PB_GiveExBall();}
    if (PB_EjectMode[Player] < 5) {
      if (PB_EjectMode[Player] == 4) {
        AddBlinkLamp(15, 100);}
      else {
        AddBlinkLamp(PB_EjectMode[Player]+13, 100);}
      PB_EjectMode[Player] = PB_EjectMode[Player] + 5;}
    break;
  case 15:                                            // right outlane
    PB_AddBonus(3);
    if (Lamp[57]) {
      Lamp[57] = false;
      PB_ExBallsLit[Player]--;
      PB_GiveExBall();}
    break;
	case 16:                                            // outhole
    ActivateTimer(200, 0, PB_ClearOuthole);           // check again in 200ms
    break;
  case 19:                                            // advance planet
    if (Lamp[18]) {
      Lamp[18] = false;
      if (Lamp[51]) {																	// special lit?
      	PB_AddExBall();}
      PB_AddBonus(1);
      PB_AdvancePlanet();}
    break;
  case 20:                                            // shooter lane
    if (!PB_SkillShot) {
      if (PB_SkillMultiplier < 10) {
        PB_SkillMultiplier++;
        WriteUpper2(" VORTEX   X   ");
        WriteLower2("              ");
        ShowNumber(15, PB_SkillMultiplier);           // show multiplier
        ShowMessage(3);
        if (PB_SkillMultiplier == 10) {
          // super skill shot
        }}
      PB_SkillShot = true;}                           // the first shot is a skill shot  
    break;                            // TODO handle PB_SkillMultiplier
	case 25:
	case 26:
    if (!PB_IgnoreLock) {
      PB_IgnoreLock = true;
      PB_AddBonus(1);
		  ActivateTimer(1000, 0, PB_HandleLock);}         // handle locked balls after 1s
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
		if (PB_ChestMode) {														    // visor closed?
      if (PB_ChestLightsTimer) {
        KillTimer(PB_ChestLightsTimer);               // disable timer to change row / column blinking
        PB_ChestLightsTimer = 0;}      
      AppByte = Switch-27;                            // buffer the switch number
      if (Switch > 32) {                              // is it a row?
        AppByte = 16 - AppByte;}                      // turn the rows upside down
			if (PB_ChestMode < 11) {												// visor can be opened with one row / column hit
        for (i=0; i<5; i++) {                         // turn off blinking row / column
          RemoveBlinkLamp(PB_ChestRows[PB_ChestMode][i]);}
				if (AppByte == PB_ChestMode) {						    // correct row / column hit?
					PB_AddBonus(1);
          PB_OpenVisorProc();}										    // open visor
				else {																				// incorrect row / column hit
					PB_ChestMode = Switch-17;                   // Store row / column hit
					PB_SetChestLamps(Switch-28);                // add the lamps for the hit row / column in PB_ChestLamp
          PB_ClearChest();                            // turn off chest lamps
          PB_ChestLightHandler(1);}}                  // call effect routine
      else {                                          // the cumbersome way to open the visor
        PB_ChestMode = Switch-17;                     // Store row / column hit
        PB_SetChestLamps(Switch-28);                  // add the lamps for the hit row / column in PB_ChestLamp
        if (PB_LitChestLamps == 25) {                 // complete chest lit?
          PB_OpenVisorProc();}                        // open visor
        else {
          PB_ClearChest();                            // turn off chest lamps
          PB_ChestLightHandler(1);}}}                 // call effect routine                                      
    else {                                            // visor open
      PB_SetChestLamps(Switch-28);                    // add the lamps for the hit row / column in PB_ChestLamp
    }
		break;
	case 38:																						// eject hole
    if (!PB_EjectIgnore) {
      PB_EjectIgnore = true;
      PB_AddBonus(1);
      if (PB_EjectMode[Player] < 5) {
		    Points[Player] += 2000;
        ShowPoints(Player);
		    ActivateTimer(1000, 3, ClearEjectHole);}
      else {
        if (PB_EjectMode[Player] == 9) {
          RemoveBlinkLamp(15);
          Lamp[15] = true;
          PB_EjectMode[Player] = 4;
          Points[Player] += Multiballs * 75000;}
        else {
          RemoveBlinkLamp(PB_EjectMode[Player] + 8);
          Lamp[PB_EjectMode[Player] + 8] = true;
          PB_PlayFlashSequence((byte*) PB_OpenVisorSeq); // play flasher sequence
          Points[Player] += Multiballs * (PB_EjectMode[Player] - 4) * 25000;
          ShowPoints(Player);
          PB_EjectMode[Player] = PB_EjectMode[Player] - 4;
          if (PB_EjectMode[Player] == 4) {
            PB_AddExBall();}}
        ActivateTimer(1000, 3, ClearEjectHole);}}
    break;
  case 39:                                            // solar ramp exit
    uint16_t Buffer;
    PB_AddBonus(1);
    if (PB_SolarValueTimer) {                         // solar ramp lit
      KillTimer(PB_SolarValueTimer);
      PB_SolarValueTimer = 0;      
      RemoveBlinkLamp(35);
      Points[Player] += PB_SolarValue * 1000;
      ShowPoints(Player);
      Buffer = PB_SolarValue;
      PB_SolarValue = 100;
      PB_ClearOutLock(0);}
    else {                                            // solar ramp not lit
      if (BonusMultiplier < 5) {                      // increase bonus multiplier
        Lamp[8+BonusMultiplier] = true;               // turn on the corresponding lamp
        BonusMultiplier++;}
      if (PB_SolarValue < 5000) {
        PB_SolarValue += 50;
        if (PB_SolarValue > 5000) {
          PB_SolarValue = 5000;}}
      Buffer = PB_SolarValue;}
    if (Player < 3) {
      WriteLower2("SOLAR =       ");
      ShowNumber(31, Buffer*1000);}
    else {
      WriteUpper2("SOLAR =       ");
      ShowNumber(15, Buffer*1000);}
    break;
  case 45:																						// score energy switch
		if (PB_EnergyActive) {
			Points[Player] += PB_EnergyValue[Player] * 2000;				// score energy value
      WriteUpper2(" ENERGY VALUE ");
      WriteLower2("              ");
      ShowNumber(31, PB_EnergyValue[Player]*2000);
      ShowMessage(3);
			PB_EnergyOff(0);}
		break;
	case 46:
		if (PB_CloseVisorFlag) {
			PB_CloseVisorFlag = false;
			ReleaseSolenoid(13);}
		break;
	case 47:
		if (PB_OpenVisorFlag) {
			PB_OpenVisorFlag = false;
			ReleaseSolenoid(13);}
		break;
	case 49:																						// drop targets
	case 50:
	case 51:
    if (!PB_DropWait) {
      PB_DropWait = true;
      PB_AddBonus(1);
      Points[Player] += 1000;
      ShowPoints(Player);
      ActivateTimer(200, Switch, PB_HandleDropTargets);}
		break;
  case 56:                                            // standup targets
  case 59:
  case 60:
    Points[Player] += 10;
    break;
	case 65:																						// lower jet bumper
		ActivateSolenoid(0, 17);
    if (PB_EnergyValue[Player] < 250) {
      PB_EnergyValue[Player]++;
      WriteUpper2(" ENERGY VALUE ");
      WriteLower2("              ");
      ShowNumber(31, PB_EnergyValue[Player] * 2000);
      ShowMessage(3);}
		break;
	case 67:																						// left jet bumper
		ActivateSolenoid(0, 19);
    if (PB_EnergyValue[Player] < 250) {
      PB_EnergyValue[Player]++;
      WriteUpper2(" ENERGY VALUE ");
      WriteLower2("              ");
      ShowNumber(31, PB_EnergyValue[Player] * 2000);
      ShowMessage(3);}
		break;
	case 68:																						// left slingshot
    PB_MoveExBallLamps(0);
		ActivateSolenoid(0, 20);
		break;
	case 69:																						// right slingshot
    PB_MoveExBallLamps(1);
		ActivateSolenoid(0, 21);
		break;
  case 70:                                            // upper jet bumper
    ActivateSolenoid(0, 22);
    if (PB_EnergyValue[Player] < 250) {
      PB_EnergyValue[Player]++;
      WriteUpper2(" ENERGY VALUE ");
      WriteLower2("              ");
      ShowNumber(31, PB_EnergyValue[Player] * 2000);
      ShowMessage(3);}
    break;
	}
}

void PB_MoveExBallLamps(byte Direction) {
  if (PB_ExBallsLit[Player]) {
    if (Direction) {
      Direction = Lamp[57];
      for (byte c=0; c<3; c++) {
        Lamp[PB_ExBallLamps[3-c]] = Lamp[PB_ExBallLamps[3-c-1]];}
      Lamp[49] = Direction;}
    else {
      Direction = Lamp[49];
      for (byte c=0; c<3; c++) {
        Lamp[PB_ExBallLamps[c]] = Lamp[PB_ExBallLamps[c+1]];}
      Lamp[57] = Direction;}}}

void PB_AddExBall() {
  if (PB_ExBallsLit[Player] < 4) {
    PB_ExBallsLit[Player]++;
    byte c = 0;
    while (Lamp[PB_ExBallLamps[c]]) {
      c++;}
    Lamp[PB_ExBallLamps[c]] = true;}}

void PB_AfterExBallRelease(byte Event) {
  if (Switch[20]) {                                   // ball still in the shooting lane?
    ActivateTimer(2000, Event, PB_AfterExBallRelease);}  // come back in2s
  else {                                              // ball has been shot
    RemoveBlinkLamp(33);
    if (ExBalls) {                                    // player still has an extra balls
      Lamp[33] = true;}}}
      
void PB_GiveExBall() {
  ExBalls++;
  Lamp[33] = true;}
  
void PB_AddBonus(byte BonusToAdd) {
  if (Bonus < 99) {
    Bonus = Bonus + BonusToAdd;
    if (Bonus > 99) {
      Bonus = 99;}
    if (Player < 3) {
      WriteLower2("BONUS =       ");
      ShowNumber(31, Bonus*1000);
      DispRow2 = DisplayLower2;}
    else {
      WriteUpper2("BONUS =       ");
      ShowNumber(15, Bonus*1000);
      DispRow1 = DisplayUpper2;}
    if (ShowMessageTimer) {                           // timer already running?
      KillTimer(ShowMessageTimer);}                   // kill it      
    ShowMessageTimer = ActivateTimer(2000, 0, ShowMessage);}}
    
void ClearEjectHole(byte Solenoid) {                   // activate solenoid after delay time
  PB_EjectIgnore = false;
  ActA_BankSol(0, Solenoid);}

void PB_StartChestPattern(byte Dummy) {
  PB_ChestPatternCounter = 0;
  LampPattern = Lamp;
  PB_ChestLightHandler(0);}

void PB_OpenVisorProc() {                   				// measures to open the visor
  PB_ChestMode = 0;                                 // indicate that the visor is open
  PB_Chest_Status[Player]++;                        // increase the number of visor openings
  if (PB_Chest_Status[Player] == 2) {								// visor opened two times?
  	PB_AddExBall();}
  PB_LitChestLamps = 0;                             // reset the counter
  PB_ResetPlayersChestLamps(Player);                // reset the stored lamps
  PB_ClearChest();                                  // turn off chest lamps
  PB_PlayFlashSequence((byte*) PB_OpenVisorSeq);    // play flasher sequence
  PatPointer = PB_OpenVisorPat;                     // set the pointer to the current series
  FlowRepeat = 1;                                   // set the repetitions
  PB_ChestPatterns = (byte*)PB_WalkingLines;
  LampReturn = PB_StartChestPattern;
  ShowLampPatterns(0);                              // start the player
  PB_CloseVisorFlag = false;
  ActivateSolenoid(4000, 11);                       // turn the backbox GI off
  ActivateTimer(2000, 0, PB_OpenVisor);
  ActivateSolenoid(0, 13);}
  
void PB_ChestLightHandler(byte State) {               // handle chest lights timer
  if (State) {                                        // is there an animation for a row / column hit running?
    if (State < 6) {                                  // turn on phase
      Lamp[PB_ChestRows[AppByte][State-1]] = true;}
    else {                                            // turn off phase
      Lamp[PB_ChestRows[AppByte][State-6]] = false;}
    State++;
    if (State < 11) {                                 // not yet done
      PB_ChestLightsTimer = ActivateTimer(100, State, PB_ChestLightHandler);} // come back with the current state set
    else {
      PB_ChestPatterns = (byte*)PB_RandomChestPat;    // set pattern
      PB_ChestPatternCounter = 0;
      PB_ChestLightsTimer = ActivateTimer(500, 0, PB_ChestLightHandler);}} // come back with no state set
  else {    
    if (PB_ChestMode && (PB_ChestMode < 11)) {        // visor is closed and can be opened with one row / column hit
      PB_ChestMode++;                                 // proceed to next row / column
      if (PB_ChestMode == 11) {                       // last row reached?
        PB_ChestMode = 1;}                            // start with first column      
      for (i=0; i<5; i++) {                           // turn off previous blinking row / column
        RemoveBlinkLamp(PB_ChestRows[PB_ChestMode-1][i]);}
      for (i=0; i<5; i++) {                           // turn on next blinking row / column
        AddBlinkLamp(PB_ChestRows[PB_ChestMode][i], 75);}
      PB_ChestLightsTimer = ActivateTimer(1000, 0, PB_ChestLightHandler);}
    else {
      byte Mask;
      byte Buffer;
      if (!PB_ChestPatterns[6*PB_ChestPatternCounter]) {
        PB_ChestPatternCounter = 0;}
      for (byte x=0; x<5; x++) {                      // for all columns
        Mask = 1;                                     // mask to access the stored lamps for this player
        Buffer = PB_ChestPatterns[6*PB_ChestPatternCounter+x+1]; // buffer the current column
        for (i=0; i<5; i++) {                         // for all rows
          if (PB_ChestLamp[Player-1][x] & Mask) {     // if the lamp is stored
            Lamp[28+8*x+i] = true;}                   // turn it on
          else {                                      // otherwise
            Lamp[28+8*x+i] = Buffer & 1;}             // it is controlled by the pattern
          Mask = Mask<<1;                             // adjust the mask
          Buffer = Buffer>>1;}}                       // and the buffer
      PB_ChestPatternCounter++;
      PB_ChestLightsTimer = ActivateTimer(PB_ChestPatterns[6*(PB_ChestPatternCounter-1)]*10, 0, PB_ChestLightHandler);}}}

void PB_ClearChest() {                                // turn off chest lamps
  byte x = 0;
  byte y = 0;
  for (x=0; x<5; x++) {
    for (y=0; y<5; y++) {
      Lamp[28+8*x+y] = false;}}}

void PB_SetChestLamps(byte Switch) {                  // add the lamps for the hit row / column in PB_ChestLamp
	byte Pos = 0;
	byte Buffer;
	byte Buffer2 = PB_LampsToLight;
	if (Switch < 5) {                                   // is it a column?
		Buffer = PB_ChestLamp[Player-1][Switch];          // buffer it
    Pos = 16;                                         // start with a mask value of 10000b 
    while (Pos && Buffer2) {                          // until all rows are processed or the required number of lamps has been lit
      if (!(Buffer & Pos)) {                          // if the lamp is not lit
      	if (Buffer2 == 1) {														// only once per target hit
      		PB_AddBonus(1);}
        Buffer2--;                                    // reduce the number of lamps to be lit
        PB_LitChestLamps++;                           // increase the number of lit chest lamps
        Buffer = Buffer | Pos;}                       // set the corresponding bit in the buffer
      Pos = Pos>>1;}                                  // next row
    PB_ChestLamp[Player-1][Switch] = Buffer;          // copy the buffer to the chest lamp array for this player
//    if (Buffer2 == PB_LampsToLight) {
//      Column already full
	}
  else {                                              // it is a row
    Buffer = 1<<(Switch-5);                           // calculate the mask from the row
    Pos = 0;                                          // start on the left of the chest
    while ((Pos < 5) && Buffer2) {                    // until all columns are processed or the required number of lamps has been lit
      if (!(PB_ChestLamp[Player-1][Pos] & Buffer)) {  // if the lamp is not lit
      	if (Buffer2 == 1) {														// only once per taget hit
      		PB_AddBonus(1);}
        Buffer2--;                                    // reduce the number of lamps to be lit
        PB_LitChestLamps++;                           // increase the number of lit chest lamps
        PB_ChestLamp[Player-1][Pos] = PB_ChestLamp[Player-1][Pos] | Buffer;} // set the bit for this lamp in the chest lamp array for this player
      Pos++;}
//    if (Buffer2 == PB_LampsToLight) {
//      row already full    
	}
      
}
      
void PB_CountLitChestLamps() {												// count the lit chest lamps for the current player
	byte Buffer;
	PB_LitChestLamps = 0;																// reset counter
	for (byte x=0; x<5; x++) {													// for all rows
		Buffer = PB_ChestLamp[Player-1][x];									// buffer the current row
		for (i=0; i<5; i++) {															// for all columns
			if (Buffer & 1) {																// lamp lit?
				PB_LitChestLamps++;}													// increase counter
			Buffer = Buffer>>1;}}}													// shift buffer to the next lamp

void PB_ResetPlayersChestLamps(byte Player) {         // Reset the chest lamps for this player
  for (byte c=0; c<5; c++) {
    PB_ChestLamp[Player-1][c] = 0;}}

void PB_PlayFlashSequence(byte* Sequence) {           // prepare for playing a flasher sequence
  ActivateSolenoid(0, 14);                            // switch A/C relay to C
  C_BankActive = true;                                // indicate that the C bank is active
  PB_FlashSequence = Sequence;                        // set the PB_FlashSequence pointer to the current sequence
  ActivateTimer(50, 0, PB_FlSeqPlayer);}              // start the sequence in 50ms when the A/C relay had some time to switch

void PB_FlSeqPlayer(byte Step) {                      // play flasher sequence
  if (PB_FlashSequence[Step]) {                       // if the next list entry is not zero
    ActivateSolenoid(*(GameDefinition.SolTimes+PB_FlashSequence[Step+1]+23), PB_FlashSequence[Step+1]); // Activate the flasher with its standard activate time
    ActivateTimer(PB_FlashSequence[Step]*10, Step+2, PB_FlSeqPlayer);} // increase the step counter and play the next list entry in duration*10ms
  else {                                              // if the next list entry is zero
    PB_FlashSequence = 0;                             // set the list pointer to zero
    ReleaseSolenoid(14);                              // release the A/C relay
    C_BankActive = false;}}                           // and indicate the C bank as not active
    
void PB_HandleLock(byte State) {
	if (!State) {                                       // routine didn't call itself
		PB_IgnoreLock = false;
		InLock++;}
	c = 0;
	if (Switch[25]) {                                   // count locked balls
		c++;}
	if (Switch[26]) {
		c++;}
	if (c != InLock) {                                  // not as expected?
		InLock = c;                                       // take the new count value
		ActivateTimer(200, 1, PB_HandleLock);}            // and come back to recheck
	else {                                              // number of locked balls as expected
		if (InLock) {                                     // locked ball found?
			if (PB_ChestMode) {                             // visor is supposed to be closed
				PB_ClearOutLock(1);}                          // remove balls from lock
			else {
				if (InLock == 1) {
					if (Multiballs > 1) {                       // multiball already running?
						AddBlinkLamp(35, 100);                    // start blinking of solar energy ramp
						PB_OpenVisorFlag = false;
						ActivateTimer(2000, 0, PB_CloseVisor);    // close visor
						ActivateSolenoid(0, 13);                  // start visor motor
						PB_SolarValueTimer = ActivateTimer(8000, 0,PB_ReopenVisor);} // 8s to score the solar value
					else {                                      // multiball not yet running
						PB_GiveBall(1);}}                         // give second ball
				else {                                        // both balls in lock
					if (Multiballs == 1) {                      // multiball not yet running?
						Multiballs = 2;                           // start multiball
						ActivateTimer(1000, 0, PB_ClearOutLock);  // eject first ball
						PB_ClearOutLock(0);}                      // eject second ball
					else {
						PB_ClearOutLock(1);}}}}}}                 // eject 1 ball and close visor

void PB_ReopenVisor(byte Dummy) {                     // reopen visor if solar value ramp was not hit in time
  PB_SolarValueTimer = 0;
  RemoveBlinkLamp(35);
  PB_ClearOutLock(0);}

void PB_ClearOutLock(byte CloseVisor) {
  if (SolenoidStatus(13)) {                           // visor motor on?
    ActivateTimer(1100, CloseVisor, PB_ClearOutLock);} // come back later
  else {
    if (Switch[47]) {                                 // visor is open
      if (Switch[25]) {                               // left eye?
        ActA_BankSol(0, 7);}                          // eject it
      else {
        if (Switch[26]) {                             // right eye
          ActA_BankSol(0, 8);}}                       // eject it
      if (CloseVisor) {                               // closed visor requested?
        ActivateTimer(1000, 13, DelaySolenoid);       // start visor motor in 1s
        ActivateTimer(3000, 00, PB_CloseVisor);}}     // set close flag
    else {
      ActivateTimer(1000, 0, PB_OpenVisor);
      ActivateSolenoid(0, 13);                        // activate visor motor       
      ActivateTimer(2000, CloseVisor, PB_ClearOutLock);}}}
    
void PB_HandleDropTargets(byte Target) {
	PB_DropWait = false;																// stop ignoring drop target switches
	if (Switch[49] && Switch[50] && Switch[51]) {				// all targets down
		if (PB_DropTimer) {																// any targets down before?
			KillTimer(PB_DropTimer);												// turn off timer
			PB_DropTimer = 0;
			PB_DropBlinkLamp = 41;
			PB_CycleDropLights(1);													// start the blinking drop target lights
			RemoveBlinkLamp(17);}														// stop blinking of timer lamp
		Points[Player] += Multiballs * 25000;
		ActA_BankSol(0, 4);																// reset drop targets
    PB_AdvancePlanet();}
	else {
		if (!PB_DropTimer) {															// first target hit
			if (Target-8 == PB_DropBlinkLamp) {							// blinking target hit?
				ActA_BankSol(0, 5);														// raise ramp
				AddBlinkLamp(34, 500);												// blink energy lamp
				PB_EnergyActive = true;												// energy value on
				ActivateTimer(game_settings[PB_EnergyTimer]*1000, 0, PB_EnergyOff);}
			PB_CycleDropLights(0);													// stop blinking of drop target lights
			AddBlinkLamp(17, 500);													// blink drop target timer lamp
			PB_DropTimer = ActivateTimer(game_settings[PB_DropTime]*1000, 0, PB_ResetDropTargets);}}}

void PB_AdvancePlanet() {
  PB_Planet[Player]++;                                // player has reached next planet
  if (PB_Planet[Player] > 10) {                       // sun already reached before?
    PB_Planet[Player] = 10;}                          // set it back to the sun
  else {
    if  (PB_Planet[Player] == 10) {                   //  10 = Sun
      Lamp[51] = true;}																// light special
    else {
      if (PB_Planet[Player] == game_settings[PB_ReachPlanet]) { // target planet reached
      	Lamp[51] = true;															// light special
        RemoveBlinkLamp(18+game_settings[PB_ReachPlanet]);} // stop blinking
      Lamp[PB_Planet[Player]+18] = true;}}}

void PB_ResetDropTargets(byte Dummy) {
	RemoveBlinkLamp(17);																// stop drop target timer lamp
  PB_DropBlinkLamp = 41;
  PB_DropTimer = 0;
	PB_CycleDropLights(1);															// start the blinking drop target lights
	ActA_BankSol(0, 4);}																// reset drop targets

void PB_EnergyOff(byte Dummy) {
	if (PB_EnergyActive) {
		RemoveBlinkLamp(34);															// stop blinking of energy lamp
		PB_EnergyActive = false;													// energy value off
		PB_DropRamp = true;}}															// ramp needs to be dropped

void PB_CycleDropLights(byte State) {
	if ((State == 2) || ((State == 1) && !PB_DropLightsTimer)) { // State = 1 is a new start and not allowed when timer is already running
		if (PB_DropBlinkLamp) {														// blink lamp active?
			if (PB_DropBlinkLamp == 43) {										// last lamp blinking?
				AddBlinkLamp(41, 100);												// start again with the first one
				RemoveBlinkLamp(43);													// remove the current one
				PB_DropBlinkLamp = 41;}												// reset the number of the currently blinking lamp
			else {																					// not the last one
				AddBlinkLamp(PB_DropBlinkLamp+1, 100);				// start the next one
				RemoveBlinkLamp(PB_DropBlinkLamp);						// remove the current one
				PB_DropBlinkLamp++;}													// increase number of currently blinking lamp
			PB_DropLightsTimer = ActivateTimer(3000, 2, PB_CycleDropLights);}}
	else {
    if (PB_DropLightsTimer) {
      KillTimer(PB_DropLightsTimer);
      PB_DropLightsTimer = 0;}
		if (PB_DropBlinkLamp) {														// blink lamp active?
			RemoveBlinkLamp(PB_DropBlinkLamp);
			PB_DropBlinkLamp = 0;}}}

void PB_BallEnd(byte Event) {													// ball has been kicked into trunk
	AppByte = PB_CountBallsInTrunk();
	if ((AppByte == 5)||(AppByte < 3-Multiballs-InLock)) {
		InLock = 0;
		if (Multiballs == 1) {
			for (i=0; i<2; i++) {                         	// check how many balls are on the ball ramp
				if (Switch[25+i]) {
					InLock++;}}}
		WriteLower(" BALL   ERROR ");
    if (Switch[16]) {                                 // ball still in outhole?
      ActA_BankSol(0, 1);                             // make the coil a bit stronger
      ActivateTimer(2000, Event, PB_BallEnd);}        // and come back in 2s
    else {
  		if (Event < 11) {                               // have I been here already?
	  		Event++;
		  	ActivateTimer(1000, Event, PB_BallEnd);}      // if not try again in 1s
		  else {                                          // ball may be still in outhole
			  BlockOuthole = false;
			  Event = 0;
			  PB_ClearOuthole(0);}}}
	else {
		if (Multiballs == 2) {														// multiball running?
			Multiballs = 1;																	// turn it off
			if (AppByte == 2) {															// 2 balls detected in the trunk
				ActivateTimer(1000, 0, PB_BallEnd);}					// come back and check again
			else {
        PB_ClearOutLock(1);
        if (PB_ChestLightsTimer) {                    // stop the chest light animation
          KillTimer(PB_ChestLightsTimer);
          PB_ChestLightsTimer = 0;}
        PB_ChestMode = 1;
        PB_ClearChest();                              // turn off chest lamps
        PB_ChestLightHandler(0);
				BlockOuthole = false;}}												// remove outhole block
		else {
			LockedBalls[Player] = 0;
      if (BlinkScoreTimer) {
        KillTimer(BlinkScoreTimer);
        BlinkScoreTimer = 0;}      
			//BonusToAdd = Bonus;
			//BonusCountTime = 20;
			//CountBonus(AppByte);
      PB_CycleDropLights(0);                          // stop the blinking drop target lights
      if (PB_ChestLightsTimer) {                      // stop the chest light animation
        KillTimer(PB_ChestLightsTimer);
        PB_ChestLightsTimer = 0;}
      for (i=0; i<5; i++) {                         // turn off blinking row / column
        RemoveBlinkLamp(PB_ChestRows[PB_ChestMode][i]);}
      PB_ClearChest();                                // turn off chest lamps
      if (!PB_ChestMode) {
        PB_Chest_Status[Player] = PB_Chest_Status[Player] + 100;} // indicate that the visor has been open
      RemoveBlinkLamp(18+game_settings[PB_ReachPlanet]);
			if (BallWatchdogTimer) {
				KillTimer(BallWatchdogTimer);
				BallWatchdogTimer = 0;}
      if (PB_EjectMode[Player] > 4) {                 // any blinking eject mode lamps?
        if (PB_EjectMode[Player] == 9) {              // turn them off
          RemoveBlinkLamp(15);}
        else {
          RemoveBlinkLamp(PB_EjectMode[Player] + 8);}}
      for (i=0; i<4; i++) {                           // turn off all eject mode lamps
        Lamp[13+i] = false;}
      WriteUpper("BONUS      X  ");
      WriteLower("      =       ");
      IntBuffer = Bonus * BonusMultiplier;
      DisplayScore(2, BonusMultiplier);
      DisplayScore(4, IntBuffer * 1000);
      ActivateTimer(3000, 1, PB_CountBonus);}}}

void PB_CountBonus(byte ClearDisplay) {
  if (ClearDisplay) {
    WriteUpper("              ");
    WriteLower("              ");}
//    ClearDisplay++;}
//  if (ClearDisplay  == 2) {
//    WriteUpper("              ");
//    WriteLower("              ");
//    ClearDisplay = 0;}
  IntBuffer--;
  Points[Player] += 1000;
  ShowPoints(Player);
  if (Player < 3) {
    WriteLower("BONUS =       ");
    DisplayScore(4, IntBuffer*1000);}
  else {
    WriteUpper("BONUS =       ");
    DisplayScore(2, IntBuffer*1000);}
  if (IntBuffer) {
    ActivateTimer(20, 0, PB_CountBonus);}
  else {
    PB_BallEnd2();}}

void PB_BallEnd2() {
	BlockOuthole = false;														// remove outhole block
	if (ExBalls) {                                	// Player has extra balls
		AddBlinkLamp(33, 250);                      	// Let the extra ball lamp blink
		ExBalls--;
		ActivateTimer(2000, 0, PB_AfterExBallRelease);
		ActivateTimer(1000, AppByte, PB_NewBall);}
	else {                                        	// Player has no extra balls
		Lamp[51] = false;
		if (false) {
//		if ((Points[Player] > HallOfFame.Scores[3]) && (Ball == APC_settings[NofBalls])) { // last ball & high score?
//			Switch_Pressed = DummyProcess;              // Switches do nothing
			// CheckHighScore(Player);
		}
		else {
      if ((PB_EjectMode[Player] == 4) || (PB_EjectMode[Player] == 9)) { // eject hole mode maxed out?
        PB_EjectMode[Player] = 0;}                // reset it for the next ball
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
					GameDefinition.AttractMode();}}}}}

void PB_ResetHighScores(bool change) {                // delete the high scores file
  if (change) {                                       // if the start button has been pressed
    if (SDfound) {
      SD.remove(GameDefinition.HighScoresFileName);
      struct HighScores HScBuffer;                    // create new high score table
      HallOfFame = HScBuffer;                         // copy it to the hall of fame
      WriteLower(" SCORES DONE  ");}
    else {
      WriteLower(" SCORES NO SD ");}}
  else {
    WriteLower(" SCORES       ");}}              

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


