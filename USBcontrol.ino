// USB interface for APC based pinball machines

unsigned int USB_SolTimes[32] = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 0, 0, 40, 40, 40, 40, 40, 40, 40, 40};	// Activation times for solenoids
const byte USB_CommandLength[101] = {0,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 0 - 9
																		1,1,1,0,0,0,0,0,0,0,		// Length of USB commands from 10 - 19
																		1,1,1,1,2,2,0,0,0,0,		// Length of USB commands from 20 - 29
																		255,255,255,255,255,255,255,1,0,0,	// Length of USB commands from 30 - 39
																		1,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 40 - 49
																		1,0,255,255,1,0,0,0,0,0,	// Length of USB commands from 50 - 59
																		10,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 60 - 69
																		0,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 70 - 79
																		0,0,0,0,0,0,0,0,0,0,		// Length of USB commands from 80 - 89
																		0,0,0,0,0,0,0,0,0,0,0};	// Length of USB commands from 90 - 101

																											// offsets of settings in the settings array
const byte USB_Watchdog = 0;													// watchdog enable setting

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

struct SettingTopic USB_setList[4] = {{"USB WATCHDOG  ",HandleBoolSetting,0,0,0}, // defines the game specific settings
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
	USB_WatchdogHandler(1);															// initiate reset and start watchdog
	WriteUpper("  USB  CONTROL  ");
	WriteLower("                ");}

void USB_WatchdogHandler(byte Event) {								// Arg = 0->Reset WD / 1-> Reset & start WD / 2-> WD has run out / 3-> stop WD
	static byte USB_WatchdogTimer;
	byte i=0;
	if (!Event) {																				// reset watchdog
		Serial.write((byte) 0);														// send OK
		KillTimer(USB_WatchdogTimer);											// restart timer
		USB_WatchdogTimer = ActivateTimer(1000, 2, USB_WatchdogHandler);}
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
	switch (Switch) {
	case 8:                                             // high score reset
		digitalWrite(Blanking, LOW);                      // invoke the blanking
		break;
	case 72:																						// advance button
		if (QuerySwitch(73)) {														// Up/Down switch pressed?
			ActivateTimer(1000, 0, USB_Testmode);}					// look again in 1s
		else {
			byte i = 0;
			while (USB_ChangedSwitches[i] && (i<63)) {
				i++;}
			USB_ChangedSwitches[i] = Switch | 128;}
		break;
	default:
		byte i = 0;
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
	if (QuerySwitch(72)) {
		USB_WatchdogHandler(3);														// stop USB watchdog
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
	case 1:																							// get firmware version
		Serial.print("0.02");
		Serial.write((byte) 0);
		break;
	case 2:																							// get API version
		Serial.print("0.09");
		Serial.write((byte) 0);
		break;
	case 3:																							// get number of lamps
		Serial.write((byte) 64);
		break;
	case 9:																							// get number of switches
		Serial.write((byte) 73);
		break;
	case 4:																							// get number of solenoids
		Serial.write((byte) 24);
		break;
	case 7:
		switch (APC_settings[DisplayType]) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			Serial.print("14-segment");
			Serial.write((byte) 0);
			break;
		case 5:
			Serial.print("7-segment");
			Serial.write((byte) 0);
			break;
		case 6:
			Serial.print("8-segment");
			Serial.write((byte) 0);
			break;}
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
		if (SerialBuffer[0] < 26) {												// max 24 solenoids
			Serial.write((byte) QuerySolenoid(SerialBuffer[0]));}
		break;
	case 21:																						// set solenoid # to on
		if (SerialBuffer[0] < 25) {												// max 24 solenoids
			if (!USB_SolTimers[SerialBuffer[0]-1]) {				// recycling time over for this coil?
				if (!SolChange) {                             // change request for another solenoid pending?
					SolNumber = SerialBuffer[0];                // activate solenoid permanently
					SolState = true;
					SolChange = true;}
				else {                                        // if a change request is already pending
					i = 0;
					while (SolDelayed[i]) {                     // look for a free slot in the list of solenoids to be processed later
						i++;
						if (i > 20) {
							ErrorHandler(31,SerialBuffer[0],0);
							break;}}
					SolDelayed[i] = SerialBuffer[0];            // insert the solenoid number
					DurDelayed[i] = 0;                   				// and its duration into the list
					ActivateTimer(25, SerialBuffer[0], ActivateLater);}}}	// and try again later
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
	case 60:																						// configure hardware rule for solenoid
		i = 0;
		c = 0;
		if (!SerialBuffer[7] && !SerialBuffer[8] && !SerialBuffer[9]) { // all flags zero means disable rules
			while (USB_HWrule_ActSw[i][0]) {									// check for HW activation rules for this switch
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

void USB_FireSolenoid(byte Duration, byte Coil) {			// consider solenoid recycling time when activating solenoids
	if (!USB_SolTimers[Coil-1]) {												// recycling time over for this coil?
		if (!SolChange) {                               	// change request for another solenoid pending?
			SolNumber = Coil;                  							// activate solenoid permanently
			SolState = true;
			SolChange = true;
			USB_SolTimers[Coil-1] = ActivateTimer((unsigned int) Duration, Coil, USB_ReleaseSolenoid);}
		else {                                        		// if a change request is already pending
			i = 0;
			while (SolDelayed[i]) {                     		// look for a free slot in the list of solenoids to be processed later
				i++;
				if (i > 20) {
					ErrorHandler(31,Coil,0);
					break;}}
			SolDelayed[i] = Coil;              							// insert the solenoid number
			DurDelayed[i] = 0;                   						// and its duration into the list
			ActivateTimer(25, Coil, USB_ActivateLater);}}}

void USB_ActivateLater(byte Solenoid) {               // handle delayed solenoid change requests
	byte i = 0;
	unsigned int Duration;
	while (SolDelayed[i] != Solenoid) {             		// search the list of delayed solenoid requests
		i++;}
	SolDelayed[i] = 0;                              		// remove its entry
	Duration = DurDelayed[i];                       		// get the duration
	USB_FireSolenoid(Duration, Solenoid);}         	 		// and try again to activate it

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
