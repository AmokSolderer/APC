#include <SdFat.h>
SdFat SD;
#include <SPI.h>
#include "Arduino.h"
#include "Sound.h"

const int SwMax = 72;                                 // number of existing switches (max. 72)
const int LampMax = 64;                               // number of existing lamps (max. 64)
const int DispColumns = 16;                           // Number of columns of the used display unit
 // Disp Test Pattern const byte DispPattern[4] = {134,69,42,25};

const byte AlphaUpper[118] = {0,0,0,0,0,0,0,0,107,21,0,0,0,0,0,0,0,0,0,0,64,191,64,21,0,0,64,4,0,0,0,40, // Blank $ * + - / for upper row alphanumeric displays
  63,0,6,0,93,4,15,4,102,4,107,4,123,4,14,0,127,4,111,4,0,0,0,0,136,0,65,4,0,34,0,0,0,0, // 0 1 2 3 4 5 6 7 8 9 < = > and fill bytes
  126,4,15,21,57,0,15,17,121,4,120,4,59,4,118,4,0,17,23,0,112,136,49,0,54,10,54,130,63,0, // Pattern A B C D E F G H I J K L M N O
  124,4,63,128,124,132,107,4,8,17,55,0,48,40,54,160,0,170,0,26,9,40}; // Pattern P Q R S T U V W X Y Z

const byte AlphaLower[118] = {0,0,0,0,0,0,0,0,182,35,0,0,0,0,0,0,0,0,0,0,2,247,2,35,0,0,2,2,0,0,0,80, // Blank $ * + - / for lower row alphanumeric displays
  252,0,136,0,122,2,184,2,142,2,182,2,246,2,152,0,254,2,190,2,0,0,0,0,0,68,0,0,0,144,0,0,0,0, // 0 1 2 3 4 5 6 7 8 9 < > and fill bytes
  222,2,184,35,116,0,184,33,118,2,86,2,244,2,206,2,0,33,232,0,70,68,100,0,204,192,204,132,252,0, // Pattern A B C D E F G H I J K L M N O
  94,2,252,4,94,6,182,2,16,33,236,0,68,80,204,20,0,212,0,224,48,80}; // Pattern P Q R S T U V W X Y Z

const byte NumLower[118] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // Blank and fill bytes for lower row numeric displays
	  252,0,136,0,122,0,186,0,142,0,182,0,246,0,152,0,254,0,190,0,0,0,0,0,0,0,72,0,0,0,0,0,0,0, // 0 1 2 3 4 5 6 7 8 9 = and fill bytes
	  222,0,230,0,116,0,234,0,118,0,86,0,246,0,206,0,68,0,232,0,206,0,100,0,194,0,194,0,226,0, // Pattern A B C D E F G H I J K L M N O
	  94,0,252,0,66,0,182,0,84,0,236,0,236,0,236,0,206,0,78,0,122,0}; // Pattern P Q R S T U V W X Y Z

const byte LeftCredit[118] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // Blank and fill bytes for the left credit (numeric)
  63,0,6,0,93,0,79,0,102,0,107,0,123,0,14,0,127,0,111,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0 1 2 3 4 5 6 7 8 9 and fill bytes
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // Pattern A B C D E F G H I J K L M N O
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Pattern P Q R S T U V W X Y Z

const byte RightCredit[118] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // Blank and fill bytes for the right credit (numeric)
  252,0,136,0,122,0,186,0,142,0,182,0,246,0,152,0,254,0,190,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0 1 2 3 4 5 6 7 8 9 and fill bytes
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // Pattern A B C D E F G H I J K L M N O
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Pattern P Q R S T U V W X Y Z

const int LampPeriod = 2;                             // Lamp cycle period in ms
const bool NoLamps[LampMax+1] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const bool AllLamps[LampMax+1] = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

// system variables

unsigned int IntBuffer;                               // general purpose buffer
unsigned int IntBuffer2;                              // general purpose buffer
byte ByteBuffer = 0;                                  // general purpose buffer
byte ByteBuffer2 = 0;                                 // general purpose buffer
byte ByteBuffer3 = 0;                                 // general purpose buffer
byte i = 0;                                           // general purpose counter
byte x = 0;                                           // general purpose counter
byte c = 0;                                           // reserved counter
bool SDfound = false;                                 // SD card present?
byte SwitchStack = 0;                                 // determines which switch events stack is active
byte ChangedSw[2][30];                                // two stacks of switches with pending events
byte SwEvents[2];                                     // contains the number of pending switch events in each stack
bool Switch[SwMax+3];                                 // stores the present status of all switches (Advance is 72, HighScoreRest is 71)
bool SwHistory[SwMax+3];                              // stores the previous switch status
int SwDrv = 0;                                        // switch driver being accessed at the moment
const byte *DispRow1;                                  // determines which patterns are to be shown (2 lines with 16 chars each)
const byte *DispRow2;
const byte *DispPattern1;
const byte *DispPattern2;
byte DisplayUpper[32];                                // changeable display buffer
byte DisplayLower[32];
byte DisplayUpper2[32];                               // second changeable display buffer
byte DisplayLower2[32];
byte DispCol = 0;                                     // display column being illuminated at the moment
const bool *LampPattern;                              // determines which lamp pattern is to be shown (for Lamps > 8)
const bool *LampBuffer;
byte StrobeLightsTimer = 0;
bool Lamp[LampMax+1];                                 // changeable lamp status buffer
bool LEDFlag;																					// stores whether Sel5 has to be triggered by the rising or falling edge
byte LEDCommandBytes = 0;															// number of command bytes to be send to the LED exp board
byte LEDCount = 0;																		// points to the next command byte to be send to the LED exp board
byte LEDCommand[20];																	// command bytes to be send to the LED exp board
byte LampCol = 0;                                     // lamp column being illuminated at the moment
byte LampWait = 1;                                    // counter for lamp waiting time until next column is applied
const struct LampPat *PatPointer;                     // Pointer to the lamp flow to be shown
struct LampPat {                                      // a lamp pattern sequence played by ShowLampPatterns
  uint16_t Duration;
  bool Pattern[LampMax-7];};
struct LampFlow {                                     // defines a series of lamp patterns shown by AttractLampCycle
	uint16_t Repeat;
  const struct LampPat *FlowPat;};
short FlowRepeat = 0;                                 // number of repeats for the LampFlow
void (*LampReturn)(byte);                             // Pointer to the procedure to be executed after the lamp flow has been shown
byte BlinkTimers = 0;                                 // number of active blink timers
byte BlinkTimer[65];                                  // timer used for blinking lamps
byte BlinkingNo[65];                                  // number of lamps using this BlinkTimer
bool BlinkState[65];                                  // current state of the lamps of this blink timer
unsigned int BlinkPeriod[65];                         // blink period for this timer
byte BlinkingLamps[65][65];                           // [BlinkTimer] used by these [lamps]
bool SolChange = false;                               // Indicates that the state of a solenoid has to be changed
byte SolNumber = 0;                                   // Determines which solenoid is to be changed ...
bool SolState = false;                                // and what the desired state is
byte SolDelayed[20];                                  // Queue for waiting solenoid requests
bool C_BankActive = false;														// A/C relay currently doing C bank?
byte A_BankWaitingNo = 0;															// amount of coils in A bank waiting list
byte A_BankWaiting[10];																// list of waiting A bank coils
unsigned int A_BankWaitDuration[10];									// duration values for waiting A bank coils
unsigned int DurDelayed[20];                          // duration values for waiting solenoid requests
byte *FlashSequence;                                  // pointer to the current flash lamp sequence
byte SettingsRepeatTimer = 0;													// numberof the timer of the key repeat function in the settings function
byte BlinkScoreTimer = 0;                             // number of the timer for the score blinking
byte BallWatchdogTimer = 0;                           // number of the ball watchdog timer
byte CheckReleaseTimer = 0;														// number of the timer for the ball release check
byte ShowMessageTimer = 0;                            // number of the timer to show a temporary message
bool BlockTimers = false;                             // blocks the timer interrupt while timer properties are being changed
byte ActiveTimers = 0;                                // Number of active timers
unsigned int TimerValue[64];                          // Timer values
byte TimerStack = 0;                                  // determines which timer events stack is active
byte RunOutTimers[2][30];                             // two stacks of timers with pending events
byte TimerEvents[2];                                  // contains the number of pending timer events in each stack
byte TimerArgument[64];
void (*TimerEvent[64])(byte);                         // pointers to the procedures to be executed on the timer event
void (*TimerBuffer)(byte);
void (*Switch_Pressed)(byte);                         // Pointer to current behavior mode for activated switches
void (*Switch_Released)(byte);                        // Pointer to current behavior mode for released switches
char EnterIni[3];
uint16_t *SoundBuffer;																// buffers sound data from SD to be processed by interrupt
uint16_t *MusicBuffer;																// buffers music data from SD to be processed by interrupt
uint16_t *Buffer16b;																	// 16bit pointer to the audio DAC buffer
uint32_t *Buffer32b;																	// 32bit pointer to the audio DAC buffer
byte MBP = 0;																					// Music Buffer Pointer - next block to write to inside of the music buffer
byte MusicIRpos = 0;																	// next block of the music buffer to be read from the interrupt
byte StopMusic = 0;																		// last music buffer block with music data
bool StartMusic = false;															// music startup active -> filling music buffer
bool PlayingMusic = false;														// StartMusic done -> continuously playing music
File MusicFile;																				// file handle for the music file (SD)
bool AfterMusicPending = false;												// indicates that there's an after music event to be executed
void (*AfterMusic)() = 0;															// program to execute after music file has ended
const char *NextMusicName;														// points to the name of the next music file to be played (if any)
byte MusicPriority = 0;																// stores the priority of the music file currently being played
byte SBP = 0;																					// Sound Buffer Pointer - next block to write to inside of the music buffer
byte SoundIRpos = 0;																	// next block of the sound buffer to be read from the interrupt
byte StopSound = 0;																		// last sound buffer block with sound data
bool StartSound = false;															// sound startup active -> filling sound buffer
bool PlayingSound = false;														// StartSound done -> continuously playing sound
File SoundFile;																				// file handle for the sound file (SD)
bool AfterSoundPending = false;												// indicates that there's an after sound event to be executed
void (*AfterSound)() = 0;															// program to execute after sound file has ended
byte SoundPriority = 0;																// stores the priority of the sound file currently being played
bool SoundPrio = false;																// indicates which channel has to be processed first
char *NextSoundName;
const char TestSounds[2][15] = {{"Musik.bin"},0};
void ExitSettings(bool change);
void HandleTextSetting(bool change);
void HandleNumSetting(bool change);
void HandleBoolSetting(bool change);
void RestoreDefaults(bool change);
void HandleVolumeSetting(bool change);

struct SettingTopic {
	char Text[15];																			// display text
	void (*EventPointer)(bool);													// Pointer to the subroutine to process this topic
	char *TxTpointer;																		// if text setting -> pointer to a text array
	byte LowerLimit;																		// if num setting -> lower limit of selection value
	byte UpperLimit;};																	// if text setting -> amount of text entries -1 / if num setting -> upper limit of selection value

const char APC_set_file_name[13] = "APC_SET.BIN";
const byte APC_defaults[64] =  {0,3,1,1,0,0,0,0,		 	// system default settings
											  				0,0,0,0,0,0,0,0,
																0,0,0,0,0,0,0,0,
																0,0,0,0,0,0,0,0,
																0,0,0,0,0,0,0,0,
																0,0,0,0,0,0,0,0,
																0,0,0,0,0,0,0,0,
																0,0,0,0,0,0,0,0};

const byte ActiveGame = 0;														// Select the active game
const byte NofBalls = 1;															// Balls per game
const byte FreeGame = 2;															// Free game mode?
const byte DimInserts = 3;                            // Reduce lighting time of playfield lamps by 50%
const byte Volume = 4;                             		// Volume of the speaker
const byte LEDsetting = 5;                            // Setting for the APC_LED_EXP board
const byte DisplayType = 6;														// which display is used?

char TxTGameSelect[3][15] = {{" BLACK KNIGHT "},{" JEDI  KNIGHT "},{"    PINBOT    "}};
char TxTLEDSelect[3][15] = {{"   NO   LEDS  "},{"PLAYFLD ONLY  "},{"PLAYFLDBACKBOX"}};
char TxTDisplaySelect[5][15] = {{"4 ALPHA+CREDIT"},{" SYS11 PINBOT "},{" SYS11  F-14  "},{" SYS11  BK2K  "},{" SYS11   TAXI "}};
struct SettingTopic APC_setList[10] = {{" ACTIVE GAME  ",HandleTextSetting,&TxTGameSelect[0][0],0,2},
      {" NO OF  BALLS ",HandleNumSetting,0,1,5},
		  {"  FREE  GAME  ",HandleBoolSetting,0,0,0},
		  {"  DIM  INSERTS",HandleBoolSetting,0,0,0},
      {"SPEAKERVOLUME ",HandleVolumeSetting,0,0,255},
      {"  LED   LAMPS ",HandleTextSetting,&TxTLEDSelect[0][0],0,2},
			{"DISPLAY TYPE  ",HandleTextSetting,&TxTDisplaySelect[0][0],0,4},
		  {"RESTOREDEFAULT",RestoreDefaults,0,0,0},
		  {"  EXIT SETTNGS",ExitSettings,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
struct GameDef {
	struct SettingTopic *GameSettingsList;							// points to the settings definition of the current game
	byte *GameDefaultsPointer;													// points to the default settings of the selected game
	const char *GameSettingsFileName;													// points to the name of the settings file for the selected game
	const char *HighScoresFileName;														// contains the name of the high scores file for the selected game
	void (*AttractMode)();															// Pointer to Attract mode of current game
	const unsigned int *SolTimes;												// Default activation times of solenoids
};

struct GameDef GameDefinition;
struct SettingTopic *SettingsList;

															// game variables

bool BlockOuthole = false;														// blocks the outhole as long as the previous ball loss is not handled completely
byte LockedBalls[5];                                  // locked balls of all players (starting from 1)
byte InLock = 0;                                      // number of balls currently in lock
byte Multiballs = 1;                                  // balls on playfield (same as multiball point multiplier)
byte NoPlayers = 0;                                   // number of players
byte Player = 0;                                      // current player
byte Coins = 0;                                       // inserted coins
byte Ball = 0;                                        // No of current ball
byte ExBalls = 0;                                     // No of extra balls
byte Bonus = 1;                                       // bonus points of the current player
byte BonusMultiplier = 1;															// current bonus multiplier
unsigned int Points[5];                               // points of all players (starting from 1)
struct HighScores {
  unsigned int Scores[4] = {4000000, 3000000, 2000000, 1000000}; // all time high scores
  char Initials[12] = {49,49,49,50,50,50,51,51,51,52,52,52};};
struct HighScores HallOfFame;
byte AppByte;                                         // general purpose application buffer
byte AppByte2;                                        // general purpose application buffer
byte AppByte3;                                        // general purpose application buffer
bool AppBool = false;                                 // general purpose application bool
byte APC_settings[64];																// system settings to be stored on the SD
byte game_settings[64];																// game settings to be stored on the SD
byte *SettingsPointer;																// points to the settings being changed
const char *SettingsFileName;																// points to the settings file currently being edited

const int UpDown = 53;                                // arduino pin connected to the auto/up - manual/Down button
const int Blanking = 22;                              // arduino pin to control the blanking signal
const byte VolumePin = 13;														// arduino pin to control the sound volume
const unsigned long SwitchMask[8] = {65536, 16777216, 8388608, 4194304, 64, 16, 8, 4};
const unsigned long AllSelects = 871346688;           // mask for all port C pins belonging to select signals except special switch select
const unsigned long Sel5 = 2097152;										// mask for the Sel5 select signal
const unsigned long AllData = 510;
uint16_t SwDrvMask = 2;                               // mask for switch row select
uint16_t LampColMask = 2;                             // mask for lamp column select
byte SolBuffer[3];                                    // stores the state of the solenoid latches

void setup() {
	pinMode(Blanking, OUTPUT);                          // initialize blanking pin
	pinMode(VolumePin, OUTPUT);                         // initialize volume PWM pin
	digitalWrite(Blanking, LOW);                        // and activate the blanking
	digitalWrite(VolumePin, HIGH);                      // set volume to 0
	pinMode(UpDown, INPUT);                          		// initialize Up/Down pin
	Serial.begin(115200);
	SPI.begin();
	REG_PIOC_PER = 871363582;                           // set required Port C pins to controlled In/Out
	REG_PIOC_PUDR = 871363582;                          // disable Pull-ups
	REG_PIOC_OER = 871363582;                           // set pins to outputs
	REG_PIOA_PER = 29950044;														// set required Port A pins to controlled In/Out
	REG_PIOA_PUDR = 29425756;														// disable Pull-ups
	REG_PIOA_ODR = 29425756;														// set pins to inputs
	REG_PIOA_OER = 524288;                              // set pin 19 to output (DisBlank)
	REG_PIOD_PER = 15;                            			// set required Port D pins to controlled In/Out
	REG_PIOD_PUDR = 15;                           			// disable Pull-ups
	REG_PIOD_OER = 15;                            			// set pins to outputs
	REG_PIOD_CODR = 15;                               	// clear strobe select signals
	REG_PIOC_CODR = AllSelects;                       	// clear all select signals
	REG_PIOC_SODR = 16384;                            	// set the special switch select (low active)
	REG_PIOC_CODR = AllSelects + AllData;               // clear all select signals and the data bus
  REG_PIOC_SODR = AllData - SwDrvMask;                // put select pattern on data bus
  REG_PIOC_SODR = 32768;                              // use Sel12
  REG_PIOC_CODR = AllSelects + AllData;               // clear all select signals and the data bus
  REG_PIOC_SODR = 16777216;                           // use Sel2
  REG_PIOC_CODR = AllSelects + AllData;               // clear all select signals and the data bus
  REG_PIOC_SODR = 8388608;                            // use Sel3
  REG_PIOC_CODR = AllSelects + AllData;               // clear all select signals and the data bus
  REG_PIOC_SODR = 4194304;                            // use Sel4  
  MusicBuffer = (uint16_t *) malloc(2048 * 2);
  SoundBuffer = (uint16_t *) malloc(2048 * 2);
  for (i=1; i< SwMax+1; i++) {
  	Switch[i] = false;
  	SwHistory[i] = 0; }                               // initialize switch status
  for (i=0; i<LampMax; i++) {                         // initialize lamp status
  	Lamp[i+1] = false; }
  for (i=0; i< 8; i++) {                              // initialize switch input pins
  	pinMode(54 + i, INPUT); }
  g_Sound.begin(44100, 100);													// initialize sound
  Buffer32b = g_Sound.next;														// fill first part of audio DAC buffer with silence
  for (i=0; i<64; i++) {
  	*Buffer32b = 402655232;														// 2048 on both channels and the channel 2 tag
  	Buffer32b++;}
	g_Sound.next = Buffer32b;
	g_Sound.enqueue();
  pmc_set_writeprotect(false);                        // enable writing to pmc registers
  pmc_enable_periph_clk(ID_TC7);                      // enable TC7
  TC_Configure(/* clock */TC2,/* channel */1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
  TC_SetRC(TC2, 1, 656);                              // set counter value to achieve an interrupt period of 1ms
  TC_Start(TC2, 1);
  TC2->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;              // IER = interrupt enable register
  TC2->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;             // IDR = interrupt disable register
  NVIC_EnableIRQ(TC7_IRQn);                           // enable interrupt
  delay(1000);
  DispPattern1 = AlphaUpper;
  DispPattern2 = AlphaLower;
  DispRow1 = DisplayUpper;
  DispRow2 = DisplayLower;
  LampPattern = NoLamps;
  digitalWrite(Blanking, HIGH);                       // Release the blanking
  if (SD.begin(52, SD_SCK_MHZ(20))) {                 // look for an SD card and set max SPI clock to 20MHz
    WriteUpper("SD CARD FOUND ");
    SDfound = true;}
  else {
    WriteUpper(" NO SD  CARD  ");}
  Init_System();}

void Init_System() {
	if (SDfound) {																			// SD card found?
		File Settings = SD.open(APC_set_file_name);				// look for system settings
		if (!Settings) {
			WriteLower("NO SYS SETTNGS");										// if no system settings
			for(i=0;i<64;i++) {															// use default settings
				APC_settings[i] = APC_defaults[i];}}
		else {																						// if system settings found
			Settings.read(&APC_settings, sizeof APC_settings);} // read them
		Settings.close();}
	else {																							// no SD card?
		for(i=0;i<64;i++) {																// use default settings
			APC_settings[i] = APC_defaults[i];}}
	delay(2000);
	Init_System2(0);}

void Init_System2(byte State) {
	switch(APC_settings[ActiveGame]) {									// init calls for all valid games
	case 0:
		BK_init();
		break;

	case 2:
		PB_init();
		break;
	default:
		WriteUpper("NO GAMESELECTD");
		while (true) {}
	}
	if ((APC_settings[DisplayType] == 1) || (APC_settings[DisplayType] == 2)) { // display with numerical lower row
		DispPattern2 = NumLower;}													// use patterns for num displays
	if (SDfound) {
		File HighScore = SD.open(GameDefinition.HighScoresFileName);
		if (!HighScore) {
			WriteUpper("NO HIGHSCORES ");}
		else {
			HighScore.read(&HallOfFame, sizeof HallOfFame);}
		HighScore.close();
		File Settings = SD.open(GameDefinition.GameSettingsFileName);
		if (!Settings) {
			WriteLower("NO GAMESETTNGS");
     if (!State) {                                    // only initialize the settings in the boot up run
			  for(i=0;i<64;i++) {
				  game_settings[i] = *(GameDefinition.GameDefaultsPointer+i);}}}
		else {
			Settings.read(&game_settings, sizeof game_settings);}
		Settings.close();}
	else {
    if (!State) {                                     // only initialize the settings in the boot up run
		  for(i=0;i<64;i++) {
			  game_settings[i] = *(GameDefinition.GameDefaultsPointer+i);}}}
	digitalWrite(VolumePin,HIGH);										    // turn off the digital volume control
	if (State) {
		ActivateTimer(2000, 0, EnterAttractMode);}
	else {
		delay(2000);
		EnterAttractMode(0);}}

void EnterAttractMode(byte Event) {                   // Enter the attract mode from a timer
    UNUSED(Event);
    
    GameDefinition.AttractMode();}

void TC7_Handler() {                                  // interrupt routine - runs every ms
	TC_GetStatus(TC2, 1);                               // clear status
	int i;                                              // general purpose counter
	int buff;
	uint16_t c;

	*(DisplayLower) = RightCredit[32 + 2 * ActiveTimers]; // debugging

	// Switches

	for (i=0; i<8; i++) {
    if (REG_PIOA_PDSR & SwitchMask[i]) {
      c = 1;}
    else {
      c = 0;}
		if (SwHistory[SwDrv*8+i+1] == c) {
			Switch[SwDrv*8+i+1] = !SwHistory[SwDrv*8+i+1]; 	// update the switch status
      SwHistory[SwDrv*8+i+1] = !SwHistory[SwDrv*8+i+1];
			SwEvents[SwitchStack]++;												// increase the number of pending switch events
			c = 0;
			while (ChangedSw[SwitchStack][c] && (c<30)) {		// look for a free slot
				c++;}
			ChangedSw[SwitchStack][c] = SwDrv*8+i+1;}}
	SwDrvMask = SwDrvMask<<1;                  					// and the corresponding select pattern
	REG_PIOC_CODR = AllSelects - Sel5 + AllData;        // clear all select signals except Sel5 and the data bus
	if (SwDrv < 7) {
		REG_PIOC_SODR = AllData - SwDrvMask;           		// put select pattern on data bus
		SwDrv++;                                  				// next switch driver
		REG_PIOC_SODR = 32768;}                        		// use Sel12
	else {
		if (SwDrv == 7) {
			REG_PIOC_SODR = AllData;
			SwDrv++;                                  			// next switch driver
			REG_PIOC_SODR = 32768;                        	// use Sel12
			REG_PIOC_CODR = 16384;}                       	// enable Sel13
		else {
			SwDrvMask = 2;
			REG_PIOC_SODR = AllData - SwDrvMask;          	// put select pattern on data bus
			SwDrv = 0;
			REG_PIOC_SODR = 32768+16384;}}                	// use Sel12 and disable Sel13

	// Display

	REG_PIOA_SODR = 524288;                             // disable latch outputs to hide writing cycle
	if (DispCol == (DispColumns-1)) {               		// last display column reached?
		DispCol = 0;}                                 		// start again from column 0
	else {
		DispCol++;}                                   		// prepare for next column
	REG_PIOD_CODR = 15;                               	// clear strobe select signals
	REG_PIOD_SODR = DispCol;                            // set display column
	REG_PIOC_CODR = AllSelects - Sel5 + AllData;        // clear all select signals except Sel5 and the data bus
	REG_PIOC_SODR = *(DispRow1+2*DispCol)<<1;           // set 1st byte of the display pattern for the upper row
	REG_PIOC_SODR = 524288;                             // use Sel8
	REG_PIOC_CODR = AllSelects - Sel5 + AllData;        // clear all select signals except Sel5 and the data bus
	REG_PIOC_SODR = *(DispRow1+2*DispCol+1)<<1;         // set 2nd byte of the display pattern for the upper row
	REG_PIOC_SODR = 262144;                             // use Sel9
	REG_PIOC_CODR = AllSelects - Sel5 + AllData;        // clear all select signals except Sel5 and the data bus
	REG_PIOC_SODR = *(DispRow2+2*DispCol)<<1;           // set 1st byte of the display pattern for the upper row
	REG_PIOC_SODR = 131072;                             // use Sel10
	REG_PIOC_CODR = AllSelects - Sel5 + AllData;        // clear all select signals except Sel5 and the data bus
	REG_PIOC_SODR = *(DispRow2+2*DispCol+1)<<1;         // set 1st byte of the display pattern for the upper row
	REG_PIOC_SODR = 65536;                             	// use Sel11

	// Timers

	if (!BlockTimers) {
		buff = ActiveTimers;
		i = 1;
		while (buff) {                                		// Number of timers to process (active timers)
			if (TimerValue[i]) {                        		// Timer active?
				buff--;                                   		// Decrease number of timers to process
				TimerValue[i]--;                          		// Decrease timer value
				if (TimerValue[i]==0) {                   		// Timer run out?
          c = 0;
          while (RunOutTimers[TimerStack][c]) {
            c++;}
          RunOutTimers[TimerStack][c] = i;
          TimerEvents[TimerStack]++;                  // increase the number of pending timer events                     		
					if (!ActiveTimers) {                    		// number of active timers already 0?
						ErrorHandler(9,i,0);}                 		// that's wrong
					else {
						ActiveTimers--;}}}                    		// reduce number of active timers
			i++;}}

	// Solenoids

	if (SolChange) {                                		// is there a solenoid state to be changed?
		REG_PIOC_CODR = AllSelects - Sel5 + AllData;      // clear all select signals and the data bus
		if (SolNumber > 8) {                          		// does the solenoid not belong to the first latch?
			if (SolNumber < 17) {                       		// does it belong to the second latch?
				c = 1;
				i = 8388608;                              		// select second latch
				SolNumber = SolNumber-8;}                 		// and reduce the solenoid number accordingly
			else {                                      		// it belongs to the third latch
				c = 2;
				i = 4194304;                  								// select third latch
				SolNumber = SolNumber-16;}}               		// and match the solenoid number
		else {
			c = 0;
			i = 16777216;}
		SolNumber--;                                  		// latch counts from 0
		if (SolState) {
			SolBuffer[c] = SolBuffer[c] | 1<<SolNumber;}
		else {
			SolBuffer[c] = SolBuffer[c] & (255-(1<<SolNumber));}
		REG_PIOC_SODR = SolBuffer[c]<<1;
		SolChange = false;																// reset flag
		REG_PIOC_SODR = i;}                           		// trigger latch

  REG_PIOA_CODR = 524288;                             // enable latch outputs to send the pattern to display
  REG_PIOC_CODR = AllSelects + AllData;           		// clear all select signals and the data bus

  // Lamps

  if (!APC_settings[LEDsetting]) {
  	if (LampWait == LampPeriod) {                     // Waiting time has passed
  		LampCol++;                                      // prepare for next lamp column
  		LampColMask = LampColMask<<1;
  		c = 2;                                          // clear buffer
  		if (LampCol == 8){                              // max column reached?
  			LampCol = 0;
  			LampColMask = 2;
  			for (i=0; i<8; i++) {                         // write select pattern to buffer
  				if (*(Lamp+i+1)) {
  					REG_PIOC_SODR = c;}
  				c = c<<1;}}
  		else {
  			for (i=0; i<8; i++) {                         // write select pattern to buffer
  				if (*(LampPattern+LampCol*8+i+1)) {
  					REG_PIOC_SODR = c;}
  				c = c<<1;}}
  		REG_PIOC_SODR = 33554432;                       // use Sel1
  		REG_PIOC_CODR = AllSelects + AllData;           // clear all select signals and the data bus
  		REG_PIOC_SODR = LampColMask;
  		LampWait = 1;                                  	// restart lamp waiting counter
  		REG_PIOC_SODR = 268435456;}                     // use Sel0
  	else {
  		if (APC_settings[DimInserts]) {                 // if inserts have to be dimmed
  			REG_PIOC_CODR = AllSelects + AllData;         // clear all select signals and the data bus
  			REG_PIOC_SODR = 268435456;}                   // use Sel0 to disable column driver outputs at half time
  		LampWait++;}}                                   // increase wait counter
  else {                                              // LEDs selected
  	if (LampCol > 19) {                               // 20ms over
  		LampCol = 0;}                                   // start from the beginning
  	if (LampCol < 8) {                                // the first 8 cycles are for transmitting the status of the lamp matrix
  		REG_PIOC_CODR = AllData;
  		c = 2;
  		if (!LampCol){                                  // max column reached?
  			for (i=0; i<8; i++) {                         // write select pattern to buffer
  				if (*(Lamp+i+1)) {
  					REG_PIOC_SODR = c;}
  				c = c<<1;}}
  		else {
  			for (i=0; i<8; i++) {                         // write select pattern to buffer
  				if (*(LampPattern+LampCol*8+i+1)) {
  					REG_PIOC_SODR = c;}
  				c = c<<1;}}
  		if (LEDFlag) {
  			LEDFlag = false;
  			REG_PIOC_CODR = Sel5;}                        // activate Sel5 falling edge
  		else {
  			LEDFlag = true;
  			REG_PIOC_SODR = Sel5;}}                       // activate Sel5 rising edge
  	else {                                            // the lamp matrix is already sent
  		if (LampCol < 13) {                             // still time to send a command?
  			if (LEDCommandBytes) {                        // are there any pending LED commands?
  				REG_PIOC_CODR = AllData;
  				REG_PIOC_SODR = (LEDCommand[LEDCount])<<1;  // send the first byte
  				LEDCount++;                                 // increase the counter
  				if (LEDCount == LEDCommandBytes) {          // not all command bytes sent?
  					LEDCommandBytes = 0;
  					LEDCount = 0;}
  				if (LEDFlag) {
  					LEDFlag = false;
  					REG_PIOC_CODR = Sel5;}                    // activate Sel5 falling edge
  				else {
  					LEDFlag = true;
  					REG_PIOC_SODR = Sel5;}}}
  		else {                                          // this must be LampCol 36
  			if (LampCol == 17) {
  				REG_PIOC_CODR = AllData;
  				REG_PIOC_SODR = 170<<1;                     // time to sync
  				REG_PIOC_SODR = Sel5;
  				if (LEDFlag) {
  					LEDFlag = false;
  					REG_PIOC_CODR = Sel5;}                    // activate Sel5 falling edge
  				else {
  					LEDFlag = true;
  					REG_PIOC_SODR = Sel5;}}}}
  	LampCol++;}
    
	// Sound

  if (g_Sound.next != g_Sound.running) {
  	if (PlayingMusic) {
  		if (PlayingSound) {															// playing music and sound
  			Buffer16b = (uint16_t *) g_Sound.next;				// get address of the next DAC buffer part to be filled
  			for (i=0; i<64; i++) {
  				*Buffer16b = *(MusicBuffer+MusicIRpos*128+2*i);	// write channel 1
  				Buffer16b++;
  				*Buffer16b = *(SoundBuffer+SoundIRpos*128+2*i) | 4096;  // write channel 2
  				Buffer16b++;}
  			g_Sound.next = (uint32_t *) Buffer16b;				// write back the updated address
  			g_Sound.enqueue();
  			MusicIRpos++;																	// increase the IRQ read pointer for the music buffer
  			if (MusicIRpos == 16) {												// last buffer block read?
  				MusicIRpos = 0;}														// start over
  			SoundIRpos++;																	// increase the IRQ read pointer for the sound buffer
  			if (SoundIRpos == 16) {												// last buffer block read?
  				SoundIRpos = 0;}														// start over
  			if (StopMusic) {															// end of music file reached?
  				if (MusicIRpos == MBP) {										// remaining music data played?
  					PlayingMusic = false;											// stop playing music
  					MBP = 0;																	// reset write pointer
  					MusicIRpos = 0;														// reset read pointer
  					StopMusic = 0;														// mark stop sequence as completed
  					if (AfterMusic) {													// anything to do after the music?
  						AfterMusicPending = true;}}}						// indicate now is the time to do so
  			if (StopSound) {															// end of sound file reached?
  				if (SoundIRpos == SBP) {										// remaining sound data played?
  					PlayingSound = false;											// stop playing sound
  					SBP = 0;																	// reset write pointer
  					SoundIRpos = 0;														// reset read pointer
  					StopSound = 0;														// mark stop sequence as completed
  					if (AfterSound) {													// anything to do after the sound?
  						AfterSoundPending = true;}}}}						// indicate now is the time to do so
  		else {																					// playing music only
  			Buffer16b = (uint16_t *) g_Sound.next;				// same as above but music only
  			for (i=0; i<64; i++) {
  				*Buffer16b = *(MusicBuffer+MusicIRpos*128+2*i);
  				Buffer16b++;
  				*Buffer16b = 6144;  												// 2048 | 4096
  				Buffer16b++;}
  			g_Sound.next = (uint32_t *) Buffer16b;
  			g_Sound.enqueue();
  			MusicIRpos++;
  			if (MusicIRpos == 16) {
  				MusicIRpos = 0;}
  			if (StopMusic) {
  				if (MusicIRpos == MBP) {
  					PlayingMusic = false;
  					MBP = 0;
  					MusicIRpos = 0;
  					StopMusic = 0;
  					if (AfterMusic) {
  						AfterMusicPending = true;}}}}}
  	else {																						// not playing music
  		if (PlayingSound) {															// playing sound only
  			Buffer16b = (uint16_t *) g_Sound.next;				// same as above but sound only
  			for (i=0; i<64; i++) {
  				*Buffer16b = 2048;
  				Buffer16b++;
  				*Buffer16b = *(SoundBuffer+SoundIRpos*128+2*i) | 4096;
  				Buffer16b++;}
  			g_Sound.next = (uint32_t *) Buffer16b;
  			g_Sound.enqueue();
  			SoundIRpos++;
  			if (SoundIRpos == 16) {
  				SoundIRpos = 0;}
  			if (StopSound) {
  				if (SoundIRpos == SBP) {
  					PlayingSound = false;
  					SBP = 0;
  					SoundIRpos = 0;
  					StopSound = 0;
  					if (AfterSound) {
  						AfterSoundPending = true;}}}}
  		else {																					// neither sound nor music
  			Buffer32b = g_Sound.next;
  			for (i=0; i<64; i++) {
  				*Buffer32b = 402655232;											// 2048 on both channels and the channel tag
  				Buffer32b++;}
  			g_Sound.next = Buffer32b;
  			g_Sound.enqueue();}}}
}

void loop() {

	c = 0;                                  						// initialize counter
	if (SwEvents[SwitchStack]) {                        // switch event pending?
		SwitchStack = 1-SwitchStack;                      // switch to the other stack to avoid a conflict with the interrupt
		while (SwEvents[1-SwitchStack]) {									// as long as there are switch events to process
			if (ChangedSw[1-SwitchStack][c]) {							// pending switch event found?
				SwEvents[1-SwitchStack]--;										// decrease number of pending events
				i = ChangedSw[1-SwitchStack][c];							// buffer the switch number
				ChangedSw[1-SwitchStack][c] = 0;							// clear the event
				if (Switch[i]) {                              // process SET switches
					Switch_Pressed(i);}													// access the set switch handler
				else {																				// process released switches
					Switch_Released(i);}}												// access the released switch handler
			if (c < 29) {                                   // number of pending events still in the allowed range?
				c++;}																				  // increase counter
			else {
				ErrorHandler(21,0,c);}}}
	c = 0;                                  						// initialize counter
	if (TimerEvents[TimerStack]) {                      // timer event pending?
		TimerStack = 1-TimerStack;                        // switch to the other stack to avoid a conflict with the interrupt
		while (TimerEvents[1-TimerStack]) {               // as long as there are timer events to process
			if (RunOutTimers[1-TimerStack][c]) {            // number of run out timer found?
				TimerEvents[1-TimerStack]--;                  // decrease number of pending events
				i = RunOutTimers[1-TimerStack][c];            // buffer the timer number
				TimerBuffer = TimerEvent[i];                  // Buffer the event for this timer
				if (!TimerBuffer) {                           // TimerEvent must be specified
					ErrorHandler(20,0,c);}
				RunOutTimers[1-TimerStack][c] = 0;            // delete the timer from the list
				TimerEvent[i] = 0;                            // delete the event to show the timer as free
				TimerBuffer(TimerArgument[i]);}               // call event procedure
			c++;}}                                          // increase search counter
	if (SoundPrio) {																		// which channel has to be processed first?
		if (!StopSound && (SBP != SoundIRpos)) {					// still sound data to read?
			ReadSound();}																		// read it
		else {																						// no sound data?
			if (AfterSoundPending) {												// is there an after sound event pending?
				AfterSoundPending = false;										// reset the flag
				if (AfterSound) {															// really?
					AfterSound();}}															// call it
			else {																					// no after sound event
				if (!StopMusic && (MBP != MusicIRpos)) {			// proceed with music
					ReadMusic();}
				else {																				// no music data?
					if (AfterMusicPending) {										// is there an after music event pending?
						AfterMusicPending = false;								// reset the flag
						if (AfterMusic) {													// really?
							AfterMusic();}}}}}}											// call it
	else {																							// same as above but with the priority on music
		if (!StopMusic && (MBP != MusicIRpos)) {
			ReadMusic();}
		else {
			if (AfterMusicPending) {
				AfterMusicPending = false;
				if (AfterMusic) {
					AfterMusic();}}
			else {
				if (!StopSound && (SBP != SoundIRpos)) {
					ReadSound();}
				if (AfterSoundPending) {
					AfterSoundPending = false;
					if (AfterSound) {
						AfterSound();}}}}}}

void ReadMusic() {																		// read music data from SD
if (MusicFile.available() > 255) {										// enough data remaining in file to fill one block?
	MusicFile.read(MusicBuffer+MBP*128,2*128);					// read one block
	MBP++;																							// increase read pointer
	if (MBP == 16) {																		// last block reached?
		MBP = 0;																					// start over
		if (!PlayingMusic) {															// not already playing?
			PlayingMusic = true;														// start playing
			StartMusic = false;}}}													// stop init phase
else {																								// not enough data to fill one block
	byte Avail = MusicFile.available();									// determine how much is left
	MusicFile.read(MusicBuffer+MBP*128,Avail);					// read it
	for (i=0; i<128-Avail/2; i++) {											// fill the rest with silence
		*(MusicBuffer+MBP*128+Avail/2+i) = 2048;}
	MBP++;																							// increase read pointer
	if (MBP == 16) {																		// last block read?
		MBP = 0;}																					// start over
	StopMusic = MBP;																		// mark this block as being the last
	MusicFile.close();}																	// close file
SoundPrio = true;}																		// switch priority to sound

void ReadSound() {																		// same as above but for the sound channel
if (SoundFile.available() > 255) {
	SoundFile.read(SoundBuffer+SBP*128,2*128);
	SBP++;
	if (SBP == 16) {
		SBP = 0;
		if (!PlayingSound) {
			PlayingSound = true;
			StartSound = false;}}}
else {
	byte Avail = SoundFile.available();
	SoundFile.read(SoundBuffer+SBP*128,Avail);
	for (i=0; i<128-Avail/2; i++) {
		*(SoundBuffer+SBP*128+Avail/2+i) = 2048;}
	SBP++;
	if (SBP == 16) {
		SBP = 0;}
	StopSound = SBP;
	SoundFile.close();}
SoundPrio = false;}

void SwitchPressed(int SwNumber) {
  Serial.print(" Switch pressed ");
  Serial.println(SwNumber); }
  
void SwitchReleased(int SwNumber) {
  Serial.print(" Switch released ");
  Serial.println(SwNumber); }

void DummyProcess(byte Dummy) {
    UNUSED(Dummy);
}

byte ActivateTimer(unsigned int Value, byte Argument, void (*EventPointer)(byte)) {
  byte i = 1;                                     		// reset counter
  BlockTimers = true;																	// block IRQ timer handling to avoid interference
  while (TimerEvent[i]) {             		            // search for a free timer
    i++;}
  TimerArgument[i] = Argument;                    		// initialize it
  TimerEvent[i] = EventPointer;
  TimerValue[i] = Value;                          
  ActiveTimers++;                                 		// increase the number of active timers
  BlockTimers = false;																// release the IRQ block
  return i;}                                      		// and return its number

void KillAllTimers() {
  BlockTimers = true;                                 // block IRQ timer handling to avoid interference
  ActiveTimers = 0;
  TimerStack = 0;
  for (i=1; i<64; i++) {                          		// check all 64 timers
    TimerValue[i] = 0;
    TimerArgument[i] = 0;
    TimerEvent[i] = 0;}
  for (byte x=0; x<2; x++) {
    TimerEvents[x] = 0;
    for (i=0; i<30; i++) {
      RunOutTimers[x][i] = 0;}}
  BlockTimers = false;}																// release the IRQ block

void KillTimer(byte TimerNo) {
	bool FoundFlag = false;
  byte ToFind = 0;
  byte c = 0;
	BlockTimers = true;                                 // block IRQ timer handling to avoid interference
  for (byte x=0; x<2; x++) {                          // for both timer event stacks
    ToFind = TimerEvents[x];                          // determine the number of pending events in the current stack
    while (ToFind) {                                  // search for all of these
      if (RunOutTimers[x][c]) {                       // run out timer found
        ToFind--;                                     // reduce the goal
        if (RunOutTimers[x][c] == TimerNo) {          // is it the timer to be killed?
          FoundFlag = true;                           // indicate it
          RunOutTimers[x][c] = 0;                     // remove it from the list
          TimerEvents[x]--;                           // reduce the number of pending events for this stack
          ToFind = 0;}}                               // stop searching
      c++;}}      
	if (!FoundFlag)  {                                  // no pending timer event?
		if (TimerValue[TimerNo]) {                        // timer still active?
			if (!ActiveTimers) {                        		// number of active timers already 0?
				ErrorHandler(10,TimerNo,ActiveTimers);}   		// that's wrong
			else {
				ActiveTimers--;}}                         		// reduce number of active timers
		else {
			ErrorHandler(11,TimerNo,(uint) TimerEvent[TimerNo]);}}
	TimerValue[TimerNo] = 0;                        		// set counter to 0
	TimerEvent[TimerNo] = 0;														// clear Timer Event
	BlockTimers = false;}																// release the IRQ block

void WriteUpper(const char* DisplayText) {              		// dont use columns 0 and 8 as they are reserved for the credit display
  for (i=0; i<7; i++) { 
    *(DisplayUpper+2+2*i) = DispPattern1[(int)((*(DisplayText+i)-32)*2)];
    *(DisplayUpper+2+2*i+1) = DispPattern1[(int)((*(DisplayText+i)-32)*2)+1];
    *(DisplayUpper+18+2*i) = DispPattern1[(int)((*(DisplayText+7+i)-32)*2)];
    *(DisplayUpper+18+2*i+1) = DispPattern1[(int)((*(DisplayText+7+i)-32)*2)+1];}}

void WriteLower(const char* DisplayText) {
  for (i=0; i<7; i++) { 
    *(DisplayLower+2+2*i) = DispPattern2[(int)((*(DisplayText+i)-32)*2)];
    *(DisplayLower+2+2*i+1) = DispPattern2[(int)((*(DisplayText+i)-32)*2)+1];
    *(DisplayLower+18+2*i) = DispPattern2[(int)((*(DisplayText+7+i)-32)*2)];
    *(DisplayLower+18+2*i+1) = DispPattern2[(int)((*(DisplayText+7+i)-32)*2)+1];}}

void WriteUpper2(const char* DisplayText) {
  for (i=0; i<7; i++) { 
    *(DisplayUpper2+2+2*i) = DispPattern1[(int)((*(DisplayText+i)-32)*2)];
    *(DisplayUpper2+2+2*i+1) = DispPattern1[(int)((*(DisplayText+i)-32)*2)+1];
    *(DisplayUpper2+18+2*i) = DispPattern1[(int)((*(DisplayText+7+i)-32)*2)];
    *(DisplayUpper2+18+2*i+1) = DispPattern1[(int)((*(DisplayText+7+i)-32)*2)+1];}}

void WriteLower2(const char* DisplayText) {
  for (i=0; i<7; i++) { 
    *(DisplayLower2+2+2*i) = DispPattern2[(int)((*(DisplayText+i)-32)*2)];
    *(DisplayLower2+2+2*i+1) = DispPattern2[(int)((*(DisplayText+i)-32)*2)+1];
    *(DisplayLower2+18+2*i) = DispPattern2[(int)((*(DisplayText+7+i)-32)*2)];
    *(DisplayLower2+18+2*i+1) = DispPattern2[(int)((*(DisplayText+7+i)-32)*2)+1];}}

void ScrollUpper(byte Step) {													// call with Step = 1 and the text being in DisplayUpper2
	for (i=2; i<14; i++) {															// scroll display 1
		DisplayUpper[i] = DisplayUpper[i+2];}
	DisplayUpper[14] = DisplayUpper[18];								// add put the leftmost char of the display 2 to the end
	DisplayUpper[15] = DisplayUpper[19];
	for (i=18; i<30; i++) {                         		// scroll display 2
			DisplayUpper[i] = DisplayUpper[i+2];}
	DisplayUpper[30] = DisplayUpper2[2*Step];						// add the corresponding char of DisplayUpper2
	DisplayUpper[31] = DisplayUpper2[2*Step+1];
	Step++;																							// increase step
  if (Step == 8) {																		// skip position 9 (belongs to the credit display
    Step++;}
	if (Step < 16) {																		// if its not already over
		ActivateTimer(50, Step, ScrollUpper);}}						// come back

void AddScrollUpper(byte Step) {											// call with Step = 1 and the text being in DisplayUpper2
	if (Step > 8) {
		for (i=0; i<2*(Step-9);i++) {
			DisplayUpper[32-(2*Step)+i] = DisplayUpper[34-(2*Step)+i];}
		DisplayUpper[14] = DisplayUpper[18];
		DisplayUpper[15] = DisplayUpper[19];
		for (i=0; i<12; i++) {
			DisplayUpper[18+i] = DisplayUpper[20+i];}}
	else {
		for (i=0; i<2*(Step-1); i++) {
			DisplayUpper[32-(2*Step)+i] = DisplayUpper[34-(2*Step)+i];}}
	DisplayUpper[30] = DisplayUpper2[2*Step];
	DisplayUpper[31] = DisplayUpper2[2*Step+1];
	Step++;
  if (Step == 8) {																		// skip position 9 (belongs to the credit display
    Step++;}
  if (!DisplayUpper[32-(2*Step)] && !DisplayUpper[33-(2*Step)] && Step < 14) {
  	ActivateTimer(50, Step, AddScrollUpper);}}

void ScrollLower(byte Step) {													// call with Step = 1 and the text being in DisplayLower2
	if (Step < 8) {																			// do display 3 first
		for (i=2; i<14; i++) {														// scroll display 3
			DisplayLower[i] = DisplayLower[i+2];}
		DisplayLower[14] = DisplayLower2[2*Step];					// add the corresponding char of DisplayLower2
		DisplayLower[15] = DisplayLower2[2*Step+1];}
	else {																							// do display 4
		for (i=18; i<30; i++) {														// scroll display 4
      DisplayLower[i] = DisplayLower[i+2];}
		DisplayLower[30] = DisplayLower2[2*Step+2];				// add the corresponding char of DisplayLower2
		DisplayLower[31] = DisplayLower2[2*Step+3];}
  Step++;
  if (Step < 15) {																		// if its not already over
    ActivateTimer(50, Step, ScrollLower);}}						// come back

void ScrollLower2(byte Step) {													// call with Step = 1 and the text being in DisplayUpper2
	for (i=2; i<14; i++) {															// scroll display 1
		DisplayLower[i] = DisplayLower[i+2];}
	DisplayLower[14] = DisplayLower[18];								// add put the leftmost char of the display 2 to the end
	DisplayLower[15] = DisplayLower[19];
	for (i=18; i<30; i++) {                         		// scroll display 2
			DisplayLower[i] = DisplayLower[i+2];}
	DisplayLower[30] = DisplayLower2[2*Step];						// add the corresponding char of DisplayLower2
	DisplayLower[31] = DisplayLower2[2*Step+1];
	Step++;																							// increase step
  if (Step == 8) {																		// skip position 9 (belongs to the credit display
    Step++;}
	if (Step < 16) {																		// if its not already over
		ActivateTimer(50, Step, ScrollLower2);}}					// come back

void ShowMessage(byte Seconds) {                      // switch to the second display buffer for Seconds
  if (Seconds) {                                      // time <> 0?
    if (ShowMessageTimer) {                           // timer already running?
      KillTimer(ShowMessageTimer);}                   // kill it
    SwitchDisplay(0);                                 // switch to DispUpper2 and DispLower2
    ShowMessageTimer =  ActivateTimer(Seconds*1000, 0, ShowMessage);} // and start timer to come back
  else {                                              // no time specified means the routine called itself
    ShowMessageTimer = 0;                             // indicate that timer is not running any more
    SwitchDisplay(1);}}                               // switch back to DispRow1
    
void ConvertToBCD(int Number) {
  ByteBuffer = Number % 10;
  ByteBuffer2 = (Number-ByteBuffer)/10;}

void ActivateSolenoid(unsigned int Duration, byte Solenoid) {
  if (!SolChange) {                               		// change request for another solenoid pending?
    if (!Duration) {
      Duration = *(GameDefinition.SolTimes+Solenoid-1);} // if no duration is specified use the solenoid specific default
    SolNumber = Solenoid;                         		// activate solenoid
    SolState = true;
    if (Duration) {                               		// duration = 0 means solenoid is permanently on
      ActivateTimer(Duration, Solenoid, ReleaseSolenoid);} // otherwise use a timer to turn it off again
    SolChange = true;}
  else {                                        			// if a change request is already pending
    i = 0;
    while (SolDelayed[i]) {                     			// look for a free slot in the list of solenoids to be processed later
      i++;}
    SolDelayed[i] = Solenoid;                   			// insert the solenoid number
    DurDelayed[i] = Duration;                   			// and its duration into the list
    ActivateTimer(25, Solenoid, ActivateLater);}}			// and try again later

void ActivateLater(byte Solenoid) {               		// handle delayed solenoid change requests
  byte i = 0;
  unsigned int Duration;
  while (SolDelayed[i] != Solenoid) {             		// search the list of delayed solenoid requests
    i++;}
  SolDelayed[i] = 0;                              		// remove its entry
  Duration = DurDelayed[i];                       		// get the duration
  ActivateSolenoid(Duration, Solenoid);}         	 		// and try again to activate it

void DelaySolenoid(byte Solenoid) {                   // activate solenoid after delay time
  ActivateSolenoid(0, Solenoid);}

void ReleaseAllSolenoids() {
	for (i=0; i< 3; i++) {															// clear all solenoid buffers
		SolBuffer[i] = 0;}
	ReleaseSolenoid(1);																	// release one solenoid of each buffer
	ReleaseSolenoid(9);
	ReleaseSolenoid(17);}

void ReleaseSolenoid(byte Solenoid) {
  if (!SolChange) {                               		// change request for another solenoid pending?
    SolNumber = Solenoid;                         		// if not process it
    SolState = false;
    SolChange = true;}
  else {                                          		// if yes
    ActivateTimer(1, Solenoid, ReleaseSolenoid);}}		// try again later

bool SolenoidStatus(byte Solenoid) {                  // determine the current state of a solenoid
  bool State = SolBuffer[Solenoid / 8] & (1<<(Solenoid % 8));
  return State;}

void ActA_BankSol(unsigned int Duration, byte Solenoid) {
	if (!C_BankActive) {
		ActivateSolenoid(Duration, Solenoid);}
	else {
		i = 0;
		while (A_BankWaiting[i]) {                     		// look for a free slot in the list of solenoids to be processed later
			i++;}
		if (i > 9) {																			// list full?
			ErrorHandler(20,0,Solenoid);}         					// show error 20
		A_BankWaitDuration[i] = Duration;									// store duration value
		A_BankWaiting[i] = Solenoid;                   		// insert the solenoid number
		A_BankWaitingNo++;}}															// increase number of waiting coils

void ActA_BankLater(byte Dummy) {
  UNUSED(Dummy);

	if (!C_BankActive) {																// C bank still in use?
		i = 0;
		while (!A_BankWaiting[i]) {                     	// look for a used slot in the list of solenoids to be processed later
			i++;}
		if (i > 9) {																			// list empty?
			ErrorHandler(21,0,0);}         									// show error 20
		ActivateSolenoid(A_BankWaitDuration[i], A_BankWaiting[i]);
		A_BankWaiting[i] = 0;															// delete list entry
		A_BankWaitingNo--;																// reduce number of entries
		if (A_BankWaitingNo)	{														// is there another entry?
			ActivateTimer(25, 0, ActA_BankLater);}}					// then come back
	else {
		ActivateTimer(500, 0, ActA_BankLater);}}

void ActC_BankSol(byte Solenoid) {
	ActivateSolenoid(0, 14);
	C_BankActive = true;
	ActivateTimer(50, Solenoid, ActC_BankSol2);
	ActivateTimer(*(GameDefinition.SolTimes+Solenoid+23)+100, 0, PB_ResetAC_Relay);}

void ActC_BankSol2(byte Solenoid) {
	ActivateSolenoid(*(GameDefinition.SolTimes+Solenoid+23), Solenoid);}

void PB_ResetAC_Relay(byte Dummy) {
  UNUSED(Dummy);

	C_BankActive = false;
	ReleaseSolenoid(14);
  if (A_BankWaitingNo) {
	  ActivateTimer(100, 0, ActA_BankLater);}}

void ShowPoints(byte Player) {                    		// display the points of the selected player
  DisplayScore(Player, Points[Player]);}

void BlinkScore(byte Event) {
  switch (Player) {
    case 1:                                           // for the players 1 and 3
    case 3:
      ByteBuffer = 2;                                 // start in column 1
      break;
    case 2:                                           // for the players 2 and 4
    case 4:
      ByteBuffer = 18;                                // start in column 9
      break;}
  if (Event) {
    if (Player < 3) {
      for (i=0; i<14; i++) {
        *(DisplayUpper+ByteBuffer+i) = 0;}}
    else {
      for (i=0; i<14; i++) {
        *(DisplayLower+ByteBuffer+i) = 0;}}
    BlinkScoreTimer = ActivateTimer(300, 0, BlinkScore);}
  else {
    ShowPoints(Player);
    BlinkScoreTimer = ActivateTimer(2000, 1, BlinkScore);}}
    
void DisplayScore (byte Position, unsigned int Score) {
	byte i=0;                                       		// use a private counter
	switch (Position) {
	case 1:                                       			// for the players 1 and 3
	case 3:
		ByteBuffer = 2;                             			// start in column 1
		break;
	case 2:                                       			// for the players 2 and 4
	case 4:
		ByteBuffer = 18;                            			// start in column 9
		break;}
	if (Score) {                                  			// if the score is not 0
		while (Score && i<7) {                      			// for all 7 display digits
			ByteBuffer2 = Score % 10;                 			// extract the least significant digit
			Score = (Score-ByteBuffer2) / 10;         			// prepare for the next digit
			ByteBuffer2 = 32+2*ByteBuffer2;           			// determine the corresponding display pattern
			if (Position < 3) {                       			// depending on the player number show it in the upper display row
				if ((i==3) || (i==6)) {
					*(DisplayUpper+ByteBuffer+12-2*i) = 128 | DispPattern1[ByteBuffer2];
					*(DisplayUpper+ByteBuffer+13-2*i) = 64 | DispPattern1[ByteBuffer2+1];} // add a comma if necessary
				else {
					*(DisplayUpper+ByteBuffer+12-2*i) = DispPattern1[ByteBuffer2];
					*(DisplayUpper+ByteBuffer+13-2*i) = DispPattern1[ByteBuffer2+1];}}
			else {                                    			// the same for the lower display row
				if ((i==3) || (i==6)) {
					*(DisplayLower+ByteBuffer+12-2*i) = 1 | DispPattern2[ByteBuffer2];
					*(DisplayLower+ByteBuffer+13-2*i) = 8 | DispPattern2[ByteBuffer2+1];}
				else {
					*(DisplayLower+ByteBuffer+12-2*i) = DispPattern2[ByteBuffer2];
					*(DisplayLower+ByteBuffer+13-2*i) = DispPattern2[ByteBuffer2+1];}} //
			i++;}}
	else {                                        			// if the points are 0
		if (Position < 3) {
			*(DisplayUpper+ByteBuffer+12) = DispPattern1[32];        // just show two 0s
			*(DisplayUpper+ByteBuffer+13) = DispPattern1[33];
			*(DisplayUpper+ByteBuffer+10) = DispPattern1[32];
			*(DisplayUpper+ByteBuffer+11) = DispPattern1[33];}
		else {
			*(DisplayLower+ByteBuffer+12) = DispPattern2[32];        // just show two 0s
			*(DisplayLower+ByteBuffer+13) = DispPattern2[33];
			*(DisplayLower+ByteBuffer+10) = DispPattern2[32];
			*(DisplayLower+ByteBuffer+11) = DispPattern2[33];}}}

void ShowNumber(byte Position, unsigned int Number) {
	byte Buffer = 0;
	byte i = 0;
	if (Number) {
		while (Number && i<7) {
			Buffer = Number % 10;
			Number = (Number-Buffer)/10;
			Buffer = 32+2*Buffer;
			if (Position < 16) {
				if ((i==3) || (i==6)) {
					*(DisplayUpper2+2*(Position-i)) = 128 | DispPattern1[Buffer];
					*(DisplayUpper2+1+2*(Position-i)) = 64 | DispPattern1[Buffer+1];}
				else {
					*(DisplayUpper2+2*(Position-i)) = DispPattern1[Buffer];
				  *(DisplayUpper2+2*(Position-i)+1) = DispPattern1[Buffer+1];}}
			else {
				if ((i==3) || (i==6)) {
					*(DisplayLower2+2*(Position-16-i)) = 1 | DispPattern2[Buffer];
					*(DisplayLower2+1+2*(Position-16-i)) = 8 | DispPattern2[Buffer+1];}
				else {
					*(DisplayLower2+2*(Position-16-i)) = DispPattern2[Buffer];
				  *(DisplayLower2+2*(Position-16-i)+1) = DispPattern2[Buffer+1];}}
			i++;}}
	else {
		if (Position < 14) {
			*(DisplayUpper2+2*Position) = DispPattern1[32];    // just show 0
			*(DisplayUpper2+2*Position+1) = DispPattern1[33];}
		else {
			*(DisplayLower2+2*Position-14) = DispPattern2[32]; // just show 0
			*(DisplayLower2+2*Position-13) = DispPattern2[33];}}}

void ShowAllPoints(byte Dummy) {                  		// just a dummy event to access it via timer
  UNUSED(Dummy);

  WriteUpper("              ");                   		// erase display
  WriteLower("              ");
  for (i=1; i<=NoPlayers; i++) {                  		// display the points of all active players
    ShowPoints(i);}}

void SwitchDisplay(byte Event) {                      // switch between different display buffers
  if (Event == 1) {
    DispRow1 = DisplayUpper;
    DispRow2 = DisplayLower;}
  else {
    DispRow1 = DisplayUpper2;
    DispRow2 = DisplayLower2;}}

void BlinkLamps(byte BlTimer) {
  ByteBuffer = 0;
  for (i=0; i<BlinkingNo[BlTimer]; i++) {             // for all lamps controlled by this blink timer
    while (!BlinkingLamps[BlTimer][ByteBuffer]) {     // skip unused lamp slots
      ByteBuffer++;
      if (ByteBuffer > 65) {                          // max 64 blink timers possible (starting from 1)
        ErrorHandler(3,BlTimer,BlinkingNo[BlTimer]);}}// show error 3
    Lamp[BlinkingLamps[BlTimer][ByteBuffer]] = BlinkState[BlTimer]; // toggle the state of the lamps
    ByteBuffer++;} 
  BlinkState[BlTimer] = !BlinkState[BlTimer];         // invert the target state for the next run
  BlinkTimer[BlTimer] = ActivateTimer(BlinkPeriod[BlTimer], BlTimer, BlinkLamps);} // and start a new timer

void AddBlinkLamp(byte Lamp, unsigned int Period) {
  RemoveBlinkLamp(Lamp);                              // if the lamp is already blinking then stop it
  if (Period) {																				// Only if the Period is not 0
		byte a = 0;
		byte x = 0;
		byte b = 0;
		bool Flag = false;
		while (a < BlinkTimers) {                         // search all active blink timers
			if (BlinkTimer[x]) {                            // timer active?
				a++;                                          // increase the number of active blink timers found
				if (BlinkPeriod[x] == Period) {               // does the period of the found timer match to the desired period?
					b = x;                                      // store the timer number
					Flag = true;                                // indicate that a match has been found
					break;}}                                    // and end the search
			x++;                                            // or continue searching
			if (x > 65) {                                   // max 64 blink timers possible (starting from 1)
				ErrorHandler(4,BlinkTimer[x],Lamp);}}         // show error 4
		x = 0;
		if (Flag) {                                       // has a timer with the same period been found?
			while (BlinkingLamps[b][x]) {                   // search for a free lamp slot
				x++;
				if (x > 65) {                                 // max 64 blink timers possible (starting from 1)
					ErrorHandler(5,BlinkingLamps[b][x],Lamp);}} // show error 5
			BlinkingLamps[b][x] = Lamp;                     // add the lamp to blink
			BlinkingNo[b]++;}                               // and increase the number of lamps to be controlled by this timer
		else {                                            // if no matching timer has been found
			while (BlinkTimer[x]) {                         // look for a free timer slot
				x++;
				if (x > 65) {                                 // max 64 blink timers possible (starting from 1)
					ErrorHandler(6,0,Lamp);}}                   // show error 6
			BlinkingLamps[x][0] = Lamp;                     // add the lamp
			BlinkingNo[x] = 1;                              // set the number of lamps for this timer to 1
			BlinkState[x] = true;                           // start with lamps on
			BlinkPeriod[x] = Period;
			BlinkTimers++;                                  // increase the number of blink timers
			BlinkTimer[x] = ActivateTimer(Period, x, BlinkLamps);}}} // start a timer and store it's number

void RemoveBlinkLamp(byte LampNo) {                   // stop the lamp from blinking
  byte a = 0;
  byte b = 0;
  byte x = 0;
  byte y = 0;
  while (a < BlinkTimers) {                           // search all active blink timers
    b = 0;
    y = 0;
    if (BlinkTimer[x]) {                              // blink timer in use found?
      a++;                                            // increase the number of active blink timers found
      while (b < BlinkingNo[x]){                      // check the list of lamps controlled by this timer
        if (BlinkingLamps[x][y]) {                    // active lamp found?
          b++;                                        // increase the number of active lamps found for this blink timer
          if (BlinkingLamps[x][y] == LampNo) {        // is it the lamp to be removed?
            BlinkingLamps[x][y] = 0;                  // delete it from the list
            Lamp[LampNo] = false;                     // turn it off
            BlinkingNo[x]--;                          // decrease the number of lamps controlled by this timer
            if (!BlinkingNo[x]) {                     // = 0?
              KillTimer(BlinkTimer[x]);               // kill the timer
              BlinkTimer[x] = 0;                      // delete it from the list
              BlinkTimers--;}                         // decrease the number of blink timers
            break;}}                                  // and end the search        
        y++;                                          // increase the counter for the list of this blink timer
        if (y > 65) {                                 // max 64 lamps existing (starting from 1)
          ErrorHandler(7,BlinkTimer[x],LampNo);}}}    // show error 7
    x++;                                              // increase the counter for the list of active blink timers
    if (x > 65) {                                     // max 64 blink timers possible (starting from 1)
      ErrorHandler(8,BlinkTimer[x],LampNo);}}}        // show error 8
     
void ErrorHandler(unsigned int Error, unsigned int Number2, unsigned int Number3) {
  
  WriteUpper("ERROR         ");                       // Show Error Message
  WriteLower("              ");
  LampPattern = NoLamps;                              // Turn off all lamps
  Serial.print("Error = ");
  Serial.println(Error);
  Serial.print("Number2 = ");
  Serial.println(Number2);
  Serial.print("Number3 = ");
  Serial.println(Number3);  
  //ReleaseAllSolenoids();
  //KillAllTimers();
  DisplayScore(2, Error);
  DisplayScore(3, Number2);
  DisplayScore(4, Number3);
  while (1) {}}

void ShowLampPatterns(byte Step) {                    // shows a series of lamp patterns - start with step being zero
  IntBuffer = (PatPointer+Step)->Duration;            // buffer the duration for the current pattern
  if (StrobeLightsTimer) {
    LampBuffer = ((PatPointer+Step)->Pattern)-8;}     // show the pattern
  else {
    LampPattern = ((PatPointer+Step)->Pattern)-8;}    // show the pattern
  Step++;                                             // increase the pattern number
  if (!((PatPointer+Step)->Duration)) {               // if the duration for the next pattern is 0
    Step = 0;                                         // reset the pattern
    FlowRepeat--;                                     // decrease the number of repetitions
    if (!FlowRepeat) {                                // if no more repetitions pending
    	ByteBuffer3 = 0;																// indicate that the process has stopped
      if (LampReturn) {                               // is a return pointer given?
        LampReturn(0);}                               // call the procedure
      return;}}                                       // otherwise just quit
  ByteBuffer3 = ActivateTimer(IntBuffer, Step, ShowLampPatterns);} // come back after the given duration

void PlayFlashSequence(byte* Sequence) {              // prepare for playing a flasher sequence
  ActivateSolenoid(0, 14);                            // switch A/C relay to C
  C_BankActive = true;                                // indicate that the C bank is active
  FlashSequence = Sequence;                           // set the FlashSequence pointer to the current sequence
  ActivateTimer(50, 0, FlSeqPlayer);}                 // start the sequence in 50ms when the A/C relay had some time to switch

void FlSeqPlayer(byte Step) {                         // play flasher sequence
  if (FlashSequence[Step]) {                          // if the next list entry is not zero
    byte Duration;
    if (FlashSequence[Step+1] < 9) {
      Duration = *(GameDefinition.SolTimes+FlashSequence[Step+1]+23);}
    else {
      Duration = *(GameDefinition.SolTimes+FlashSequence[Step+1]);}
    ActivateSolenoid(Duration, FlashSequence[Step+1]); // Activate the flasher with its standard activate time
    ActivateTimer(FlashSequence[Step]*10, Step+2, FlSeqPlayer);} // increase the step counter and play the next list entry in duration*10ms
  else {                                              // if the next list entry is zero
    FlashSequence = 0;                                // set the list pointer to zero
    ReleaseSolenoid(14);                              // release the A/C relay
    C_BankActive = false;                             // and indicate the C bank as not active
  if (A_BankWaitingNo) {                              // any pending A bank solenoids?
    ActivateTimer(100, 0, ActA_BankLater);}}}         // handle them when the relay has switched back to the A bank
    
void StrobeLights(byte State) {
  if (State) {
    LampPattern = LampBuffer;                         // show the pattern
    State = 0;}
  else {
    LampPattern = NoLamps;
    State = 1;}
  StrobeLightsTimer = ActivateTimer(30, State, StrobeLights);}

void PlayMusic(byte Priority, const char* Filename) {
	if (!StartMusic) {
		if (!PlayingMusic) {															// no music in play at the moment?
			MusicFile = SD.open(Filename);									// open file
			if (!MusicFile) {
				Serial.println("error opening file");
				while (true);}
			if (Priority > 99) {
				MusicPriority = Priority -100;}
			else {
				MusicPriority = Priority;}
			MusicFile.read(MusicBuffer, 2*128);							// read first block
			StartMusic = true;															// indicate the startup phase
			MBP++;}																					// increase read pointer
		else {																						// music already playing
			if (Priority > 99) {														// Priority > 99 means new prio has to be larger (not equal) to play
				Priority = Priority - 100;
				if (Priority > MusicPriority) {
					MusicPriority = Priority;
					MusicFile.close();													// close the old file
					MusicFile = SD.open(Filename);							// open the new one
					if (!MusicFile) {
						Serial.println("error opening file");
						while (true);}
					if (!PlayingMusic) {												// neglect old data if still in the startup phase
						MBP = 0;}}}
			else {
				if (Priority >= MusicPriority) {
					MusicPriority = Priority;
					MusicFile.close();													// close the old file
					MusicFile = SD.open(Filename);							// open the new one
					if (!MusicFile) {
						Serial.println("error opening file");
						while (true);}
					if (!PlayingMusic) {												// neglect old data if still in the startup phase
						MBP = 0;}}}}}}

void StopPlayingMusic() {
	if (StartMusic || PlayingMusic) {
		MusicFile.close();
		StopMusic = MBP;}}

void PlayRandomMusic(byte Priority, byte Amount, char* List) {
	Amount = random(Amount);
	PlayMusic(Priority, List+Amount*12);}

void PlayNextMusic() {
	PlayMusic(50, NextMusicName);}

void PlaySound(byte Priority, const char* Filename) {
	if (!StartSound) {
		if (!PlayingSound) {															// no sound in play at the moment?
			SoundFile = SD.open(Filename);									// open file
			if (!SoundFile) {
				Serial.println("error opening file");
				while (true);}
			if (Priority > 99) {
				SoundPriority = Priority -100;}
			else {
				SoundPriority = Priority;}
			SoundFile.read(SoundBuffer, 2*128);							// read first block
			StartSound = true;															// indicate the startup phase
			SBP++;}																					// increase read pointer
		else {																						// music already playing
			if (Priority > 99) {														// Priority > 99 means new prio has to be larger (not equal) to play
				Priority = Priority - 100;
				if (Priority > SoundPriority) {
					SoundPriority = Priority;
					SoundFile.close();													// close the old file
					SoundFile = SD.open(Filename);							// open the new one
					if (!SoundFile) {
						Serial.println("error opening file");
						while (true);}
					if (!PlayingSound) {												// neglect old data if still in the startup phase
						SBP = 0;}}}
			else {
				if (Priority >= SoundPriority) {
					SoundPriority = Priority;
					SoundFile.close();													// close the old file
					SoundFile = SD.open(Filename);							// open the new one
					if (!SoundFile) {
						Serial.println("error opening file");
						while (true);}
					if (!PlayingSound) {												// neglect old data if still in the startup phase
						SBP = 0;}}}}}}

void StopPlayingSound() {
	if (StartSound || PlayingSound) {
		SoundFile.close();
		StopSound = SBP;}}

void PlayRandomSound(byte Priority, byte Amount, char* List) {
	Amount = random(Amount);
	PlaySound(Priority, List+Amount*12);}

void PlayNextSound() {
	PlaySound(50, NextSoundName);}

void Settings_Enter() {
  WriteUpper("   SETTINGS   ");                     	// Show Test Mode
  WriteLower("              ");
  LampPattern = NoLamps;                              // Turn off all lamps
  AppByte = 0;
  AppByte2 = 0;
  Switch_Pressed = SelectSettings;
  ActivateTimer(2000, 0, SelectSettings);}         		// Wait 1 second and proceed to Display Test

void SelectSettings(byte Switch) {										// select system or game settings
	switch (Switch) {
		case 0:
		  WriteUpper(" SYSTEMSETTNGS");
		  WriteLower("              ");
		  break;
		case 3:
			if (AppByte) {
				SettingsPointer = game_settings;
				SettingsList = GameDefinition.GameSettingsList;
				SettingsFileName = GameDefinition.GameSettingsFileName;
				AppByte = 0;}
			else {
				SettingsPointer = APC_settings;
				SettingsList = APC_setList;
				SettingsFileName = (char*)APC_set_file_name;}
			Switch_Pressed = SelSetting;
			SelSetting(0);
			break;
		case 72:
			if (AppByte) {
			  WriteUpper(" SYSTEMSETTNGS");
			  WriteLower("              ");
			  AppByte = 0;}
			else {
			  WriteUpper("  GAME SETTNGS");
			  WriteLower("              ");
			  AppByte = 1;}}}

void RepeatSelectKey(byte Run) {											// Repeat the start button (No 3) if it is pressed permanently
	SelSetting(3);
	if (Run <10) {																			// increase the repeat frequency after some runs
    Run++;
    if (Run < 4) {
      SettingsRepeatTimer = ActivateTimer(500, Run, RepeatSelectKey);}
    else {
      SettingsRepeatTimer = ActivateTimer(100, Run, RepeatSelectKey);}}
	else {
		SettingsRepeatTimer = ActivateTimer(50, Run, RepeatSelectKey);}}

void StopRepeatSelectKey(byte Switch) {								// stop repeating the start button if it has been released
	if (Switch ==3) {
		if (SettingsRepeatTimer) {
			KillTimer(SettingsRepeatTimer);
			SettingsRepeatTimer = 0;}
		Switch_Released = DummyProcess;}}

void SelSetting(byte Switch) {												// Switch mode of the settings
	switch (Switch) {
		case 72:																					// Advance button pressed
			StopPlayingMusic();
			if (!digitalRead(UpDown)) {											// go forward or backward depending on UpDown switch
				AppByte++;																		// show next setting
				if (!SettingsList[AppByte].EventPointer) {		// end marker of settings list reached?
					AppByte = 0;}}															// switch to the first
			else {
        if (!AppByte) {																// already showing setting 0?
				while (SettingsList[AppByte].EventPointer) {	// search for the end marker of the settings list
					AppByte++;}}
				AppByte--;}																		// and go one back to reach the last settings entry
				/* no break */
		case 0:																						// show the current setting
		  ConvertToBCD(AppByte+1);                        // convert the actual settings number +1
		  *(DisplayUpper) = LeftCredit[32 + 2 * ByteBuffer2]; // and show it
		  *(DisplayUpper+16) = LeftCredit[32 + 2 * ByteBuffer];
			WriteUpper( SettingsList[AppByte].Text);				// show the text
			SettingsList[AppByte].EventPointer(false);			// call the corresponding method and indicate no changes
			break;
		case 3:
      if (!SettingsRepeatTimer) {											// start timer to determine whether key 3 is pressed longer than 1s
        SettingsRepeatTimer = ActivateTimer(1000, 0, RepeatSelectKey);
        Switch_Released = StopRepeatSelectKey;}
			SettingsList[AppByte].EventPointer(true);}}			// call the corresponding method and indicate changes

void HandleBoolSetting(bool change) {									// handling method for boolean settings
	if (change) {																				// if the setting has been changed
		AppByte2 = 1;																			// set the change indicator
		if (SettingsPointer[AppByte]) {										// invert the current setting
			SettingsPointer[AppByte] = 0;}
		else {
			SettingsPointer[AppByte] = 1;}}
	if (SettingsPointer[AppByte]) {											// show the current state of the setting
		WriteLower("           YES");}
	else {
		WriteLower("            NO");}}

void RestoreDefaults(bool change) {										// restore the default settings
	if (change) {																				// if the start button has been pressed
		AppByte2 = 1;																			// set the change indicator
		if (SettingsPointer == APC_settings) {
			for(i=0;i<64;i++) {
				*(SettingsPointer+i) = APC_defaults[i];}}
		else {
			for(i=0;i<64;i++) {															// and change all settings to their default values
				*(SettingsPointer+i) = *(GameDefinition.GameDefaultsPointer+i);}}
    WriteLower("SETTNGS DONE  ");}
	else {
		WriteLower("SETTNGS       ");}}

void ExitSettings(bool change) {											// exit settings and save them if necessary
	if (SettingsRepeatTimer) {													// kill key repeat timer for button 3
		KillTimer(SettingsRepeatTimer);
		SettingsRepeatTimer = 0;}
  WriteLower("              ");
	if (change) {																				// if the start button has been pressed
		if (AppByte2) {																		// and the change indicator has been set
	    if (SDfound) {																	// check for the SD card
	      SD.remove(SettingsFileName);									// remove the old settings file
	      File HighScore = SD.open(SettingsFileName,FILE_WRITE);  // open the settings file on the SD card
	      HighScore.write((byte*) SettingsPointer, sizeof game_settings); // and write the settings array
	      HighScore.close();
	      WriteUpper("SETTNGS SAVED ");}
	    else {
	      WriteUpper("  NO   SD CARD");}}
	  else {																						// change indicator has not been set
		  WriteUpper("  NO   CHANGES");}
	  *(DisplayUpper) = 0;															// clear leftmost credit displays
	  *(DisplayUpper+16) = 0;
	  Init_System2(1);}}																// and quit to Attract Mode

void HandleNumSetting(bool change) {									// handling method for numeric settings
	if (change) {																				// if the start button has been pressed
		AppByte2 = 1;																			// set the change indicator
		if (!digitalRead(UpDown)) {												// go forward or backward depending on UpDown switch
			if (SettingsPointer[AppByte] != SettingsList[AppByte].UpperLimit) { // upper numeric limit reached?
				SettingsPointer[AppByte]++;}}									// if limit not reached just increase the numeric value
		else {																						// if the start button has not been pressed
			if (SettingsPointer[AppByte] != SettingsList[AppByte].LowerLimit) { // lower numeric limit reached?
				SettingsPointer[AppByte]--;}}}								// if limit not reached just decrease the numeric value
  WriteLower("              ");
	DisplayScore(4,SettingsPointer[AppByte]);}					// show the current value

void HandleTextSetting(bool change) {									// handling method for text settings
	if (change) {																				// if the start button has been pressed
		AppByte2 = 1;																			// set the change indicator
		if (!digitalRead(UpDown)) {												// go forward or backward depending on UpDown switch
			if (SettingsPointer[AppByte] == SettingsList[AppByte].UpperLimit) { // last text setting reached?
				SettingsPointer[AppByte] = 0;}								// start from 0
			else {
				SettingsPointer[AppByte]++;}}									// if limit not reached just choose the next entry
		else {
			if (!SettingsPointer[AppByte]) {								// entry 0 reached?
				SettingsPointer[AppByte] = SettingsList[AppByte].UpperLimit;} // go to the last entry
			else {
				SettingsPointer[AppByte]--;}}}								// if limit not reached just choose the previous entry
	WriteLower(SettingsList[AppByte].TxTpointer+15*SettingsPointer[AppByte]);} // show the current text element

void HandleVolumeSetting(bool change) {
	HandleNumSetting(change);
	if (change) {
		analogWrite(VolumePin,255-APC_settings[Volume]);}	// adjust PWM to volume setting
	else {
		PlayMusic(50, "Musik.bin");}}
