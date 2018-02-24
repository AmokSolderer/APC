// Rules for the Pinbot pinball machine

const unsigned int PB_SolTimes[24] = {30,30,30,50,30,200,30,30,500,500,999,999,999,999,500,500,50,500,50,50,50,50,0,0}; // Activation times for solenoids

struct SettingTopic PB_setList[4] = {{"DROP TG TIME  ",HandleNumSetting,0,3,10},
       {"RESTOREDEFAULT",RestoreDefaults,0,0,0},
       {"  EXIT SETTNGS",ExitSettings,0,0,0},
       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

const byte PB_defaults[64] = {5,0,0,0,0,0,0,0,		 		// game default settings
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
	WriteUpper("THE APC PINBOT");
}

void PB_AttractModeSW(byte Select) {
	switch(Select) {
	case 3:																							// credit button
			// enter game
		break;
	case 72:
		Switch_Pressed = DummyProcess;
		LampPattern = NoLamps;                          	// Turn off all lamps
		ReleaseAllSolenoids();
		if (!digitalRead(UpDown)) {
			Settings_Enter();}
		else {
			WriteUpper("  TEST  MODE  ");
			WriteLower("              ");
			AppByte = 0;
			ActivateTimer(1000, 0, PB_Testmode);}
		break;
	}}

void PB_Testmode(byte Select) {
	Switch_Pressed = PB_Testmode;
	switch(AppByte) {																		// which testmode?
	case 0:																							// display test
		switch(Select) {																	// switch events
		case 0:
			*(DisplayLower) = 0;
			*(DisplayLower+16) = 0;
			*(DisplayUpper) = 0;
			*(DisplayUpper+16) = 0;
			WriteUpper("DISPLAY TEST  ");
			WriteLower("              ");
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
			PB_Testmode(0);
			break;}
		break;
	case 1:
		WriteUpper(" SWITCHEDGES  ");
	}}

void PB_DisplayCycle(byte CharNo) {                   // Display cycle test
  if (!digitalRead(UpDown)) {                         // cycle only if Up/Down switch is not pressed
    if (CharNo == 118) {                             	// if the last character is reached
    	CharNo = 34;}                                 	// start from the beginning
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
  AppByte = ActivateTimer(500, CharNo, DisplayCycle);}   // restart timer


