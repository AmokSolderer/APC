// USB interface for APC based pinball machines

unsigned int USB_SolTimes[32] = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 0, 0, 40, 40, 40, 40, 40, 40, 40, 40};	// Activation times for solenoids
const byte USB_CommandLength[102] = {0,0,0,0,0,0,0,1,0,0,		// Length of USB commands from 0 - 9
															1,1,1,0,0,0,0,0,0,0,		// Length of USB commands from 10 - 19
															1,1,1,1,2,2,0,0,0,0,		// Length of USB commands from 20 - 29
															250,250,250,250,250,0,0,2,0,0,		// Length of USB commands from 30 - 39
															1,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 40 - 49
															2,1,251,0,2,0,0,0,0,0,	// Length of USB commands from 50 - 59
															10,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 60 - 69
															0,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 70 - 79
															0,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 80 - 89
															0,0,0,0,0,0,0,0,0,0,0,0};	// Length of USB commands from 90 - 101
const byte USB_DisplayDigitNum[8][6] = {{4,7,7,7,7,0},{4,7,7,7,7,0},{0,7,7,7,7,0},{0,16,16,0,0,0},{0,16,16,7,0,0},{0,16,16,7,4,0},{4,6,6,6,6,0},{4,7,7,7,7,0}};
const byte USB_DisplayTypes[8][6] = {{3,4,4,4,4,0},{3,4,4,3,3,0},{0,4,4,3,3,0},{0,4,4,0,0,0},{0,4,3,3,0,0},{0,4,3,3,3,0},{1,1,1,1,1,0},{1,2,2,2,2,0}};

																											// offsets of settings in the settings array
#define USB_Watchdog 0																// watchdog enable setting
#define USB_Debug 1																		// USB debug mode

const byte USB_defaults[64] = {0,0,0,0,0,0,0,0,		 		// game default settings
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0};

byte USB_ChangedSwitches[64];
byte USB_HWrule_ActSw[16][3];													// hardware rules for activated switches
byte USB_HWrule_RelSw[16][3];													// hardware rules for released switches
byte USB_SolRecycleTime[22];													// recycle time for each solenoid
byte USB_SolTimers[22];																// stores the sol timer numbers and indicates which solenoids are blocked due to active recycling time
byte USB_DisplayProtocol[5];													// stores the selected display protocol
char USB_RepeatSound[13];															// name of the sound file to be repeated
char USB_RepeatMusic[13];															// name of the music file to be repeated
byte USB_WaitingSoundFiles[2][14];										// names of the waiting sound files first byte is for channel and commands
byte USB_WaitSoundTimer;															// number of the timer for the sound sequencing

char TxTUSB_debug[3][17] = {{"          OFF   "},{"        USB     "},{"        AUDIO   "}};

struct SettingTopic USB_setList[5] = {{"USB WATCHDOG  ",HandleBoolSetting,0,0,0}, // defines the game specific settings
		{" DEBUG  MODE    ",HandleTextSetting,&TxTUSB_debug[0][0],0,2},
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
	for (i=0; i<6; i++) {
		USB_DisplayProtocol[i] = USB_DisplayTypes[APC_settings[DisplayType]][i];} // use default protocol for displays
	USB_WatchdogHandler(1);															// initiate reset and start watchdog
	WriteUpper("  USB  CONTROL  ");
	WriteLower("                ");}

void USB_WatchdogHandler(byte Event) {								// Arg = 0->Reset WD / 1-> Reset & start WD / 2-> WD has run out / 3-> stop WD
	static byte USB_WatchdogTimer;
	byte i=0;
	if (!Event) {																				// reset watchdog
		Serial.write((byte) 0);														// send OK
		if (USB_WatchdogTimer) {
			KillTimer(USB_WatchdogTimer);}									// restart timer
		if (game_settings[USB_Watchdog]) {								// watchdog enabled?
			USB_WatchdogTimer = ActivateTimer(1000, 2, USB_WatchdogHandler);} // start the timer
		else {																						// watchdog disabled?
			USB_WatchdogTimer = 0;}}												// mark timer as inactive
	else {
		if (Event == 3) {																	// stop watchdog
			if (USB_WatchdogTimer) {
				KillTimer(USB_WatchdogTimer);
				USB_WatchdogTimer = 0;}}
		else {
			if (Event == 1) {																// initiate reset and start watchdog
				if (USB_WatchdogTimer) {
					KillTimer(USB_WatchdogTimer);
					USB_WatchdogTimer = 0;}
				for (i=0; i<16; i++) {												// delete all HW rules
					USB_HWrule_ActSw[i][0] = 0;
					USB_HWrule_RelSw[i][0] = 0;}
				Serial.write((byte) 0);}											// send OK
			else {																					// timer has run out
				if (!game_settings[USB_Watchdog]) {						// watchdog disabled?
					USB_WatchdogTimer = 0;
					return;}																		// then leave
				WriteUpper2(" USB WATCHDOG   ");
				WriteLower2("                ");
				ShowMessage(3);}
			ReleaseAllSolenoids();													// switch off all coils
			for (i=0; i<63; i++) {													// clear switch buffer
				USB_ChangedSwitches[i] = 0;}
			for (i=0; i<8; i++) {														// turn off all lamps
				LampColumns[i] = 0;}
			USB_WatchdogTimer = ActivateTimer(1000, 2, USB_WatchdogHandler);}}}	// restart watchdog

void USB_SwitchHandler(byte Switch) {
	byte i = 0;
	switch (Switch) {
	case 8:                                             // high score reset
		digitalWrite(Blanking, LOW);                      // invoke the blanking
		break;
	case 72:																						// advance button
		while (USB_ChangedSwitches[i] && (i<63)) {
			i++;}
		USB_ChangedSwitches[i] = Switch | 128;						// send switch code to USB
		if (QuerySwitch(73)) {														// Up/Down switch pressed?
			ActivateTimer(1000, 0, USB_Testmode);}					// look again in 1s
		break;
	default:
		while (USB_HWrule_ActSw[i][0]) {									// check for HW rules for this switch
			if (USB_HWrule_ActSw[i][0] == Switch) {
				if (USB_HWrule_ActSw[i][2]) {									// duration != 0 ?
					USB_FireSolenoid( USB_HWrule_ActSw[i][2], USB_HWrule_ActSw[i][1]);}
				else {
					USB_KillSolenoid(USB_HWrule_ActSw[i][1]);}
				break;}
			i++;}
		i = 0;																						// add switch number to list of changed switches
		while (USB_ChangedSwitches[i] && (i<63)) {
			i++;}
		USB_ChangedSwitches[i] = Switch | 128;}}

void USB_ReleasedSwitches(byte Switch) {
	switch (Switch) {
	case 8:                                             // high score reset
		break;
	default:
		byte i = 0;
		while (USB_HWrule_RelSw[i][0]) {									// check for HW rules for this switch
			if (USB_HWrule_RelSw[i][0] == Switch) {
				if (USB_HWrule_RelSw[i][2]) {									// duration != 0 ?
					USB_FireSolenoid( USB_HWrule_RelSw[i][2], USB_HWrule_RelSw[i][1]);}
				else {
					USB_KillSolenoid(USB_HWrule_RelSw[i][1]);}
				break;}
			i++;}
		i = 0;																						// add switch number to list of changed switches
		while (USB_ChangedSwitches[i] && (i<63)) {
			i++;}
		USB_ChangedSwitches[i] = Switch;}}

void USB_Testmode(byte Dummy) {												// enter system settings if advance button still pressed
	UNUSED(Dummy);
	if (QuerySwitch(72)) {															// advance button still pressed?
		USB_WatchdogHandler(3);														// stop USB watchdog
		SerialCommand = 0;
		Settings_Enter();}}

void USB_SerialCommand() {
	static byte Command;
	static bool CommandPending;
	static byte SerialBuffer[128];
	static byte BufferPointer;
	byte c = 0;
	byte i = 0;
	if (!CommandPending) {															// any unfinished business?
		Command = Serial.read();}													// if not read new command
	if (USB_CommandLength[Command] > 249) {							// command doesn't have a constant length
		switch (USB_CommandLength[Command]) {
		case 250:																					// argument length is stored in the first byte
			if (BufferPointer) {														// length byte already stored?
				c = SerialBuffer[0];}													// read previously stored argument length
			else {
				if (Serial.available()) {											// length byte available?
					BufferPointer = 1;													// indicated that the length is read
					c = Serial.read();}													// read argument length
				else {
					BufferPointer = 0;
					CommandPending = true;											// command not finished
					return;}}
			if (Serial.available() >= c) { 									// enough bytes in the serial buffer?
				for (i=0; i<c; i++) {													// read the required amount of bytes
					SerialBuffer[i] = Serial.read();}}
			else {																					// not enough bytes in the buffer
				CommandPending = true;												// command not finished
				SerialBuffer[0] = c;													// store argument length for next round
				return;}
			break;
		case 251:
			c = Serial.available();
			i = BufferPointer;
			if (!BufferPointer) {														// first run?
				if (c < 3) {																	// 3 bytes needed at least
					CommandPending = true;
					return;}
				SerialBuffer[0] = Serial.read();							// store track number
				i++;
				SerialBuffer[1] = Serial.read();							// store options byte
				i++;}
			do {																						// receive bytes
				SerialBuffer[i] = Serial.read();							// and store them
				i++;}
			while ((SerialBuffer[i-1]) && ((i - BufferPointer) < c)); // until a 0 is read or serial buffer is empty
			if (SerialBuffer[i-1]) {												// last byte not zero
				CommandPending = true;												// command not finished
				BufferPointer = i;
				return;}
			break;
		case 255:																					// argument is terminated by a zero byte
			c = Serial.available();
			i = BufferPointer;
			if (!c) {																				// no further received bytes
				CommandPending = true;
				return;}
			do {																						// receive bytes
				SerialBuffer[i] = Serial.read();							// and store them
				i++;}
			while ((SerialBuffer[i-1]) && ((i - BufferPointer) < c)); // until a 0 is read or serial buffer is empty
			if (SerialBuffer[i-1]) {												// last byte not zero
				CommandPending = true;												// command not finished
				BufferPointer = i;
				return;}
			break;}}
	else {																							// argument has a specific length
		if (Serial.available() >= USB_CommandLength[Command]) { // enough bytes in the serial buffer?
			for (i=0; i<USB_CommandLength[Command]; i++) {	// read the required amount of bytes
				SerialBuffer[i] = Serial.read();}}
		else {																						// not enough bytes in the buffer
			CommandPending = true;													// command not finished
			return;}}
	CommandPending = false;
	BufferPointer = 0;
	if (game_settings[USB_Debug] == 1) {
		for (i=1; i<24; i++) {                        		// move all characters in the lower display row 4 chars to the left
			DisplayLower[i] = DisplayLower[i+8];}
		*(DisplayLower+30) = DispPattern2[32 + 2 * (Command % 10)]; // and insert the command number to the right of the row
		*(DisplayLower+31) = DispPattern2[33 + 2 * (Command % 10)];
		*(DisplayLower+28) = DispPattern2[32 + 2 * (Command - (Command % 10)) / 10];
		*(DisplayLower+29) = DispPattern2[33 + 2 * (Command - (Command % 10)) / 10];
		*(DisplayLower+26) = DispPattern2[32 + 2 * (Command - (Command % 100)) / 100];
		*(DisplayLower+27) = DispPattern2[33 + 2 * (Command - (Command % 100)) / 100];}
	switch (Command) {																	// execute command if complete
	case 0:																							// get connected hardware
		Serial.print("APC");
		Serial.write((byte) 0);
		break;
	case 1:																							// get firmware version
		Serial.print(APC_Version);
		Serial.write((byte) 0);
		break;
	case 2:																							// get API version
		Serial.print("0.09");
		Serial.write((byte) 0);
		break;
	case 3:																							// get number of lamps
		Serial.write((byte) 65);
		break;
	case 4:																							// get number of solenoids
		Serial.write((byte) 25);
		break;
	case 6:																							// get number of displays
		switch (APC_settings[DisplayType]) {
		case 0:																						// 4 ALPHA+CREDIT
		case 1:																						// Sys11 Pinbot
		case 2:																						// Sys11 F-14
		case 5:																						// Sys11 Riverboat Gambler
		case 6:																						// Sys3 - 6
		case 7:																						// Sys7 + 9
			Serial.write((byte) 5);
			break;
		case 3:																						// Sys11 BK2K
			Serial.write((byte) 3);
			break;
		case 4:																						// Sys11 Taxi
			Serial.write((byte) 4);
			break;
		default:																					// unknown display type
			Serial.write((byte) 0);
			break;}
		break;
	case 7:																							// Display details
		Serial.write((byte) USB_DisplayTypes[APC_settings[DisplayType]][SerialBuffer[0]]);
		Serial.write((byte) USB_DisplayDigitNum[APC_settings[DisplayType]][SerialBuffer[0]]);
		break;
	case 9:																							// get number of switches
		Serial.write((byte) 73);
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
	case 19:																						// get number of modern lights
		Serial.write((byte) 0);
		break;
	case 20:																						// get status of solenoid
		if (SerialBuffer[0] < 26) {												// max 24 solenoids
			Serial.write((byte) QuerySolenoid(SerialBuffer[0]));}
		break;
	case 21:																						// set solenoid # to on
		if (SerialBuffer[0] < 25) {												// max 24 solenoids
			if (!USB_SolTimers[SerialBuffer[0]-1]) {				// recycling time over for this coil?
				SolChange = false;														// block IRQ solenoid handling
				if (SerialBuffer[0] > 8) {										// does the solenoid not belong to the first latch?
					if (SerialBuffer[0] < 17) {									// does it belong to the second latch?
						SolBuffer[1] |= 1<<(SerialBuffer[0]-9);		// latch counts from 0
						SolLatch |= 2;}														// select second latch
					else {
						SolBuffer[2] |= 1<<(SerialBuffer[0]-17);
						SolLatch |= 4;}}													// select third latch
				else {
					SolBuffer[0] |= 1<<(SerialBuffer[0]-1);
					SolLatch |= 1;}															// select first latch
				SolChange = true;}}
		else if (SerialBuffer[0] == 25) {									// 25 is a shortcut for both flipper fingers
			ActivateSolenoid(0, 23);												// enable both flipper fingers
			ActivateSolenoid(0, 24);}
		break;
	case 22:																						// set solenoid # to off
		if (SerialBuffer[0] < 25) {												// max 24 solenoids
			USB_KillSolenoid(SerialBuffer[0]);}
		else if (SerialBuffer[0] == 25) {									// 25 is a shortcut for both flipper fingers
			ReleaseSolenoid(23);														// disable both flipper fingers
			ReleaseSolenoid(24);}
		break;
	case 23:																						// pulse solenoid
		if (SerialBuffer[0] < 25) {												// max 24 solenoids
			USB_FireSolenoid(USB_SolTimes[SerialBuffer[0]-1], SerialBuffer[0]);}
		break;
	case 24:																						// set solenoid pulse time
		if (SerialBuffer[0] < 25) {												// max 24 solenoids
			USB_SolTimes[SerialBuffer[0]-1] = SerialBuffer[1];}
		break;
	case 25:																						// set solenoid recycle time
		USB_SolRecycleTime[SerialBuffer[0]-1] = SerialBuffer[1];
		break;
	case 30:																						// set display 0 to (credit display)
		switch (APC_settings[DisplayType]) {							// which display is used?
		case 0:																						// 4 ALPHA+CREDIT
		case 1:																						// Sys11 Pinbot
			switch (USB_DisplayProtocol[0]) {								// which protocol shall be used?
			case 1:																					// BCD
			case 2:																					// BCD with comma (not possible as credit has no comma)
				*(DisplayUpper) = LeftCredit[(SerialBuffer[0]+16)*2];
				//*(DisplayUpper+1) = LeftCredit[((SerialBuffer[0]+16)*2)+1];
				*(DisplayUpper+16) = LeftCredit[(SerialBuffer[1]+16)*2];
				//*(DisplayUpper+17) = LeftCredit[((SerialBuffer[1]+16)*2)+1];
				*(DisplayLower) = RightCredit[(SerialBuffer[2]+16)*2];
				//*(DisplayLower+1) = RightCredit[((SerialBuffer[2]+16)*2)+1];
				*(DisplayLower+16) = RightCredit[(SerialBuffer[3]+16)*2];
				//*(DisplayLower+17) = RightCredit[((SerialBuffer[3]+16)*2)+1];
				break;
			case 3:																					// 7 segment pattern (1 byte)
				*(DisplayUpper) = SerialBuffer[0];
				*(DisplayUpper+16) = SerialBuffer[1];
				*(DisplayLower) = ConvertPattern(0, SerialBuffer[2]);
				*(DisplayLower+16) = ConvertPattern(0, SerialBuffer[3]);
				break;
			case 4:																					// 14 segment pattern (2 bytes)
				*(DisplayUpper) = SerialBuffer[0];
				*(DisplayUpper+16) = SerialBuffer[2];
				*(DisplayLower) = ConvertPattern(0, SerialBuffer[4]);
				*(DisplayLower+16) = ConvertPattern(0, SerialBuffer[6]);
				break;
			case 5:																					// ASCII
			case 6:																					// ASCII with comma (not possible as credit has no comma)
				WritePlayerDisplay((char*)SerialBuffer, 0);
				break;}
			break;
			case 6:																					// Sys3 - 6 display

				break;
			case 7:																					// Sys7 + 9 display
				switch (USB_DisplayProtocol[0]) {							// which protocol shall be used?
				case 1:																				// BCD
				case 2:																				// BCD with comma
					DisplayBCD(0, SerialBuffer);
					break;
				case 5:
				case 6:
					WritePlayerDisplay((char*)SerialBuffer, 0);
					break;}
				break;}
		break;
	case 31:																						// set display 1 to
		switch (APC_settings[DisplayType]) {							// which display is used?
		case 0:																						// 4 ALPHA+CREDIT
		case 1:																						// Sys11 Pinbot
		case 2:																						// Sys11 F-14
			switch (USB_DisplayProtocol[1]) {								// which protocol shall be used?
			case 1:																					// BCD
				for (i=0; i<7; i++) {
					*(DisplayUpper+2*i+2) = DispPattern1[32+2*SerialBuffer[i]];
					*(DisplayUpper+2*i+3) = DispPattern1[33+2*SerialBuffer[i]];}
				break;
			case 2:																					// BCD with comma
				for (i=0; i<7; i++) {
					if (SerialBuffer[i] & 128) {								// comma set?
						*(DisplayUpper+2*i+2) = 128 | DispPattern1[32+2*(SerialBuffer[i] & 15)];
						*(DisplayUpper+2*i+3) = 64 | DispPattern1[33+2*(SerialBuffer[i] & 15)];}
					else {
						*(DisplayUpper+2*i+2) = DispPattern1[32+2*SerialBuffer[i]];
						*(DisplayUpper+2*i+3) = DispPattern1[33+2*SerialBuffer[i]];}}
				break;
			case 3:																					// 7 segment pattern (1 byte)
				for (i=0; i<7; i++) {
					*(DisplayUpper+2*i+2) = SerialBuffer[i];
					if (SerialBuffer[i] & 64) {									// g segment set?
						*(DisplayUpper+2*i+3) = 4;}								// turn on m segment of alpha display
					else {
						*(DisplayUpper+2*i+1) = 0;}}
				break;
			case 4:																					// 14 segment pattern (2 bytes)
				for (i=0; i<14; i++) {
					*(DisplayUpper+i+2) = SerialBuffer[i];}
				break;
			case 5:																					// ASCII
			case 6:																					// ASCII with comma
				WritePlayerDisplay((char*)SerialBuffer, 1);
				break;}
			break;
			case 3:																					// Sys11 BK2K
			case 4:																					// Sys11 Taxi
				switch (USB_DisplayProtocol[1]) {							// which protocol shall be used?
				case 1:																				// BCD
					for (i=0; i<16; i++) {
						*(DisplayUpper+2*i) = DispPattern1[32+2*SerialBuffer[i]];
						*(DisplayUpper+2*i+1) = DispPattern1[33+2*SerialBuffer[i]];}
					break;
				case 2:																				// BCD with comma
					for (i=0; i<16; i++) {
						if (SerialBuffer[i] & 128) {							// comma set?
							*(DisplayUpper+2*i) = 128 | DispPattern1[32+2*(SerialBuffer[i] & 15)];
							*(DisplayUpper+2*i+1) = 64 | DispPattern1[33+2*(SerialBuffer[i] & 15)];}
						else {
							*(DisplayUpper+2*i) = DispPattern1[32+2*SerialBuffer[i]];
							*(DisplayUpper+2*i+1) = DispPattern1[33+2*SerialBuffer[i]];}}
					break;
				case 3:																				// 7 segment pattern (1 byte)
					for (i=0; i<16; i++) {
						*(DisplayUpper+2*i) = SerialBuffer[i];
						if (SerialBuffer[i] & 64) {								// g segment set?
							*(DisplayUpper+2*i+1) = 4;}							// turn on m segment of alpha display
						else {
							*(DisplayUpper+2*i+1) = 0;}}
					break;
				case 4:																				// 14 segment pattern (2 bytes)
					for (i=0; i<32; i++) {
						*(DisplayUpper+i) = SerialBuffer[i];}
					break;
				case 5:																				// ASCII
				case 6:																				// ASCII with comma
					WritePlayerDisplay((char*)SerialBuffer, 1);
					break;}
				break;
				case 6:																				// Sys3 - 6 display

					break;
				case 7:																				// Sys7 + 9 display
					switch (USB_DisplayProtocol[1]) {						// which protocol shall be used?
					case 1:																			// BCD
					case 2:																			// BCD with comma
						DisplayBCD(1, SerialBuffer);
						break;
					case 5:																			// ASCII
					case 6:																			// ASCII with comma
						WritePlayerDisplay((char*)SerialBuffer, 1);
						break;}
					break;}
		break;
	case 32:																						// set display 2 to
		switch (APC_settings[DisplayType]) {							// which display is used?
		case 0:																						// 4 ALPHA+CREDIT
		case 1:																						// Sys11 Pinbot
		case 2:																						// Sys11 F-14
			switch (USB_DisplayProtocol[2]) {								// which protocol shall be used?
			case 1:																					// BCD
				for (i=0; i<7; i++) {
					*(DisplayUpper+2*i+18) = DispPattern1[32+2*SerialBuffer[i]];
					*(DisplayUpper+2*i+19) = DispPattern1[33+2*SerialBuffer[i]];}
				break;
			case 2:																					// BCD with comma
				for (i=0; i<7; i++) {
					if (SerialBuffer[i] & 128) {								// comma set?
						*(DisplayUpper+2*i+18) = 128 | DispPattern1[32+2*(SerialBuffer[i] & 15)];
						*(DisplayUpper+2*i+19) = 64 | DispPattern1[33+2*(SerialBuffer[i] & 15)];}
					else {
						*(DisplayUpper+2*i+18) = DispPattern1[32+2*SerialBuffer[i]];
						*(DisplayUpper+2*i+19) = DispPattern1[33+2*SerialBuffer[i]];}}
				break;
			case 3:																					// 7 segment pattern (1 byte)
				for (i=0; i<7; i++) {
					*(DisplayUpper+2*i+18) = SerialBuffer[i];
					if (SerialBuffer[i] & 64) {									// g segment set?
						*(DisplayUpper+2*i+19) = 4;}							// turn on m segment of alpha display
					else {
						*(DisplayUpper+2*i+19) = 0;}}
				break;
			case 4:																					// 14 segment pattern (2 bytes)
				for (i=0; i<14; i++) {
					*(DisplayUpper+i+18) = SerialBuffer[i];}
				break;
			case 5:																					// ASCII
			case 6:																					// ASCII with comma
				WritePlayerDisplay((char*)SerialBuffer, 2);
				break;}
			break;
			case 3:																					// Sys11 BK2K
			case 4:																					// Sys11 Taxi
				if (!game_settings[USB_Debug]) {							// display can be used for debug information
					switch (USB_DisplayProtocol[2]) {						// which protocol shall be used?
					case 1:																			// BCD
						for (i=0; i<16; i++) {
							*(DisplayLower+2*i) = DispPattern2[32+2*SerialBuffer[i]];
							*(DisplayLower+2*i+1) = DispPattern2[33+2*SerialBuffer[i]];}
						break;
					case 2:																			// BCD with comma
						for (i=0; i<16; i++) {
							if (SerialBuffer[i] & 128) {						// comma set?
								*(DisplayLower+2*i) = 1 | DispPattern2[32+2*(SerialBuffer[i] & 15)];
								*(DisplayLower+2*i+1) = 8 | DispPattern2[33+2*(SerialBuffer[i] & 15)];}
							else {
								*(DisplayLower+2*i) = DispPattern2[32+2*SerialBuffer[i]];
								*(DisplayLower+2*i+1) = DispPattern2[33+2*SerialBuffer[i]];}}
						break;
					case 3:																			// 7 segment pattern (1 byte)
						for (i=0; i<16; i++) {
							*(DisplayLower+2*i) = ConvertPattern(0, SerialBuffer[i]);}
						break;
					case 4:																			// 14 segment pattern (2 bytes)
						for (i=0; i<16; i++) {
							*(DisplayLower+2*i) = ConvertPattern(0, SerialBuffer[2*i]);
							*(DisplayLower+2*i+1) = ConvertPattern(1, SerialBuffer[2*i+1]);}
						break;
					case 5:																			// ASCII
					case 6:																			// ASCII with comma
						WritePlayerDisplay((char*)SerialBuffer, 2);
						break;}
					break;
					case 6:																			// Sys3 - 6 display

						break;
					case 7:																			// Sys7 + 9 display
						switch (USB_DisplayProtocol[2]) {					// which protocol shall be used?
						case 1:																		// BCD
						case 2:																		// BCD with comma
							DisplayBCD(2, SerialBuffer);
							break;
						case 5:																		// ASCII
						case 6:																		// ASCII with comma
							WritePlayerDisplay((char*)SerialBuffer, 2);
							break;}
						break;}}
		break;
	case 33:																						// set display 3 to
		if (!game_settings[USB_Debug]) {									// display can be used for debug information
			switch (APC_settings[DisplayType]) {						// which display is used?
			case 0:																					// 4 ALPHA+CREDIT
				switch (USB_DisplayProtocol[3]) {							// which protocol shall be used?
				case 1:																				// BCD
					for (i=0; i<7; i++) {
						*(DisplayLower+2*i+2) = DispPattern2[32+2*SerialBuffer[i]];
						*(DisplayLower+2*i+3) = DispPattern2[33+2*SerialBuffer[i]];}
					break;
				case 2:																				// BCD with comma
					for (i=0; i<7; i++) {
						if (SerialBuffer[i] & 128) {							// comma set?
							*(DisplayLower+2*i+2) = 1 | DispPattern2[32+2*(SerialBuffer[i] & 15)];
							*(DisplayLower+2*i+3) = 8 | DispPattern2[33+2*(SerialBuffer[i] & 15)];}
						else {
							*(DisplayLower+2*i+2) = DispPattern2[32+2*SerialBuffer[i]];
							*(DisplayLower+2*i+3) = DispPattern2[33+2*SerialBuffer[i]];}}
					break;
				case 3:																				// 7 segment pattern (1 byte)
					for (i=0; i<7; i++) {
						*(DisplayUpper+2*i+2) = ConvertPattern(0, SerialBuffer[i]);
						if (SerialBuffer[i] & 64) {								// g segment set?
							*(DisplayLower+2*i+3) = 2;}							// turn on m segment of alpha display
						else {
							*(DisplayLower+2*i+3) = 0;}}
					break;
				case 4:																				// 14 segment pattern (2 bytes)
					for (i=0; i<14; i++) {
						*(DisplayLower+2*i+2) = ConvertPattern(0, SerialBuffer[2*i]);
						*(DisplayLower+2*i+3) = ConvertPattern(1, SerialBuffer[2*i+1]);}
					break;
				case 5:																				// ASCII
				case 6:																				// ASCII with comma
					WritePlayerDisplay((char*)SerialBuffer, 3);
					break;}
				break;
				case 1:																				// Sys11 Pinbot
				case 2:																				// Sys11 F-14
					switch (USB_DisplayProtocol[3]) {						// which protocol shall be used?
					case 1:																			// BCD
						for (i=0; i<7; i++) {
							*(DisplayLower+2*i+2) = DispPattern2[32+2*SerialBuffer[i]];}
						break;
					case 2:																			// BCD with comma
						for (i=0; i<7; i++) {
							if (SerialBuffer[i] & 128) {						// comma set?
								*(DisplayLower+2*i+2) = 1 | DispPattern2[32+2*(SerialBuffer[i] & 15)];}
							else {
								*(DisplayLower+2*i+2) = DispPattern2[32+2*SerialBuffer[i]];}}
						break;
					case 3:																			// 7 segment pattern (1 byte)
						for (i=0; i<7; i++) {
							*(DisplayLower+2*i+2) = ConvertPattern(0, SerialBuffer[i]);}
						break;
					case 4:																			// 14 segment pattern (2 bytes)
						for (i=0; i<14; i++) {
							*(DisplayLower+2*i+2) = ConvertPattern(0, SerialBuffer[i]);}
						break;
					case 5:																			// ASCII
					case 6:																			// ASCII with comma
						WritePlayerDisplay((char*)SerialBuffer, 3);
						break;}
					break;
					case 4:																			// Sys11 Taxi

						break;
					case 6:																			// Sys3 - 6 display

						break;
					case 7:																			// Sys7 + 9 display
						switch (USB_DisplayProtocol[3]) {					// which protocol shall be used?
						case 1:																		// BCD
						case 2:																		// BCD with comma
							DisplayBCD(3, SerialBuffer);
							break;
						case 5:																		// ASCII
						case 6:																		// ASCII with comma
							WritePlayerDisplay((char*)SerialBuffer, 3);
							break;}
						break;}}
		break;
	case 34:																						// set display 4 to
		if (!game_settings[USB_Debug]) {									// display can be used for debug information
			switch (APC_settings[DisplayType]) {						// which display is used?
			case 0:																					// 4 ALPHA+CREDIT
				switch (USB_DisplayProtocol[4]) {							// which protocol shall be used?
				case 1:																				// BCD
					for (i=0; i<7; i++) {
						*(DisplayLower+2*i+18) = DispPattern2[32+2*SerialBuffer[i]];
						*(DisplayLower+2*i+19) = DispPattern2[33+2*SerialBuffer[i]];}
					break;
				case 2:																				// BCD with comma
					for (i=0; i<7; i++) {
						if (SerialBuffer[i] & 128) {							// comma set?
							*(DisplayLower+2*i+18) = 1 | DispPattern2[32+2*(SerialBuffer[i] & 15)];
							*(DisplayLower+2*i+19) = 8 | DispPattern2[33+2*(SerialBuffer[i] & 15)];}
						else {
							*(DisplayLower+2*i+18) = DispPattern2[32+2*SerialBuffer[i]];
							*(DisplayLower+2*i+19) = DispPattern2[33+2*SerialBuffer[i]];}}
					break;
				case 3:																				// 7 segment pattern (1 byte)
					for (i=0; i<7; i++) {
						*(DisplayUpper+2*i+18) = ConvertPattern(0, SerialBuffer[i]);
						if (SerialBuffer[i] & 64) {								// g segment set?
							*(DisplayLower+2*i+19) = 2;}						// turn on m segment of alpha display
						else {
							*(DisplayLower+2*i+19) = 0;}}
					break;
				case 4:																				// 14 segment pattern (2 bytes)
					for (i=0; i<14; i++) {
						*(DisplayLower+2*i+2) = ConvertPattern(0, SerialBuffer[2*i]);
						*(DisplayLower+2*i+3) = ConvertPattern(1, SerialBuffer[2*i+1]);}
					break;
				case 5:																				// ASCII
				case 6:																				// ASCII with comma
					WritePlayerDisplay((char*)SerialBuffer, 4);
					break;}
				break;
				case 1:																				// Sys11 Pinbot
				case 2:																				// Sys11 F-14
					switch (USB_DisplayProtocol[4]) {						// which protocol shall be used?
					case 1:																			// BCD
						for (i=0; i<7; i++) {
							*(DisplayLower+2*i+18) = DispPattern2[32+2*SerialBuffer[i]];}
						break;
					case 2:																			// BCD with comma
						for (i=0; i<7; i++) {
							if (SerialBuffer[i] & 128) {						// comma set?
								*(DisplayLower+2*i+18) = 16 | DispPattern2[32+2*(SerialBuffer[i] & 15)];}
							else {
								*(DisplayLower+2*i+18) = DispPattern2[32+2*SerialBuffer[i]];}}
						break;
					case 3:																			// 7 segment pattern (1 byte)
						for (i=0; i<7; i++) {
							*(DisplayLower+2*i+18) = ConvertPattern(0, SerialBuffer[i]);}
						break;
					case 4:																			// 14 segment pattern (2 bytes)
						for (i=0; i<14; i++) {
							*(DisplayLower+2*i+18) = ConvertPattern(0, SerialBuffer[i]);}
						break;
					case 5:																			// ASCII
					case 6:																			// ASCII with comma
						WritePlayerDisplay((char*)SerialBuffer, 4);
						break;}
					break;
					case 6:																			// Sys3 - 6 display

						break;
					case 7:																			// Sys7 + 9 display
						switch (USB_DisplayProtocol[4]) {					// which protocol shall be used?
						case 1:																		// BCD
						case 2:																		// BCD with comma
							DisplayBCD(4, SerialBuffer);
							break;
						case 5:																		// ASCII
						case 6:																		// ASCII with comma
							WritePlayerDisplay((char*)SerialBuffer, 4);
							break;}
						break;}}
		break;
	case 37:																						// select display protocol
		if (SerialBuffer[0] < 5) {
			USB_DisplayProtocol[SerialBuffer[0]] = SerialBuffer[1];}
		break;
	case 40:																						// get status of switch #
		if (SerialBuffer[0] < 74) {												// max 73 switches
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
	case 50:
		if (SerialBuffer[0] == 1) {												// channel 1?
			if (!SerialBuffer[1]) {													// stop sound
				AfterSound = 0;
				StopPlayingSound();
				break;}
			if (SerialBuffer[1] == 85) {										// sound command 0x55
				break;}
			if (SerialBuffer[1] == 105) { 									// strange misplaced sound during multiball
				break;}
			if (SerialBuffer[1] == 170) {										// sound command 0xaa
				break;}
			if (SerialBuffer[1] == 255) {										// sound command 0xff
				break;}
			char FileName[9] = "0_00.snd";
			if ((SerialBuffer[1] >> 4) < 10) {
				FileName[2] = 48 + (SerialBuffer[1] >> 4);}
			else {
				FileName[2] = 55 + (SerialBuffer[1] >> 4);}
			if ((SerialBuffer[1] & 15) < 10) {
				FileName[3] = 48 + (SerialBuffer[1] & 15);}
			else {
				FileName[3] = 55 + (SerialBuffer[1] & 15);}
			if (SD.exists(FileName)) {
				if (game_settings[USB_Debug] == 2) {					// display can be used for debug information
					*(DisplayLower+2) = NumLower[2 * (FileName[2] - 32)]; // show the number of the sound to be played
					*(DisplayLower+3) = NumLower[2 * (FileName[2] - 32) + 1];
					*(DisplayLower+4) = NumLower[2 * (FileName[3] - 32)];
					*(DisplayLower+5) = NumLower[2 * (FileName[3] - 32) + 1];}
				if (SerialBuffer[1] < 128) {									// play speech with a higher priority
					PlaySound(50, (char*) FileName);}
				else {
					PlaySound(51, (char*) FileName);}}
			else {
				*(DisplayLower+12) = NumLower[2 * (FileName[2] - 32)]; // show the number of the missing sound
				*(DisplayLower+13) = NumLower[2 * (FileName[2] - 32) + 1];
				*(DisplayLower+14) = NumLower[2 * (FileName[3] - 32)];
				*(DisplayLower+15) = NumLower[2 * (FileName[3] - 32) + 1];}}
		else {																						// channel 2
			if (!SerialBuffer[1]) {													// sound command 0x00 - stop music
				AfterMusic = 0;
				StopPlayingMusic();
				break;}
			if (SerialBuffer[1] == 127) {										// sound command 0x7f - stop sound
				AfterSound = 0;
				StopPlayingSound();
				break;}
			if (SerialBuffer[1] > 29 && SerialBuffer[1] < 48) {	// unknown sound commands 0x1d to 0x30
				break;}
			if (SerialBuffer[1] > 79 && SerialBuffer[1] < 89) {	// unknown sound commands 0x4f to 0x59
				break;}
			if (SerialBuffer[1] > 95 && SerialBuffer[1] < 100) { // music volume command 0x6X
				MusicVolume = SerialBuffer[1] - 96;
				break;}
			if (SerialBuffer[1] == 170) {										// unknown sound command 0xaa
				break;}
			if (SerialBuffer[1] == 255) {										// unknown sound command 0xff
				break;}
			char FileName[9] = "1_00.snd";
			if ((SerialBuffer[1] >> 4) < 10) {
				FileName[2] = 48 + (SerialBuffer[1] >> 4);}
			else {
				FileName[2] = 55 + (SerialBuffer[1] >> 4);}
			if ((SerialBuffer[1] & 15) < 10) {
				FileName[3] = 48 + (SerialBuffer[1] & 15);}
			else {
				FileName[3] = 55 + (SerialBuffer[1] & 15);}
			if (SD.exists(FileName)) {
				if (game_settings[USB_Debug] == 2) {					// display can be used for debug information
					*(DisplayLower+18) = NumLower[2 * (FileName[2] - 32)]; // show the number of the music to be played
					*(DisplayLower+19) = NumLower[2 * (FileName[2] - 32) + 1];
					*(DisplayLower+20) = NumLower[2 * (FileName[3] - 32)];
					*(DisplayLower+21) = NumLower[2 * (FileName[3] - 32) + 1];}
				if ((SerialBuffer[1] < 128)) {								// play sounds > 127 on the sound channel
					PlayMusic(50, (char*) FileName);}
				else {
					PlaySound(50, (char*) FileName);}}
			else {
				*(DisplayLower+28) = NumLower[2 * (FileName[2] - 32)]; // show the number of the missing music
				*(DisplayLower+29) = NumLower[2 * (FileName[2] - 32) + 1];
				*(DisplayLower+30) = NumLower[2 * (FileName[3] - 32)];
				*(DisplayLower+31) = NumLower[2 * (FileName[3] - 32) + 1];}}
		break;
	case 51:																						// stop sound
		if (SerialBuffer[0] == 1) {												// channel 1?
			AfterMusic = 0;
			StopPlayingMusic();}
		else {
			AfterSound = 0;
			StopPlayingSound();}
		break;
	case 52:																						// play soundfile
		if (SerialBuffer[0] == 1) {												// channel 1?
			if (!USB_WaitSoundTimer) {											// no sound wait timer active?
				PlayMusic(50, (char*) SerialBuffer+2);				// play the sound
				USB_WaitSoundTimer = ActivateTimer(15, 0, USB_ResetWaitSoundTimers); // start a timer
				if (SerialBuffer[1] & 1) {										// looping active?
					for (i=0; i<12; i++) {
						USB_RepeatMusic[i] = SerialBuffer[2+i];}
					NextMusicName = USB_RepeatMusic;
					AfterMusic = PlayNextMusic;}
				else {
					AfterMusic = 0;}}
			else {																					// sound wait timer active
				if (!USB_WaitingSoundFiles[0][1]) {						// any waiting sounds?
					if (SerialBuffer[1] & 1) {									// if not check for looping
						USB_WaitingSoundFiles[0][0] = 2;}					// set the looping flag
					for (i=0; i<12; i++) {											// copy the filename to the waiting stack
						USB_WaitingSoundFiles[0][i+1] = SerialBuffer[2+i];}}
				else {																				// waiting stack not empty
					if (USB_WaitingSoundFiles[0][0] & 1) {			// is the waiting sound for channel 2?
						if (SerialBuffer[1] & 1) {								// then copy the sound data to stack position 2
							USB_WaitingSoundFiles[1][0] = 2;}
						for (i=0; i<12; i++) {										// copy the filename to the waiting stack
							USB_WaitingSoundFiles[1][1+i] = SerialBuffer[2+i];}}
					else {																			// waiting sound is also for channel 1
						if (USB_WaitingSoundFiles[1][1]) {				// is there a sound in waiting position 2?
							for (i=1; i<13; i++) {									// if yes move it to position 1 and copy the new sound to position 2
								USB_WaitingSoundFiles[0][i] = USB_WaitingSoundFiles[1][i];
								USB_WaitingSoundFiles[1][i] = SerialBuffer[1+i];}
							USB_WaitingSoundFiles[0][0] = USB_WaitingSoundFiles[1][0];	// copy command byte
							if (SerialBuffer[1] & 1) {							// handle looping flag
								USB_WaitingSoundFiles[1][0] = 2;}
							else {
								USB_WaitingSoundFiles[1][0] = 0;}}
						else {																		// no sound at stack position 2
							if (SerialBuffer[1] & 1) {							// overwrite stack position 1
								USB_WaitingSoundFiles[0][0] = 2;}
							else {
								USB_WaitingSoundFiles[0][0] = 0;}
							for (i=0; i<12; i++) {									// copy the filename to the waiting stack
								USB_WaitingSoundFiles[0][i+1] = SerialBuffer[2+i];}}}}}}
		else {																						// channel 2
			if (!USB_WaitSoundTimer) {
				PlaySound(50, (char*) SerialBuffer+2);
				USB_WaitSoundTimer = ActivateTimer(15, 0, USB_ResetWaitSoundTimers);
				if (SerialBuffer[1] & 1) {										// looping active?
					for (i=0; i<12; i++) {
						USB_RepeatSound[i] = SerialBuffer[2+i];}
					NextSoundName = USB_RepeatSound;
					AfterSound = PlayNextSound;}
				else {
					AfterSound = 0;}}
			else {
				if (!USB_WaitingSoundFiles[0][1]) {						// any waiting sounds?
					if (SerialBuffer[1] & 1) {									// is not check for looping
						USB_WaitingSoundFiles[0][0] = 3;}					// set the looping flag
					else {
						USB_WaitingSoundFiles[0][0] = 1;}					// or just set the channel 2 flag
					for (i=0; i<12; i++) {											// copy the filename to the waiting stack
						USB_WaitingSoundFiles[0][i+1] = SerialBuffer[2+i];}}
				else {																				// waiting stack not empty
					if (!(USB_WaitingSoundFiles[0][0] & 1)) {		// is the waiting sound for channel 1?
						if (SerialBuffer[1] & 1) {								// if not copy the sound data to stack position 2
							USB_WaitingSoundFiles[1][0] = 3;}				// set the looping flag
						else {
							USB_WaitingSoundFiles[1][0] = 1;}				// or just set the channel 2 flag
						for (i=0; i<12; i++) {										// copy the filename to the waiting stack
							USB_WaitingSoundFiles[1][1+i] = SerialBuffer[2+i];}}
					else {																			// waiting sound is also for channel 2
						if (USB_WaitingSoundFiles[1][1]) {				// is there a sound is waiting at position 2?
							for (i=1; i<13; i++) {									// if yes move it to position 1 and copy the new sound to position 2
								USB_WaitingSoundFiles[0][i] = USB_WaitingSoundFiles[1][i];
								USB_WaitingSoundFiles[1][i] = SerialBuffer[1+i];}
							USB_WaitingSoundFiles[0][0] = USB_WaitingSoundFiles[1][0];
							if (SerialBuffer[1] & 1) {							// handle looping flag
								USB_WaitingSoundFiles[1][0] = 3;}
							else {
								USB_WaitingSoundFiles[1][0] = 1;}}
						else {																		// no sound at stack position 2
							if (SerialBuffer[1] & 1) {							// overwrite stack position 1
								USB_WaitingSoundFiles[0][0] = 3;}
							else {
								USB_WaitingSoundFiles[0][0] = 1;}
							for (i=0; i<12; i++) {									// copy the filename to the waiting stack
								USB_WaitingSoundFiles[0][i+1] = SerialBuffer[2+i];}}}}}}
		break;
	case 54:																						// sound volume setting
		APC_settings[Volume] = 2*SerialBuffer[1];					// set system volume
		analogWrite(VolumePin,255-APC_settings[Volume]);	// and apply it
		break;
	case 60:																						// configure hardware rule for solenoid
		i = 0;
		c = 0;
		if (!SerialBuffer[7] && !SerialBuffer[8] && !SerialBuffer[9]) { // all flags zero means disable rules
			while (USB_HWrule_ActSw[i][0]) {								// check for HW activation rules for this switch
				if (USB_HWrule_ActSw[i][1] == SerialBuffer[0]) { // rule for this solenoid found?
					c = i;
					byte x;
					while (USB_HWrule_ActSw[c][0]) {						// move all entries up
						for(x=0; x<3; x++) {
							USB_HWrule_ActSw[c][x] = USB_HWrule_ActSw[c+1][x];}
						c++;}}
				else {
					i++;}}
			i = 0;
			while (USB_HWrule_RelSw[i][0]) {								// check for HW release rules for this switch
				if (USB_HWrule_RelSw[i][1] == SerialBuffer[0]) { // rule for this solenoid found?
					c = i;
					byte x;
					while (USB_HWrule_RelSw[c][0]) {						// move all entries up
						for(x=0; x<3; x++) {
							USB_HWrule_RelSw[c][x] = USB_HWrule_RelSw[c+1][x];}
						c++;}}
				else {
					i++;}}}
		else {																						// create new HW rule
			if (SerialBuffer[4]) {													// pulse time > 0?
				while ((SerialBuffer[1+i] != 127) && (i<3)) {	// stop on a non active switch
					if (SerialBuffer[1+i] < 127) {							// non inverted switch
						if (SerialBuffer[7+i] & 1) {							// activate coil on switch?
							c = 0;
							while (USB_HWrule_ActSw[c][0] && (c<15)) {	// look for a free slot
								c++;}
							USB_HWrule_ActSw[c][0] = SerialBuffer[1+i];	// set switch as trigger
							USB_HWrule_ActSw[c][1] = SerialBuffer[0];		// store coil number
							USB_HWrule_ActSw[c][2] = SerialBuffer[4];}	// store pulse duration
						if (SerialBuffer[7+i] & 2) {							// deactivate coil on switch release?
							c = 0;
							while (USB_HWrule_RelSw[c][0] && (c<15)) {	// look for a free slot
								c++;}
							USB_HWrule_RelSw[c][0] = SerialBuffer[1+i];	// set switch release as trigger
							USB_HWrule_RelSw[c][1] = SerialBuffer[0];	// store coil number
							USB_HWrule_RelSw[c][2] = 0;}}						// store pulse duration 0 (means coil release)
					else {
						if (SerialBuffer[7+i] & 1) {							// activate coil on switch?
							c = 0;
							while (USB_HWrule_RelSw[c][0] && (c<15)) {	// look for a free slot
								c++;}
							USB_HWrule_RelSw[c][0] = SerialBuffer[1+i] - 128;	// set switch release as trigger
							USB_HWrule_RelSw[c][1] = SerialBuffer[0];	// store coil number
							USB_HWrule_RelSw[c][2] = SerialBuffer[4];}	// store pulse duration
						if (SerialBuffer[7+i] & 2) {							// deactivate coil on switch release?
							c = 0;
							while (USB_HWrule_ActSw[c][0] && (c<15)) {	// look for a free slot
								c++;}
							USB_HWrule_ActSw[c][0] = SerialBuffer[1+i] - 128;	// set switch as trigger
							USB_HWrule_ActSw[c][1] = SerialBuffer[0];	// store coil number
							USB_HWrule_ActSw[c][2] = 0;}}						// store pulse duration 0 (means coil release)
					i++;}}}
		break;
	case 100:																						// init
		USB_WatchdogHandler(1);
		break;
	case 101:
		USB_WatchdogHandler(0);
		break;
	default:
		if (APC_settings[DisplayType] == 3) {           	// 2x16 alphanumeric display (BK2K type)
			WriteUpper2("UNKNOWN COMMAND ");}
		else {
			WriteUpper2("UNKNOWNCOMMAND  ");}
		WriteLower2("                ");
		ShowNumber(31, Command);               						// show command number
		ShowMessage(3);}}

void USB_ResetWaitSoundTimers(byte Dummy) {						// reset the timer and play waiting sounds
	UNUSED(Dummy);
	if (USB_WaitingSoundFiles[0][1]) {									// any waiting sounds?
		if (USB_WaitingSoundFiles[0][0] & 1) {						// sound for channel 2 waiting?
			PlaySound(50, (char*) USB_WaitingSoundFiles+1);
			if (USB_WaitingSoundFiles[0][0] & 2) {					// looping active?
				for (i=0; i<12; i++) {
					USB_RepeatSound[i] = USB_WaitingSoundFiles[0][1+i];}
				NextSoundName = USB_RepeatSound;
				AfterSound = PlayNextSound;}
			else {
				AfterSound = 0;}}
		else {																						// waiting sound is for channel 1
			PlayMusic(50, (char*) USB_WaitingSoundFiles+1);
			if (USB_WaitingSoundFiles[0][0] & 2) {					// looping active?
				for (i=0; i<12; i++) {
					USB_RepeatMusic[i] = USB_WaitingSoundFiles[0][1+i];}
				NextMusicName = USB_RepeatMusic;
				AfterMusic = PlayNextMusic;}
			else {
				AfterMusic = 0;}}
		if (USB_WaitingSoundFiles[1][1]) {								// any sound waiting at stack position 2?
			for (i=0; i<13; i++) {													// if yes move it to position 1 and clear position 2
				USB_WaitingSoundFiles[0][i] = USB_WaitingSoundFiles[1][i];
				USB_WaitingSoundFiles[1][i] = 0;}}
		else {																						// no sound waiting at stack position 2
			for (i=0; i<13; i++) {													// clear stack position 1
				USB_WaitingSoundFiles[0][i] = 0;}}
		USB_WaitSoundTimer = ActivateTimer(15, 0, USB_ResetWaitSoundTimers);}	// start a new timer
	else {
		USB_WaitSoundTimer = 0;}}

void USB_FireSolenoid(byte Duration, byte Solenoid) {	// consider solenoid recycling time when activating solenoids
	if (!USB_SolTimers[Solenoid-1]) {										// recycling time over for this coil?
		SolChange = false;																// block IRQ solenoid handling
		if (Solenoid > 8) {																// does the solenoid not belong to the first latch?
			if (Solenoid < 17) {														// does it belong to the second latch?
				SolBuffer[1] |= 1<<(Solenoid-9);							// latch counts from 0
				SolLatch |= 2;}																// select second latch
			else {
				SolBuffer[2] |= 1<<(Solenoid-17);
				SolLatch |= 4;}}															// select third latch
		else {
			SolBuffer[0] |= 1<<(Solenoid-1);
			SolLatch |= 1;}																	// select first latch
		USB_SolTimers[Solenoid-1] = ActivateTimer((unsigned int) Duration, Solenoid, USB_ReleaseSolenoid);
		SolChange = true;}}

void USB_KillSolenoid(byte Coil) {										// stop solenoid immediately
	if (QuerySolenoid(Coil)) {													// solenoid active?
		if (USB_SolTimers[Coil-1]) {											// solenoid duration timer active?
			KillTimer(USB_SolTimers[Coil-1]);}							// kill it
		USB_ReleaseSolenoid(Coil);}}											// release solenoid

void USB_ReleaseSolenoid(byte Coil) {									// solenoid timer has run out
	ReleaseSolenoid(Coil);
	USB_SolTimers[Coil-1] = 0;													// mark running timer as void
	if (USB_SolRecycleTime[Coil-1]) {										// is a recycling time specified?
		USB_SolTimers[Coil-1] = ActivateTimer((unsigned int) USB_SolRecycleTime[Coil-1], Coil, USB_ReleaseSolBlock);}} // start the release timer

void USB_ReleaseSolBlock(byte Coil) {									// release the coil block when the recycling time is over
	USB_SolTimers[Coil-1] = 0;}
