#define SoundCommandCh1 0
#define SoundCommandCh2 1
#define SwitchCommand 2
#define SolenoidCommand 3

byte (*PinMameException)(byte, byte);
byte SoundSeries[3];																	// buffer to handle pre system11 sound series
byte LastCh1Sound;																		// preSys11: stores the number of the last sound that has been played on Ch1

char USB_RepeatSound[13];															// name of the sound file to be repeated

void EX_Init(byte GameNumber) {
	switch(GameNumber) {
	case 20:																						// Jungle Lord
		PinMameException = EX_JungleLord;
		break;
	default:
		PinMameException = EX_DummyProcess;}}

byte EX_DummyProcess(byte Type, byte Command) {
	UNUSED(Type);
	UNUSED(Command);
	return(0);}

byte EX_JungleLord(byte Type, byte Command){
	switch(Type){
	case SoundCommandCh1:
		if (Command == 38){ 															// sound command 0x26 - start game
			char FileName[13] = "0_26_000.snd";							// generate base filename
			FileName[7] = 48 + random(4) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);								// play the corresponding sound file
			return(1);}
		else if (Command == 42){													// sound command 0x2a - background sound - sound series
			SoundSeries[0] = 0;
			if (SoundSeries[2] < 29)
				SoundSeries[2]++;
			char FileName[13] = "0_2a_000.snd";
			FileName[7] = 48 + (SoundSeries[2] % 10);
			FileName[6] = 48 + (SoundSeries[2] % 100) / 10;
			for (i=0; i<12; i++) {
				USB_RepeatSound[i] = FileName[i];}
			NextSoundName = USB_RepeatSound;
			AfterSound = PlayNextSound;
			PlaySound(51, (char*) FileName);
			return(1);}
		else if (Command == 45){													// sound command 0x2d - multiball start - sound series
			if (SoundSeries[0] != 45) {
				SoundSeries[0] = 45;
				SoundSeries[1] = 0;}
			if (SoundSeries[1] < 31)
				SoundSeries[1]++;
			else
				SoundSeries[1] = 1;
			char FileName[13] = "0_2d_000.snd";
			FileName[7] = 48 + (SoundSeries[1] % 10);
			FileName[6] = 48 + (SoundSeries[1] % 100) / 10;
			PlaySound(51, (char*) FileName);
			return(1);}
		else
			return(0);
	case SwitchCommand:
		return(0);
	case SolenoidCommand:
		return(0);
	default:
		return(0);}}

//					if (game_settings[USB_PinMameGame] == 34) {	// game = Black Knight
//						if (USB_SerialBuffer[1] == 48) {					// sound command 0x30
//							if (QuerySolenoid(11)) {								// GI off?
//								PlaySound(152, "0_30_001.snd");				// play multiball ball release sequence
//								break;}}
//						if (USB_SerialBuffer[1] == 56) {					// sound command 0x38
//							if (QuerySolenoid(11)) {								// GI off?
//								if (LastCh1Sound != 56) {							// ignore all subsequent 0x38 commands
//									AfterSound = 0;
//									LastCh1Sound = USB_SerialBuffer[1];	// buffer sound number
//									PlaySound(51, "0_38_001.snd");}			// play multiball start sequence
//								break;}}
//						if (USB_SerialBuffer[1] == 43) {					// sound command 0x2b - start game
//							PlayRandomSound(52, 5, (char *)USB_BK_NewGameSounds);
//							break;}
//						if (USB_SerialBuffer[1] == 45) {					// sound command 0x2d - activated spinner - sound series
//							if (SoundSeries[0] != 45) {
//								SoundSeries[0] = 45;
//								SoundSeries[1] = 0;}
//							SoundSeries[1]++;
//							char FileName[13] = "0_2d_000.snd";
//							FileName[7] = 48 + (SoundSeries[1] % 10);
//							FileName[6] = 48 + (SoundSeries[1] % 100) / 10;
//							FileName[5] = 48 + SoundSeries[1] / 100;
//							LastCh1Sound = USB_SerialBuffer[1];			// buffer sound number
//							PlaySound(51, (char*) FileName);
//							break;}
//						if (USB_SerialBuffer[1] == 46) {					// sound command 0x2e - background sound - sound series
//							SoundSeries[0] = 0;
//							if (SoundSeries[2] < 29)
//								SoundSeries[2]++;
//							char FileName[13] = "0_2e_000.snd";
//							FileName[7] = 48 + (SoundSeries[2] % 10);
//							FileName[6] = 48 + (SoundSeries[2] % 100) / 10;
//							FileName[5] = 48 + SoundSeries[2] / 100;
//							for (i=0; i<12; i++) {
//								USB_RepeatSound[i] = FileName[i];}
//							NextSoundName = USB_RepeatSound;
//							AfterSound = PlayNextSound;
//							LastCh1Sound = USB_SerialBuffer[1];			// buffer sound number
//							PlaySound(51, (char*) FileName);
//							break;}
//						if (USB_SerialBuffer[1] == 52) {					// sound command 0x34 - bonus count
//							AfterSound = 0;
//							if (!QueryLamp(49) && !QueryLamp(57) && !QueryLamp(61)) { // only bonus lamp 1 lit?
//								PlaySound(51, "0_34_002.snd");
//								break;}
//							if (LastCh1Sound != 52) {
//								LastCh1Sound = USB_SerialBuffer[1];		// buffer sound number
//								SoundSeries[2] = 1;										// Reset BG sound
//								PlaySound(51, "0_34_001.snd");}
//							break;}
//						if (USB_SerialBuffer[1] == 58) {					// sound command 0x3a
//							PlaySound(152, "0_3a.snd");							// play multiball ball release sequence
//							break;}}
//					else if (game_settings[USB_PinMameGame] == 20) {	// game = Jungle Lord
//						if (USB_SerialBuffer[1] == 38) {					// sound command 0x26 - start game
//							char FileName[13] = "0_26_000.snd";			// generate base filename
//							FileName[7] = 48 + random(4) + 1;				// change the counter according to random number
//							PlaySound(52, (char*) FileName);				// play the corresponding sound file
//							break;}
//						if (USB_SerialBuffer[1] == 42) {					// sound command 0x2a - background sound - sound series
//							SoundSeries[0] = 0;
//							if (SoundSeries[2] < 29)
//								SoundSeries[2]++;
//							char FileName[13] = "0_2a_000.snd";
//							FileName[7] = 48 + (SoundSeries[2] % 10);
//							FileName[6] = 48 + (SoundSeries[2] % 100) / 10;
//							FileName[5] = 48 + SoundSeries[2] / 100;
//							for (i=0; i<12; i++) {
//								USB_RepeatSound[i] = FileName[i];}
//							NextSoundName = USB_RepeatSound;
//							AfterSound = PlayNextSound;
//							LastCh1Sound = USB_SerialBuffer[1];			// buffer sound number
//							PlaySound(51, (char*) FileName);
//							break;}
//						if (USB_SerialBuffer[1] == 45) {					// sound command 0x2d - multiball start - sound series
//							if (SoundSeries[0] != 45) {
//								SoundSeries[0] = 45;
//								SoundSeries[1] = 0;}
//							if (SoundSeries[1] < 31)
//								SoundSeries[1]++;
//							else
//								SoundSeries[1] = 1;
//							char FileName[13] = "0_2d_000.snd";
//							FileName[7] = 48 + (SoundSeries[1] % 10);
//							FileName[6] = 48 + (SoundSeries[1] % 100) / 10;
//							FileName[5] = 48 + SoundSeries[1] / 100;
//							LastCh1Sound = USB_SerialBuffer[1];			// buffer sound number
//							PlaySound(51, (char*) FileName);
//							break;}}
//					else if (game_settings[USB_PinMameGame] == 21) {	// game = Pharaoh
//						if (USB_SerialBuffer[1] == 5) {						// sound command 0x05 - random speech
//							char FileName[13] = "0_05_000.snd";			// generate base filename
//							FileName[7] = 48 + random(4) + 1;				// change the counter according to random number
//							PlaySound(52, (char*) FileName);				// play the corresponding sound file
//							break;}
//						else if (USB_SerialBuffer[1] == 10) {			// sound command 0x0a - random speech
//							char FileName[13] = "0_0a_000.snd";			// generate base filename
//							FileName[7] = 48 + random(2) + 1;				// change the counter according to random number
//							PlaySound(52, (char*) FileName);				// play the corresponding sound file
//							break;}
//						else if (USB_SerialBuffer[1] == 13) {			// sound command 0x0d - background sound - sound series
//							if (SoundSeries[2] < 31)								// sound series has 31 different tunes
//								SoundSeries[2]++;											// switch to the next tune when sound command is called again
//							char FileName[13] = "0_0d_000.snd";			// generate base filename
//							FileName[7] = 48 + (SoundSeries[2] % 10);	// change the 7th character of filename according to current tune
//							FileName[6] = 48 + (SoundSeries[2] % 100) / 10;	// the same with the 6th character
//							for (i=0; i<12; i++) {									// store filename to be repeated
//								USB_RepeatSound[i] = FileName[i];}
//							NextSoundName = USB_RepeatSound;				// set this filename to be started by PlayNextSound
//							AfterSound = PlayNextSound;							// Call PlayNextSounds when current sound has run out
//							LastCh1Sound = USB_SerialBuffer[1];			// buffer sound number
//							PlaySound(51, (char*) FileName);
//							break;}
//						else if (USB_SerialBuffer[1] == 16) {			// sound command 0x10 - random speech
//							char FileName[13] = "0_10_000.snd";			// generate base filename
//							FileName[7] = 48 + random(2) + 1;				// change the counter according to random number
//							PlaySound(52, (char*) FileName);				// play the corresponding sound file
//							break;}
//						else if (USB_SerialBuffer[1] == 17) {			// sound command 0x11 - random speech
//							char FileName[13] = "0_11_000.snd";			// generate base filename
//							FileName[7] = 48 + random(2) + 1;				// change the counter according to random number
//							PlaySound(52, (char*) FileName);				// play the corresponding sound file
//							break;}
//						else if (USB_SerialBuffer[1] == 23) {			// sound command 0x17 - random speech
//							char FileName[13] = "0_17_000.snd";			// generate base filename
//							FileName[7] = 48 + random(2) + 1;				// change the counter according to random number
//							PlaySound(52, (char*) FileName);				// play the corresponding sound file
//							break;}
//						else if (USB_SerialBuffer[1] == 26) {			// sound command 0x1a - random speech
//							char FileName[13] = "0_1a_000.snd";			// generate base filename
//							FileName[7] = 48 + random(4) + 1;				// change the counter according to random number
//							PlaySound(52, (char*) FileName);				// play the corresponding sound file
//							break;}}																// exit special sound handling
