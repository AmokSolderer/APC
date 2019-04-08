// USB interface for APC based pinball machines

const byte USB_SearchCoils[15] = {1,4,6,8,13,15,16,17,18,19,20,21,22,14,0}; // coils to fire when the ball watchdog timer runs out
const unsigned int USB_SolTimes[32] = {30,30,50,50,50,50,30,1000,100,0,0,0,50,50,50,50,50,1000,50,50,50,999,0,0,100,100,100,100,100,100,100,100}; // Activation times for solenoids
const byte USB_CommandLength[101] = {0,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 0 - 9
																			1,1,1,0,0,0,0,0,0,0,	// Length of USB commands from 10 - 19
																			1,1,1,1,2,0,0,0,0,0,	// Length of USB commands from 20 - 29
																			255,255,255,255,255,255,255,1,0,0,	// Length of USB commands from 30 - 39
																			1,0,0,0,0,0,0,0,0,0,	// Length of USB commands from 40 - 49
																			1,0,255,255,1,0,0,0,0,0,	// Length of USB commands from 50 - 59
																			0,0,0,0,0,0,0,0,0,0,	// Length of USB commands from 60 - 69
																			0,0,0,0,0,0,0,0,0,0,	// Length of USB commands from 70 - 79
																			0,0,0,0,0,0,0,0,0,0,	// Length of USB commands from 80 - 89
																			0,0,0,0,0,0,0,0,0,0,0};// Length of USB commands from 90 - 101

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
	for (i=0; i< LampMax+1; i++) {											// turn off all lamps
		Lamp[i] = false;}
	LampPattern = Lamp;
	Switch_Pressed = USB_SwitchHandler;
	Switch_Released = DummyProcess;
	WriteUpper("  USB  CONTROL  ");
	WriteLower("                ");}

void USB_SwitchHandler(byte Switch) {
	switch (Switch) {
	case 8:                                             // high score reset
		digitalWrite(Blanking, LOW);                      // invoke the blanking
		break;
	}}

void USB_SerialCommand() {
	static byte Command;
	static bool CommandPending;
	static byte SerialBuffer[128];
	static byte BufferPointer;
	byte c = 0;
	byte i = 0;
	if (!CommandPending) {															// any unfinished business?
		Command = Serial.read();}													// if not read new command
	CommandPending = false;
	if (USB_CommandLength[Command] == 255) {
		c = Serial.available();
		i = BufferPointer;
		do {
			SerialBuffer[i] = Serial.read();
			i++;}
		while ((SerialBuffer[i-1]) && ((i - BufferPointer) < c));
		if (SerialBuffer[i-1]) {
			CommandPending = true;
			BufferPointer = i;
			return;}
		BufferPointer = 0;}
	else {
		if (Serial.available() >= USB_CommandLength[Command]) {
			for (i=0; i<USB_CommandLength[Command]; i++) {
				SerialBuffer[i] = Serial.read();}}
		else {
			CommandPending = true;
			return;}}
	switch (Command) {
	if (APC_settings[DebugMode]) {
		for (i=1; i<24; i++) {                        // move all characters in the lower display row 4 chars to the left
			DisplayLower[i] = DisplayLower[i+8];}
		*(DisplayLower+30) = DispPattern2[32 + 2 * (Command % 10)]; // and insert the switch number to the right of the row
		*(DisplayLower+31) = DispPattern2[33 + 2 * (Command % 10)];
		*(DisplayLower+28) = DispPattern2[32 + 2 * (Command - (Command % 10)) / 10];
		*(DisplayLower+29) = DispPattern2[33 + 2 * (Command - (Command % 10)) / 10];}
	case 0:																						// get connected hardware
		Serial.print("APC");
		Serial.write((byte) 0);
		break;
	case 10:																					// get status of lamp
		if (SerialBuffer[0] < 65) {														// max 64 lamps
			Serial.write((byte) Lamp[SerialBuffer[0]]);}
		else {
			Serial.write((byte) 2);}
		break;
	case 11:																					// turn on lamp
		if (SerialBuffer[0] < 65) {														// max 64 lamps
			Lamp[SerialBuffer[0]] = true;}
		break;
	case 12:																					// turn off lamp
		if (SerialBuffer[0] < 65) {														// max 64 lamps
			Lamp[SerialBuffer[0]] = false;}
		break;
	default:
		if (APC_settings[DisplayType] == 3) {           // 2x16 alphanumeric display (BK2K type)
			WriteUpper2("UNKNOWN COMMAND ");}
		else {
			WriteUpper2("UNKNOWNCOMMAND  ");}
		WriteLower2("                ");
		ShowNumber(31, Command);               					// show command number
		ShowMessage(3);}}
