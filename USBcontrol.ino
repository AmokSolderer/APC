// USB interface for APC based pinball machines

const byte USB_SearchCoils[15] = {1,4,6,8,13,15,16,17,18,19,20,21,22,14,0}; // coils to fire when the ball watchdog timer runs out
const unsigned int USB_SolTimes[32] = {30,30,50,50,50,50,30,1000,100,0,0,0,50,50,50,50,50,1000,50,50,50,999,0,0,100,100,100,100,100,100,100,100}; // Activation times for solenoids

const byte USB_defaults[64] = {0,0,0,0,0,0,0,0,		 		// game default settings
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0};

struct SettingTopic USB_setList[4] = {{" TIMED  MAGNA ",HandleBoolSetting,0,0,0}, // defines the game specific settings
		{"RESTOREDEFAULT",RestoreDefaults,0,0,0},
		{"  EXIT SETTNGS",ExitSettings,0,0,0},
		{"",NULL,0,0,0}};

struct GameDef USB_GameDefinition = {
		USB_setList,																			// GameSettingsList
		(byte*)USB_defaults,															// GameDefaultsPointer
		"USB_SET.BIN",																		// GameSettingsFileName
		"USBSCORE.BIN",																		// HighScoresFileName
		USB_AttractMode,																	// AttractMode
		USB_SolTimes};																		// Default activation times of solenoids

void USB_init() {
	SerialCommand = USB_SerialCommand;
	Switch_Pressed = DummyProcess;
	GameDefinition = USB_GameDefinition;}								// read the game specific settings and highscores

void USB_AttractMode() {                              // Attract Mode
	DispRow1 = DisplayUpper;
	DispRow2 = DisplayLower;
	digitalWrite(VolumePin,HIGH);                       // set volume to zero
	LampPattern = NoLamps;
	//Switch_Pressed = USB_AttractModeSW;
	Switch_Released = DummyProcess;
	WriteUpper("  USB  CONTROL  ");
	WriteLower("                ");}

void USB_SerialCommand(byte Command) {
	if (SerialCommandPending) {													// any unfinished business?
	}
	else {
		switch (Command) {
		case 0:
			Serial.print("APC2v2");
			break;
		default:
			if (APC_settings[DisplayType] == 3) {           // 2x16 alphanumeric display (BK2K type)
				WriteUpper2("UNKNOWN COMMAND ");}
			else {
				WriteUpper2("UNKNOWNCOMMAND  ");}
			WriteLower2("                ");
			ShowNumber(31, Command);               					// show command number
			ShowMessage(3);}}}
