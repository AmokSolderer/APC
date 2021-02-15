#define SoundCommandCh1 0
#define SoundCommandCh2 1
#define SwitchActCommand 2
#define SwitchRelCommand 3
#define SolenoidActCommand 4
#define SolenoidRelCommand 5
#define LampOnCommand 6
#define LampOffCommand 7

byte (*PinMameException)(byte, byte);

char USB_RepeatSound[13];															// name of the sound file to be repeated
byte EX_EjectSolenoid;																// eject coil for improved ball release
unsigned int USB_SolTimes[32] = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 0, 0, 40, 40, 40, 40, 40, 40, 40, 40};	// Activation times for solenoids

byte EX_DummyProcess(byte Type, byte Command) {
	UNUSED(Type);
	UNUSED(Command);
	return(0);}

void EX_BallRelease(byte State) {											// repeat ball eject in case ball got stuck
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
		if (Command == 38){ 															// sound command 0x26 - start game
			char FileName[13] = "0_26_000.snd";							// generate base filename
			FileName[7] = 48 + random(4) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);								// play the corresponding sound file
			return(1);}																			// do not try to play this as a normal sound
		else if (Command == 42){													// sound command 0x2a - background sound - sound series
			SoundSeries[1] = 0;															// reset the multiball start sound
			if (SoundSeries[0] < 29)												// BG sound has 29 tunes
				SoundSeries[0]++;															// every call of this sound proceeds with the next tune
			char FileName[13] = "0_2a_000.snd";							// generate base filename
			FileName[7] = 48 + (SoundSeries[0] % 10);				// change the 7th character of filename according to current tune
			FileName[6] = 48 + (SoundSeries[0] % 100) / 10;	// the same with the 6th character
			for (i=0; i<12; i++) {													// store the name of this sound
				USB_RepeatSound[i] = FileName[i];}
			NextSoundName = USB_RepeatSound;								// select this sound to be repeated
			AfterSound = PlayNextSound;											// determine that PlayNextSound is executed when the sound has run out
			PlaySound(51, (char*) FileName);								// play the sound
			return(1);}																			// this was a special sound so do not proceed with standard sound handling
		else if (Command == 44) {													// sound command 0x2c - stop sound
			AfterSound = 0;
			SoundSeries[0] = 0;															// Reset BG sound
			SoundSeries[1] = 0;															// reset the multiball start sound
			StopPlayingSound();
			return(1);}
		else if (Command == 45){													// sound command 0x2d - multiball start - sound series
			if (SoundSeries[1] < 31)												// this sound has 31 tunes
				SoundSeries[1]++;															// every call of this sound proceeds with next tune
			else
				SoundSeries[1] = 1;														// start all over again
			char FileName[13] = "0_2d_000.snd";							// generate base filename
			FileName[7] = 48 + (SoundSeries[1] % 10);				// change the 7th character of filename according to current tune
			FileName[6] = 48 + (SoundSeries[1] % 100) / 10;	// the same with the 6th character
			PlaySound(51, (char*) FileName);								// play the sound
			return(1);}																			// this was a special sound so do not proceed with standard sound handling
		else
			return(0);																			// this number does not belong to a special sound so proceed with standard sound handling
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
		if (Command == 37) {															// sound command 0x25 - random speech
			char FileName[13] = "0_05_000.snd";							// generate base filename
			FileName[7] = 48 + random(4) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);								// play the corresponding sound file
			return(1);}																			// do not proceed with standard sound handling
		else if (Command == 42) {													// sound command 0x2a - random speech
			char FileName[13] = "0_0a_000.snd";							// generate base filename
			FileName[7] = 48 + random(2) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);								// play the corresponding sound file
			return(1);}																			// do not proceed with standard sound handling
		else if (Command == 44) {													// sound command 0x2c - stop sound
			AfterSound = 0;																	// disable auto restart of BG sound
			SoundSeries = 0;																// Reset BG sound
			StopPlayingSound();
			return(1);}																			// do not proceed with standard sound handling
		else if (Command == 45) {													// sound command 0x2d - background sound - sound series
			if (SoundSeries < 31)														// sound series has 31 different tunes
				SoundSeries++;																// switch to the next tune when sound command is called again
			char FileName[13] = "0_0d_000.snd";							// generate base filename
			FileName[7] = 48 + (SoundSeries % 10);					// change the 7th character of filename according to current tune
			FileName[6] = 48 + (SoundSeries % 100) / 10;		// the same with the 6th character
			for (i=0; i<12; i++) {													// store filename to be repeated
				USB_RepeatSound[i] = FileName[i];}
			NextSoundName = USB_RepeatSound;								// set this filename to be started by PlayNextSound
			AfterSound = PlayNextSound;											// Call PlayNextSounds when current sound has run out
			PlaySound(51, (char*) FileName);
			return(1);}																			// do not proceed with standard sound handling
		else if (Command == 48) {													// sound command 0x30 - random speech
			char FileName[13] = "0_10_000.snd";							// generate base filename
			FileName[7] = 48 + random(2) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);								// play the corresponding sound file
			return(1);}																			// do not proceed with standard sound handling
		else if (Command == 49) {													// sound command 0x31 - random speech
			char FileName[13] = "0_11_000.snd";							// generate base filename
			FileName[7] = 48 + random(2) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);								// play the corresponding sound file
			return(1);}																			// do not proceed with standard sound handling
		else if (Command == 55) {													// sound command 0x37 - random speech
			char FileName[13] = "0_17_000.snd";							// generate base filename
			FileName[7] = 48 + random(2) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);								// play the corresponding sound file
			return(1);}																			// do not proceed with standard sound handling
		else if (Command == 58) {													// sound command 0x3a - random speech
			char FileName[13] = "0_1a_000.snd";							// generate base filename
			FileName[7] = 48 + random(4) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);								// play the corresponding sound file
			return(1);}																			// do not proceed with standard sound handling
		return(0);																				// no special sound rule found so proceed with standard sound handling
	default:
		return(0);}}																			// no exception rule found for this type so proceed as normal

byte EX_BlackKnight(byte Type, byte Command){
	static byte SoundSeries[3];													// buffer to handle pre system11 sound series
	static byte LastCh1Sound;														// preSys11: stores the number of the last sound that has been played on Ch1
	switch(Type){
	case SoundCommandCh1:																// sound commands for channel 1
		if (Command == 48) {															// sound command 0x30
			if (QuerySolenoid(11)) {												// GI off?
				PlaySound(152, "0_30_001.snd");								// play multiball ball release sequence
				return(1);}}
		else if (Command == 56) {													// sound command 0x38
			if (QuerySolenoid(11)) {												// GI off?
				if (LastCh1Sound != 56) {											// ignore all subsequent 0x38 commands
					AfterSound = 0;
					LastCh1Sound = Command;											// buffer sound number
					PlaySound(51, "0_38_001.snd");}							// play multiball start sequence
				return(1);}}
		else if (Command == 43) {													// sound command 0x2b - start game
			char FileName[13] = "0_2b_000.snd";							// generate base filename
			FileName[7] = 48 + random(5) + 1;								// change the counter according to random number
			PlaySound(52, (char*) FileName);								// play the corresponding sound file
			return(1);}
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
			PlaySound(51, (char*) FileName);
			return(1);}
		else if (Command == 44) {													// sound command 0x2c - stop sound
			AfterSound = 0;
			SoundSeries[0] = 0;															// Reset last sound series number
			SoundSeries[1] = 0;															// reset the multiball start sound
			SoundSeries[2] = 0;															// Reset BG sound
			StopPlayingSound();
			return(1);}
		else if (Command == 46) {													// sound command 0x2e - background sound - sound series
			SoundSeries[0] = 0;
			if (SoundSeries[2] < 29)
				SoundSeries[2]++;
			char FileName[13] = "0_2e_000.snd";
			FileName[7] = 48 + (SoundSeries[2] % 10);
			FileName[6] = 48 + (SoundSeries[2] % 100) / 10;
			FileName[5] = 48 + SoundSeries[2] / 100;
			for (i=0; i<12; i++) {
				USB_RepeatSound[i] = FileName[i];}
			NextSoundName = USB_RepeatSound;
			AfterSound = PlayNextSound;
			LastCh1Sound = Command;													// buffer sound number
			PlaySound(51, (char*) FileName);
			return(1);}
		else if (Command == 52) {													// sound command 0x34 - bonus count
			AfterSound = 0;
			if (!QueryLamp(49) && !QueryLamp(57) && !QueryLamp(61)) { // only bonus lamp 1 lit?
				PlaySound(51, "0_34_002.snd");
				return(1);}
			if (LastCh1Sound != 52) {
				LastCh1Sound = Command;												// buffer sound number
				SoundSeries[2] = 0;														// Reset BG sound
				PlaySound(51, "0_34_001.snd");}
			return(1);}
		else if (Command == 58) {													// sound command 0x3a
			PlaySound(152, "0_3a.snd");											// play multiball ball release sequence
			return(1);}
		LastCh1Sound = Command;														// buffer sound number
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
		return(0);
	default:
		return(0);}}

byte EX_Blank(byte Type, byte Command){
	switch(Type){
	case SoundCommandCh1:																// sound commands for channel 1
		if (Command == 38){ 															// sound command 0x26
												// enter your special sound command 0x26 here
			return(1);}																			// do not proceed with standard sound handling
		else
			return(0);																			// proceed with standard sound handling
	case SoundCommandCh2:																// sound commands for channel 2
		if (Command == 38){ 															// sound command 0x26
												// enter your special sound command 0x26 here
			return(1);}																			// do not proceed with standard sound handling
		else
			return(0);																			// proceed with standard sound handling
	case SwitchActCommand:															// activated switches
		if (Command == 43) {															// handle the activation of switch 43
												// enter your special handling for switch 43 here
		}
			return(0);																			// switch will also be reported to PinMame. Use return(1) to hide the activation from PinMame
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
	default:
		PinMameException = EX_DummyProcess;}}
