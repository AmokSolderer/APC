// Rules for the Pinbot pinball machine

const unsigned int PB_SolTimes[24] = {30,30,30,50,30,200,30,30,500,500,999,999,999,999,500,500,50,500,50,50,50,50,0,0}; // Activation times for solenoids
const unsigned int PB_C_BankTimes[8] = {50,500,500,500,500,500,500,500};

struct SettingTopic PB_setList[4] = {{"DROP TG TIME  ",HandleNumSetting,0,3,10},
       {"RESTOREDEFAULT",RestoreDefaults,0,0,0},
       {"  EXIT SETTNGS",ExitSettings,0,0,0},
       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

const struct LampPat PB_AttractPat1[4] = {{300,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														           	{300,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {300,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
																				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

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
	PatPointer = PB_AttractPat1;
	FlowRepeat = 100;
	ShowLampPatterns(0);
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
		if (digitalRead(UpDown)) {
			WriteUpper("  TEST  MODE  ");
			WriteLower("              ");
			AppByte = 0;
			ActivateTimer(1000, 0, PB_Testmode);}
		else {
			Settings_Enter();}
		break;
	}}

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
	case 4:
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
				// AppByte++;}
				GameDefinition.AttractMode();
				return;
			PB_Testmode(0);}
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
		ActivateSolenoid(700, 14);												// activate the relay
		ActivateTimer(100, Solenoid, PB_ActivateC_Bank);	// and delay the solenoid
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
		ActivateSolenoid(0, Solenoid);                    // activate the solenoid
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

void PB_ActivateC_Bank(byte Solenoid) {
	ActivateSolenoid(PB_C_BankTimes[Solenoid], Solenoid);}

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


