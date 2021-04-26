#define SoundCommandCh1 0
#define SoundCommandCh2 1
#define SwitchActCommand 2
#define SwitchRelCommand 3
#define SolenoidActCommand 4
#define SolenoidRelCommand 5
#define LampOnCommand 6
#define LampOffCommand 7
#define WriteToDisplay0 8
#define WriteToDisplay1 9
#define WriteToDisplay2 10
#define WriteToDisplay3 11
#define WriteToDisplay4 12

byte (*PinMameException)(byte, byte);

byte USB_SerialBuffer[128];														// received command arguments
char USB_RepeatSound[13];															// name of the sound file to be repeated
byte EX_EjectSolenoid;																// eject coil for improved ball release
unsigned int USB_SolTimes[32] = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 0, 0, 40, 40, 40, 40, 40, 40, 40, 40};	// Activation times for solenoids

byte EX_DummyProcess(byte Type, byte Command) {				// plays just the standard sounds
	if (Type == SoundCommandCh1) {											// sound commands for channel 1
		char FileName[9] = "0_00.snd";										// handle standard sound
		if (USB_GenerateFilename(1, Command, FileName)) {	// create filename and check whether file is present
			PlaySound(51, (char*) FileName);}}
	else if (Type == SoundCommandCh2) {									// sound commands for channel 2
		char FileName[9] = "1_00.snd";										// handle standard music
		if (USB_GenerateFilename(2, Command, FileName)) {	// create filename and check whether file is present
			PlayMusic(51, (char*) FileName);}}
	return(0);}																					// no exception rule found for this type so proceed as normal

void EX_BallRelease(byte State) {											// repeat ball eject in case the ball got stuck
	static byte Timer;																	// stores the timer number
	switch (State) {																		// determines what to do
	case 0:																							// kill the timer
		if (Timer) {																			// if a timer is active
			KillTimer(Timer);																// kill it
			Timer = 0;}																			// and set Timer = 0 to indicate that no timer is active
		break;
	case 1:																							// start a timer
		if (!Timer) {																			// if no timer is active
			Timer = ActivateTimer(3000, 2, EX_BallRelease);}	// start one for 3s with 2 as the argument
		break;
	case 2:																							// timer has run out
		ActivateSolenoid(40, EX_EjectSolenoid);						// activate the shooter lane feeder again
		Timer = ActivateTimer(3000, 2, EX_BallRelease);}}	// and restart the timer

byte EX_JungleLord(byte Type, byte Command){
	static byte SoundSeries[2];													// buffer to handle pre system11 sound series
	switch(Type){
	case SoundCommandCh1:																// sound commands for channel 1
		if (Command == 127) {	}														// ignore sound command 0x7f - audio bus init - not relevant for APC sound
		else if (Command == 38){ 													// sound command 0x26 - start game
			char FileName[13] = "0_26_000.snd";							// generate base filename
			FileName[7] = 48 + random(4) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);}								// play the corresponding sound file
		else if (Command == 42){													// sound command 0x2a - background sound - sound series
			SoundSeries[1] = 0;															// reset the multiball start sound
			if (SoundSeries[0] < 29)												// BG sound has 29 tunes
				SoundSeries[0]++;															// every call of this sound proceeds with the next tune
			char FileName[13] = "0_2a_000.snd";							// generate base filename
			FileName[7] = 48 + (SoundSeries[0] % 10);				// change the 7th character of filename according to current tune
			FileName[6] = 48 + (SoundSeries[0] % 100) / 10;	// the same with the 6th character
			for (byte i=0; i<12; i++) {											// store the name of this sound
				USB_RepeatSound[i] = FileName[i];}
			QueueNextSound(USB_RepeatSound);								// select this sound to be repeated
			PlaySound(51, (char*) FileName);}								// play the sound
		else if (Command == 44) {													// sound command 0x2c - stop sound
			AfterSound = 0;
			SoundSeries[0] = 0;															// Reset BG sound
			SoundSeries[1] = 0;															// reset the multiball start sound
			StopPlayingSound();}
		else if (Command == 45){													// sound command 0x2d - multiball start - sound series
			if (SoundSeries[1] < 31)												// this sound has 31 tunes
				SoundSeries[1]++;															// every call of this sound proceeds with next tune
			else
				SoundSeries[1] = 1;														// start all over again
			char FileName[13] = "0_2d_000.snd";							// generate base filename
			FileName[7] = 48 + (SoundSeries[1] % 10);				// change the 7th character of filename according to current tune
			FileName[6] = 48 + (SoundSeries[1] % 100) / 10;	// the same with the 6th character
			PlaySound(51, (char*) FileName);}								// play the sound
		else {																						// standard sound
			char FileName[9] = "0_00.snd";									// handle standard sound
			if (USB_GenerateFilename(1, Command, FileName)) {	// create filename and check whether file is present
				PlaySound(51, (char*) FileName);}}
		return(0);																				// return number not relevant for sounds
	case SwitchActCommand:															// activated switches
		if (Command == 43) {															// ball successfully ejected
			EX_BallRelease(0);}															// stop ball release timer
		else if (Command == 49) {													// right magnet button
			if (QueryLamp(8) && QueryLamp(2)) {							// right magnet and ball in play lamp lit?
				ActivateSolenoid(0, 22);}}										// activate right magnet
		else if (Command == 50) {													// left magnet button
			if (QueryLamp(39) && QueryLamp(2)) {						// left magnet and ball in play lamp lit?
				ActivateSolenoid(0, 21);}}										// activate left magnet
		return(0);																				// all switches are reported to PinMame
	case SwitchRelCommand:															// deactivated switches
		if (Command == 49){																// right magnet button
			ReleaseSolenoid(22);}														// turn off right magnet
		else if (Command == 50) {													// left magnet button
			ReleaseSolenoid(21);}														// turn off left magnet
		return(0);																				// all switches are reported to PinMame
	case SolenoidActCommand:														// activated solenoids
		if (Command == EX_EjectSolenoid){									// ball eject coil
			if (QueryLamp(2)) {															// ball in play lamp lit?
				EX_BallRelease(1);}}													// start ball release timer
		return(0);																				// solenoid will be activated
	default:
		return(0);}}																			// no exception rule found for this type so proceed as normal

byte EX_Pharaoh(byte Type, byte Command){
	static byte SoundSeries;														// buffer to handle pre system11 sound series
	switch(Type){
	case SoundCommandCh1:																// sound commands for channel 1
		if (Command == 127) {	}														// ignore sound command 0x7f - audio bus init - not relevant for APC sound
		else if (Command == 37) {													// sound command 0x25 - random speech
			char FileName[13] = "0_25_000.snd";							// generate base filename
			FileName[7] = 48 + random(4) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);}								// play the corresponding sound file
		else if (Command == 42) {													// sound command 0x2a - random speech
			char FileName[13] = "0_2a_000.snd";							// generate base filename
			FileName[7] = 48 + random(2) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);}								// play the corresponding sound file
		else if (Command == 44) {													// sound command 0x2c - stop sound
			AfterSound = 0;																	// disable auto restart of BG sound
			SoundSeries = 0;																// Reset BG sound
			StopPlayingSound();}
		else if (Command == 45) {													// sound command 0x2d - background sound - sound series
			if (SoundSeries < 31)														// sound series has 31 different tunes
				SoundSeries++;																// switch to the next tune when sound command is called again
			char FileName[13] = "0_2d_000.snd";							// generate base filename
			FileName[7] = 48 + (SoundSeries % 10);					// change the 7th character of filename according to current tune
			FileName[6] = 48 + (SoundSeries % 100) / 10;		// the same with the 6th character
			for (byte i=0; i<12; i++) {											// store filename to be repeated
				USB_RepeatSound[i] = FileName[i];}
			QueueNextSound(USB_RepeatSound);								// set this filename to be started by PlayNextSound
			PlaySound(51, (char*) FileName);}
		else if (Command == 48) {													// sound command 0x30 - random speech
			char FileName[13] = "0_30_000.snd";							// generate base filename
			FileName[7] = 48 + random(2) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);}								// play the corresponding sound file
		else if (Command == 49) {													// sound command 0x31 - random speech
			char FileName[13] = "0_31_000.snd";							// generate base filename
			FileName[7] = 48 + random(2) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);}								// play the corresponding sound file
		else if (Command == 55) {													// sound command 0x37 - random speech
			char FileName[13] = "0_37_000.snd";							// generate base filename
			FileName[7] = 48 + random(2) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);}								// play the corresponding sound file
		else if (Command == 58) {													// sound command 0x3a - random speech
			char FileName[13] = "0_3a_000.snd";							// generate base filename
			FileName[7] = 48 + random(4) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);}								// play the corresponding sound file
		else {																						// standard sound
			char FileName[9] = "0_00.snd";									// handle standard sound
			if (USB_GenerateFilename(1, Command, FileName)) {	// create filename and check whether file is present
				PlaySound(51, (char*) FileName);}}
		return(0);																				// return number not relevant for sounds
	default:
		return(0);}}																			// no exception rule found for this type so proceed as normal

byte EX_BlackKnight(byte Type, byte Command){
	static byte SoundSeries[3];													// buffer to handle pre system11 sound series
	static byte LastCh1Sound;														// preSys11: stores the number of the last sound that has been played on Ch1
	switch(Type){
	case SoundCommandCh1:																// sound commands for channel 1
		if (Command == 127) {	}														// ignore sound command 0x7f - audio bus init - not relevant for APC sound
//		else if (Command == 48) {													// sound command 0x30
//			if (QuerySolenoid(11)) {												// GI off?
//				PlaySound(152, "0_30_001.snd");}}							// play multiball ball release sequence
//		else if (Command == 56) {													// sound command 0x38
//			if (QuerySolenoid(11)) {												// GI off?
//				if (LastCh1Sound != 56) {											// ignore all subsequent 0x38 commands
//					AfterSound = 0;
//					LastCh1Sound = Command;											// buffer sound number
//					PlaySound(51, "0_38_001.snd");}}}						// play multiball start sequence
		else if (Command == 43) {													// sound command 0x2b - start game
			char FileName[13] = "0_2b_000.snd";							// generate base filename
			FileName[7] = 48 + random(5) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);}								// play the corresponding sound file
		else if (Command == 45) {													// sound command 0x2d - activated spinner - sound series
			if (SoundSeries[0] != 45) {
				SoundSeries[0] = 45;
				SoundSeries[1] = 0;}
			SoundSeries[1]++;
			char FileName[13] = "0_2d_000.snd";
			FileName[7] = 48 + (SoundSeries[1] % 10);
			FileName[6] = 48 + (SoundSeries[1] % 100) / 10;
			FileName[5] = 48 + SoundSeries[1] / 100;
			LastCh1Sound = Command;													// buffer sound number
			PlaySound(51, (char*) FileName);}
		else if (Command == 44) {													// sound command 0x2c - stop sound
			AfterSound = 0;
			SoundSeries[0] = 0;															// Reset last sound series number
			SoundSeries[1] = 0;															// reset the multiball start sound
			SoundSeries[2] = 0;															// Reset BG sound
			StopPlayingSound();}
		else if (Command == 46) {													// sound command 0x2e - background sound - sound series
			SoundSeries[0] = 0;
			if (SoundSeries[2] < 29)
				SoundSeries[2]++;
			char FileName[13] = "0_2e_000.snd";
			FileName[7] = 48 + (SoundSeries[2] % 10);
			FileName[6] = 48 + (SoundSeries[2] % 100) / 10;
			FileName[5] = 48 + SoundSeries[2] / 100;
			for (byte i=0; i<12; i++) {
				USB_RepeatSound[i] = FileName[i];}
			QueueNextSound(USB_RepeatSound);
			LastCh1Sound = Command;													// buffer sound number
			PlaySound(51, (char*) FileName);}
		else if (Command == 52) {													// sound command 0x34 - bonus count
			AfterSound = 0;
			if (!QueryLamp(49) && !QueryLamp(57) && !QueryLamp(61)) { // only bonus lamp 1 lit?
				PlaySound(51, "0_34_002.snd");}
			else if (LastCh1Sound != 52) {
				LastCh1Sound = Command;												// buffer sound number
				SoundSeries[2] = 0;														// Reset BG sound
				PlaySound(51, "0_34_001.snd");}}
//		else if (Command == 58) {													// sound command 0x3a
//			PlaySound(152, "0_3a.snd");}										// play multiball ball release sequence
		else {																						// standard sound
			LastCh1Sound = Command;													// buffer sound number
			char FileName[9] = "0_00.snd";									// handle standard sound
			if (USB_GenerateFilename(1, Command, FileName)) {	// create filename and check whether file is present
				PlaySound(51, (char*) FileName);}}
		return(0);
	case SwitchActCommand:															// activated switches
		if (Command == 45) {															// ball successfully ejected
			EX_BallRelease(0);}															// stop ball release timer
		else if (Command == 9) {													// right magnet button
			if (QueryLamp(9) && QueryLamp(2)) {							// right magnet and ball in play lamp lit?
				ActivateSolenoid(0, 9);}}											// activate right magnet
		else if (Command == 10) {													// left magnet button
			if (QueryLamp(10) && QueryLamp(2)) {						// left magnet and ball in play lamp lit?
				ActivateSolenoid(0, 10);}}										// activate left magnet
		return(0);																				// all switches are reported to PinMame
	case SolenoidActCommand:														// activated solenoids
		if (Command == EX_EjectSolenoid){									// ball eject coil
			if (QueryLamp(2)) {															// ball in play lamp lit?
				EX_BallRelease(1);}}													// start ball release timer
		return(0);																				// return number not relevant for sounds
	default:
		return(0);}}

byte EX_Pinbot(byte Type, byte Command){
	switch(Type){
	case SoundCommandCh1:																// sound commands for channel 1
		if (!Command){ 																		// sound command 0x00 - stop sound
			AfterSound = 0;
			StopPlayingSound();}
		else if (Command == 85) {	}												// ignore sound command 0x55
		else if (Command == 105) { }											// ignore strange misplaced sound during multiball
		else if (Command == 170) { }											// ignore sound command 0xaa
		else if (Command == 255) { }											// ignore sound command 0xff
		else {																						// proceed with standard sound handling
			char FileName[9] = "0_00.snd";									// handle standard sound
			if (USB_GenerateFilename(1, Command, FileName)) {	// create filename and check whether file is present
				if (Command < 128) {													// play speech with a higher priority
					PlaySound(50, (char*) FileName);}
				else {
					PlaySound(51, (char*) FileName);}}}
		return(0);																				// return number not relevant for sounds
	case SoundCommandCh2:																// sound commands for channel 2
		if (!Command) {																		// sound command 0x00 - stop music
			AfterMusic = 0;
			StopPlayingMusic();}
		else if (Command == 127) {												// sound command 0x7f - stop sound
			AfterSound = 0;
			StopPlayingSound();}
		else if (Command > 29 && Command < 48) { }				// ignore unknown sound commands 0x1d to 0x30
		else if (Command > 79 && Command < 89) { }				// ignore unknown sound commands 0x4f to 0x59
		else if (Command > 95 && Command < 100) { 				// music volume command 0x6X
			MusicVolume = Command - 96;}
		else if (Command == 170) { }											// ignore unknown sound command 0xaa
		else if (Command == 255) { }											// ignore unknown sound command 0xff
		else if (Command == 1) {													// music track 1
			PlayMusic(50, "1_01L.snd");											// play music track
			QueueNextMusic("1_01L.snd");}										// track is looping so queue it also
		else if (Command == 2) {													// music track 2
			PlayMusic(50, "1_02.snd");											// play non looping part of music track
			QueueNextMusic("1_02L.snd");}										// queue looping part as next music to be played
		else if (Command == 3) {													// music track 3
			PlayMusic(50, "1_03L.snd");											// play music track
			QueueNextMusic("1_03L.snd");}										// track is looping so queue it also
		else if (Command == 4) {													// music track 4
			PlayMusic(50, "1_04.snd");											// play non looping part of music track
			QueueNextMusic("1_04L.snd");}										// queue looping part as next music to be played
		else if (Command == 5) {													// music track 5
			PlayMusic(50, "1_05.snd");											// play non looping part of music track
			QueueNextMusic("1_04L.snd");}										// queue looping part as next music to be played
		else if (Command == 6) {													// music track 6
			PlayMusic(50, "1_06.snd");											// play non looping part of music track
			QueueNextMusic("1_06L.snd");}										// queue looping part as next music to be played
		else if (Command == 8) {													// music track 8
			PlayMusic(50, "1_08.snd");											// play non looping part of music track
			QueueNextMusic("1_01L.snd");}										// queue looping part as next music to be played
		else if (Command == 10) {													// music track 0xa
			PlayMusic(50, "1_0a.snd");											// play non looping part of music track
			QueueNextMusic("1_02L.snd");}										// queue looping part as next music to be played
		else if (Command == 65) {													// music track 0x41
			PlayMusic(50, "1_41.snd");											// play non looping part of music track
			QueueNextMusic("1_01L.snd");}										// queue looping part as next music to be played
		else if (Command == 66) {													// music track 0x42
			PlayMusic(50, "1_0a.snd");											// play non looping part of music track
			QueueNextMusic("1_02L.snd");}										// queue looping part as next music to be played
		else if (Command == 148) {												// music track 0x94
			PlayMusic(50, "1_94.snd");											// play non looping part of music track
			QueueNextMusic("1_94L.snd");}										// queue looping part as next music to be played
		else if (Command == 168) {												// music track 0xa8
			PlayMusic(50, "1_a8.snd");											// play non looping part of music track
			QueueNextMusic("1_a8L.snd");}										// queue looping part as next music to be played
		else {
			char FileName[9] = "1_00.snd";									// handle standard sound
			if (USB_GenerateFilename(2, Command, FileName)) {	// create filename and check whether file is present
				if (Command < 128) {													// play only music on the music channel
					AfterMusic = 0;															// stop looping music
					PlayMusic(50, (char*) FileName);}						// play on the music channel
				else {
					PlaySound(50, (char*) FileName);}}}					// play on the sound channel
		return(0);																				// return number not relevant for sounds
	default:																						// use default treatment for undefined types
		return(0);}}																			// no exception rule found for this type so proceed as normal

byte EX_Rollergames(byte Type, byte Command){
	switch(Type){
	case SoundCommandCh2:																// sound commands for channel 1
		if (!Command){ 																		// sound command 0x00 - stop sound
			AfterSound = 0;
			StopPlayingSound();
			AfterMusic = 0;
			StopPlayingMusic();}
		else if (Command > 95 && Command < 100) { 				// music volume command 0x6X
			MusicVolume = Command - 96;}
		else if (Command == 1) {													// music track 1
			PlayMusic(50, "1_01.snd");											// play non looping part of music track 1
			QueueNextMusic("1_01L.snd");}										// queue looping part as next music to be played
		else if (Command == 3 || Command == 65) {					// music track 3 identical to 0x41
			PlayMusic(50, "1_03.snd");											// play non looping part of music track
			QueueNextMusic("1_03L.snd");}										// queue looping part as next music to be played
		else if (Command == 4) {													// music track 0x04
			PlayMusic(50, "1_04.snd");											// play music track
			QueueNextMusic("1_04.snd");}										// track is looping so queue it also
		else if (Command == 6) {													// music track 6 Multiball start
			PlayMusic(50, "1_06.snd");											// play non looping part of music track
			QueueNextMusic("1_06L.snd");}										// queue looping part as next music to be played
		else if (Command == 7) {													// music track 0x07
			PlayMusic(50, "1_07.snd");											// play music track
			QueueNextMusic("1_07.snd");}										// track is looping so queue it also
		else if (Command == 8) {													// music track 8 Multiball lock
			PlayMusic(50, "1_08.snd");											// play non looping part of music track
			QueueNextMusic("1_08L.snd");}										// queue looping part as next music to be played
		else if (Command == 9 || Command == 66) {					// music track 9 identical to 0x42
			PlayMusic(50, "1_09.snd");											// play non looping part of music track
			QueueNextMusic("1_09L.snd");}										// queue looping part as next music to be played
		else if (Command == 11) {													// music track 0x0b
			PlayMusic(50, "1_0bL.snd");											// play music track
			QueueNextMusic("1_0bL.snd");}										// track is looping so queue it also
		else if (Command == 12) {													// music track 0x0c
			PlayMusic(50, "1_0c.snd");											// play non looping part of music track
			QueueNextMusic("1_0cL.snd");}										// queue looping part as next music to be played
		else if (Command == 13) {													// music track 0x0d
			PlayMusic(50, "1_0d.snd");											// play non looping part of music track
			QueueNextMusic("1_0dL.snd");}										// queue looping part as next music to be played
		else if (Command == 14) {													// music track 0x0e
			PlayMusic(50, "1_0e.snd");											// play non looping part of music track
			QueueNextMusic("1_0eL.snd");}										// queue looping part as next music to be played
		else if (Command == 15) {													// music track 0x0f
			PlayMusic(50, "1_0f.snd");											// play music track
			QueueNextMusic("1_0bL.snd");}										// queue looping part as next music to be played
		else if (Command == 67) {													// music track 0x43
			PlayMusic(50, "1_43.snd");											// play non looping part of music track
			QueueNextMusic("1_43L.snd");}										// queue looping part as next music to be played
		else if (Command == 68) {													// music track 0x44
			PlayMusic(50, "1_44.snd");											// play non looping part of music track
			QueueNextMusic("1_43L.snd");}										// queue looping part as next music to be played
		else {																						// standard sound
			if (Command == 78) {														// 0x4e ends music track
				StopPlayingMusic();}
			char FileName[9] = "1_00.snd";									// handle standard sound
			if (USB_GenerateFilename(2, Command, FileName)) {	// create filename and check whether file is present
				PlaySound(51, (char*) FileName);}}
		return(0);
	default:																						// use default treatment for undefined types
		return(0);}}

byte EX_Blank(byte Type, byte Command){
	switch(Type){
	case SoundCommandCh1:																// sound commands for channel 1
		if (Command == 38){ 															// sound command 0x26
			// enter your special sound command 0x26 here
		}
		else {
			char FileName[9] = "0_00.snd";									// handle standard sound
			if (USB_GenerateFilename(1, Command, FileName)) {	// create filename and check whether file is present
				PlaySound(51, (char*) FileName);}}
		return(0);																				// return number not relevant for sounds
	case SoundCommandCh2:																// sound commands for channel 2
		if (Command == 38){ 															// sound command 0x26
			// enter your special sound command 0x26 here
		}
		else {
			char FileName[9] = "1_00.snd";									// handle standard music
			if (USB_GenerateFilename(2, Command, FileName)) {	// create filename and check whether file is present
				PlayMusic(51, (char*) FileName);}}
		return(0);																				// return number not relevant for sounds
	case SwitchActCommand:															// activated switches
		if (Command == 43) {															// handle the activation of switch 43
			// enter your special handling for switch 43 here
		}
		return(0);																				// switch will also be reported to PinMame. Use return(1) to hide the activation from PinMame
	case SwitchRelCommand:															// released switches
		if (Command == 43){																// handle the deactivation of switch 43
			// enter your special handling for switch 43 here
		}
		return(0);																				// switch will also be reported to PinMame. Use return(1) to hide the deactivation from PinMame
	case SolenoidActCommand:														// activate solenoids
		if (Command == 2){																// handle the activation of solenoid 2
			// enter your special handling for activating solenoid 2 here
		}
		return(0);																				// solenoid will be activated. Use return(1) to suppress this
	case SolenoidRelCommand:														// deactivate solenoids
		if (Command == 2){																// handle the deactivation of solenoid 2
			// enter your special handling for deactivating solenoid 2 here
		}
		return(0);																				// solenoid will be deactivated. Use return(1) to suppress this
	case LampOnCommand:																	// turn on lamp
		if (Command == 2){																// handle the turn on of lamp 2
			// enter your special handling for activating lamp 2 here
		}
		return(0);																				// lamp will be turn on. Use return(1) to suppress this
	case LampOffCommand:																// turn off lamp
		if (Command == 2){																// handle the turn off of lamp 2
			// enter your special handling for turning off lamp 2 here
		}
		return(0);																				// lamp will be turn off. Use return(1) to suppress this
	case WriteToDisplay0:																// write to display 0 (Credit)
		if (USB_SerialBuffer[0] == 1) {										// if the first digit is zero
			// enter your special handling for display 0 here
		}
		return(0);																				// display content will be written. Use return(1) to suppress this
	default:																						// use default treatment for undefined types
		return(0);}}

void EX_Init(byte GameNumber) {
	switch(GameNumber) {
	case 20:																						// Jungle Lord
		EX_EjectSolenoid = 2;															// specify eject coil for improved ball release
		USB_SolTimes[20] = 0;															// allow permanent on state for magna save relais
		USB_SolTimes[21] = 0;
		PinMameException = EX_JungleLord;									// use exception rules for Jungle Lord
		break;
	case 21:																						// Pharaoh
		PinMameException = EX_Pharaoh;										// use exception rules for Pharaoh
		break;
	case 34:																						// Black Knight
		EX_EjectSolenoid = 6;															// specify eject coil for improved ball release
		USB_SolTimes[8] = 0;															// allow permanent on state for magna save relais
		USB_SolTimes[9] = 0;
		PinMameException = EX_BlackKnight;								// use exception rules for Jungle Lord
		break;
	case 43:																						// Pinbot
		PinMameException = EX_Pinbot;											// use exception rules for Pinbot
		break;
	case 67:																						// Rollergames
		PinMameException = EX_Rollergames;								// use exception rules for Rollergames
		break;
	default:
		PinMameException = EX_DummyProcess;}}

