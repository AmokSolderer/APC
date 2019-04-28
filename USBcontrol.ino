// USB interface for APC based pinball machines

const byte USB_SearchCoils[15] = {1,4,6,8,13,15,16,17,18,19,20,21,22,14,0}; // coils to fire when the ball watchdog timer runs out
unsigned int USB_SolTimes[32]; 												// Activation times for solenoids
const byte USB_CommandLength[101] = {0,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 0 - 9
																		1,1,1,0,0,0,0,0,0,0,		// Length of USB commands from 10 - 19
																		1,1,1,1,2,0,0,0,0,0,		// Length of USB commands from 20 - 29
																		255,255,255,255,255,255,255,1,0,0,	// Length of USB commands from 30 - 39
																		1,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 40 - 49
																		1,0,255,255,1,0,0,0,0,0,	// Length of USB commands from 50 - 59
																		0,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 60 - 69
																		0,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 70 - 79
																		0,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 80 - 89
																		0,0,0,0,0,0,0,0,0,0,0};	// Length of USB commands from 90 - 101

const byte USB_defaults[64] = {0,0,0,0,0,0,0,0,		 		// game default settings
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0};

byte USB_ChangedSwitches[64];

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
	if (APC_settings[Volume]) {                     		// system set to digital volume control?
		analogWrite(VolumePin,255-APC_settings[Volume]);} // adjust PWM to volume setting
	else {
		digitalWrite(VolumePin,HIGH);}                		// turn off the digital volume control
	for (i=0; i<8; i++) {																// turn off all lamps
		LampColumns[i] = 0;}
	LampPattern = LampColumns;
	Switch_Pressed = USB_SwitchHandler;
	Switch_Released = USB_ReleasedSwitches;
	WriteUpper("  USB  CONTROL  ");
	WriteLower("                ");}

void USB_SwitchHandler(byte Switch) {
	switch (Switch) {
	case 8:                                             // high score reset
		digitalWrite(Blanking, LOW);                      // invoke the blanking
		break;
	case 72:																						// advance button
		if (QuerySwitch(73)) {														// Up/Down switch pressed?
			ActivateTimer(1000, 0, USB_Testmode);}					// look again in 1s
		else {
			byte i = 0;
			while (USB_ChangedSwitches[i]) {
				i++;}
			USB_ChangedSwitches[i] = Switch | 128;}
		break;
	default:
		byte i = 0;
		while (USB_ChangedSwitches[i]) {
			i++;}
		USB_ChangedSwitches[i] = Switch | 128;}}

void USB_ReleasedSwitches(byte Switch) {
	switch (Switch) {
	case 8:                                             // high score reset
		break;
	default:
		byte i = 0;
		while (USB_ChangedSwitches[i]) {
			i++;}
		USB_ChangedSwitches[i] = Switch;}}

void USB_Testmode(byte Dummy) {												// enter system settings if advance button still pressed
	UNUSED(Dummy);
	if (QuerySwitch(72)) {
		SerialCommand = 0;
		Settings_Enter();}
	else {
		byte i = 0;
		while (USB_ChangedSwitches[i]) {
			i++;}
		USB_ChangedSwitches[i] = 72 | 128;}}

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
		if (!c) {
			BufferPointer = 0;
			CommandPending = true;
			return;}
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
	switch (Command) {																	// execute command if complete
	if (APC_settings[DebugMode]) {
		for (i=1; i<24; i++) {                        		// move all characters in the lower display row 4 chars to the left
			DisplayLower[i] = DisplayLower[i+8];}
		*(DisplayLower+30) = DispPattern2[32 + 2 * (Command % 10)]; // and insert the switch number to the right of the row
		*(DisplayLower+31) = DispPattern2[33 + 2 * (Command % 10)];
		*(DisplayLower+28) = DispPattern2[32 + 2 * (Command - (Command % 10)) / 10];
		*(DisplayLower+29) = DispPattern2[33 + 2 * (Command - (Command % 10)) / 10];}
	case 0:																							// get connected hardware
		Serial.print("APC");
		Serial.write((byte) 0);
		break;
	case 3:																							// get number of lamps
	case 9:																							// get number of switches
		Serial.write((byte) 64);
		break;
	case 4:																							// get number of solenoids
		Serial.write((byte) 24);
		break;
	case 10:																						// get status of lamp
		if (SerialBuffer[0] < 65) {												// max 64 lamps
			Serial.write((byte) QueryLamp(SerialBuffer[0]));}
		else {
			Serial.write((byte) 2);}
		break;
	case 11:																						// turn on lamp
		if (SerialBuffer[0] < 65) {												// max 64 lamps
			TurnOnLamp(SerialBuffer[0]);}
		break;
	case 12:																						// turn off lamp
		if (SerialBuffer[0] < 65) {												// max 64 lamps
			TurnOffLamp(SerialBuffer[0]);}
		break;
	case 20:																						// get status of solenoid
		if (SerialBuffer[0] < 25) {												// max 24 solenoids
			Serial.write((byte) QuerySolenoid(SerialBuffer[0]));}
		break;
	case 21:																						// set solenoid # to on
		if (SerialBuffer[0] < 25) {												// max 24 solenoids
			if (!SolChange) {                               // change request for another solenoid pending?
				SolNumber = SerialBuffer[0];                  // activate solenoid permanently
				SolState = true;
				SolChange = true;}
			else {                                        	// if a change request is already pending
				i = 0;
				while (SolDelayed[i]) {                     	// look for a free slot in the list of solenoids to be processed later
					i++;
					if (i > 20) {
						ErrorHandler(31,SerialBuffer[0],0);
						break;}}
				SolDelayed[i] = SerialBuffer[0];              // insert the solenoid number
				DurDelayed[i] = 0;                   					// and its duration into the list
				ActivateTimer(25, SerialBuffer[0], ActivateLater);}}	// and try again later
		break;
	case 22:																						// set solenoid # to off
		if (SerialBuffer[0] < 25) {												// max 24 solenoids
			ReleaseSolenoid(SerialBuffer[0]);}
		break;
	case 23:																						// pulse solenoid
		if (SerialBuffer[0] < 25) {												// max 24 solenoids
			ActivateSolenoid(0, SerialBuffer[0]);}
		break;
	case 24:																						// set solenoid pulse time
		if (SerialBuffer[0] < 25) {												// max 24 solenoids
			USB_SolTimes[SerialBuffer[0]-1] = SerialBuffer[1];}
		break;
	case 30:																						// set display 0 to (credit display)
		if (APC_settings[DisplayType] > 4) {
			DisplayBCD(0, SerialBuffer);}
		else {
			WritePlayerDisplay((char*)SerialBuffer, 0);}
		break;
	case 31:																						// set display 1 to
		if (APC_settings[DisplayType] > 4) {
			DisplayBCD(1, SerialBuffer);}
		else {
			WritePlayerDisplay((char*)SerialBuffer, 1);}
		break;
	case 32:																						// set display 2 to
		if (APC_settings[DisplayType] > 4) {
			DisplayBCD(2, SerialBuffer);}
		else {
			WritePlayerDisplay((char*)SerialBuffer, 2);}
		break;
	case 33:																						// set display 3 to
		if (APC_settings[DisplayType] > 4) {
			DisplayBCD(3, SerialBuffer);}
		else {
			WritePlayerDisplay((char*)SerialBuffer, 3);}
		break;
	case 34:																						// set display 4 to
		if (APC_settings[DisplayType] > 4) {
			DisplayBCD(4, SerialBuffer);}
		else {
			WritePlayerDisplay((char*)SerialBuffer, 4);}
		break;
	case 40:																						// get status of switch #
		if (SerialBuffer[0] < 65) {												// max 64 switches
			if (QuerySwitch(SerialBuffer[0])) {							// query state
				Serial.write((byte) 1);}
			else {
				Serial.write((byte) 0);}}
		else {
			Serial.write((byte) 2);}
		break;
	case 41:																						// get changed switches
		if (USB_ChangedSwitches[0]) {											// any changed switches?
			i = 0;
			Serial.write((byte) USB_ChangedSwitches[0]);		// send it
			do {
				USB_ChangedSwitches[i] = USB_ChangedSwitches[i+1];
				i++;}
			while (USB_ChangedSwitches[i]);} 								// still more changed switches?
		else {
			Serial.write((byte) 127);}											// no changed switches at all
		break;
	case 51:																						// stop sound
		StopPlayingSound();
		break;
	case 52:																						// play soundfile
		PlaySound(50, (char*) SerialBuffer);
		break;
	case 54:																						// sound volume setting
		APC_settings[Volume] = 2*SerialBuffer[0];					// set system volume
		analogWrite(VolumePin,255-APC_settings[Volume]);	// and apply it
		break;
	default:
		if (APC_settings[DisplayType] == 3) {           	// 2x16 alphanumeric display (BK2K type)
			WriteUpper2("UNKNOWN COMMAND ");}
		else {
			WriteUpper2("UNKNOWNCOMMAND  ");}
		WriteLower2("                ");
		ShowNumber(31, Command);               						// show command number
		ShowMessage(3);}}
