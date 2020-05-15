#include <SdFat.h>																		// APC.ino is the operating system for the Arduino Pinball Controller
SdFat SD;
#include <SPI.h>
#include <Wire.h>
#include "Arduino.h"
#include "Sound.h"
#define UpDown 53                                			// arduino pin connected to the auto/up - manual/Down button
#define Blanking  22		                              // arduino pin to control the blanking signal
#define VolumePin 13																	// arduino pin to control the sound volume
#define SwMax 72                                 			// number of existing switches (max. 72)
#define LampMax 64                               			// number of existing lamps (max. 64)
#define DispColumns 16                           			// Number of columns of the used display unit
#define AllSelects 871346688							           	// mask for all port C pins belonging to select signals except special switch select
#define Sel5 2097152																	// mask for the Sel5 select signal
#define HwExtSels 606077440														// mask for all Hw extension port select signals
#define AllData 510
#define HwExtStackPosMax 20														// size of the HwExtBuffer

const char APC_Version[6] = "00.14";                  // Current APC version - includes the other INO files also

void HandleBoolSetting(bool change);
void HandleTextSetting(bool change);
void HandleNumSetting(bool change);
void HandleVolumeSetting(bool change);
void RestoreDefaults(bool change);
void ExitSettings(bool change);

const byte AlphaUpper[128] = {0,0,0,0,0,0,0,0,107,21,0,0,0,0,0,0,0,0,0,0,64,191,64,21,0,0,64,4,0,0,0,40, // Blank $ * + - / for upper row alphanumeric displays
		63,0,6,0,93,4,15,4,102,4,107,4,123,4,14,0,127,4,111,4,0,0,0,0,136,0,65,4,0,34,0,0,0,0, // 0 1 2 3 4 5 6 7 8 9 < = > and fill bytes
		126,4,15,21,57,0,15,17,121,4,120,4,59,4,118,4,0,17,23,0,112,136,49,0,54,10,54,130,63,0, // Pattern A B C D E F G H I J K L M N O
		124,4,63,128,124,132,107,4,8,17,55,0,48,40,54,160,0,170,0,26,9,40,0,0,0,0,0,0,0,0,1,0}; // Pattern P Q R S T U V W X Y Z _

const byte AlphaLower[128] = {0,0,0,0,0,0,0,0,182,35,0,0,0,0,0,0,0,0,0,0,2,247,2,35,0,0,2,2,0,0,0,80, // Blank $ * + - / for lower row alphanumeric displays
		252,0,136,0,122,2,184,2,142,2,182,2,246,2,152,0,254,2,190,2,0,0,0,0,0,68,0,0,0,144,0,0,0,0, // 0 1 2 3 4 5 6 7 8 9 < > and fill bytes
		222,2,184,35,116,0,184,33,118,2,86,2,244,2,206,2,0,33,232,0,70,68,100,0,204,192,204,132,252,0, // Pattern A B C D E F G H I J K L M N O
		94,2,252,4,94,6,182,2,16,33,236,0,68,80,204,20,0,212,0,224,48,80,0,0,0,0,0,0,0,0,32,0}; // Pattern P Q R S T U V W X Y Z _

const byte NumLower[118] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // Blank and fill bytes for lower row numeric displays
		252,0,136,0,122,0,186,0,142,0,182,0,246,0,152,0,254,0,190,0,0,0,0,0,0,0,34,0,0,0,0,0,0,0, // 0 1 2 3 4 5 6 7 8 9 = and fill bytes
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
const byte NoLamps[8] = {0,0,0,0,0,0,0,0};
const byte AllLamps[8] = {255,255,255,255,255,255,255,255};

// system variables

unsigned int IntBuffer;                               // general purpose buffer
byte ByteBuffer = 0;                                  // general purpose buffer
byte ByteBuffer2 = 0;                                 // general purpose buffer
byte ByteBuffer3 = 0;                                 // general purpose buffer
byte i = 0;                                           // general purpose counter
byte x = 0;                                           // general purpose counter
bool SDfound = false;                                 // SD card present?
byte SwitchStack = 0;                                 // determines which switch events stack is active
byte ChangedSw[2][30];                                // two stacks of switches with pending events
byte SwEvents[2];                                     // contains the number of pending switch events in each stack
uint32_t SwitchRows[10] = {29425756,29425756,29425756,29425756,29425756,29425756,29425756,29425756,29425756,29425756};// stores the status of all switch rows
const byte *DispRow1;                                 // determines which patterns are to be shown (2 lines with 16 chars each)
const byte *DispRow2;
const byte *DispPattern1;
const byte *DispPattern2;
byte DisplayUpper[32];                                // changeable display buffer
byte DisplayLower[32];
byte DisplayUpper2[32];                               // second changeable display buffer
byte DisplayLower2[32];
const byte *LampPattern;                              // determines which lamp pattern is to be shown (for Lamps > 8)
const byte *LampBuffer;
byte StrobeLightsTimer = 0;
byte LampColumns[8];																	// stores the status of all lamp columns
byte LEDCommandBytes = 0;															// number of command bytes to be send to the LED exp board
byte LEDCommand[20];																	// command bytes to be send to the LED exp board
byte LampWait = 1;                                    // counter for lamp waiting time until next column is applied
const struct LampPat *PatPointer;                     // Pointer to the lamp flow to be shown
struct LampPat {                                      // a lamp pattern sequence played by ShowLampPatterns
	uint16_t Duration;
	byte Pattern[7];};
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
byte SolLatch = 0;																		// Indicates which solenoid latches must be updated
bool C_BankActive = false;														// A/C relay currently doing C bank?
byte SolWaiting[64][2];																// list of waiting A/C solenoid requests
byte ACselectRelay = 0;																// solenoid number of the A/C select relay
byte ActSolSlot = 0;																	// currently processed slot in SolWaiting
byte NextSolSlot = 0;																	// next free slot in SolWaiting
byte SettingsRepeatTimer = 0;													// number of the timer of the key repeat function in the settings function
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
void (*SerialCommand)();															// Pointer to the serial command handler (0 if serial command mode is off)
char EnterIni[3];
byte HwExt_Buf[20][2];																// ringbuffer for bytes to be send to the HW_ext interface (first bytes specifies the select line to be activated
byte HwExtIRQpos = 0;																	// next buffer position for the interrupt to work on
byte HwExtBufPos = 0;																	// next buffer position to be written to
uint16_t *SoundBuffer;																// buffers sound data from SD to be processed by interrupt
uint16_t *MusicBuffer;																// buffers music data from SD to be processed by interrupt
uint16_t *Buffer16b;																	// 16bit pointer to the audio DAC buffer
uint32_t *Buffer32b;																	// 32bit pointer to the audio DAC buffer
byte MBP = 0;																					// Music Buffer Pointer - next block to write to inside of the music buffer
byte MusicIRpos = 0;																	// next block of the music buffer to be read from the interrupt
byte StopMusic = 0;																		// last music buffer block with music data
bool StartMusic = false;															// music startup active -> filling music buffer
bool PlayingMusic = false;														// StartMusic done -> continuously playing music
byte MusicVolume = 0;																	// 0 = max volume
File MusicFile;																				// file handle for the music file (SD)
byte AfterMusicPending = 0;														// indicates an after music event -> 0 - no event, 1 - event pending, 2 - event is blocked by StopPlayingMusic
void (*AfterMusic)() = 0;															// program to execute after music file has ended
const char *NextMusicName;														// points to the name of the next music file to be played (if any)
byte MusicPriority = 0;																// stores the priority of the music file currently being played
byte SBP = 0;																					// Sound Buffer Pointer - next block to write to inside of the music buffer
byte SoundIRpos = 0;																	// next block of the sound buffer to be read from the interrupt
byte StopSound = 0;																		// last sound buffer block with sound data
bool StartSound = false;															// sound startup active -> filling sound buffer
bool PlayingSound = false;														// StartSound done -> continuously playing sound
File SoundFile;																				// file handle for the sound file (SD)
byte AfterSoundPending = 0;														// indicates an after sound event -> 0 - no event, 1 - event pending, 2 - event is blocked by StopPlayingSound
void (*AfterSound)() = 0;															// program to execute after sound file has ended
byte SoundPriority = 0;																// stores the priority of the sound file currently being played
bool SoundPrio = false;																// indicates which channel has to be processed first
char *NextSoundName;
const char TestSounds[3][15] = {{"MUSIC.BIN"},{"SOUND.BIN"},0};

struct SettingTopic {																	// one topic of a list of settings
	char Text[17];																			// display text
	void (*EventPointer)(bool);													// Pointer to the subroutine to process this topic
	char *TxTpointer;																		// if text setting -> pointer to a text array
	byte LowerLimit;																		// if num setting -> lower limit of selection value
	byte UpperLimit;};																	// if text setting -> amount of text entries -1 / if num setting -> upper limit of selection value

const char APC_set_file_name[13] = "APC_SET.BIN";
const byte APC_defaults[64] =  {0,3,3,1,1,0,0,0,		 	// system default settings
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0};

#define DisplayType 0																	// which display is used?
#define ActiveGame  1																	// Select the active game
#define NofBalls  2																		// Balls per game
#define FreeGame  3																		// Free game mode?
#define ConnType  4																		// Remote control mode?
#define DimInserts  5                       		     	// Reduce lighting time of playfield lamps by 50%
#define Volume  6                         		    		// Volume of the speaker
#define LEDsetting  7                         		   	// Setting for the APC_LED_EXP board
#define DebugMode  8                            		 	// debug mode enabled?

char TxTGameSelect[5][17] = {{" BASE  CODE     "},{" BLACK KNIGHT   "},{"    PINBOT      "},{"  USB  CONTROL  "},{"   TUTORIAL     "}};
char TxTLEDSelect[3][17] = {{"   NO   LEDS    "},{"PLAYFLD ONLY    "},{"PLAYFLDBACKBOX  "}};
char TxTDisplaySelect[8][17] = {{"4 ALPHA+CREDIT  "},{" SYS11 PINBOT   "},{" SYS11  F-14    "},{" SYS11  BK2K    "},{" SYS11   TAXI   "},{" SYS11 RIVERBOAT"},{"123456123456    "},{"12345671234567  "}};
char TxTConType[3][17] = {{"        OFF     "},{"       ONBOARD  "},{"        USB     "}};

struct SettingTopic APC_setList[12] = {
		{"DISPLAY TYPE    ",HandleTextSetting,&TxTDisplaySelect[0][0],0,6},
		{" ACTIVE GAME    ",HandleTextSetting,&TxTGameSelect[0][0],0,4},
		{" NO OF  BALLS   ",HandleNumSetting,0,1,5},
		{"  FREE  GAME    ",HandleBoolSetting,0,0,0},
		{"CONNECT TYPE    ",HandleTextSetting,&TxTConType[0][0],0,3},
		{"  DIM  INSERTS  ",HandleBoolSetting,0,0,0},
		{"SPEAKER VOLUME  ",HandleVolumeSetting,0,0,255},
		{"  LED   LAMPS   ",HandleTextSetting,&TxTLEDSelect[0][0],0,2},
    {" DEBUG MODE     ",HandleBoolSetting,0,0,0},
		{"RESTOREDEFAULT  ",RestoreDefaults,0,0,0},
		{"  EXIT SETTNGS  ",ExitSettings,0,0,0},
		{"",NULL,0,0,0}};

struct GameDef {
	struct SettingTopic *GameSettingsList;							// points to the settings definition of the current game
	byte *GameDefaultsPointer;													// points to the default settings of the selected game
	const char *GameSettingsFileName;										// points to the name of the settings file for the selected game
	const char *HighScoresFileName;											// contains the name of the high scores file for the selected game
	void (*AttractMode)();															// Pointer to Attract mode of current game
	const unsigned int *SolTimes;												// Default activation times of solenoids
};

struct GameDef GameDefinition;												// invoke a game definition structure
struct SettingTopic *SettingsList;										// points to the current settings menu list

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
const char *SettingsFileName;													// points to the settings file currently being edited
const unsigned long SwitchMask[8] = {65536, 16777216, 8388608, 4194304, 64, 16, 8, 4};
uint16_t SwDrvMask = 2;                               // mask for switch row select
byte SolBuffer[3];                                    // stores the state of the solenoid latches

void setup() {
	pinMode(Blanking, OUTPUT);                          // initialize blanking pin
	pinMode(VolumePin, OUTPUT);                         // initialize volume PWM pin
	digitalWrite(Blanking, LOW);                        // and activate the blanking
	digitalWrite(VolumePin, HIGH);                      // set volume to 0
	pinMode(UpDown, INPUT);                          		// initialize Up/Down pin
	Serial.begin(115200);																// needed for USB and serial communication
	SPI.begin();																				// needed for SD card handling
	Wire1.begin(68);																		// start I2C handling
	Wire1.onReceive(I2C_receive);												// define I2C receive function
	Wire1.onRequest(I2C_transmit);											// define I2C transmit function
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
	for (i=0; i<8; i++) {                         			// initialize lamp status
		LampColumns[i+1] = 0; }
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
  Switch_Pressed = DummyProcess;
  Switch_Released = DummyProcess;
	digitalWrite(Blanking, HIGH);                       // Release the blanking
	if (SD.begin(52, SD_SCK_MHZ(20))) {                 // look for an SD card and set max SPI clock to 20MHz
		WriteUpper("SD CARD FOUND   ");
		SDfound = true;}
	else {
		WriteUpper(" NO SD  CARD    ");}
	Init_System();}

void Init_System() {
	if (SDfound) {																			// SD card found?
		File Settings = SD.open(APC_set_file_name);				// look for system settings
		if (!Settings) {
			WriteLower("NO SYS SETTNGS  ");										// if no system settings
			for(i=0;i<64;i++) {															// use default settings
				APC_settings[i] = APC_defaults[i];}}
		else {																						// if system settings found
			Settings.read(&APC_settings, sizeof APC_settings);} // read them
		Settings.close();}
	else {																							// no SD card?
		for(i=0;i<64;i++) {																// use default settings
			APC_settings[i] = APC_defaults[i];}}
	if ((APC_settings[DisplayType] == 1) || (APC_settings[DisplayType] == 2)) { // display with numerical lower row
		DispPattern2 = NumLower;}													// use patterns for num displays
	if (APC_settings[DisplayType] < 6) {								// non BCD display
		WriteLower("APC REV         ");
		*(DisplayLower+24) = DispPattern2[2*(APC_Version[0]-32)];
		*(DisplayLower+25) = DispPattern2[2*(APC_Version[0]-32)+1];
		*(DisplayLower+26) = DispPattern2[2*(APC_Version[1]-32)] | 1; // Comma
		*(DisplayLower+27) = DispPattern2[2*(APC_Version[1]-32)+1] | 8; // Dot
		*(DisplayLower+28) = DispPattern2[2*(APC_Version[3]-32)];
		*(DisplayLower+29) = DispPattern2[2*(APC_Version[3]-32)+1];
		*(DisplayLower+30) = DispPattern2[2*(APC_Version[4]-32)];
		*(DisplayLower+31) = DispPattern2[2*(APC_Version[4]-32)+1];}
	else {																							// BCD display
		*(DisplayLower+24) = ConvertNumLower((byte) APC_Version[0]-48,(byte) *(DisplayLower+24));
		*(DisplayLower+26) = ConvertNumLower((byte) APC_Version[0]-48,(byte) *(DisplayLower+26));
		*(DisplayLower+27) = 8;														// Comma
		*(DisplayLower+28) = ConvertNumLower((byte) APC_Version[0]-48,(byte) *(DisplayLower+28));
		*(DisplayLower+30) = ConvertNumLower((byte) APC_Version[0]-48,(byte) *(DisplayLower+30));}
	delay(2000);
	Init_System2(0);}

void Init_System2(byte State) {
	switch(APC_settings[ActiveGame]) {									// init calls for all valid games
	case 0:
		BC_init();
		break;
	case 1:
		BK_init();
		break;
	case 2:
		PB_init();
		break;
	case 3:
		USB_init();
		break;
	case 4:
		TT_init();
		break;
	default:
		WriteUpper("NO GAMESELECTD  ");
		while (true) {}
	}
	if ((APC_settings[DisplayType] == 1) || (APC_settings[DisplayType] == 2)) { // display with numerical lower row
		DispPattern2 = NumLower;}													// use patterns for num displays
	if (SDfound) {
		File HighScore = SD.open(GameDefinition.HighScoresFileName);
		if (!HighScore) {
			WriteUpper("NO HIGHSCORES   ");}
		else {
			HighScore.read(&HallOfFame, sizeof HallOfFame);}
		HighScore.close();
		File Settings = SD.open(GameDefinition.GameSettingsFileName);
		if (!Settings) {
			WriteLower("NO GAMESETTNGS  ");
			if (!State) {                                   // only initialize the settings in the boot up run
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
	static byte SwDrv = 0;                              // switch driver being accessed at the moment
	static byte DispCol = 0;                            // display column being illuminated at the moment
	static byte LampCol = 0;                            // lamp column being illuminated at the moment
	static uint16_t LampColMask = 2;                    // mask for lamp column select
	static bool LEDFlag;																// stores whether the select has to be triggered by the rising or falling edge
	static byte LEDCount = 0;														// points to the next command byte to be send to the LED exp board
	const uint32_t HwExtSelMask[4] = {2097152, 536870912, 512, 67108864}; // mask for sel5, sel6, sel7, SPI_CS1
	int i;                                              // general purpose counter
	uint32_t Buff;
	uint16_t c;

	if (APC_settings[DebugMode]) {
		*(DisplayLower) = RightCredit[32 + 2 * ActiveTimers];} // show the number of active timers

	if (APC_settings[DimInserts] || (LampWait == LampPeriod)) { // if inserts have to be dimmed or waiting time has passed
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals and the data bus
		REG_PIOC_SODR = 268435456;}                   		// use Sel0 to disable column driver outputs at half time

	// Switches

	i = 0;
	Buff = REG_PIOA_PDSR;
	while ((SwitchRows[SwDrv] != (Buff & 29425756)) && (SwEvents[SwitchStack] < 30)) {		// as  long as something is different at the switch port
		if (Buff & SwitchMask[i]) {												// scan the switch port bit by bit
			if (!(SwitchRows[SwDrv] & SwitchMask[i]))  {		// different from the stored switch state?
				SwitchRows[SwDrv] = SwitchRows[SwDrv] | SwitchMask[i];	// then change it
				SwEvents[SwitchStack]++;											// increase the number of pending switch events
				c = 0;
				while (ChangedSw[SwitchStack][c] && (c<29)) {	// look for a free slot
					c++;}
				ChangedSw[SwitchStack][c] = SwDrv*8+i+1;}}		// store the switch number to be processed in the main loop
		else {
			if (SwitchRows[SwDrv] & SwitchMask[i])  {				// different from the stored switch state?
				SwitchRows[SwDrv] = SwitchRows[SwDrv] & (29425756 - SwitchMask[i]); // then change it
				SwEvents[SwitchStack]++;											// increase the number of pending switch events
				c = 0;
				while (ChangedSw[SwitchStack][c] && (c<29)) {	// look for a free slot
					c++;}
				ChangedSw[SwitchStack][c] = SwDrv*8+i+1;}}		// store the switch number to be processed in the main loop
		i++;}
	SwDrvMask = SwDrvMask<<1;                  					// and the corresponding select pattern
	REG_PIOC_CODR = AllSelects - HwExtSels + AllData;        // clear all select signals except HwExtSels and the data bus
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
			if (((bool)(SwitchRows[9] & 65536) != (bool)(REG_PIOB_PDSR & 16384)) && (SwEvents[SwitchStack] < 30)) {	// check state of the Up/Down button
				if (SwitchRows[9] & 65536) {
					SwitchRows[9] = SwitchRows[9] & (29425756 - 65536);}
				else {
					SwitchRows[9] = SwitchRows[9] | 65536;}
				SwEvents[SwitchStack]++;											// increase the number of pending switch events
				c = 0;
				while (ChangedSw[SwitchStack][c] && (c<30)) {	// look for a free slot
					c++;}
				ChangedSw[SwitchStack][c] = 73;}
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
	if (APC_settings[DisplayType] == 3) {               // 2x16 alphanumeric display (BK2K type)
		REG_PIOD_CODR = 15;                               // clear strobe select signals
		REG_PIOD_SODR = DispCol;                          // set display column
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals except HwExtSels and the data bus
		byte Buf = ~(*(DispRow1+2*DispCol));
		REG_PIOC_SODR = Buf<<1;                           // set 1st byte of the display pattern for the upper row
		REG_PIOC_SODR = 524288;                           // use Sel8
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals except HwExtSels and the data bus
		Buf = ~(*(DispRow1+2*DispCol+1));
		REG_PIOC_SODR = Buf<<1;                           // set 2nd byte of the display pattern for the upper row
		REG_PIOC_SODR = 262144;                           // use Sel9
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals except HwExtSels and the data bus
		Buf = ~(*(DispRow2+2*DispCol));
		REG_PIOC_SODR = Buf<<1;                           // set 1st byte of the display pattern for the lower row
		REG_PIOC_SODR = 131072;                           // use Sel10
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals except HwExtSels and the data bus
		Buf = ~(*(DispRow2+2*DispCol+1));
		REG_PIOC_SODR = Buf<<1;                           // set 1st byte of the display pattern for the lower row
		REG_PIOC_SODR = 65536;}                           // use Sel11
	else {
		REG_PIOD_CODR = 15;                               // clear strobe select signals
		REG_PIOD_SODR = DispCol;                          // set display column
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals except HwExtSels and the data bus
		REG_PIOC_SODR = *(DispRow1+2*DispCol)<<1;         // set 1st byte of the display pattern for the upper row
		REG_PIOC_SODR = 524288;                           // use Sel8
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals except HwExtSels and the data bus
		REG_PIOC_SODR = *(DispRow1+2*DispCol+1)<<1;       // set 2nd byte of the display pattern for the upper row
		REG_PIOC_SODR = 262144;                           // use Sel9
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals except HwExtSels and the data bus
		REG_PIOC_SODR = *(DispRow2+2*DispCol)<<1;         // set 1st byte of the display pattern for the lower row
		REG_PIOC_SODR = 131072;                           // use Sel10
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals except HwExtSels and the data bus
		REG_PIOC_SODR = *(DispRow2+2*DispCol+1)<<1;       // set 1st byte of the display pattern for the lower row
		REG_PIOC_SODR = 65536;}                           // use Sel11

	// Timers

	if (!BlockTimers) {
		Buff = ActiveTimers;
		i = 1;
		while (Buff) {                                		// Number of timers to process (active timers)
			if (TimerValue[i]) {                        		// Timer active?
				Buff--;                                   		// Decrease number of timers to process
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
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals and the data bus
		if (SolLatch & 1) {
			c = SolBuffer[0];
			REG_PIOC_SODR = c<<1;
			REG_PIOC_SODR = 16777216;}											// select first latch
		if (SolLatch & 2) {
			REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals and the data bus
			c = SolBuffer[1];
			REG_PIOC_SODR = c<<1;
			REG_PIOC_SODR = 8388608;}												// select second latch
		if (SolLatch & 4) {
			REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals and the data bus
			c = SolBuffer[2];
			REG_PIOC_SODR = c<<1;
			REG_PIOC_SODR = 4194304;}												// select third latch
		SolLatch = 0;
		SolChange = false;}																// reset flag

	REG_PIOA_CODR = 524288;                             // enable latch outputs to send the pattern to display
	REG_PIOC_CODR = AllSelects - HwExtSels + AllData;   // clear all select signals and the data bus

	// Lamps

	if (!APC_settings[LEDsetting]) {
		if (LampWait == LampPeriod) {                     // Waiting time has passed
			LampCol++;                                      // prepare for next lamp column
			LampColMask = LampColMask<<1;
			c = 2;                                          // clear buffer
			if (LampCol == 8){                              // max column reached?
				LampCol = 0;
				LampColMask = 2;
				c = LampColumns[LampCol];}										// column 0 is always from LampColumns
			else {
				c = *(LampPattern+LampCol);}									// columns > 0 are referenced via LampPattern
			REG_PIOC_SODR = c<<1;														// write lamp pattern
			REG_PIOC_SODR = 33554432;                       // use Sel1
			REG_PIOC_CODR = AllSelects + AllData;           // clear all select signals and the data bus
			REG_PIOC_SODR = LampColMask;
			LampWait = 1;                                  	// restart lamp waiting counter
			REG_PIOC_SODR = 268435456;}                     // use Sel0
		else {																						// waiting time has not yet passed
			LampWait++;}}                                   // increase wait counter
	else {                                              // LEDs selected
		if (LampCol > 19) {                               // 20ms over
			LampCol = 0;}                                   // start from the beginning
		if (LampCol < 8) {                                // the first 8 cycles are for transmitting the status of the lamp matrix
			byte LampData;
			if (!LampCol){                                  // max column reached?
				LampData = LampColumns[LampCol];}
			else {
				LampData = *(LampPattern+LampCol);}
			if (LEDFlag) {
				LEDFlag = false;
				WriteToHwExt(LampData, 1);}                   // write lamp pattern with Sel5 falling edge
			else {
				LEDFlag = true;
				WriteToHwExt(LampData, 129);}}                // activate Sel5 rising edge
		else {                                            // the lamp matrix is already sent
			if ((LampCol < 13) || LEDCount) {               // still time to send a command or command still running?
				if (LEDCommandBytes) {                        // are there any pending LED commands?
					if (LEDFlag) {
						LEDFlag = false;
						WriteToHwExt(LEDCommand[LEDCount], 1);}   // write LED command with Sel5 falling edge
					else {
						LEDFlag = true;
						WriteToHwExt(LEDCommand[LEDCount], 129);}
					LEDCount++;                                 	// increase the counter
					if (LEDCount == LEDCommandBytes) {          	// not all command bytes sent?
						LEDCommandBytes = 0;
						LEDCount = 0;}}}
			else {                                          // LampCol > 13
				if (LampCol == 17) {													// time to sync
					if (LEDFlag) {
						LEDFlag = false;
						WriteToHwExt(170, 1);}                    // write sync command with Sel5 falling edge
					else {
						LEDFlag = true;
						WriteToHwExt(170, 129);}}}}
		LampCol++;}

	// Hardware extension interface

	while (HwExtIRQpos != HwExtBufPos) {								// for all bytes in the ringbuffer
		HwExtIRQpos++;																		// go to next task
		if (HwExtIRQpos >= HwExtStackPosMax) {						// end of buffer reached?
			HwExtIRQpos = 0;}																// start from zero
		REG_PIOC_CODR = AllSelects - HwExtSels + AllData; // clear all select signals and the data bus
		c = HwExt_Buf[HwExtIRQpos][0];										// get data byte
		REG_PIOC_SODR = c<<1;															// and put it on the data bus
		if (HwExt_Buf[HwExtIRQpos][1] & 128) {						// rising select edge requested?
			for (i=0;i<4;i++) {															// for all HwExt selects
				if (HwExt_Buf[HwExtIRQpos][1] & 1) {					// is the corresponding bit set?
					REG_PIOC_SODR = HwExtSelMask[i];						// generate a rising edge
					HwExt_Buf[HwExtIRQpos][1] = HwExt_Buf[HwExtIRQpos][1]>>1;}}}	// shift to the next bit
		else {																						// falling select edge requested
			for (i=0;i<4;i++) {															// for all HwExt selects
				if (HwExt_Buf[HwExtIRQpos][1] & 1) {					// is the corresponding bit set?
					REG_PIOC_CODR = HwExtSelMask[i];						// generate a falling edge
					HwExt_Buf[HwExtIRQpos][1] = HwExt_Buf[HwExtIRQpos][1]>>1;}}}}	// shift to the next bit

	// Sound

	if (g_Sound.next != g_Sound.running) {
		if (PlayingMusic) {
			if (PlayingSound) {															// playing music and sound
				Buffer16b = (uint16_t *) g_Sound.next;				// get address of the next DAC buffer part to be filled
				if (MusicVolume) {														// reduce the music volume?
					for (i=0; i<64; i++) {
						*Buffer16b = *(MusicBuffer+MusicIRpos*128+2*i) >> MusicVolume;	// write channel 1
						Buffer16b++;
						*Buffer16b = *(SoundBuffer+SoundIRpos*128+2*i) | 4096;  // write channel 2
						Buffer16b++;}}
				else {																				// full volume
					for (i=0; i<64; i++) {
						*Buffer16b = *(MusicBuffer+MusicIRpos*128+2*i);	// write channel 1
						Buffer16b++;
						*Buffer16b = *(SoundBuffer+SoundIRpos*128+2*i) | 4096;  // write channel 2
						Buffer16b++;}}
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
						if (AfterMusic && AfterMusicPending != 2) {	// anything to do after the music?
							AfterMusicPending = 1;}									// indicate now is the time to do so
						else {
							AfterMusicPending = 0;}}}
				if (StopSound) {															// end of sound file reached?
					if (SoundIRpos == SBP) {										// remaining sound data played?
						PlayingSound = false;											// stop playing sound
						SBP = 0;																	// reset write pointer
						SoundIRpos = 0;														// reset read pointer
						StopSound = 0;														// mark stop sequence as completed
						if (AfterSound && AfterSoundPending != 2) {	// anything to do after the sound and not blocked by StopPlayingSound?
							AfterSoundPending = 1;}									// indicate now is the time to do so
						else {
							AfterSoundPending = 0;}}}}
			else {																					// playing music only
				Buffer16b = (uint16_t *) g_Sound.next;				// same as above but music only
				if (MusicVolume) {														// reduce the music volume?
					for (i=0; i<64; i++) {
						*Buffer16b = *(MusicBuffer+MusicIRpos*128+2*i) >> MusicVolume;
						Buffer16b++;
						*Buffer16b = 6144;  											// 2048 | 4096
						Buffer16b++;}}
				else {																				// full volume
					for (i=0; i<64; i++) {
						*Buffer16b = *(MusicBuffer+MusicIRpos*128+2*i);
						Buffer16b++;
						*Buffer16b = 6144;  											// 2048 | 4096
						Buffer16b++;}}
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
						if (AfterMusic && AfterMusicPending != 2) {
							AfterMusicPending = 1;}
						else {
							AfterMusicPending = 0;}}}}}
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
						if (AfterSound && AfterSoundPending != 2) {
							AfterSoundPending = 1;}
						else {
							AfterSoundPending = 0;}}}}
			else {																					// neither sound nor music
				Buffer32b = g_Sound.next;
				for (i=0; i<64; i++) {
					*Buffer32b = 402655232;											// 2048 on both channels and the channel tag
					Buffer32b++;}
				g_Sound.next = Buffer32b;
				g_Sound.enqueue();}}}}

void loop() {
	byte c = 0;                                  				// initialize counter
	if (SwEvents[SwitchStack]) {                        // switch event pending?
		SwitchStack = 1-SwitchStack;                      // switch to the other stack to avoid a conflict with the interrupt
		while (SwEvents[1-SwitchStack]) {									// as long as there are switch events to process
			if (ChangedSw[1-SwitchStack][c]) {							// pending switch event found?
				SwEvents[1-SwitchStack]--;										// decrease number of pending events
				i = ChangedSw[1-SwitchStack][c];							// buffer the switch number
				ChangedSw[1-SwitchStack][c] = 0;							// clear the event
				if (QuerySwitch(i)) {                         // process SET switches
					Switch_Pressed(i);}													// access the set switch handler
				else {																				// process released switches
					Switch_Released(i);}}												// access the released switch handler
			if (c < 29) {                                   // number of pending events still in the allowed range?
				c++;}																				  // increase counter
			else {
				if (c > 29) {
					ErrorHandler(21,0,c);}}}}
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
			if (AfterSoundPending == 1) {										// is there an after sound event pending?
				AfterSoundPending = 0;												// reset the flag
				if (AfterSound) {															// really?
					AfterSound();}}															// call it
			else {																					// no after sound event
				if (!StopMusic && (MBP != MusicIRpos)) {			// proceed with music
					ReadMusic();}
				else {																				// no music data?
					if (AfterMusicPending == 1) {								// is there an after music event pending?
						AfterMusicPending = 0;										// reset the flag
						if (AfterMusic) {													// really?
							AfterMusic();}}}}}}											// call it
	else {																							// same as above but with the priority on music
		if (!StopMusic && (MBP != MusicIRpos)) {
			ReadMusic();}
		else {
			if (AfterMusicPending == 1) {
				AfterMusicPending = 0;
				if (AfterMusic) {
					AfterMusic();}}
			else {
				if (!StopSound && (SBP != SoundIRpos)) {
					ReadSound();}
				if (AfterSoundPending == 1) {
					AfterSoundPending = 0;
					if (AfterSound) {
						AfterSound();}}}}}
	if (SerialCommand && APC_settings[ConnType]) { 			// Remote mode?
		if ((APC_settings[ConnType] > 1) && Serial.available()) {	// USB selected?
			SerialCommand();}}}															// use the first received byte as a command

void ReadMusic() {																		// read music data from SD
	if (MusicFile.available() > 255) {									// enough data remaining in file to fill one block?
		MusicFile.read(MusicBuffer+MBP*128,2*128);				// read one block
		MBP++;																						// increase read pointer
		if (MBP == 16) {																	// last block reached?
			MBP = 0;																				// start over
			if (!PlayingMusic) {														// not already playing?
				PlayingMusic = true;													// start playing
				StartMusic = false;}}}												// stop init phase
	else {																							// not enough data to fill one block
		byte Avail = MusicFile.available();								// determine how much is left
		MusicFile.read(MusicBuffer+MBP*128,Avail);				// read it
		for (i=0; i<128-Avail/2; i++) {										// fill the rest with silence
			*(MusicBuffer+MBP*128+Avail/2+i) = 2048;}
		MBP++;																						// increase read pointer
		if (MBP == 16) {																	// last block read?
			MBP = 0;}																				// start over
		StopMusic = MBP;																	// mark this block as being the last
		MusicFile.close();}																// close file
	SoundPrio = true;}																	// switch priority to sound

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

bool QuerySwitch(byte Switch) {												// return status of switch
	Switch--;																						// arrays start with 0
	return !(SwitchRows[(Switch - (Switch % 8))/8] & SwitchMask[Switch % 8]);}

void TurnOnLamp(byte Lamp) {
	Lamp--;
	LampColumns[(Lamp - (Lamp % 8))/8] |= 1<<(Lamp % 8);}

void TurnOffLamp(byte Lamp) {
	Lamp--;
	LampColumns[(Lamp - (Lamp % 8))/8] &= 255-(1<<(Lamp % 8));}

bool QueryLamp(byte Lamp) {
	Lamp--;
	return LampColumns[(Lamp - (Lamp % 8))/8] & 1<<(Lamp % 8);}

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

bool WriteToHwExt(byte Data, byte Selects) {					// write data and selects to ringbuffer
	byte BufPosition = HwExtBufPos;
	BufPosition++;
	if (BufPosition >= HwExtStackPosMax) {							// end of buffer reached?
		BufPosition = 0;}																	// start from zero
	if (HwExtIRQpos == BufPosition) {										// ringbuffer full
		return false;}																		// return fail signal
	else {
		HwExt_Buf[BufPosition][0] = Data;									// write data
		HwExt_Buf[BufPosition][1] = Selects;							// write selects
		HwExtBufPos = BufPosition;												// store new buffer position
		return true;}}																		// return OK signal

byte ConvertNumUpper(byte Number, byte Pattern) {			// convert a number to be shown in the upper row of numerical displays
	const byte NumMaskUpper[5] = {184,64,4,2,1};				// Bitmasks for the upper row of numerical displays
	byte Mask = 1;
	Pattern &= NumMaskUpper[0];													// clear all bits belonging to this digit
	for (byte c=1;c<5;c++) {														// for 4 bit
		if (Number & Mask) {															// check bits from number
			Pattern |= NumMaskUpper[c];}										// apply the corresponding bitmask
		Mask = Mask << 1;}
	return Pattern;}

byte ConvertNumLower(byte Number, byte Pattern) {			// convert a number to be shown in the lower row of numerical displays
	const byte NumMaskLower[5] = {71,16,8,128,32};			// Bitmasks for the lower row of numerical displays
	byte Mask = 1;
	Pattern &= NumMaskLower[0];													// clear all bits belonging to this digit
	for (byte c=1;c<5;c++) {														// for 4 bit
		if (Number & Mask) {															// check bits from number
			Pattern |= NumMaskLower[c];}										// apply the corresponding bitmask
		Mask = Mask << 1;}
	return Pattern;}

byte ConvertPattern(byte Select, byte Pattern) {			// convert the main display pattern to those of the lower row etc
	const byte ConvTable[3][8] = {{32,128,8,16,64,4,2,1},{1,128,2,64,32,16,8,4},{0,0,0,0,0,0,0,0}}; // conversion table
	byte Result = 0;																		// clear Result
	for (byte i=0;i<8;i++) {														// for every bit
		if (Pattern & 1) {																// is the LSB set in the pattern?
			Result |= ConvTable[Select][i];}								// set the corresponding bit in the result
		Pattern = Pattern >> 1;}													// shift the pattern
	return Result;}

void WritePlayerDisplay(char* DisplayText, byte Player) {	// write ASCII text to player displays - credit is Player 0
	switch (APC_settings[DisplayType]) {
	case 0:																							// 4 Alpha + Credit
	case 1:																							// Sys11 Pinbot
	case 2:																							// Sys11 F-14
		if (Player) {																			// player display?
			if (Player < 3) {																// upper row?
				Player--;
				for (i=0;i<7;i++) {														// for all digits
					if (*(DisplayText+i) & 128) {								// comma set?
						*(DisplayUpper+2+16*Player+2*i) = 128 | DispPattern1[(int)(((*(DisplayText+i) & 127)-32)*2)];
						*(DisplayUpper+3+16*Player+2*i) = 64 | DispPattern1[(int)(((*(DisplayText+i) & 127)-32)*2)+1];}
					else {
						*(DisplayUpper+2+16*Player+2*i) = DispPattern1[(int)((*(DisplayText+i)-32)*2)];
						*(DisplayUpper+3+16*Player+2*i) = DispPattern1[(int)((*(DisplayText+i)-32)*2)+1];}}}
			else {																					// lower row
				Player = Player - 3;
				for (i=0;i<7;i++) {														// for all digits
					if (*(DisplayText+i) & 128) {								// comma set?
						*(DisplayLower+2+16*Player+2*i) = 1 | DispPattern2[(int)(((*(DisplayText+i) & 127)-32)*2)];
						*(DisplayLower+3+16*Player+2*i) = 8 | DispPattern2[(int)(((*(DisplayText+i) & 127)-32)*2)+1];}
					else {
						*(DisplayLower+2+16*Player+2*i) = DispPattern2[(int)((*(DisplayText+i)-32)*2)];
						*(DisplayLower+3+16*Player+2*i) = DispPattern2[(int)((*(DisplayText+i)-32)*2)+1];}}}}
		else {																						// credit display
			*(DisplayUpper) = LeftCredit[(*(DisplayText)-32)*2];
			*(DisplayUpper+1) = LeftCredit[((*(DisplayText)-32)*2)+1];
			*(DisplayUpper+16) = LeftCredit[(*(DisplayText+1)-32)*2];
			*(DisplayUpper+17) = LeftCredit[((*(DisplayText+1)-32)*2)+1];
			*(DisplayLower) = RightCredit[(*(DisplayText+2)-32)*2];
			*(DisplayLower+1) = RightCredit[((*(DisplayText+2)-32)*2)+1];
			*(DisplayLower+16) = RightCredit[(*(DisplayText+3)-32)*2];
			*(DisplayLower+17) = RightCredit[((*(DisplayText+3)-32)*2)+1];}
		break;
	case 3:																							// Sys11 BK2K
		if (Player == 1) {
			for (i=0;i<16;i++) {														// for all digits
				if (*(DisplayText+i) & 128) {									// comma set?
					*(DisplayUpper+2*i) = 128 | DispPattern1[(int)(((*(DisplayText+i) & 127)-32)*2)];
					*(DisplayUpper+2*i+1) = 64 | DispPattern1[(int)(((*(DisplayText+i) & 127)-32)*2)+1];}
				else {
					*(DisplayUpper+2*i) = DispPattern1[(int)((*(DisplayText+i)-32)*2)];
					*(DisplayUpper+2*i+1) = DispPattern1[(int)((*(DisplayText+i)-32)*2)+1];}}}
		else {
			for (i=0;i<16;i++) {														// for all digits
				if (*(DisplayText+i) & 128) {									// comma set?
					*(DisplayLower+2*i) = 1 | DispPattern2[(int)(((*(DisplayText+i) & 127)-32)*2)];
					*(DisplayLower+2*i+1) = 8 | DispPattern2[(int)(((*(DisplayText+i) & 127)-32)*2)+1];}
				else {
					*(DisplayLower+2*i) = DispPattern2[(int)((*(DisplayText+i)-32)*2)];
					*(DisplayLower+2*i+1) = DispPattern2[(int)((*(DisplayText+i)-32)*2)+1];}}}
		break;
	case 6:																							// numbers only type display
	case 7:
		if (Player) {																			// player display?
			if (Player < 3) {																// upper row?
				Player--;
				for (i=0;i<7;i++) {														// for all digits
					if (*(DisplayText+i) & 128) {								// comma set?
						*(DisplayLower+2+16*Player+2*i) = ConvertNumUpper((byte) (*(DisplayText+i) & 127)-48,(byte) *(DisplayLower+2+16*Player+2*i));
						*(DisplayLower+3+16*Player+2*i) = 128;}
					else {
						*(DisplayLower+2+16*Player+2*i) = ConvertNumUpper((byte) *(DisplayText+i)-48,(byte) *(DisplayLower+2+16*Player+2*i));
						*(DisplayLower+3+16*Player+2*i) = 0;}}}
			else {																					// lower row
				Player = Player - 3;
				for (i=0;i<7;i++) {														// for all digits
					if (*(DisplayText+i) & 128) {								// comma set?
						*(DisplayLower+2+16*Player+2*i) = ConvertNumLower((byte) (*(DisplayText+i) & 127)-48,(byte) *(DisplayLower+2+16*Player+2*i));
						*(DisplayLower+3+16*Player+2*i) = 1;}
					else {
						*(DisplayLower+2+16*Player+2*i) = ConvertNumLower((byte) *(DisplayText+i)-48,(byte) *(DisplayLower+2+16*Player+2*i));
						*(DisplayLower+3+16*Player+2*i) = 0;}}}}
		else {																						// credit display
			*(DisplayLower) = ConvertNumUpper((byte) *(DisplayText)-48,(byte) *(DisplayLower));
			*(DisplayLower+16) = ConvertNumUpper((byte) *(DisplayText+1)-48,(byte) *(DisplayLower+16));
			*(DisplayLower) = ConvertNumLower((byte) *(DisplayText+2)-48,(byte) *(DisplayLower));
			*(DisplayLower+16) = ConvertNumLower((byte) *(DisplayText+3)-48,(byte) *(DisplayLower+16));}}}

void WriteUpper(const char* DisplayText) {            
	if (APC_settings[DisplayType] == 3) {               // 2x16 alphanumeric display (BK2K type)
		for (i=0; i<16; i++) {
			*(DisplayUpper+2*i) = DispPattern1[(int)((*(DisplayText+i)-32)*2)];
			*(DisplayUpper+2*i+1) = DispPattern1[(int)((*(DisplayText+i)-32)*2)+1];}}
	else {
		if (APC_settings[DisplayType] > 5) {            	// numbers only type display
			for (i=0;i<7;i++) {															// for all digits
				*(DisplayLower+2+2*i) = ConvertNumUpper((byte) *(DisplayText+i)-48,(byte) *(DisplayLower+2+2*i));
				*(DisplayLower+18+2*i) = ConvertNumUpper((byte) *(DisplayText+7+i)-48,(byte) *(DisplayLower+18+2*i));}}
		else {                                            // Sys11 display with credit
			for (i=0; i<7; i++) {
				*(DisplayUpper+2+2*i) = DispPattern1[(int)((*(DisplayText+i)-32)*2)];
				*(DisplayUpper+2+2*i+1) = DispPattern1[(int)((*(DisplayText+i)-32)*2)+1];
				*(DisplayUpper+18+2*i) = DispPattern1[(int)((*(DisplayText+7+i)-32)*2)];
				*(DisplayUpper+18+2*i+1) = DispPattern1[(int)((*(DisplayText+7+i)-32)*2)+1];}}}}

void WriteLower(const char* DisplayText) {
	if (APC_settings[DisplayType] == 3) {               // 2x16 alphanumeric display (BK2K type)
		for (i=0; i<16; i++) {
			*(DisplayLower+2*i) = DispPattern2[(int)((*(DisplayText+i)-32)*2)];
			*(DisplayLower+2*i+1) = DispPattern2[(int)((*(DisplayText+i)-32)*2)+1];}}
	else {
		if (APC_settings[DisplayType] > 5) {            	// numbers only type display
			for (i=0;i<7;i++) {															// for all digits
				*(DisplayLower+2+2*i) = ConvertNumLower((byte) *(DisplayText+i)-48,(byte) *(DisplayLower+2+2*i));
				*(DisplayLower+18+2*i) = ConvertNumLower((byte) *(DisplayText+7+i)-48,(byte) *(DisplayLower+18+2*i));}}
		else {																						// Sys11 display with credit
			for (i=0; i<7; i++) {
				*(DisplayLower+2+2*i) = DispPattern2[(int)((*(DisplayText+i)-32)*2)];
				*(DisplayLower+2+2*i+1) = DispPattern2[(int)((*(DisplayText+i)-32)*2)+1];
				*(DisplayLower+18+2*i) = DispPattern2[(int)((*(DisplayText+7+i)-32)*2)];
				*(DisplayLower+18+2*i+1) = DispPattern2[(int)((*(DisplayText+7+i)-32)*2)+1];}}}}

void HandleDisplaySetting(bool change) {
	HandleTextSetting(change);
	if (APC_settings[DisplayType] > 5) {            		// numbers only type display
		byte HighMask;
		byte LowMask;
		for (i=0;i<7;i++) {																// for all digits
			*(DisplayLower+2+2*i) &= B11110000;							// clear least significant nibble
			HighMask = B1000;																// prepare bitmasks
			LowMask = 1;
			for (byte c=0;c<4;c++) {												// for 4 bit
				if (i & LowMask) {														// get bits from buffer
					*(DisplayLower+2+2*i) |= HighMask;					// apply the upside down to the display bus
					HighMask = HighMask >> 1;
					LowMask = LowMask << 1;}}}}}

void WriteUpper2(const char* DisplayText) {
  if (APC_settings[DisplayType] == 3) {               // 2x16 alphanumeric display (BK2K type)
    for (i=0; i<16; i++) {
      *(DisplayUpper2+2*i) = DispPattern1[(int)((*(DisplayText+i)-32)*2)];
      *(DisplayUpper2+2*i+1) = DispPattern1[(int)((*(DisplayText+i)-32)*2)+1];}}
  else {
  	for (i=0; i<7; i++) {
  		*(DisplayUpper2+2+2*i) = DispPattern1[(int)((*(DisplayText+i)-32)*2)];
  		*(DisplayUpper2+2+2*i+1) = DispPattern1[(int)((*(DisplayText+i)-32)*2)+1];
  		*(DisplayUpper2+18+2*i) = DispPattern1[(int)((*(DisplayText+7+i)-32)*2)];
  		*(DisplayUpper2+18+2*i+1) = DispPattern1[(int)((*(DisplayText+7+i)-32)*2)+1];}}}

void WriteLower2(const char* DisplayText) {
  if (APC_settings[DisplayType] == 3) {               // 2x16 alphanumeric display (BK2K type)
    for (i=0; i<16; i++) {
      *(DisplayLower2+2*i) = DispPattern2[(int)((*(DisplayText+i)-32)*2)];
      *(DisplayLower2+2*i+1) = DispPattern2[(int)((*(DisplayText+i)-32)*2)+1];}}
  else {
  	for (i=0; i<7; i++) {
  		*(DisplayLower2+2+2*i) = DispPattern2[(int)((*(DisplayText+i)-32)*2)];
  		*(DisplayLower2+2+2*i+1) = DispPattern2[(int)((*(DisplayText+i)-32)*2)+1];
  		*(DisplayLower2+18+2*i) = DispPattern2[(int)((*(DisplayText+7+i)-32)*2)];
  		*(DisplayLower2+18+2*i+1) = DispPattern2[(int)((*(DisplayText+7+i)-32)*2)+1];}}}

void ScrollUpper(byte Step) {													// call with Step = 0 and the text being in DisplayUpper2
  if (APC_settings[DisplayType] == 3) {               // 2x16 alphanumeric display (BK2K type)
    for (i=0; i<30; i++) {
      DisplayUpper[i] = DisplayUpper[i+2];}
    DisplayUpper[30] = DisplayUpper2[2*Step];         // add the corresponding char of DisplayUpper2
    DisplayUpper[31] = DisplayUpper2[2*Step+1];
    Step++;}                                          // increase step
  else {                                              // dont use columns 0 and 8 as they are reserved for the credit display
    if (!Step) {
      Step++;}
  	for (i=2; i<14; i++) {													  // scroll display 1
  		DisplayUpper[i] = DisplayUpper[i+2];}
  	DisplayUpper[14] = DisplayUpper[18];						  // add put the leftmost char of the display 2 to the end
  	DisplayUpper[15] = DisplayUpper[19];
  	for (i=18; i<30; i++) {                           // scroll display 2
  		DisplayUpper[i] = DisplayUpper[i+2];}
  	DisplayUpper[30] = DisplayUpper2[2*Step];					// add the corresponding char of DisplayUpper2
  	DisplayUpper[31] = DisplayUpper2[2*Step+1];
  	Step++;																						// increase step
  	if (Step == 8) {																	// skip position 9 (belongs to the credit display
  		Step++;}}
	if (Step < 16) {																		// if its not already over
		ActivateTimer(50, Step, ScrollUpper);}}						// come back

void AddScrollUpper(byte Step) {											// call with Step = 0 and the text being in DisplayUpper2
  if (!Step) {
    Step++;}
	if (Step > 8) {																			// scroll into the left display?
		for (i=0; i<2*(Step-9);i++) {											// for all characters in the left display that have to be scrolled
			DisplayUpper[32-(2*Step)+i] = DisplayUpper[34-(2*Step)+i];}	// scroll them
		DisplayUpper[14] = DisplayUpper[18];							// get the leftmost character of the right display
		DisplayUpper[15] = DisplayUpper[19];
		for (i=0; i<12; i++) {														// scroll the right display by 6 characters
			DisplayUpper[18+i] = DisplayUpper[20+i];}}
	else {																							// scroll only the right display
		for (i=0; i<2*(Step-1); i++) {
			DisplayUpper[32-(2*Step)+i] = DisplayUpper[34-(2*Step)+i];}}
	DisplayUpper[30] = DisplayUpper2[2*Step];						// fill the rightmost character of the right display
	DisplayUpper[31] = DisplayUpper2[2*Step+1];
	Step++;
	if (Step == 8) {																		// skip position 9 (belongs to the credit display
		Step++;}
	if (!DisplayUpper[32-(2*Step)] && !DisplayUpper[33-(2*Step)] && Step < 14) {	// stop when reaching anything else but blanks or after 14 characters
		ActivateTimer(50, Step, AddScrollUpper);}}

void ScrollLower(byte Step) {													// call with Step = 0 and the text being in DisplayLower2
    if (!Step) {
    Step++;}
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

void ScrollLower2(byte Step) {												// call with Step = 0 and the text being in DisplayUpper2
  if (APC_settings[DisplayType] == 3) {               // 2x16 alphanumeric display (BK2K type)
    for (i=0; i<30; i++) {
      DisplayLower[i] = DisplayLower[i+2];}
    DisplayLower[30] = DisplayLower2[2*Step];         // add the corresponding char of DisplayLower2
    DisplayLower[31] = DisplayLower2[2*Step+1];
    Step++;}                                          // increase step
  else {
    if (!Step) {
      Step++;}
  	for (i=2; i<14; i++) {														// scroll display 1
  		DisplayLower[i] = DisplayLower[i+2];}
  	DisplayLower[14] = DisplayLower[18];							// add put the leftmost char of the display 2 to the end
  	DisplayLower[15] = DisplayLower[19];
  	for (i=18; i<30; i++) {                         	// scroll display 2
  		DisplayLower[i] = DisplayLower[i+2];}
  	DisplayLower[30] = DisplayLower2[2*Step];					// add the corresponding char of DisplayLower2
  	DisplayLower[31] = DisplayLower2[2*Step+1];
  	Step++;																						// increase step
  	if (Step == 8) {																	// skip position 9 (belongs to the credit display
  		Step++;}}
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

void ActivateSolenoid(unsigned int Duration, byte Solenoid) {
	SolChange = false;																	// block IRQ solenoid handling
	if (Solenoid > 8) {																	// does the solenoid not belong to the first latch?
		if (Solenoid < 17) {															// does it belong to the second latch?
			SolBuffer[1] |= 1<<(Solenoid-9);								// latch counts from 0
			SolLatch |= 2;}																	// select second latch
		else {
			SolBuffer[2] |= 1<<(Solenoid-17);
			SolLatch |= 4;}}																// select third latch
	else {
		SolBuffer[0] |= 1<<(Solenoid-1);
		SolLatch |= 1;}																		// select first latch
	if (!Duration) {
		Duration = *(GameDefinition.SolTimes+Solenoid-1);} // if no duration is specified use the solenoid specific default
	if (Duration) {                               			// duration = 0 means solenoid is permanently on
		ActivateTimer(Duration, Solenoid, ReleaseSolenoid);} // otherwise use a timer to turn it off again
	SolChange = true;}

void DelaySolenoid(byte Solenoid) {                   // activate solenoid after delay time
	ActivateSolenoid(0, Solenoid);}

void ReleaseAllSolenoids() {
	for (i=0; i< 3; i++) {															// clear all solenoid buffers
		SolBuffer[i] = 0;}
	ReleaseSolenoid(1);																	// release one solenoid of each buffer
	ReleaseSolenoid(9);
	ReleaseSolenoid(17);}

void ReleaseSolenoid(byte Solenoid) {
	SolChange = false;																	// block IRQ solenoid handling
	if (Solenoid > 8) {																	// does the solenoid not belong to the first latch?
		if (Solenoid < 17) {															// does it belong to the second latch?
			SolBuffer[1] &= 255-(1<<(Solenoid-9));					// latch counts from 0
			SolLatch |= 2;}																	// select second latch
		else {
			SolBuffer[2] &= 255-(1<<(Solenoid-17));
			SolLatch |= 4;}}																// select third latch
	else {
		SolBuffer[0] &= 255-(1<<(Solenoid-1));
		SolLatch |= 1;}																		// select first latch
	SolChange = true;}

bool QuerySolenoid(byte Solenoid) {                  	// determine the current state of a solenoid
	Solenoid--;
	return SolBuffer[(Solenoid - (Solenoid % 8)) / 8] & (1<<(Solenoid % 8));}

void ActA_BankSol(byte Solenoid) {
	if (!SolWaiting[NextSolSlot][0]) {
		SolWaiting[NextSolSlot][0] = Solenoid;
		SolWaiting[NextSolSlot][1] = 0;
		NextSolSlot++;
		if (NextSolSlot > 63) {
			NextSolSlot = 0;}
		ActSolenoid(0);}
	else {
		ErrorHandler(28,0,Solenoid);}}

void ActC_BankSol(byte Solenoid) {
	if (!SolWaiting[NextSolSlot][0]) {
		SolWaiting[NextSolSlot][0] = Solenoid+24;
		SolWaiting[NextSolSlot][1] = 0;
		NextSolSlot++;
		if (NextSolSlot > 63) {
			NextSolSlot = 0;}
		ActSolenoid(0);}
	else {
		ErrorHandler(29,0,Solenoid);}}

void PlayFlashSequence(byte* Sequence) {              // prepare for playing a flasher sequence
	byte x = 0;
	while (Sequence[x]) {
		if (!SolWaiting[NextSolSlot][0]) {
			SolWaiting[NextSolSlot][0] = Sequence[x];
			x++;
			SolWaiting[NextSolSlot][1] = Sequence[x];
			x++;
			NextSolSlot++;
			if (NextSolSlot > 63) {
				NextSolSlot = 0;}}
		else {
			ErrorHandler(30,0,0);
			break;}}
	ActSolenoid(0);}

void ActSolenoid(byte GivenState) {										// activate waiting A/C solenoids
	static byte State = 0;
	if (GivenState || !State) {													// accept new calls (Givenstate = 0) only when not already running (State = 0)
		if (ActSolSlot != NextSolSlot) {									// any solenoid waiting?
			if (ACselectRelay && (((SolWaiting[ActSolSlot][0] < 9) && C_BankActive) || ((SolWaiting[ActSolSlot][0] > 24) && !C_BankActive))) { // wrong relay state?
				if (C_BankActive) {
					ReleaseSolenoid(ACselectRelay);							// switch to A
					C_BankActive = false;}											// signal it
				else {
					ActivateSolenoid(0, ACselectRelay);					// switch to C
					C_BankActive = true;}												// signal it
				ActivateTimer(50, 1, ActSolenoid);}					  // wait 50ms for the relay to settle
			else {
				if (SolWaiting[ActSolSlot][0] < 25) {
					ActivateSolenoid(0, SolWaiting[ActSolSlot][0]);}
				else {
					ActivateSolenoid(*(GameDefinition.SolTimes+SolWaiting[ActSolSlot][0]-1), SolWaiting[ActSolSlot][0]-24);}
				if (SolWaiting[ActSolSlot][1]) {
					ActivateTimer(SolWaiting[ActSolSlot][1]*10, 1, ActSolenoid);}	// call the timer
				else {
					ActivateTimer(*(GameDefinition.SolTimes+SolWaiting[ActSolSlot][0]-1)+1, 1, ActSolenoid);}
				SolWaiting[ActSolSlot][0] = 0;								// mark current slot as free
				ActSolSlot++;																	// increase slot number
				if (ActSolSlot > 63) {												// array end reached?
					ActSolSlot = 0;}}														// start from zero
			State = 1;}																			// set routing state to active
		else if (C_BankActive){														// nothing more to do and relay still active?
			ReleaseSolenoid(ACselectRelay);									// reset it
			C_BankActive = false;
			State = 1;
			ActivateTimer(50, 1, ActSolenoid);}
		else {																						// absolutely nothing to do
			State = 0;}}}																		// set routing state to passive

void ShowPoints(byte Player) {                    		// display the points of the selected player
	DisplayScore(Player, Points[Player]);}

void BlinkScore(byte State) {													// State = 0 -> stop blinking / State = 0 -> start blinking
	static byte Timer = 0;
	byte Buffer = 0;
	if ((State > 1) || ((State == 1) && !Timer)) {
		switch (Player) {
		case 1:                                           // for the players 1 and 3
		case 3:
			Buffer = 2;                                 		// start in column 1
			break;
		case 2:                                           // for the players 2 and 4
		case 4:
			Buffer = 18;                                		// start in column 9
			break;}
		if (State == 2) {																	// called by itself - score currently displayed
			if (Player < 3) {
				for (i=0; i<14; i++) {
					*(DisplayUpper+Buffer+i) = 0;}}
			else {
				for (i=0; i<14; i++) {
					*(DisplayLower+Buffer+i) = 0;}}
			Timer = ActivateTimer(300, 3, BlinkScore);}
		else {																						// turn off score display
			ShowPoints(Player);
			Timer = ActivateTimer(2000, 2, BlinkScore);}}
	else {
		if (!State) {
			if (Timer) {
				KillTimer(Timer);
				Timer = 0;}
			ShowPoints(Player);}}}

void DisplayBCD (byte Position, byte* BCDnumber) {		// displays BCD values on numerical displays
	if (Position) {																			// player display?
		if (Position < 3) {																// upper row
			Position--;
			for(byte c=0; c<7; c++) {
				*(DisplayLower+2+16*Position+2*c) = ConvertNumUpper(*(BCDnumber+c),(byte) *(DisplayLower+2+16*Position+2*c));
				*(DisplayLower+3+16*Position+2*c) = 254 & *(DisplayLower+3+16*Position+2*c);	// delete commas
				if (128 & *(BCDnumber+c)) {										// comma needed?
					*(DisplayLower+3+16*Position+2*c) = 1 | *(DisplayLower+3+16*Position+2*c);}}}
		else {																						// lower row
			Position = Position - 3;
			for(byte c=0; c<7; c++) {
				*(DisplayLower+2+16*Position+2*c) = ConvertNumLower(*(BCDnumber+c),(byte) *(DisplayLower+2+16*Position+2*c));
				*(DisplayLower+3+16*Position+2*c) = 127 & *(DisplayLower+3+16*Position+2*c);
				if (128 & *(BCDnumber+c)) {
					*(DisplayLower+3+16*Position+2*c) = 128 | *(DisplayLower+3+16*Position+2*c);}}}}
	else {																							// credit display
		*(DisplayLower) = ConvertNumUpper((byte) *BCDnumber,(byte) *(DisplayLower));
		*(DisplayLower+16) = ConvertNumUpper((byte) *(BCDnumber+1),(byte) *(DisplayLower+16));
		*(DisplayLower) = ConvertNumLower((byte) *(BCDnumber+2),(byte) *(DisplayLower));
		*(DisplayLower+16) = ConvertNumLower((byte) *(BCDnumber+3),(byte) *(DisplayLower+16));}}

void DisplayScore (byte Position, unsigned int Score) {
	byte i=0;                                       		// use a private counter
	byte Buffer1 = 0;
	byte Buffer2 = 0;
	switch (APC_settings[DisplayType]) {
	case 0:																							// 4 Alpha + Credit type display
	case 1:																							// Sys11 Pinbot type display
	case 2:																							// Sys11 F-14 type display
		switch (Position) {
		case 1:                                       		// for the players 1 and 3
		case 3:
			Buffer1 = 2;                             				// start in column 1
			break;
		case 2:                                       		// for the players 2 and 4
		case 4:
			Buffer1 = 18;                            				// start in column 9
			break;}
		if (Score) {                                  		// if the score is not 0
			while (Score && i<7) {                      		// for all 7 display digits
				Buffer2 = Score % 10;                 				// extract the least significant digit
				Score = (Score-Buffer2) / 10;         				// prepare for the next digit
				Buffer2 = 32+2*Buffer2;           						// determine the corresponding display pattern
				if (Position < 3) {                       		// depending on the player number show it in the upper display row
					if ((i==3) || (i==6)) {
						*(DisplayUpper+Buffer1+12-2*i) = 128 | DispPattern1[Buffer2];
						*(DisplayUpper+Buffer1+13-2*i) = 64 | DispPattern1[Buffer2+1];} // add a comma if necessary
					else {
						*(DisplayUpper+Buffer1+12-2*i) = DispPattern1[Buffer2];
						*(DisplayUpper+Buffer1+13-2*i) = DispPattern1[Buffer2+1];}}
				else {                                    		// the same for the lower display row
					if ((i==3) || (i==6)) {
						*(DisplayLower+Buffer1+12-2*i) = 1 | DispPattern2[Buffer2];
						*(DisplayLower+Buffer1+13-2*i) = 8 | DispPattern2[Buffer2+1];}
					else {
						*(DisplayLower+Buffer1+12-2*i) = DispPattern2[Buffer2];
						*(DisplayLower+Buffer1+13-2*i) = DispPattern2[Buffer2+1];}} //
				i++;}}
		else {                                        		// if the points are 0
			if (Position < 3) {
				*(DisplayUpper+Buffer1+12) = DispPattern1[32]; // just show two 0s
				*(DisplayUpper+Buffer1+13) = DispPattern1[33];
				*(DisplayUpper+Buffer1+10) = DispPattern1[32];
				*(DisplayUpper+Buffer1+11) = DispPattern1[33];}
			else {
				*(DisplayLower+Buffer1+12) = DispPattern2[32]; // just show two 0s
				*(DisplayLower+Buffer1+13) = DispPattern2[33];
				*(DisplayLower+Buffer1+10) = DispPattern2[32];
				*(DisplayLower+Buffer1+11) = DispPattern2[33];}}
		break;
		case 3:																						// Sys11 BK2K type display
		case 4:																						// Sys11 Taxi type display
		case 5:
			switch (Position) {
			case 1:                                       	// for the players 1 and 3
			case 3:
				Buffer1 = 0;                             			// start in column 0
				break;
			case 2:                                       	// for the players 2 and 4
			case 4:
				Buffer1 = 16;                            			// start in column 9
				break;}
			if (Score) {                                  	// if the score is not 0
				while (Score && i<8) {                      	// for all 7 display digits
					Buffer2 = Score % 10;                 			// extract the least significant digit
					Score = (Score-Buffer2) / 10;         			// prepare for the next digit
					Buffer2 = 32+2*Buffer2;           					// determine the corresponding display pattern
					if (Position < 3) {                       	// depending on the player number show it in the upper display row
						if ((i==3) || (i==6)) {
							*(DisplayUpper+Buffer1+14-2*i) = 128 | DispPattern1[Buffer2];
							*(DisplayUpper+Buffer1+15-2*i) = 64 | DispPattern1[Buffer2+1];} // add a comma if necessary
						else {
							*(DisplayUpper+Buffer1+14-2*i) = DispPattern1[Buffer2];
							*(DisplayUpper+Buffer1+15-2*i) = DispPattern1[Buffer2+1];}}
					else {                                    	// the same for the lower display row
						if ((i==3) || (i==6)) {
							*(DisplayLower+Buffer1+14-2*i) = 1 | DispPattern2[Buffer2];
							*(DisplayLower+Buffer1+15-2*i) = 8 | DispPattern2[Buffer2+1];}
						else {
							*(DisplayLower+Buffer1+14-2*i) = DispPattern2[Buffer2];
							*(DisplayLower+Buffer1+15-2*i) = DispPattern2[Buffer2+1];}} //
					i++;}}
			else {                                        	// if the points are 0
				if (Position < 3) {
					*(DisplayUpper+Buffer1+12) = DispPattern1[32]; // just show two 0s
					*(DisplayUpper+Buffer1+13) = DispPattern1[33];
					*(DisplayUpper+Buffer1+14) = DispPattern1[32];
					*(DisplayUpper+Buffer1+15) = DispPattern1[33];}
				else {
					*(DisplayLower+Buffer1+12) = DispPattern2[32]; // just show two 0s
					*(DisplayLower+Buffer1+13) = DispPattern2[33];
					*(DisplayLower+Buffer1+14) = DispPattern2[32];
					*(DisplayLower+Buffer1+15) = DispPattern2[33];}}
			break;
			case 6:																					// Sys3 - 6 type display
			case 7:																					// Sys7 - 9 type display
				switch (Position) {
				case 1:                                       // for the players 1 and 3
				case 3:
					Buffer1 = 2;                             		// start in column 1
					break;
				case 2:                                       // for the players 2 and 4
				case 4:
					Buffer1 = 18;                            		// start in column 9
					break;}
				if (Score) {                                  // if the score is not 0
					while (Score && i<7) {                      // for all 7 display digits
						Buffer2 = Score % 10;                 		// extract the least significant digit
						Score = (Score-Buffer2) / 10;         		// prepare for the next digit
						if (Position < 3) {                       // depending on the player number show it in the upper display row
							*(DisplayLower+Buffer1+12-2*i) = ConvertNumUpper(Buffer2,(byte) *(DisplayLower+Buffer1+12-2*i));
							if ((i==3) || (i==6)) {
								*(DisplayLower+Buffer1+13-2*i) = 1 | *(DisplayLower+Buffer1+13-2*i);}} // add a comma if necessary
						else {                                    // the same for the lower display row
							*(DisplayLower+Buffer1+12-2*i) = ConvertNumLower(Buffer2,(byte) *(DisplayLower+Buffer1+12-2*i));
							if ((i==3) || (i==6)) {
								*(DisplayLower+Buffer1+13-2*i) = 128 | *(DisplayLower+Buffer1+13-2*i);}}
						i++;}}
				else {                                        // if the points are 0 just show two 0s
					if (Position < 3) {
						*(DisplayLower+Buffer1+12) = ConvertNumUpper(0, (byte) *(DisplayLower+Buffer1+12));
						*(DisplayLower+Buffer1+10) = ConvertNumUpper(0, (byte) *(DisplayLower+Buffer1+12));}
					else {
						*(DisplayLower+Buffer1+12) = ConvertNumLower(0, (byte) *(DisplayLower+Buffer1+12));
						*(DisplayLower+Buffer1+10) = ConvertNumLower(0, (byte) *(DisplayLower+Buffer1+12));}}
				break;}}

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
			*(DisplayLower2+2*(Position-16)) = DispPattern2[32]; // just show 0
			*(DisplayLower2+2*(Position-16)+1) = DispPattern2[33];}}}

void ShowAllPoints(byte Dummy) {                  		// just a dummy event to access it via timer
	UNUSED(Dummy);
	WriteUpper("                ");                   		// erase display
	WriteLower("                ");
	for (byte i=1; i<=NoPlayers; i++) {                 // display the points of all active players
		ShowPoints(i);}}

void SwitchDisplay(byte Event) {                      // switch between different display buffers
	if (Event == 1) {
		DispRow1 = DisplayUpper;
		DispRow2 = DisplayLower;}
	else {
		DispRow1 = DisplayUpper2;
		DispRow2 = DisplayLower2;}}

void BlinkLamps(byte BlTimer) {
	byte c = 0;
	for (byte i=0; i<BlinkingNo[BlTimer]; i++) {        // for all lamps controlled by this blink timer
		while (!BlinkingLamps[BlTimer][c]) {     					// skip unused lamp slots
			c++;
			if (c > 65) {                          					// max 64 blink timers possible (starting from 1)
				ErrorHandler(3,BlTimer,BlinkingNo[BlTimer]);}}// show error 3
				if (BlinkState[BlTimer]) { 										// toggle the state of the lamps
					TurnOnLamp(BlinkingLamps[BlTimer][c]);}
				else {
					TurnOffLamp(BlinkingLamps[BlTimer][c]);}
		c++;}
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
				ErrorHandler(4,0,Lamp);}}         						// show error 4
		x = 0;
		if (Flag) {                                       // has a timer with the same period been found?
			while (BlinkingLamps[b][x]) {                   // search for a free lamp slot
				x++;
				if (x > 65) {                                 // max 64 blink timers possible (starting from 1)
					ErrorHandler(5,0,Lamp);}} 									// show error 5
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
						TurnOffLamp(LampNo);                     	// turn it off
						BlinkingNo[x]--;                          // decrease the number of lamps controlled by this timer
						if (!BlinkingNo[x]) {                     // = 0?
							KillTimer(BlinkTimer[x]);               // kill the timer
							BlinkTimer[x] = 0;                      // delete it from the list
							BlinkTimers--;}                         // decrease the number of blink timers
						break;}}                                  // and end the search
				y++;                                          // increase the counter for the list of this blink timer
				if (y > 64) {                                 // max 64 lamps existing (starting from 1)
					ErrorHandler(7,BlinkTimer[x],LampNo);}}}    // show error 7
		x++;                                              // increase the counter for the list of active blink timers
		if (x > 64) {                                     // max 64 blink timers possible (starting from 1)
			ErrorHandler(8,0,LampNo);}}}        						// show error 8

void ErrorHandler(unsigned int Error, unsigned int Number2, unsigned int Number3) {
	WriteUpper2("ERROR           ");                    // Show Error Message
	WriteLower2("                ");
	Serial.print("Error = ");
	Serial.println(Error);
	Serial.print("Number2 = ");
	Serial.println(Number2);
	Serial.print("Number3 = ");
	Serial.println(Number3);
	ShowNumber(15, Error);
	ShowNumber(23, Number2);
	ShowNumber(31, Number3);
	SwitchDisplay(0);
	if (APC_settings[DebugMode]) {
		LampPattern = NoLamps;                            // Turn off all lamps
		KillAllTimers();
		ReleaseAllSolenoids();
		while(true) {}}}

void ShowFileNotFound(String Filename) {							// show file not found message
	Filename.toUpperCase();															// convert filename to upper case characters
	char NameBuffer[14];																// create a temporary buffer
	Filename.toCharArray(NameBuffer, 14);								// fill buffer with filename
	WriteUpper2(NameBuffer);														// write filename to message buffer
	WriteLower2(" NOT    FOUND   ");
	ShowMessage(5);}																		// switch to message buffer for 5 seconds

void ShowLampPatterns(byte Step) {                    // shows a series of lamp patterns - start with step being one - stop with step being zero
	static byte Timer = 0;
	if ((Step > 1) || (Step ==1 && !Timer)) {						// no kill signal
		if (Step == 1) {
			Step++;}
		unsigned int Buffer = (PatPointer+Step-2)->Duration;  // buffer the duration for the current pattern
		if (StrobeLightsTimer) {
			LampBuffer = ((PatPointer+Step-2)->Pattern)-1;} // show the pattern
		else {
			LampPattern = ((PatPointer+Step-2)->Pattern)-1;}// show the pattern
		Step++;                                           // increase the pattern number
		if (!((PatPointer+Step-2)->Duration)) {           // if the duration for the next pattern is 0
			Step = 2;                                       // reset the pattern
			FlowRepeat--;                                   // decrease the number of repetitions
			if (!FlowRepeat) {                              // if no more repetitions pending
				Timer = 0;																		// indicate that the process has stopped
				if (LampReturn) {                             // is a return pointer given?
					LampReturn(0);}                             // call the procedure
				return;}}                                     // otherwise just quit
		Timer = ActivateTimer(Buffer, Step, ShowLampPatterns);}	// come back after the given duration
	else {																							// kill signal
		if (!Step) {
			if (Timer) {
				KillTimer(Timer);
				Timer = 0;}}}}

void StrobeLights(byte State) {
	if (State) {
		LampPattern = LampBuffer;                         // show the pattern
		State = 0;}
	else {
		LampPattern = NoLamps;
		State = 1;}
	StrobeLightsTimer = ActivateTimer(30, State, StrobeLights);}

void PlayMusic(byte Priority, const char* Filename) {
	if (StartMusic) {																		// already in startup phase?
		MusicFile.close();																// close the previous file
		StartMusic = 0;																		// cancel the startup
		MBP = 0;}																					// and neglect its data
	if (!PlayingMusic) {																// no music in play at the moment?
		MusicFile = SD.open(Filename);										// open file
		if (!MusicFile) {
			ShowFileNotFound(Filename);}
		else {
			if (Priority > 99) {
				MusicPriority = Priority -100;}
			else {
				MusicPriority = Priority;}
			MusicFile.read(MusicBuffer, 2*128);							// read first block
			StartMusic = true;															// indicate the startup phase
			MBP++;}}																				// increase read pointer
	else {																							// music already playing
		if (Priority > 99) {															// Priority > 99 means new prio has to be larger (not equal) to play
			Priority = Priority - 100;
			if (Priority > MusicPriority) {
				MusicPriority = Priority;
				MusicFile.close();														// close the old file
				MusicFile = SD.open(Filename);								// open the new one
				if (!MusicFile) {
					ShowFileNotFound(Filename);}
				else {
					StopMusic = 0;															// cancel a previous stop command
					if (!PlayingMusic) {												// neglect old data if still in the startup phase
						MBP = 0;}}}}
		else {
			if (Priority >= MusicPriority) {
				MusicPriority = Priority;
				MusicFile.close();														// close the old file
				MusicFile = SD.open(Filename);								// open the new one
				if (!MusicFile) {
					ShowFileNotFound(Filename);}
				else {
					StopMusic = 0;															// cancel a previous stop command
					if (!PlayingMusic) {												// neglect old data if still in the startup phase
						MBP = 0;}}}}}}

void StopPlayingMusic() {
	if (StartMusic || PlayingMusic) {
		MusicFile.close();
		if (StartMusic) {																	// during startup
			StartMusic = 0;																	// cancel startup
			MBP = 0;}																				// neglect data
		else {
			AfterMusicPending = 2;													// no AfterMusicEvent shall be executed
			StopMusic = MBP;}}}															// play the remaining data

void PlayRandomMusic(byte Priority, byte Amount, char* List) {
	Amount = random(Amount);
	PlayMusic(Priority, List+Amount*12);}

void PlayNextMusic() {
	PlayMusic(50, NextMusicName);}

void FadeOutMusic(byte Speed) {
	analogWrite(VolumePin, 255-ByteBuffer3);
	if (ByteBuffer3) {
		ByteBuffer3--;
		ActivateTimer(Speed, Speed, FadeOutMusic);}
	else {
		StopPlayingMusic();}}

void PlaySound(byte Priority, const char* Filename) {
	if (StartSound) {
		SoundFile.close();
		StartSound = 0;
		SBP = 0;}
	if (!PlayingSound) {																// no sound in play at the moment?
		SoundFile = SD.open(Filename);										// open file
		if (!SoundFile) {
			ShowFileNotFound(Filename);}
		else {
			if (Priority > 99) {
				SoundPriority = Priority -100;}
			else {
				SoundPriority = Priority;}
			SoundFile.read(SoundBuffer, 2*128);							// read first block
			StartSound = true;															// indicate the startup phase
			SBP++;}}																				// increase read pointer
	else {																							// music already playing
		if (Priority > 99) {															// Priority > 99 means new prio has to be larger (not equal) to play
			Priority = Priority - 100;
			if (Priority > SoundPriority) {
				SoundPriority = Priority;
				SoundFile.close();														// close the old file
				SoundFile = SD.open(Filename);								// open the new one
				if (!SoundFile) {
					ShowFileNotFound(Filename);}
				else {
					StopSound = 0;															// cancel a previous stop command
					if (!PlayingSound) {												// neglect old data if still in the startup phase
						SBP = 0;}}}}
		else {
			if (Priority >= SoundPriority) {
				SoundPriority = Priority;
				SoundFile.close();														// close the old file
				SoundFile = SD.open(Filename);								// open the new one
				if (!SoundFile) {
					ShowFileNotFound(Filename);}
				else {
					StopSound = 0;															// cancel a previous stop command
					if (!PlayingSound) {												// neglect old data if still in the startup phase
						SBP = 0;}}}}}}

void StopPlayingSound() {
	if (StartSound || PlayingSound) {
		SoundFile.close();
		if (StartSound) {																	// during startup
			StartSound = 0;																	// cancel startup
			SBP = 0;}																				// neglect data
		else {
			AfterSoundPending = 2;													// no AfterSoundEvent shall be executed
			StopSound = SBP;}}}

void PlayRandomSound(byte Priority, byte Amount, char* List) {
	Amount = random(Amount);
	PlaySound(Priority, List+Amount*12);}

void PlayNextSound() {
	PlaySound(50, NextSoundName);}

void Settings_Enter() {
	WriteUpper("   SETTINGS     ");                     // Show Test Mode
	WriteLower("                ");
	LampPattern = NoLamps;                              // Turn off all lamps
	AppByte = 0;
	AppByte2 = 0;
	Switch_Pressed = SelectSettings;
	ActivateTimer(2000, 0, SelectSettings);}         		// Wait 1 second and proceed to Display Test

void SelectSettings(byte Switch) {										// select system or game settings
	switch (Switch) {
	case 0:																							// for the initial call
		WriteUpper("SYSTEM SETTNGS  ");
		WriteLower("                ");
		if ((APC_settings[DisplayType] != 2) && (APC_settings[DisplayType] != 3) && (APC_settings[DisplayType] != 4) && (APC_settings[DisplayType] != 5)) { // No Credit display
			byte CreditBuffer[4];
			CreditBuffer[0] = 48;
			CreditBuffer[1] = 49;
			CreditBuffer[2] = 48;
			CreditBuffer[3] = 48;
			WritePlayerDisplay((char*) CreditBuffer, 0);}
		break;
	case 3:																							// start button
		if (AppByte) {																		// game settings selected
			SettingsPointer = game_settings;								// set pointer to the stored game setting values
			SettingsList = GameDefinition.GameSettingsList;	// set pointer to the list of menu topics
			SettingsFileName = GameDefinition.GameSettingsFileName;	// set the file name of the game settings
			AppByte = 0;}
		else {																						// system settings selected
			SettingsPointer = APC_settings;									// set pointer to the stored system setting values
			SettingsList = APC_setList;											// set pointer to the list of menu topics
			SettingsFileName = (char*)APC_set_file_name;}		// set the file name of the game settings
		Switch_Pressed = SelSetting;											// set SelSetting to be the switch handler
		SelSetting(0);																		// and initialize it first
		break;
	case 72:																						// advance button
		if (AppByte) {																		// switch between game and system settings
			WriteUpper("SYSTEM SETTNGS  ");
			WriteLower("                ");
			if (APC_settings[DisplayType] != 3) {						// not a BK2K display?
				byte CreditBuffer[4];
				CreditBuffer[0] = 48;
				CreditBuffer[1] = 49;
				CreditBuffer[2] = 48;
				CreditBuffer[3] = 48;
				WritePlayerDisplay((char*) CreditBuffer, 0);}
			AppByte = 0;}
		else {
			WriteUpper("  GAME SETTNGS  ");
			WriteLower("                ");
			if (APC_settings[DisplayType] != 3) {						// not a BK2K display?
				byte CreditBuffer[4];
				CreditBuffer[0] = 48;
				CreditBuffer[1] = 50;
				CreditBuffer[2] = 48;
				CreditBuffer[3] = 48;
				WritePlayerDisplay((char*) CreditBuffer, 0);}
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
	case 72:																						// Advance button pressed
		StopPlayingMusic();
		analogWrite(VolumePin, 255);
		if (QuerySwitch(73)) {														// go forward or backward depending on UpDown switch
			AppByte++;																			// show next setting
			if (!SettingsList[AppByte].EventPointer) {			// end marker of settings list reached?
				AppByte = 0;}}																// switch to the first
		else {
			if (!AppByte) {																	// already showing setting 0?
				while (SettingsList[AppByte].EventPointer) {	// search for the end marker of the settings list
					AppByte++;}}
			AppByte--;}																			// and go one back to reach the last settings entry
		/* no break */
	case 0:																							// show the current setting
		if (APC_settings[DisplayType] != 3) {							// not a Sys11c display?
			if (APC_settings[DisplayType] > 5) {						// numerical display?
				*(DisplayLower) = ConvertNumLower((byte) ((AppByte+1) - ((AppByte+1) % 10)) / 10,(byte) *(DisplayLower));
				*(DisplayLower+16) = ConvertNumLower((byte) ((AppByte+1) % 10),(byte) *(DisplayLower+16));}
			else {
				*(DisplayLower) = RightCredit[32+2*(((AppByte+1) - ((AppByte+1) % 10)) / 10)];
				*(DisplayLower+1) = RightCredit[32+2*(((AppByte+1) - ((AppByte+1) % 10)) / 10)+1];
				*(DisplayLower+16) = RightCredit[32+2*((AppByte+1) % 10)];
				*(DisplayLower+17) = RightCredit[32+2*((AppByte+1) % 10)+1];}}
		WriteUpper( SettingsList[AppByte].Text);					// show the text
		SettingsList[AppByte].EventPointer(false);				// call the corresponding method and indicate no changes
		break;
	case 3:
		if (!SettingsRepeatTimer) {												// start timer to determine whether key 3 is pressed longer than 1s
			SettingsRepeatTimer = ActivateTimer(1000, 0, RepeatSelectKey);
			Switch_Released = StopRepeatSelectKey;}
		SettingsList[AppByte].EventPointer(true);}}				// call the corresponding method and indicate changes

void HandleBoolSetting(bool change) {									// handling method for boolean settings
	if (change) {																				// if the setting has been changed
		AppByte2 = 1;																			// set the change indicator
		if (SettingsPointer[AppByte]) {										// invert the current setting
			SettingsPointer[AppByte] = 0;}
		else {
			SettingsPointer[AppByte] = 1;}}
	if (SettingsPointer[AppByte]) {											// show the current state of the setting
		if (APC_settings[DisplayType] > 5) {							// numerical display?
			WritePlayerDisplay((char*)"::::::1", 4);}
		else {
			WriteLower("           YES  ");}}
	else {
		if (APC_settings[DisplayType] > 5) {							// numerical display?
			WritePlayerDisplay((char*)"::::::0", 4);}
		else {
			WriteLower("            NO  ");}}}

void RestoreDefaults(bool change) {										// restore the default settings
	if (change) {																				// if the start button has been pressed
		AppByte2 = 1;																			// set the change indicator
		if (SettingsPointer == APC_settings) {						// system settings mode?
			for(i=0;i<64;i++) {															// change all settings to their default values
				*(SettingsPointer+i) = APC_defaults[i];}}
		else {																						// game settings mode
			for(i=0;i<64;i++) {															// change all settings to their default values
				*(SettingsPointer+i) = *(GameDefinition.GameDefaultsPointer+i);}}
		WriteLower("SETTNGS DONE    ");}
	else {
		WriteLower("SETTNGS         ");}}

void ExitSettings(bool change) {											// exit settings and save them if necessary
	if (SettingsRepeatTimer) {													// kill key repeat timer for button 3
		KillTimer(SettingsRepeatTimer);
		SettingsRepeatTimer = 0;}
	WriteLower("                ");
	if (change) {																				// if the start button has been pressed
		if (AppByte2) {																		// and the change indicator has been set
			if (SDfound) {																	// check for the SD card
				SD.remove(SettingsFileName);									// remove the old settings file
				File HighScore = SD.open(SettingsFileName,FILE_WRITE);  // open the settings file on the SD card
				HighScore.write((byte*) SettingsPointer, sizeof game_settings); // and write the settings array
				HighScore.close();
				WriteUpper("SETTNGS SAVED   ");}
			else {
				WriteUpper("  NO   SD CARD  ");}}
		else {																						// change indicator has not been set
			WriteUpper("  NO   CHANGES  ");}
		*(DisplayUpper) = 0;															// clear leftmost credit displays
		*(DisplayUpper+16) = 0;
		Init_System2(1);}}																// and quit to Attract Mode

void HandleNumSetting(bool change) {									// handling method for numeric settings
	if (change) {																				// if the start button has been pressed
		AppByte2 = 1;																			// set the change indicator
		if (QuerySwitch(73)) {														// go forward or backward depending on UpDown switch
			if (SettingsPointer[AppByte] != SettingsList[AppByte].UpperLimit) { // upper numeric limit reached?
				SettingsPointer[AppByte]++;}}									// if limit not reached just increase the numeric value
		else {																						// if the start button has not been pressed
			if (SettingsPointer[AppByte] != SettingsList[AppByte].LowerLimit) { // lower numeric limit reached?
				SettingsPointer[AppByte]--;}}}								// if limit not reached just decrease the numeric value
	WriteLower("                ");
	DisplayScore(4,SettingsPointer[AppByte]);}					// show the current value

void HandleTextSetting(bool change) {									// handling method for text settings
	if (change) {																				// if the start button has been pressed
		AppByte2 = 1;																			// set the change indicator
		if (QuerySwitch(73)) {														// go forward or backward depending on UpDown switch
			if (SettingsPointer[AppByte] == SettingsList[AppByte].UpperLimit) { // last text setting reached?
				SettingsPointer[AppByte] = 0;}								// start from 0
			else {
				SettingsPointer[AppByte]++;}}									// if limit not reached just choose the next entry
		else {
			if (!SettingsPointer[AppByte]) {								// entry 0 reached?
				SettingsPointer[AppByte] = SettingsList[AppByte].UpperLimit;} // go to the last entry
			else {
				SettingsPointer[AppByte]--;}}}								// if limit not reached just choose the previous entry
	if (APC_settings[DisplayType] > 5) {								// numerical display?
		WriteLower("                ");
		DisplayScore(4,SettingsPointer[AppByte]);}
	else {
		WriteLower(SettingsList[AppByte].TxTpointer+17*SettingsPointer[AppByte]);}} // show the current text element

void HandleVolumeSetting(bool change) {
	HandleNumSetting(change);
	if (!change) {
		PlayMusic(90, "MUSIC.BIN");}
	analogWrite(VolumePin,255-APC_settings[Volume]);}   // adjust PWM to volume setting

byte HandleHighScores(unsigned int Score) {
	byte Position = 0;
	while (HallOfFame.Scores[Position] > Score) {  			// check to which position of the highscore list it belongs
		Position++;}
	for (i=3; i>Position; i--) {                   			// move all lower highscores down
		HallOfFame.Scores[i] = HallOfFame.Scores[i-1];}
	for (i=9; i>Position*3; i--) {
		HallOfFame.Initials[i+2] = HallOfFame.Initials[i-1];}
	HallOfFame.Scores[Position] = Score;           			// and include the new highscore to the list
	for (i=0; i<3; i++) {
		HallOfFame.Initials[Position*3+i] = EnterIni[i];} // copy initials
	if (SDfound) {
		SD.remove(GameDefinition.HighScoresFileName);
		File HighScore = SD.open(GameDefinition.HighScoresFileName,FILE_WRITE);  // open the highscore file on the SD card
		HighScore.write((byte*) &HallOfFame, sizeof HallOfFame); // and write the HallOfFame structure
		HighScore.close();}
	else {
		WriteUpper("  NO   SD CARD  ");
		delay(2000);}
	return Position;}
