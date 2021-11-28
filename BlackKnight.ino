// Rules for the Black Knight pinball machine

byte LeftMagna[5];                                    // left magna save available? (for every player)
bool LeftAfterMagna = false;                          // number of the timer for the after magna save time
byte RightMagna[5];                                   // right magna save available? (for every player)
bool RightAfterMagna = false;                         // number of the timer for the after magna save time
bool LastChance = false;                              // last chance active?
bool LastChanceActive = false;                        // give a last chance ball
bool LastChanceOver = false;                          // did the player already have a last chance?
byte RightMysteryTimer = 0;                           // number of the timer for the right mystery (if active)
byte LeftMysteryTimer = 0;                            // number of the timer for the left mystery (if active)
byte TimedRightMagnaTimer = 0;                        // number of the timer for the right timed magna save
byte TimedLeftMagnaTimer = 0;                         // number of the timer for the left timed magna save
byte BonusToAdd = 0;                                  // bonus points to be added by AddBonus()
byte BonusCountTime = 0;                              // counter to reduce the waiting time between the bonus counts
const byte BonusLamp = 48;                            // first lamp of the bonus ladder
bool LowerExBall[5];                                  // extra ball in turnaround pending?
bool UpperExBall[5];                                  // extra ball in left ramp pending?
bool PlayersExBalls[5];                               // did the player already get an extra ball?
byte DropTimer[4];                                    // timer for all drop target banks
byte DropHits[16];                                    // counts how often the target banks have been cleared
bool DropWait[5];                                     // indicates that a waiting time for this drop target bank is active before it it being processed
const byte BallSearchCoils[12] = {1,8,10,9,2,3,4,5,7,19,15,0}; // coils to fire when the ball watchdog timer runs out
const unsigned int BK_SolTimes[24] = {30,50,50,50,50,10,50,50,1999,1999,0,5,5,5,999,999,50,50,50,5,5,5,0,0}; // Activation times for solenoids
const char BK_TestSounds[16][15] = {{"0_2f.snd"},{"0_30.snd"},{"0_31.snd"},{"0_32.snd"},{"0_33.snd"},{"0_15.snd"},{"0_36.snd"},{"0_37.snd"},
    {"0_38.snd"},{"0_39.snd"},{"0_3a.snd"},{"0_3b.snd"},{"0_3c.snd"},{"BK_E14.bin"},{"0_3e.snd"},0};

                                                      // offsets of settings in the settings array
#define TimedMagna 0                                  // Timed Magna saves?
#define ReplayScore 1                                 // without credits being implemented just show some respect
#define MultiballJackpot 2                            // optional multiball jackpot
#define BK_MultiballVolume 3                          // increase volume during multiball

const byte BK_defaults[64] = {0,0,0,0,0,0,0,0,        // game default settings
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0};

const char TxTJackpot[4][17] = {{"           OFF  "},{"        500000  "},{"        750000  "},{"       1000000  "}};
const char TxTReplayScore[4][17] = {{"       1000000  "},{"       1500000  "},{"       2000000  "},{"       2500000  "}};

const struct SettingTopic BK_setList[8] = {{" TIMED  MAGNA ",HandleBoolSetting,0,0,0},
                                    {" REPLAY SCORE ",HandleTextSetting,&TxTReplayScore[0][0],0,3},
                                    {" MBALL JACKPOT",HandleTextSetting,&TxTJackpot[0][0],0,3},
                                    {"MULTBALVOLUME ",BK_HandleVolumeSetting,0,0,30},
                                    {" RESET  HIGH  ",ResetHighScores,0,0,0},
                                    {"RESTOREDEFAULT",RestoreDefaults,0,0,0},
                                    {"  EXIT SETTNGS",ExitSettings,0,0,0},
                                    {"",NULL,0,0,0}};

                                //  Duration..11111110...22222111...33322222...43333333...44444444...55555554...66666555
                                //  Duration..65432109...43210987...21098765...09876543...87654321...65432109...43210987
const struct LampPat ExBallPat[13] =   {{50,0b10000000,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000},
                                        {50,0b01000000,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000},
                                        {50,0b01001000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {50,0b00101000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {50,0b00100000,0b00000000,0b00010000,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {50,0b00000000,0b00000000,0b00110000,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {50,0b00000000,0b00000000,0b01100000,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {50,0b00000000,0b00000000,0b01000100,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {50,0b00000000,0b00000000,0b00000110,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {50,0b00000000,0b00000000,0b00000011,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {50,0b00000100,0b00000000,0b00000001,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {50,0b10000100,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {0,0,0,0,0,0,0,0}};

const struct LampPat AttractPat1[52] = {{30,0b00000000,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000},
                                        {30,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00000000,0b00000000},
                                        {30,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00000001,0b00000000},
                                        {30,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00000011,0b00000000},
                                        {30,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00000111,0b11110000},
                                        {30,0b11000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00001111,0b11110000},
                                        {30,0b11000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00011111,0b11110000},
                                        {30,0b11000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00111111,0b11110000},
                                        {30,0b11000000,0b00000000,0b00000000,0b00000000,0b11000000,0b01111111,0b11110000},
                                        {30,0b11000000,0b00000000,0b00000000,0b00000000,0b11000000,0b11111111,0b11110000},
                                        {30,0b11001100,0b00000000,0b10001000,0b00000000,0b11000000,0b11111111,0b11110001},
                                        {30,0b11001111,0b00000000,0b10001000,0b00000000,0b11000000,0b11111111,0b11110011},
                                        {30,0b11001111,0b00000000,0b10001001,0b00000000,0b11000000,0b11111111,0b11110111},
                                        {30,0b11101111,0b00000000,0b10001011,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b00000000,0b10001111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b00000001,0b10011111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b00000001,0b10111111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b00000001,0b11111111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b10000011,0b11111111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11111111,0b10100011,0b11111111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11111111,0b11100011,0b11111111,0b00010001,0b11000000,0b11111111,0b11111111},
                                        {30,0b11111111,0b11100011,0b11111111,0b00110011,0b11000000,0b11111111,0b11111111},
                                        {30,0b11111111,0b11100011,0b11111111,0b11110111,0b11000000,0b11111111,0b11111111},
                                        {30,0b11111111,0b11101111,0b11111111,0b11110111,0b11000000,0b11111111,0b11111111},
                                        {30,0b11111111,0b11101111,0b11111111,0b11111111,0b11000001,0b11111111,0b11111111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11111111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b10000011,0b11111111,0b11111111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11111111,0b11111111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11111110,0b11111111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11111100,0b11111111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11111000,0b00001111},
                                        {30,0b00111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11110000,0b00001111},
                                        {30,0b00111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11100000,0b00001111},
                                        {30,0b00111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11000000,0b00001111},
                                        {30,0b00111111,0b11111111,0b11111111,0b11111111,0b00000011,0b10000000,0b00001111},
                                        {30,0b00111111,0b11111111,0b11111111,0b11111111,0b00000011,0b00000000,0b00001111},
                                        {30,0b00110011,0b11111111,0b01110111,0b11111111,0b00000011,0b00000000,0b00001110},
                                        {30,0b00110000,0b11111111,0b01110111,0b11111111,0b00000011,0b00000000,0b00001100},
                                        {30,0b00110000,0b11111111,0b01110110,0b11111111,0b00000011,0b00000000,0b00001000},
                                        {30,0b00010000,0b11111111,0b01110100,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00010000,0b11111111,0b01110000,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00010000,0b11111110,0b01100000,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00010000,0b11111110,0b01000000,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00010000,0b11111110,0b00000000,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00000000,0b01111100,0b00000000,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00000000,0b00011100,0b00000000,0b11101110,0b00000011,0b00000000,0b00000000},
                                        {30,0b00000000,0b00011100,0b00000000,0b11001100,0b00000011,0b00000000,0b00000000},
                                        {30,0b00000000,0b00011100,0b00000000,0b00001000,0b00000011,0b00000000,0b00000000},
                                        {30,0b00000000,0b00010000,0b00000000,0b00001000,0b00000011,0b00000000,0b00000000},
                                        {30,0b00000000,0b00010000,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000},
                                        {30,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                         {0,0,0,0,0,0,0,0}};

const struct LampPat AttractPat2[4] = {{250,0b11111100,0b10000100,0b00100100,0b11000001,0b01000000,0b10010010,0b00000100},
                                       {250,0b00000000,0b00111001,0b11001010,0b00100010,0b10000000,0b00100100,0b01101001},
                                       {250,0b00000011,0b01000010,0b00010001,0b00011100,0b00000011,0b01001001,0b10010010},
                                         {0,0,0,0,0,0,0,0}};

const struct LampPat AttractPat3[6] = {{90,0b00000000,0b00000001,0b00100001,0b00100010,0b01000000,0b00001000,0b00000001},
                                       {90,0b00100000,0b10000000,0b01000010,0b11000100,0b10000000,0b00010000,0b00000010},
                                       {90,0b00011100,0b00001110,0b00000100,0b00000000,0b00000001,0b00100001,0b00000100},
                                       {90,0b11000000,0b00100000,0b00000000,0b00001000,0b00000000,0b01000010,0b10011000},
                                       {90,0b00000011,0b01010000,0b10011000,0b00010001,0b00000010,0b10000100,0b01100000},
                                        {0,0,0,0,0,0,0,0}};

const struct LampPat AttractPat4[52] = {{30,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                        {30,0b00000000,0b00010000,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000},
                                        {30,0b00000000,0b00010000,0b00000000,0b00001000,0b00000011,0b00000000,0b00000000},
                                        {30,0b00000000,0b00011100,0b00000000,0b00001000,0b00000011,0b00000000,0b00000000},
                                        {30,0b00000000,0b00011100,0b00000000,0b11001100,0b00000011,0b00000000,0b00000000},
                                        {30,0b00000000,0b00011100,0b00000000,0b11101110,0b00000011,0b00000000,0b00000000},
                                        {30,0b00000000,0b01111100,0b00000000,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00010000,0b11111110,0b00000000,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00010000,0b11111110,0b01000000,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00010000,0b11111110,0b01100000,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00010000,0b11111111,0b01110000,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00010000,0b11111111,0b01110100,0b11111111,0b00000011,0b00000000,0b00000000},
                                        {30,0b00110000,0b11111111,0b01110110,0b11111111,0b00000011,0b00000000,0b00001000},
                                        {30,0b00110000,0b11111111,0b01110111,0b11111111,0b00000011,0b00000000,0b00001100},
                                        {30,0b00110011,0b11111111,0b01110111,0b11111111,0b00000011,0b00000000,0b00001110},
                                        {30,0b00111111,0b11111111,0b11111111,0b11111111,0b00000011,0b00000000,0b00001111},
                                        {30,0b00111111,0b11111111,0b11111111,0b11111111,0b00000011,0b10000000,0b00001111},
                                        {30,0b00111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11000000,0b00001111},
                                        {30,0b00111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11100000,0b00001111},
                                        {30,0b00111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11110000,0b00001111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11111000,0b00001111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11111100,0b11111111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11111110,0b11111111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b00000011,0b11111111,0b11111111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b10000011,0b11111111,0b11111111},
                                        {30,0b11111111,0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11111111},
                                        {30,0b11111111,0b11101111,0b11111111,0b11111111,0b11000001,0b11111111,0b11111111},
                                        {30,0b11111111,0b11101111,0b11111111,0b11110111,0b11000000,0b11111111,0b11111111},
                                        {30,0b11111111,0b11100011,0b11111111,0b11110111,0b11000000,0b11111111,0b11111111},
                                        {30,0b11111111,0b11100011,0b11111111,0b11001100,0b11000000,0b11111111,0b11111111},
                                        {30,0b11111111,0b11100011,0b11111111,0b00010001,0b11000000,0b11111111,0b11111111},
                                        {30,0b11111111,0b10100011,0b11111111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b10000011,0b11111111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b00000001,0b11111111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b00000001,0b10111111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b00000001,0b10011111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b00000000,0b10001111,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11101111,0b00000000,0b10001011,0b00000000,0b11000000,0b11111111,0b11111111},
                                        {30,0b11001111,0b00000000,0b10001001,0b00000000,0b11000000,0b11111111,0b11110111},
                                        {30,0b11001111,0b00000000,0b10001000,0b00000000,0b11000000,0b11111111,0b11110011},
                                        {30,0b11001100,0b00000000,0b10001000,0b00000000,0b11000000,0b11111111,0b11110001},
                                        {30,0b11000000,0b00000000,0b00000000,0b00000000,0b11000000,0b11111111,0b11110000},
                                        {30,0b11000000,0b00000000,0b00000000,0b00000000,0b11000000,0b01111111,0b11110000},
                                        {30,0b11000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00111111,0b11110000},
                                        {30,0b11000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00011111,0b11110000},
                                        {30,0b11000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00001111,0b11110000},
                                        {30,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00000111,0b11110000},
                                        {30,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00000011,0b00000000},
                                        {30,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00000001,0b00000000},
                                        {30,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,0b00000000,0b00000000},
                                        {30,0b00000000,0b00000000,0b00000000,0b00000000,0b01000000,0b00000000,0b00000000},
                                        {0,0,0,0,0,0,0,0}};

const struct LampPat AttractPat5[17] = {{50,0b01000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000100,0b11000000},
                                        {50,0b01000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000,0b11000000},
                                        {50,0b00001001,0b00000000,0b10000000,0b00000000,0b00000000,0b00010000,0b00000000},
                                        {50,0b00001001,0b00000000,0b10000000,0b00000000,0b00000000,0b00100000,0b00000000},
                                        {50,0b00100000,0b10000000,0b00010000,0b10000000,0b00000000,0b01000000,0b00000000},
                                        {50,0b00100000,0b10000000,0b00010000,0b10000000,0b00000000,0b10000000,0b00000000},
                                        {50,0b00000000,0b00011000,0b00000000,0b01110000,0b00000000,0b00000000,0b00000001},
                                        {50,0b00000000,0b00011000,0b00000000,0b01110000,0b00000000,0b00000000,0b00000010},
                                        {50,0b00000000,0b00000100,0b00000000,0b00000110,0b00000010,0b00000000,0b00000100},
                                        {50,0b00000000,0b00000100,0b00000000,0b00000110,0b00000010,0b00000000,0b00001000},
                                        {50,0b00010000,0b01100000,0b01100000,0b00000001,0b00000001,0b00000000,0b00000000},
                                        {50,0b00010000,0b01100000,0b01000000,0b00001000,0b00000001,0b00000000,0b00000000},
                                        {50,0b00000000,0b00000001,0b00000111,0b00000000,0b01000000,0b00000000,0b00000000},
                                        {50,0b00000000,0b00000001,0b00000111,0b00000000,0b10000000,0b00000000,0b00000000},
                                        {50,0b10000110,0b00000000,0b00001000,0b00000000,0b00000000,0b00000001,0b00110000},
                                        {50,0b10000110,0b00000000,0b00001000,0b00000000,0b00000000,0b00000010,0b00110000},
                                        {0,0,0,0,0,0,0,0}};

const struct LampFlow AttractFlow[6] = {{3,AttractPat1},{10,AttractPat2},{3,AttractPat4},{10,AttractPat3},{10,AttractPat5},{0,0}};
const char BK_NewGameSounds[5][13] = {{"0_2b_001.snd"},{"0_2b_002.snd"},{"0_2b_003.snd"},{"0_2b_004.snd"},{"0_2b_005.snd"}};
const byte BonusLamps[13] = {60,59,58,57,56,55,54,53,52,51,50,49,48}; // numbers of the bonus lamps
const byte BonusValues[13] = {40,30,20,10,9,8,7,6,5,4,3,2,1}; // values of the bonus lamps
const byte BK_ResetAllDropTargets[9] = {2,15,3,15,4,15,5,15,0};
const byte FirstMultLamp = 61;
const byte DropTargets[4] = {25, 29, 33, 37};
const byte DropSolenoid = 2;
const byte DropLamp = 17;
const byte BK_ACselectRelay = 0;                      // no A/C select relay

struct GameDef BK_GameDefinition = {
  BK_setList,                                         // GameSettingsList
  (byte*)BK_defaults,                                 // GameDefaultsPointer
  "BK_SET.BIN",                                       // GameSettingsFileName
  "BKSCORES.BIN",                                     // HighScoresFileName
  BK_AttractMode,                                     // AttractMode
  BK_SolTimes};                                       // Default activation times of solenoids

void BK_init() {
  digitalWrite(VolumePin,HIGH);                       // mute sound
  if (APC_settings[DebugMode]) {                      // activate serial interface in debug mode
    Serial.begin(115200);}
  ACselectRelay = BK_ACselectRelay;                   // assign the number of the A/C select relay
  GameDefinition = BK_GameDefinition;}                // read the game specific settings and highscores

void BK_AttractMode() {                               // Attract Mode
  DispRow1 = DisplayUpper;
  DispRow2 = DisplayLower;
  LampPattern = NoLamps;
  Switch_Pressed = AttractModeSW;
  Switch_Released = DummyProcess;
  AppByte2 = 0;
  LampReturn = AttractLampCycle;
  AddBlinkLamp(4, 150);                               // blink Game Over lamp
  ActivateTimer(1000, 0, AttractLampCycle);
  AttractDisplayCycle(0);}

void CheckForLockedBalls(byte Event) {                // check if balls are locked and release them
  UNUSED(Event);
  if (QuerySwitch(20)) {                              // for the outhole
    ActivateSolenoid(0, 1);}
  else if (QuerySwitch(24)) {                         // for the lower eject hole
    ActivateSolenoid(0, 8);}
  else if (QuerySwitch(41)) {                         // for the multiball lock
    ActivateSolenoid(0, 7);}}

void AttractLampCycle(byte Event) {                   // play multiple lamp pattern series
  UNUSED(Event);
  PatPointer = AttractFlow[AppByte2].FlowPat;         // set the pointer to the current series
  FlowRepeat = AttractFlow[AppByte2].Repeat;          // set the repetitions
  if (AppByte2 == 2) {
    StrobeLights(3);}
  else {
    StrobeLights(0);}
  AppByte2++;                                         // increase counter
  if (!AttractFlow[AppByte2].Repeat) {                // repetitions of next series = 0?
    AppByte2 = 0;}                                    // reset counter
  ShowLampPatterns(1);}                               // call the player

void AttractScroll(byte Dummy) {
  UNUSED(Dummy);
  WriteUpper2("    APC       ");
  AddScrollUpper(0);}

void AttractDisplayCycle(byte Event) {
  CheckForLockedBalls(0);
  switch (Event) {
  case 0:
    WriteUpper2("  THE         ");
    ActivateTimer(50, 0, ScrollUpper);
    ActivateTimer(900, 0, AttractScroll);
    WriteLower2(" BLACK KNIGHT ");
    ActivateTimer(1400, 0, ScrollLower);
    if (NoPlayers) {
      Event++;}
    else {
      Event = 2;}
    break;
  case 1:
    WriteUpper2("              ");                    // erase display
    WriteLower2("              ");
    for (i=1; i<=NoPlayers; i++) {                    // display the points of all active players
      ShowNumber(8*i-1, Points[i]);}
    ActivateTimer(50, 0, ScrollUpper);
    ActivateTimer(900, 0, ScrollLower);
    Event++;
    break;
  case 2:                                             // Show highscores
    AddBlinkLamp(6, 150);                             // blink Highest Score lamp
    WriteUpper2("1>            ");
    WriteLower2("2>            ");
    for (i=0; i<3; i++) {
      *(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[i]-32)*2];
      *(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[i]-32)*2+1];
      *(DisplayLower2+8+2*i) = DispPattern2[(HallOfFame.Initials[3+i]-32)*2];
      *(DisplayLower2+8+2*i+1) = DispPattern2[(HallOfFame.Initials[3+i]-32)*2+1];}
    ShowNumber(15, HallOfFame.Scores[0]);
    ShowNumber(31, HallOfFame.Scores[1]);
    ActivateTimer(50, 0, ScrollUpper);
    ActivateTimer(900, 0, ScrollLower);
    Event++;
    break;
  case 3:
    RemoveBlinkLamp(6);                               // stop blinking of Highest Score lamp
    WriteUpper2("3>            ");
    WriteLower2("4>            ");
    for (i=0; i<3; i++) {
      *(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2];
      *(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2+1];
      *(DisplayLower2+8+2*i) = DispPattern2[(HallOfFame.Initials[9+i]-32)*2];
      *(DisplayLower2+8+2*i+1) = DispPattern2[(HallOfFame.Initials[9+i]-32)*2+1];}
    ShowNumber(15, HallOfFame.Scores[2]);
    ShowNumber(31, HallOfFame.Scores[3]);
    ActivateTimer(50, 0, ScrollUpper);
    ActivateTimer(900, 0, ScrollLower);
    Event = 0;}
  ActivateTimer(4000, Event, AttractDisplayCycle);}

void AttractModeSW(byte Event) {                      // Attract Mode switch behaviour
  switch (Event) {
  case 8:                                             // high score reset
    digitalWrite(Blanking, LOW);                      // invoke the blanking
    break;
  case 20:                                            // outhole
    ActivateTimer(200, 0, CheckForLockedBalls);       // check again in 200ms
    break;
  case 72:                                            // Service Mode
    StrobeLights(0);
    RightMysteryTimer = 0;
    LeftMysteryTimer = 0;
    LockChaseLight(0);
    ShowLampPatterns(0);
    BlinkScore(0);
    BallWatchdogTimer = 0;
    TimedRightMagnaTimer = 0;
    TimedLeftMagnaTimer = 0;
    CheckReleaseTimer = 0;
    RemoveBlinkLamp(4);                               // stop blinking of Game Over lamp
    RemoveBlinkLamp(6);                               // stop blinking of Highest Score lamp
    TestMode_Enter();
    break;
  case 3:                                             // start game
    if (CountBallsInTrunk() == 3 || (CountBallsInTrunk() == 2 && QuerySwitch(45))) { // Ball missing?
      StrobeLights(0);
      randomSeed(TC_ReadCV(TC2, 1));
      AfterSound = BK_StartBgMusic;
      PlayRandomSound(61, 5, (char *)BK_NewGameSounds);
      ShowLampPatterns(0);
      RemoveBlinkLamp(4);
      RemoveBlinkLamp(6);
      Switch_Pressed = DummyProcess;                  // Switches do nothing
      KillAllTimers();                                // stop the lamp cycling
      if (APC_settings[Volume]) {                     // system set to digital volume control?
        analogWrite(VolumePin,255-APC_settings[Volume]);} // adjust PWM to volume setting
      else {
        digitalWrite(VolumePin,HIGH);}                // turn off the digital volume control
      for (i=0; i< 8; i++) {
        LampColumns[i] = 0;}
      LampPattern = LampColumns;
      TurnOnLamp(2);                                  // turn on Ball in Play lamp
      NoPlayers = 0;
      WriteUpper("              ");
      WriteLower("              ");
      Ball = 1;
      ResetAllDropTargets(0);
      AddPlayer();
      Player = 1;
      ExBalls = 0;
      Bonus = 1;
      BonusMultiplier = 1;
      for (i=0; i<16; i++) {
        DropHits[i] = 0;}                             // clear drop target hits
      InLock = 0;
      Multiballs = 1;
      for (i=1; i < 5; i++) {
        LowerExBall[i] = false;
        UpperExBall[i] = false;
        PlayersExBalls[i] = false;
        RightMagna[i] = 0;
        LeftMagna[i] = 0;
        LockedBalls[i] = 0;
        Points[i] = 0;}
      NewBall(3);                                     // release a new ball (3 expected balls in the trunk)
      //digitalWrite(SpcSolEnable, HIGH);               // enable special solenoids
      ActivateSolenoid(0, 23);                        // enable flipper fingers
      ActivateSolenoid(0, 24);}}}

void ResetAllDropTargets(byte Dummy) {
  for (Dummy=0; Dummy <4; Dummy++) {                  // Dummy counts through the drop target banks
    for (i=0; i<3;i++) {                              // for each target
      if (QuerySwitch(DropTargets[Dummy]+i)) {        // target down?
        ActivateSolenoid(0, DropSolenoid+Dummy);      // reset
        break;}}}}                                    // quit for next bank

void AddPlayer() {
  if ((NoPlayers < 4) && (Ball == 1)) {               // if actual number of players < 4
    NoPlayers++;                                      // add a player
    Points[NoPlayers] = 0;                            // delete the points of the new player
    ShowPoints(NoPlayers);}}                          // and show them

void NewBall(byte Balls) {                            // release ball (Balls = expected balls on ramp)
  ShowAllPoints(0);
  ShowBonus();
  if (!((Player == 1) && (Ball == 1) && !InLock)) {   // skip for the first ball of the game to wait for speech sequence
    BK_StartBgMusic();}
  *(DisplayUpper+16) = LeftCredit[32 + 2 * Ball];     // show current ball in left credit
  //*(DisplayUpper+17) = LeftCredit[33 + 2 * Ball];
  LockChaseLight(1);
  BlinkScore(1);                                      // start score blinking
  TurnOnLamp(36);                                     // bumper light on
  for (byte i=0; i<4; i++) {                          // restore the drop target lamps of the current player
    for (byte c=0; c < 3; c++) {                      // for all drop target banks
      if (DropHits[(4*(Player-1))+i] > c) {
        TurnOnLamp(DropTargets[i]+c);}
      else {
        TurnOffLamp(DropTargets[i]+c);}}}
  if (LowerExBall[Player]) {                          // restore extra ball lamps
    TurnOnLamp(23);}
  else {
    TurnOffLamp(23);}
  if (UpperExBall[Player]) {
    TurnOnLamp(41);}
  else {
    TurnOffLamp(41);}
  if (game_settings[TimedMagna]) {                    // if timed magna save mode
    if (LeftMagna[Player]) {
      if (LeftMagna[Player] == 5) {                   // 5 seconds reached?
        RemoveBlinkLamp(10);                          // turn off blinking lamp
        TurnOnLamp(10);}                              // and switch it on permanently
      else {
        AddBlinkLamp(10, 150*LeftMagna[Player]);}}
    else {
      RemoveBlinkLamp(10);                            // turn off blinking lamp
      TurnOffLamp(10);}                               // and switch it off permanently
    if (RightMagna[Player]) {
      if (RightMagna[Player] == 5) {                  // 5 seconds reached?
        RemoveBlinkLamp(9);                           // turn off blinking lamp
        TurnOnLamp(9);}                               // and switch it on permanently
      else {
        AddBlinkLamp(9, 150*RightMagna[Player]);}}
    else {
      RemoveBlinkLamp(9);                             // turn off blinking lamp
      TurnOffLamp(9);}}                               // and switch it off permanently
  else {
    if (LeftMagna[Player]) {
      TurnOnLamp(10);}
    else {
      TurnOffLamp(10);}
    if (RightMagna[Player]) {
      TurnOnLamp(9);}
    else {
      TurnOffLamp(9);}}
  Switch_Released = CheckShooterLaneSwitch;
  if (!QuerySwitch(45)) {
    ActivateSolenoid(0, 6);                           // release ball
    Switch_Pressed = BallReleaseCheck;                // set switch check to enter game
    CheckReleaseTimer = ActivateTimer(5000, Balls-1, CheckReleasedBall);} // start release watchdog
  else {
    Switch_Pressed = ResetBallWatchdog;}}

void CheckShooterLaneSwitch(byte Switch) {
  if (Switch == 45) {                                 // shooter lane switch released?
    Switch_Released = DummyProcess;
    StopPlayingMusic();
    PlaySound(50, "0_33.snd");
    if (!BallWatchdogTimer) {
      BallWatchdogTimer = ActivateTimer(30000, 0, SearchBall);}}}

void BallReleaseCheck(byte Switch) {                  // handle switches during ball release
  if ((Switch > 10)&&(Switch != 17)&&(Switch != 18)&&(Switch != 19)&&(Switch != 46)) { // playfield switch activated?
    if (CheckReleaseTimer) {
      KillTimer(CheckReleaseTimer);
      CheckReleaseTimer = 0;}                         // stop watchdog
    Switch_Pressed = ResetBallWatchdog;
    if (Switch == 45) {                               // ball is in the shooter lane
      Switch_Released = CheckShooterLaneSwitch;}      // set mode to register when ball is shot
    else {
      if (!BallWatchdogTimer) {
        BallWatchdogTimer = ActivateTimer(30000, 0, SearchBall);}}} // set switch mode to game
  GameMain(Switch);}                                  // process current switch

void ResetBallWatchdog(byte Event) {                  // handle switches during ball release
  if ((Event > 10)&&(Event != 17)&&(Event != 18)&&(Event != 19)&&(Event != 46)) { // playfield switch activated?
    if (BallWatchdogTimer) {
      KillTimer(BallWatchdogTimer);}                  // stop watchdog
    BallWatchdogTimer = ActivateTimer(30000, 0, SearchBall);}
  GameMain(Event);}                                   // process current switch

void SearchBall(byte Counter) {                       // ball watchdog timer has run out
  BallWatchdogTimer = 0;
  if (QuerySwitch(20)) {
    BlockOuthole = false;
    ActivateTimer(1000, 0, ClearOuthole);}
  else {
    if (QuerySwitch(45)) {                            // if ball is waiting to be launched
      BallWatchdogTimer = ActivateTimer(30000, 0, SearchBall);} // restart watchdog
    else {                                            // if ball is really missing
      byte c = CountBallsInTrunk();                   // recount all balls
      if (c == 3) {                                   // found 3 balls in trunk?
        BallEnd(0);}                                  // ball has probably jumped over the outhole switch
      else {
        byte c2 = 0;
        for (i=0; i<3; i++) {                         // count balls in lock
          if (QuerySwitch(41+i)) {
            if (c2 < i) {
              c = 5;}                                 // set warning flag
            c2++;}}
        if (c == 5) {                                 // balls have not settled yet
          WriteUpper("  BALL  STUCK ");
          BallWatchdogTimer = ActivateTimer(1000, 0, SearchBall);} // and try again in 1s
        else {
          if (c2 > InLock) {                          // more locked balls found than expected?
            HandleLock(0);                            // lock them
            BallWatchdogTimer = ActivateTimer(30000, 0, SearchBall);}
          else {
            if (c + c2 == 3) {                        // all balls found
              BallEnd(0);}                            // ball has probably jumped over the outhole switch
            WriteUpper("  BALL  SEARCH");
            ActivateSolenoid(0, BallSearchCoils[Counter]); // fire coil to get ball free
            Counter++;
            if (Counter == 10) {                      // all coils fired?
              Counter = 0;}                           // start again
            BallWatchdogTimer = ActivateTimer(1000, Counter, SearchBall);}}}}}} // come again in 1s if no switch is activated

byte CountBallsInTrunk() {
  byte Balls = 0;
  for (i=0; i<3; i++) {                               // check how many balls are on the ball ramp
    if (QuerySwitch(17+i)) {
      if (Balls < i) {
        return 5;}                                    // send warning
      Balls++;}}
  return Balls;}

void CheckReleasedBall(byte Balls) {                  // ball release watchdog
  CheckReleaseTimer = 0;
  BlinkScore(0);                                      // stop blinking to show messages
  WriteUpper("WAITINGFORBALL");                       // indicate a problem
  WriteLower("              ");
  if (Balls == 10) {                                  // indicating a previous trunk error
    WriteUpper("              ");
    WriteLower("              ");
    ShowAllPoints(0);
    BlinkScore(1);
    ActivateSolenoid(0, 6);}
  byte c = CountBallsInTrunk();
  if (c == Balls) {                                   // expected number of balls in trunk
    WriteUpper("  BALL MISSING");
    if (QuerySwitch(20)) {                            // outhole switch still active?
      ActivateSolenoid(0, 1);}}                       // shove the ball into the trunk
  else {
    if (c == 5) {                                     // balls not settled
      WriteLower(" TRUNK  ERROR ");
      Balls = 10;}
    else {
      if ((c > Balls) || !c) {                        // more balls in trunk than expected or none at all
        WriteUpper("              ");
        WriteLower("              ");
        ShowAllPoints(0);
        BlinkScore(1);
        ActivateSolenoid(0, 6);}}}                    // release again
  CheckReleaseTimer = ActivateTimer(5000, Balls, CheckReleasedBall);}

void BK_PlaySpinnerSound(byte Event) {                // play the sounds of the lit spinner - start with Event = 0
  static byte TimerNo;
  static byte Step = 0;
  char Filename[13] = "0_2d_001.snd";
  if (Event) {                                        // called from timer (spinner not lit any more)
    TimerNo = 0;
    Step = 0;}
  else {                                              // called from spinner switch
    if (TimerNo) {                                    // not the first time
      KillTimer(TimerNo);                             // kill old timer
      Filename[7] = ((Step+1) % 10) + 48;             // determine new filename
      Filename[6] = ((Step+1) / 10) + 48;}
    PlaySound(51, Filename);                          // play sound
    if (Step < 31) {                                  // still sound files left?
      Step++;}                                        // increase file number
    TimerNo = ActivateTimer(2000, 1, BK_PlaySpinnerSound);}}  // set timer to stop sound cycling

void BK_StopExballParty(byte dummy){
  UNUSED(dummy);
  LampReturn = 0;
  LampPattern = LampColumns;}

void BK_StopExballParty2(byte dummy){
  UNUSED(dummy);
  LampReturn = 0;
  LampPattern = LampColumns;
  ReleaseSolenoid(15);
  if (!game_settings[MultiballJackpot] || Multiballs == 1) {
    StopPlayingMusic();}
  PlaySound(61, "0_22.snd");}

void TimedRightMagna(byte dummy) {                    // runs every second as long as button is pressed
  UNUSED(dummy);
  if (QuerySwitch(9) && RightMagna[Player]) {         // button still pressed and magna seconds left?
    RightMagna[Player]--;                             // reduce magna save seconds
    AddBlinkLamp(9, 150*RightMagna[Player]);          // adjust blinking rhythm of lamp
    TimedRightMagnaTimer = ActivateTimer(1000, 0, TimedRightMagna);}// and come back in 1s
  else {                                              // magna save not to be continued
    ReleaseSolenoid(9);                               // turn off magnet
    if (!TimedLeftMagnaTimer) {
      Switch_Released = DummyProcess;}
    EndRightMagna(0);
    TimedRightMagnaTimer = 0;}}

void TimedLeftMagna(byte dummy) {                     // runs every second as long as button is pressed
  UNUSED(dummy);
  if (QuerySwitch(10) && LeftMagna[Player]) {         // button still pressed and magna seconds left?
    LeftMagna[Player]--;                              // reduce magna save seconds
    AddBlinkLamp(10, 150*LeftMagna[Player]);          // adjust blinking rhythm of lamp
    TimedLeftMagnaTimer = ActivateTimer(1000, 0, TimedLeftMagna);}// and come back in 1s
  else {                                              // magna save not to be continued
    ReleaseSolenoid(10);                              // turn off magnet
    if (!TimedRightMagnaTimer) {
      Switch_Released = DummyProcess;}
    EndLeftMagna(0);
    TimedLeftMagnaTimer = 0;}}

void TimedMagnaSW(byte Switch) {                      // magna save button released
  if (Switch==9 && TimedRightMagnaTimer) {            // was it the right switch and is the magna save active?
    KillTimer(TimedRightMagnaTimer);
    TimedRightMagnaTimer = 0;
    ReleaseSolenoid(9);                               // turn off magnet
    EndRightMagna(0);}
  if (Switch==10 && TimedLeftMagnaTimer) {            // was it the left switch and is the magna save active?
    KillTimer(TimedLeftMagnaTimer);
    TimedLeftMagnaTimer = 0;
    ReleaseSolenoid(10);                              // turn off magnet
    EndLeftMagna(0);}
  if (!TimedRightMagnaTimer && !TimedLeftMagnaTimer) { // no magna save active
    Switch_Released = DummyProcess;}}                 // ignore released switches

void GameMain(byte Event) {                           // game switch events
  *(DisplayLower) = RightCredit[(Event / 10)*2 + 32];
  *(DisplayLower+16) = RightCredit[(Event % 10)*2 + 32];
  switch (Event) {
  case 1:                                             // plumb bolt tilt
  case 2:                                             // ball roll tilt
  case 7:                                             // slam tilt
  case 46:                                            // playfield tilt
    WriteUpper(" TILT  WARNING");
    ActivateTimer(3000, 0, ShowAllPoints);
    break;
  case 3:                                             // credit button
    AddPlayer();
    break;
  case 4:                                             // right coin switch
  case 5:                                             // center coin switch
  case 6:                                             // left coin switch
    WriteUpper(" FREE   GAME  ");
    ActivateTimer(3000, 0, ShowAllPoints);
    break;
  case 8:
    digitalWrite(Blanking, LOW);                      // invoke the blanking
    break;
  case 9:                                             // right magna save button
    if (RightMagna[Player]) {                         // right magna save available?
      if (!game_settings[TimedMagna]) {               // no timed magna save
        AddBlinkLamp(9, 150);
        RightMagna[Player] = 0;                       // not any more
        ActivateTimer(4990, 0, EndRightMagna);
        ActivateSolenoid(5000, 9);}                   // use it for 5 seconds
      else {                                          // timed magna active
        Switch_Released = TimedMagnaSW;
        RightMagna[Player]--;                         // reduce magna save seconds
        AddBlinkLamp(9, 150*RightMagna[Player]);
        ActivateSolenoid(6000, 9);                    // switch on magnet
        TimedRightMagnaTimer = ActivateTimer(1000, 0, TimedRightMagna);}}   // check again in 1s
    break;
  case 10:                                            // left magna save button
    if (LeftMagna[Player]) {                          // left magna save available?
      if (!game_settings[TimedMagna]) {               // no timed magna save
        AddBlinkLamp(10, 150);
        LeftMagna[Player] = 0;                        // not any more
        ActivateTimer(4990, 0, EndLeftMagna);
        ActivateSolenoid(5000, 10);}                  // use it for 5 seconds
      else {                                          // timed magna active
        Switch_Released = TimedMagnaSW;
        LeftMagna[Player]--;                          // reduce magna save seconds
        AddBlinkLamp(10, 150*LeftMagna[Player]);
        ActivateSolenoid(6000, 10);                   // switch on magnet
        TimedLeftMagnaTimer = ActivateTimer(1000, 0, TimedLeftMagna);}} // check again in 1s
    break;
  case 11:                                            // left outlane
  case 12:                                            // right outlane
    Points[Player] += Multiballs * 5000;              // add 5000 points
    ShowPoints(Player);                               // and show the score
    AddBonus(2);
    if (LastChance) {
      LastChance = false;
      LastChanceOver = true;                          // don't give a last chance again
      LastChanceActive = true;                        // give a last chance ball
      AddBlinkLamp(11, 150);                          // let the last chance lamps blink
      AddBlinkLamp(12, 150);}
    else {
      if (Multiballs == 1) {                          // not during multiball
        StopPlayingMusic();
        PlaySound(61, "0_29.snd");}}                  // BK laughing
    break;
  case 13:                                            // spinner
    //StopPlayingMusic();
    if (RightMysteryTimer) {
      BK_PlaySpinnerSound(0);
      Points[Player] += Multiballs * 2500;}           // add 2500 points
    else {
      PlaySound(51, "0_39.snd");
      Points[Player] += Multiballs * 100;}            // add 100 points
    ShowPoints(Player);                               // and show the score
    break;
  case 14:                                            // right ramp
    if (LeftMysteryTimer) {                           // left mystery active?
      //StopPlayingMusic();
      PlaySound(52, "BK_E19.bin");
      ByteBuffer = ((byte)(TimerValue[LeftMysteryTimer])/2.56);
      if (ByteBuffer < 20) {
        Points[Player] += Multiballs * 20000;}        // at least 20000 points
      else {
        Points[Player] += Multiballs * ByteBuffer * 1000;} // or more
      KillTimer(LeftMysteryTimer);                    // turn off left mystery
      LeftMysteryTimer = 0;
      RemoveBlinkLamp(14);}                           // stop the blinking of the mystery lamp
    else {
      PlaySound(51, "0_39.snd");
      Points[Player] += Multiballs * 500;}            // add 500 points
    ShowPoints(Player);                               // and show the score
    break;
  case 15:                                            // right inlane
    if (RightMysteryTimer) {                          // right mystery timer already running?
      KillTimer(RightMysteryTimer);}                  // stop it
    else {
      AddBlinkLamp(13, 250);}                         // let the mystery lamp blink
    if (RightAfterMagna) {                            // if it's right after using the right magna save
      if (!game_settings[MultiballJackpot] || Multiballs == 1) {
        StopPlayingMusic();}
      PlaySound(51, "BK_E20.bin");
      AddBonus(5);
      Points[Player] += Multiballs * 10000;}          // add 10000 points
    else {
      if (!game_settings[MultiballJackpot] || Multiballs == 1) {
        StopPlayingMusic();}
      PlaySound(50, "0_33.snd");
      AddBonus(2);
      Points[Player] += Multiballs * 2000;}           // add 2000 points
    ShowPoints(Player);                               // and show the score
    RightMysteryTimer = ActivateTimer(5000, 0, ResetRightMystery); // and start a timer to reset the mystery
    break;
  case 16:                                            // left inlane
    if (LeftMysteryTimer) {                           // left mystery timer already running?
      KillTimer(LeftMysteryTimer);}                   // stop it
    else {
      AddBlinkLamp(14, 250);}                         // let the mystery lamp blink
    if (LeftAfterMagna) {                             // if it's right after using the right magna save
      if (!game_settings[MultiballJackpot] || Multiballs == 1) {
        StopPlayingMusic();}
      PlaySound(51, "BK_E20.bin");
      AddBonus(5);
      Points[Player] += Multiballs * 10000;}          // add 10000 points
    else {
      if (!game_settings[MultiballJackpot] || Multiballs == 1) {
        StopPlayingMusic();}
      PlaySound(50, "0_33.snd");
      AddBonus(2);
      Points[Player] += Multiballs * 2000;}           // add 2000 points
    ShowPoints(Player);                               // and show the score
    LeftMysteryTimer = ActivateTimer(5000, 0, ResetLeftMystery); // and start a timer to reset the mystery
    break;
  case 20:                                            // outhole
    ActivateTimer(200, 0, ClearOuthole);              // check again in 200ms
    break;
  case 21:                                            // left slingshot
  case 22:                                            // right slingshot
    Points[Player] += Multiballs * 10;
    ShowPoints(Player);
    break;
  case 23:                                            // turnaround
    Points[Player] += Multiballs * 5000;
    ShowPoints(Player);
    if (BonusMultiplier < 5) {
      BonusMultiplier++;
      if ((BonusMultiplier == 5) && (Bonus == 49)) {  // full bonus and multiplier?
        BK_CycleSwordLights(1);}
      else {
        AddBlinkLamp(FirstMultLamp+BonusMultiplier-2, 250);
        ActivateTimer(2000, FirstMultLamp+BonusMultiplier-2, SetBonusMultiplier);}}
    if (LowerExBall[Player]) {
      ActivateSolenoid(8000, 15);                     // ring the bell
      PatPointer = ExBallPat;
      FlowRepeat = 10;
      LampReturn = BK_StopExballParty2;
      ShowLampPatterns(1);                            // show lamp animation
      TurnOffLamp(23);
      TurnOnLamp(47);
      TurnOnLamp(1);
      LowerExBall[Player] = false;
      ExBalls++;}
    else {
      PlaySound(50, "0_38.snd");}
    break;
  case 24:                                            // lower eject hole
    if (!DropWait[4]) {                               // locks are not blocked?
      Points[Player] += Multiballs * 5000;
      ShowPoints(Player);
      if (QueryLamp(24) && Multiballs == 1) {         // lower lock lit and no multiball running?
        LockedBalls[Player]++;
        DropWait[4] = true;                           // block locks
        StartMultiball();}
      else {
        if (Multiballs > 1) {                         // multiball running?
          if (game_settings[MultiballJackpot]) {
            BK_Jackpot(2);}
          BK_GiveMultiballs(1);}                      // eject ball with some ado
        else {
          StopPlayingMusic();}
          PlaySound(50, "0_31.snd");
          ActivateTimer(1000, 8, DelaySolenoid);}}   // eject ball
    break;
  case 25:
  case 26:                                            // lower left drop targets
  case 27:
    if (!DropWait[0]) {
      PlaySound(50, "0_38.snd");
      DropWait[0] = true;
      Points[Player] += Multiballs * 1000;
      ShowPoints(Player);
      ActivateTimer(200, 0, HandleDropTargets);}
    break;
  case 29:
  case 30:                                            // lower right drop targets
  case 31:
    if (!DropWait[1]) {
      PlaySound(50, "0_38.snd");
      DropWait[1] = true;
      Points[Player] += Multiballs * 1000;
      ShowPoints(Player);
      ActivateTimer(200, 1, HandleDropTargets);}
    break;
  case 33:
  case 34:                                            // upper left drop targets
  case 35:
    if (!DropWait[2]) {
      PlaySound(50, "0_38.snd");
      DropWait[2] = true;
      Points[Player] += Multiballs * 1000;
      ShowPoints(Player);
      ActivateTimer(200, 2, HandleDropTargets);}
    break;
  case 36:                                            // jet bumper
    Points[Player] += Multiballs * 500;
    ShowPoints(Player);
    break;
  case 37:
  case 38:                                            // upper right drop targets
  case 39:
    if (!DropWait[3]) {
      PlaySound(50, "0_38.snd");
      DropWait[3] = true;
      Points[Player] += Multiballs * 1000;
      ShowPoints(Player);
      ActivateTimer(200, 3, HandleDropTargets);}
    break;
  case 41:
  case 42:                                            // multiball lock
  case 43:
    if (!DropWait[4]) {
      DropWait[4] = true;                             // set ignore flag
      ActivateTimer(2000, 0, HandleLock);}            // handle locked balls after 1s
    break;
  case 44:                                            // left ramp rollover
    Points[Player] += Multiballs * 5000;
    ShowPoints(Player);
    if (UpperExBall[Player]) {                        // upper extra ball lit?
      if (!game_settings[MultiballJackpot] || Multiballs == 1) {
        StopPlayingMusic();}
      ActivateSolenoid(8000, 15);                     // ring the bell
      PatPointer = ExBallPat;
      FlowRepeat = 10;
      LampReturn = BK_StopExballParty2;
      ShowLampPatterns(1);                            // show lamp animation
      TurnOffLamp(41);
      TurnOnLamp(47);
      TurnOnLamp(1);
      UpperExBall[Player] = false;
      ExBalls++;}
    else {
      PlaySound(50, "0_38.snd");}
    break;
  case 65:
    ActivateSolenoid(0, 17);
    break;
  case 66:
    ActivateSolenoid(0, 18);
    break;
  case 67:
    ActivateSolenoid(0, 19);
    break;
  case 72:                                            // test mode button pressed?
    NoPlayers = 0;                                    // abort game
    ReleaseAllSolenoids();
    LockChaseLight(0);
    BlinkScore(0);
    RightMysteryTimer = 0;
    LeftMysteryTimer = 0;
    BallWatchdogTimer = 0;
    TimedRightMagnaTimer = 0;
    TimedLeftMagnaTimer = 0;
    CheckReleaseTimer = 0;
    TestMode_Enter();
    break;}}

void ClearOuthole(byte Event) {
  UNUSED(Event);
  if (QuerySwitch(20)) {                              // outhole switch still active?
    if (!BlockOuthole) {                              // outhole blocked?
      BlockOuthole = true;                            // block outhole until this ball has been processed
      ActivateSolenoid(20, 1);                        // put ball in trunk
      if (LastChanceActive && InLock) {               // saved by last chance
        BlockOuthole = false;                         // remove outhole block
        TurnOffLamp(24);
        RemoveBlinkLamp(11);                          // stop the blinking of the last chance lamps
        RemoveBlinkLamp(12);
        InLock--;                                     // decrease number of balls in lock
        LastChanceActive = false;                     // deactivate last chance protection
        ActivateSolenoid(0, 7);                       // eject 1 ball from the lock
        LockedBalls[Player]--;                        // decrease number of locked balls
        if (LockedBalls[Player]) {                    // if player had two locked balls
          RemoveBlinkLamp(40);                        // remove the blinking lock arrow
          LockChaseLight(1);}} // and restart the chase light
      // ActivateTimer(1000, InLock, LastChanceLock);}
      else {
        ActivateTimer(2000, 0, BallEnd);}}
    else {
      ActivateTimer(2000, 0, ClearOuthole);}}}        // come back in 2s if outhole is blocked

void LastChanceLock(byte Event) {
  if (Event > 1) {
    DropWait[4] = true;
    ActivateSolenoid(0, 7);}}

void BallEnd(byte Event) {
  byte BallsInTrunk = CountBallsInTrunk();
  if ((BallsInTrunk == 5)||(BallsInTrunk < 4-Multiballs-InLock)) {
    InLock = 0;
    if (Multiballs == 1) {
      for (i=0; i<3; i++) {                           // check how many balls are on the ball ramp
        if (QuerySwitch(41+i)) {
          InLock++;}}}
    WriteLower(" BALL   ERROR ");
    if (QuerySwitch(20)) {                            // ball still in outhole?
      ActivateSolenoid(0, 1);                         // make the coil a bit stronger
      ActivateTimer(2000, Event, BallEnd);}           // and come back in 2s
    else {
      if (Event < 11) {                               // have I been here already?
        Event++;
        ActivateTimer(1000, Event, BallEnd);}         // if not try again in 1s
      else {                                          // ball may be still in outhole
        BlockOuthole = false;
        Event = 0;
        ClearOuthole(0);}}}
  else {
    switch (Multiballs) {
    case 3:                                           // goto 2 ball multiball
      Multiballs = 2;
      AddBlinkLamp(28, 500);
      RemoveBlinkLamp(32);
      if (BallsInTrunk != 1) {                             // not 1 ball in trunk
        ActivateTimer(1000, 0, BallEnd);}             // check again later
      else {
        BlockOuthole = false;}                        // remove outhole block
      break;
    case 2:                                           // end multiball
      Multiballs = 1;
      if (Bonus < 49 || BonusMultiplier < 5) {
        BK_CycleSwordLights(0);}                      // stop sword lamp animation
      RemoveBlinkLamp(28);                            // stop blinking of double scoring lamp
      ShowBonus();                                    // restore bonus lamps
      LockChaseLight(1);                              // restart Lock lights
      if (game_settings[MultiballJackpot]) {
        BK_Jackpot(0);
        if (APC_settings[Volume]) {
          analogWrite(VolumePin,255-APC_settings[Volume]);} // reduce volume back to normal
        StopPlayingMusic();
        PlaySound(60, "0_28.snd");}
      if (BallsInTrunk == 3) {                             // 3 balls in trunk?
        ActivateTimer(1000, 0, BallEnd);}
      else {
        BlockOuthole = false;}                        // remove outhole block
      break;
    case 1:                                           // end of ball
      if (BallsInTrunk + InLock != 3) {
        WriteUpper(" COUNT  ERROR ");
        InLock = 0;
        for (i=0; i<3; i++) {                         // check how many balls are on the ball ramp
          if (QuerySwitch(41+i)) {
            InLock++;}}
        ActivateTimer(1000, 0, BallEnd);}
      else {
        BK_PlayBgMusic(3);                            // disable BgMusic
        LockedBalls[Player] = 0;
        TurnOffLamp(24);                              // unlight lower lock
        LockChaseLight(0);
        RemoveBlinkLamp(40);
        BlinkScore(0);                                // stop score blinking
        LastChance = false;                           // deactivate last chance
        TurnOffLamp(11);
        TurnOffLamp(12);
        LastChanceOver = false;
        LastChanceActive = false;
        BonusToAdd = Bonus;
        BonusCountTime = 10;
        BK_CycleSwordLights(0);
        AfterSound = 0;
        AfterMusic = 0;
        StopPlayingMusic();
        PlaySound(60, "0_34_001.snd");                  // play bonus count sound
        CountBonus(BallsInTrunk);}}}}

void CountBonus(byte Balls) {
  byte BonusL = Bonus % 10;
  if (BonusL) {
    TurnOffLamp(BonusLamp + BonusL - 1);}
  else {
    BonusL = Bonus / 10;
    TurnOffLamp(BonusLamp + 8 + BonusL);
    if (BonusL > 1) {
      TurnOnLamp(BonusLamp + 7 + BonusL);}
    for (i=48; i<57; i++) {
      TurnOnLamp(i);}}
  Points[Player] += 1000;
  ShowPoints(Player);
  Bonus--;
  if (Bonus) {
    if (BonusCountTime) {
      BonusCountTime--;}
    ActivateTimer(20+40*BonusCountTime, Balls, CountBonus);}
  else {
    if (BonusMultiplier > 1) {
      TurnOffLamp(FirstMultLamp+BonusMultiplier-2);
      Bonus = BonusToAdd;
      ShowBonus();
      BonusMultiplier--;
      ActivateTimer(20+20*BonusCountTime, Balls, CountBonus);}
    else {
      BonusToAdd = 0;
      Bonus = 1;
      PlaySound(61, "0_34_002.snd");                    // play bonus end sound
      BallEnd2(Balls);}}}

void BallEnd2(byte Balls) {
  if (BallWatchdogTimer) {
    KillTimer(BallWatchdogTimer);
    BallWatchdogTimer = 0;}
  if (game_settings[TimedMagna]) {                    // if timed magna save mode
    Switch_Released = DummyProcess;                   // reset the released switch mode
    if (TimedRightMagnaTimer) {                       // if the left magnet is currently active
      ReleaseSolenoid(9);                             // turn it off
      KillTimer(TimedRightMagnaTimer);                // and kill its timer
      TimedRightMagnaTimer = 0;}
    if (TimedLeftMagnaTimer) {                        // same as above
      ReleaseSolenoid(10);
      KillTimer(TimedLeftMagnaTimer);
      TimedLeftMagnaTimer = 0;}
    RemoveBlinkLamp(9);                               // turn off the magna save lamps
    RemoveBlinkLamp(10);}
  if (ExBalls) {                                      // Player has extra balls
    AddBlinkLamp(47, 250);                            // Let the extra ball lamp blink
    AddBlinkLamp(1, 250);                             // also in the backbox
    ExBalls--;
    ActivateTimer(2000, 0, AfterExBallRelease);
    ActivateTimer(1000, Balls, NewBall);
    BlockOuthole = false;}                            // remove outhole block
  else {                                              // Player has no extra balls
    if ((Points[Player] > HallOfFame.Scores[3]) && (Ball == APC_settings[NofBalls])) { // last ball & high score?
      PlaySound(55, "0_24.snd");
      Switch_Pressed = DummyProcess;                  // Switches do nothing
      CheckHighScore(Player);}
    else {
      BallEnd3(Balls);}}}

void BallEnd3(byte Balls) {
  BlockOuthole = false;                               // remove outhole block
  TurnOffLamp(24);                                    // lower lock lamp off
  if (Player < NoPlayers) {                           // last player?
    Player++;
    ActivateTimer(1000, Balls, NewBall);}
  else {
    if (Ball < APC_settings[NofBalls]) {
      Player = 1;
      Ball++;
      ActivateTimer(1000, Balls, NewBall);}
    else {                                            // game end
      TurnOffLamp(36);                                // bumper light off
      ReleaseSolenoid(23);                            // disable flipper fingers
      ReleaseSolenoid(24);
      //digitalWrite(SpcSolEnable, LOW);              // disable special solenoids
      CheckForLockedBalls(0);
      TurnOffLamp(2);                                 // turn off Ball in Play lamp
      AfterSound = BK_MuteSound;                      // mute after next sound
      PlaySound(61, "0_23.snd");
      GameDefinition.AttractMode();}}}

void BK_MuteSound() {
  digitalWrite(VolumePin,HIGH);
  AfterSound = 0;}

void CheckHighScore(byte Player) {                    // show congratulations
  LampPattern = NoLamps;
  ReleaseSolenoid(23);                                // disable flipper fingers
  ReleaseSolenoid(24);
  Points[0] = Points[1];
  Points[1] = Points[Player];
  WriteUpper("        GREAT ");
  WriteLower(" SCORE PLAYER ");
  ShowPoints(1);
  Points[1] = Points[0];
  *(DisplayLower+30) = DispPattern2[32 + 2 * Player];
  *(DisplayLower+31) = DispPattern2[33 + 2 * Player];
  Points[0] = Points[Player];
  ByteBuffer = 0;
  ActivateTimer(5000, Player, ShowInitialsMessage);}

void ShowInitialsMessage(byte Player) {
  switch (ByteBuffer) {
  case 0:
    WriteUpper("USE           ");
    WriteLower("              ");
    break;
  case 1:
    WriteUpper("USE MAGNA     ");
    break;
  case 2:
    WriteUpper("USE MAGNA SAVE");
    break;
  case 3:
    WriteLower("BUTTONS       ");
    break;
  case 4:
    WriteLower("BUTTONS   TO  ");
    break;
  case 5:
    WriteUpper("ENTER         ");
    WriteLower("              ");
    break;
  case 6:
    WriteUpper("ENTER INITIALS");
    break;
  case 7:
    WriteLower("PLAYER        ");
    *(DisplayLower+14) = DispPattern2[32 + 2 * Player];
    *(DisplayLower+15) = DispPattern2[33 + 2 * Player];
    break;}
  if (ByteBuffer == 7) {
    Switch_Pressed = EnterInitials;
    EnterIni[0] = 'A';
    EnterIni[1] = 'A';
    EnterIni[2] = 'A';
    ByteBuffer = 0;
    ActivateTimer(500, 1, BlinkInitial);}
  else {
    ByteBuffer++;
    ActivateTimer(1000, Player, ShowInitialsMessage);}}

void BlinkInitial(byte State) {                       // blink actual character
  if (State) {
    *(DisplayLower+20+4*ByteBuffer) = 0;              // show a blank
    *(DisplayLower+21+4*ByteBuffer) = 0;
    State = 0;}
  else {
    for (i=0; i<3; i++) {
      *(DisplayLower+20+4*i) = DispPattern2[(EnterIni[i]-32)*2];
      *(DisplayLower+20+4*i+1) = DispPattern2[(EnterIni[i]-32)*2+1];}// restore the characters
    State = 1;}
  ByteBuffer2 = ActivateTimer(100+State*2000, State, BlinkInitial);}  // and come back

void EnterInitials(byte Event) {
  switch (Event) {
  case 3:                                             // credit button
    KillTimer(ByteBuffer2);
    ByteBuffer++;
    if (ByteBuffer > 2) {
      ByteBuffer2 = HandleHighScores(Points[Player]);
      WriteUpper(" HIGH   SCORE ");
      WriteLower("TABLE  POS    ");
      *(DisplayLower+28) = DispPattern2[32 + 2 * (ByteBuffer2+1)];
      *(DisplayLower+29) = DispPattern2[33 + 2 * (ByteBuffer2+1)];
      LampPattern = LampColumns;
      ActivateSolenoid(0, 23);                        // enable flipper fingers
      ActivateSolenoid(0, 24);
      ActivateTimer(2000, 0, BallEnd3);}
    else {
      BlinkInitial(1);}
    break;
  case 9:                                             // right magna save button
    if (EnterIni[ByteBuffer] == 57) {
      EnterIni[ByteBuffer] = 65;}
    else if (EnterIni[ByteBuffer] == 90) {
      EnterIni[ByteBuffer] = 32;}
    else if (EnterIni[ByteBuffer] == 32) {
      EnterIni[ByteBuffer] = 48;}
    else {
      EnterIni[ByteBuffer]++;}
    KillTimer(ByteBuffer2);
    BlinkInitial(0);
    break;
  case 10:                                            // left magna save button
    if (EnterIni[ByteBuffer] == 65) {
      EnterIni[ByteBuffer] = 57;}
    else if (EnterIni[ByteBuffer] == 48) {
      EnterIni[ByteBuffer] = 32;}
    else if (EnterIni[ByteBuffer] == 32) {
      EnterIni[ByteBuffer] = 90;}
    else {
      EnterIni[ByteBuffer]--;}
    KillTimer(ByteBuffer2);
    BlinkInitial(0);
    break;}}

void AfterExBallRelease(byte Event) {
  UNUSED(Event);
  BK_StartBgMusic();
  if (QuerySwitch(45)) {                              // ball still in the shooting lane?
    ActivateTimer(2000, Event, AfterExBallRelease);}  // come back in2s
  else {                                              // ball has been shot
    RemoveBlinkLamp(1);
    RemoveBlinkLamp(47);
    if (ExBalls) {                                    // player still has an extra balls
      TurnOnLamp(1);
      TurnOnLamp(47);}}}

void HandleLock(byte Event) {
  UNUSED(Event);
  bool Flag = false;
  byte LockCount = 0;
  for (i=0; i<3; i++) {                               // count balls in lock
    if (QuerySwitch(41+i)) {
      if (LockCount < i) {
        Flag = true;}                                 // set warning flag
      LockCount++;}}
  if (Flag) {
    ActivateTimer(1000, 0, HandleLock);}              // try again in 1s
  else {
    DropWait[4] = false;}                             // clear ignore flag
  if (Multiballs > 1) {                               // multiball running
    if (game_settings[MultiballJackpot]) {
      BK_Jackpot(3);}
    BK_GiveMultiballs(1);}                            // eject balls
  else {                                              // no multiball running
    if (LockCount > InLock) {                         // more than before?
      StopPlayingMusic();
      PlaySound(51, "0_3c.snd");
      Points[Player] += 5000;
      ShowPoints(Player);
      InLock++;
      LockedBalls[Player]++;                          // increase number of locked balls
      if (LockedBalls[Player] == 3) {                 // 3 locked balls?
        DropWait[4] = true;                           // set ignore flag
        StartMultiball();}
      else {                                          // less than 3 locked balls?
        if ((Ball == APC_settings[NofBalls]) && !LastChanceOver) { // last ball and no last chance yet?
          LastChance = true;                          // activate last chance
          TurnOnLamp(11);
          TurnOnLamp(12);}
        if (InLock > LockedBalls[Player]) {           // more balls in lock than player has?
          InLock--;                                   // eject 1 ball
          ActivateSolenoid(0, 7);}
        else {
          TurnOnLamp(24);                             // light lower lock
          if (BallWatchdogTimer) {
            KillTimer(BallWatchdogTimer);
            BallWatchdogTimer = 0;}
          NewBall(3-InLock);}}}}}                     // release new ball

void HandleDropTargets(byte Event) {
  DropWait[Event] = false;
  if (QuerySwitch(DropTargets[Event]) && (QuerySwitch(DropTargets[Event]+1)) && (QuerySwitch(DropTargets[Event]+2))) { // all drop targets hit?
    if (!game_settings[MultiballJackpot] || Multiballs == 1) {
      StopPlayingMusic();}
    PlaySound(51, "0_36.snd");
    AddBonus(3);                                      // add 3K bonus
    if (DropTimer[Event]) {
      KillTimer(DropTimer[Event]);}                   // kill the target bank timer
    ResetDropTargets(Event);                          // and reset the targets
    if ((Event==1) || (Event==3)) {                   // enable magna save if right target banks were hit
      if (!game_settings[TimedMagna]) {               // timed magna feature off
        TurnOnLamp(9);
        RightMagna[Player] = 1;}
      else {                                          // timed magna feature on
        if (RightMagna[Player] < 5) {                 // less than 5 magna seconds
          RightMagna[Player]++;                       // add one
          if (RightMagna[Player] == 5) {              // 5 seconds reached?
            RemoveBlinkLamp(9);                       // turn off blinking lamp
            TurnOnLamp(9);}                           // and switch it on permanently
          else {                                      // 5 seconds not reached?
            AddBlinkLamp(9, 150*RightMagna[Player]);}}}} // adapt blinking frequency
    else {                                            // enable magna save if left target banks were hit
      if (!game_settings[TimedMagna]) {               // timed magna feature off
        TurnOnLamp(10);
        LeftMagna[Player] = 1;}
      else {                                          // timed magna feature on
        if (LeftMagna[Player] < 5) {                  // less than 5 magna seconds
          LeftMagna[Player]++;                        // add one
          if (LeftMagna[Player] == 5) {               // 5 seconds reached?
            RemoveBlinkLamp(10);                      // turn off blinking lamp
            TurnOnLamp(10);}                          // and switch it on permanently
          else {                                      // 5 seconds not reached?
            AddBlinkLamp(10, 150*LeftMagna[Player]);}}}} // adapt blinking frequency
    if (DropHits[(Player-1)*4+Event] < 3) {           // target bank cleared less than 3 times?
      TurnOnLamp(DropTargets[Event]+DropHits[(Player-1)*4+Event]); // turn on the appropriate arrow lamp
      DropHits[(Player-1)*4+Event]++;                 // and increase the counter for this target bank
      if (DropHits[(Player-1)*4+Event] == 3) {        // target bank cleared 3 times?
        if (PlayersExBalls[Player]) {                 // if the player did already get an extra ball
          if ((DropHits[(Player-1)*4] == 3) && (DropHits[(Player-1)*4+1] == 3) && (DropHits[(Player-1)*4+2] == 3) && (DropHits[(Player-1)*4+3] == 3)) {
            for (i=0; i<4; i++) {                     // for all drop target banks
              DropHits[(Player-1)*4+i] = 0;           // clear their hits
              TurnOffLamp(DropTargets[i]);            // and their arrow lamps
              TurnOffLamp(DropTargets[i]+1);
              TurnOffLamp(DropTargets[i]+2);}
            TurnOnLamp(23);                           // turn on the lower extra ball lamp
            PlaySound(52, "0_32.snd");              // cheering
            PatPointer = ExBallPat;
            FlowRepeat = 10;
            LampReturn = BK_StopExballParty;
            ShowLampPatterns(1);                      // show lamp animation
            LowerExBall[Player] = true;}}             // and enable the lower extra ball
        else {                                        // if the player didn't get an extra ball yet
          for (byte Counter=0 ; Counter < 2; Counter++) {
            if ((DropHits[(Player-1)*4+Counter*2] == 3) && (DropHits[(Player-1)*4+Counter*2+1] == 3)) {
              for (i=0; i<2; i++) {                   // for all drop target banks
                DropHits[(Player-1)*4+Counter*2+i] = 0;     // clear their hits
                TurnOffLamp(DropTargets[Counter*2+i]);      // and their arrow lamps
                TurnOffLamp(DropTargets[Counter*2+i]+1);
                TurnOffLamp(DropTargets[Counter*2+i]+2);}
              TurnOnLamp(41);                         // turn on the upper extra ball lamp
              PlaySound(52, "0_32.snd");            // cheering
              PatPointer = ExBallPat;
              FlowRepeat = 10;
              LampReturn = BK_StopExballParty;
              ShowLampPatterns(1);                    // show lamp animation
              PlayersExBalls[Player] = true;
              UpperExBall[Player] = true;}}}}}}
  else {                                              // not all targets cleared
    if (QuerySwitch(DropTargets[Event]) || (QuerySwitch(DropTargets[Event]+1)) || (QuerySwitch(DropTargets[Event]+2))) { // any target down? (or false alarm)
      if (!DropTimer[Event]) {                        // no timer running for this bank already?
        //StopPlayingMusic();
        PlayMusic(51, "BK_E14.bin");
        DropTimer[Event] = ActivateTimer(6000, Event, BlinkFaster); // start one
        AddBlinkLamp(DropLamp+Event, 500);}}}}        // and let the bank lamp blink

void BlinkFaster(byte Event) {                        // increase blink frequency
  RemoveBlinkLamp(DropLamp+Event);
  AddBlinkLamp(DropLamp+Event, 200);
  DropTimer[Event] = ActivateTimer(2000, Event, ResetDropTargets);}

void ResetDropTargets(byte Event) {
  DropTimer[Event] = 0;
  RemoveBlinkLamp(DropLamp+Event);
  DropWait[Event] = true;                             // set flag for waiting time to ignore switch bouncing during drop target reset
  ActivateTimer(200, Event, ResetDropWait);           // clear it after 200ms
  ActivateSolenoid(0, DropSolenoid+Event);}           // and reset the drop targets

void ResetDropWait(byte Event) {                      // ensure waiting time to ignore switch bouncing
  if (QuerySwitch(DropTargets[Event]) && (QuerySwitch(DropTargets[Event]+1)) && (QuerySwitch(DropTargets[Event]+2))) { // drop targets still down?
    ActivateTimer(2200, Event, ResetDropWait);        // come back in 2.2s
    ActivateTimer(2000, DropSolenoid+Event, DelaySolenoid);}  // and reset the drop targets again in 2s
  else {
    DropWait[Event] = false;}}                        // reset flag to ignore switch bouncing during drop target reset

void StartMultiball() {
  if (LockedBalls[Player] == 3) {                     // 2 or 3 ball multiball?
    Multiballs = 3;}
  else {
    Multiballs = 2;}
  ActivateSolenoid(0, 11);                            // turn off GI
  LockChaseLight(0);
  RemoveBlinkLamp(40);                                // turn off the blinking of the first lock arrow
  TurnOffLamp(21);
  TurnOffLamp(40);
  TurnOffLamp(42);
  LampPattern = NoLamps;
  for (byte i=0; i<4; i++) {                          // check for running drop target timers
    if (DropTimer[i]) {                               // found one?
      KillTimer(DropTimer[i]);                        // kill it
      DropTimer[i] = 0;
      RemoveBlinkLamp(DropLamp+i);}}
  PlayFlashSequence((byte*) BK_ResetAllDropTargets);  // reset all drop targets
  ActivateTimer(800, 0, BK_Multiball2);}

void BK_Multiball2(byte Step) {
  static byte Counter;
  if (!Step) {
    StopPlayingMusic();
    AfterSound = 0;
    PlaySound(55, "0_38_001.snd");
    Counter = 0;
    Step++;
    LampPattern = (AttractPat1->Pattern)-1;}
  else {
    if (Step < 26) {                                  // 26 steps to fill the lamps to the top
      switch (Counter) {
      case 0:                                         // no lamps
        LampPattern = NoLamps;
        Counter++;
        break;
      case 1:
        LampPattern = LampColumns;                    // last pattern
        Counter++;
        break;
      case 2:
        LampPattern = ((AttractPat1+Step)->Pattern)-1;  // turn on the lamps from the bottom to the top
        Counter = 0;
        Step++;}}
    else {
      if (Step == 26) {                               // all lamps on
        Counter = 0;
        Step++;
        if (Multiballs == 3) {                        // 2 or 3 ball multiball?
          PlaySound(55, "0_21.snd");}               // fight against three enemies
        else {
          PlaySound(55, "0_20.snd");}}              // fight against two enemies
      else {
        switch (Counter) {
        case 0:
          LampPattern = NoLamps;
          Counter++;
          break;
        case 1:
          LampPattern = LampColumns;
          Counter++;
          break;
        case 2:
          LampPattern = ((AttractPat1+51-Step)->Pattern)-1; // turn off the lamps from the top to the bottom
          Counter = 0;
          Step++;}}}}
  if (Step < 52) {
    ActivateTimer(30, Step, BK_Multiball2);}
  else {
    WriteUpper2(" MULTI  BALL  ");                    // switch display to alternate buffer
    DispRow1 = DisplayUpper2;
    if (LastChance) {                                 // last chance active?
      LastChance = false;                             // deactivate it
      TurnOffLamp(11);
      TurnOffLamp(12);}
    TurnOffLamp(24);                                  // unlight lower lock
    if (Multiballs == 3) {                            // 2 or 3 ball multiball?
      AddBlinkLamp(32, 500);}                         // let triple points lamp blink
    else {
      AddBlinkLamp(28, 500);}                         // let double points lamp blink
    LockedBalls[Player] = 0;
    ActivateTimer(3000, 1, SwitchDisplay);            // switch display back to main buffer in 3 seconds
    BK_CycleSwordLights(1);                           // start sword lamp animation
    DropWait[4] = false;                              // clear ignore flag
    if (game_settings[MultiballJackpot]) {
      AfterSound = BK_StartJackpotMusic;}
    else {
      AfterSound = BK_ResumeBgMusic;}
    ActivateTimer(500, 1, BK_GiveMultiballs);}}

void BK_GiveMultiballs(byte Step) {                   // release locked balls with multiball effects - call with Step = 1
  static bool running = 0;
  if ((Step > 1) || ((Step == 1) && !running)) {      // is there a ball in one of the locks?
    if (Step == 1) {                                  // start new cycle
      if (QuerySwitch(24) || QuerySwitch(41)) {
        DropWait[4] = true;
        running = true;
        PlaySound(55, "0_30_001.snd");}
      else {
        DropWait[4] = false;
        return;}}
    if (Step < 7) {                                   // still in flickering phase?
      if (Step & 1) {                                 // flicker GI based on the LSB of Step
        LampPattern = NoLamps;                        // turn off all lamps
        ActivateSolenoid(0, 11);                      // turn off GI
        ActivateTimer(30, Step+1, BK_GiveMultiballs);}
      else {
        LampPattern = AllLamps;                       // turn on all lamps
        ReleaseSolenoid(11);                          // turn on GI
        ActivateTimer(120, Step+1, BK_GiveMultiballs);}}
    else {                                            // time to release a ball
      running = false;
      InLock = 0;                                     // reset locked balls counter
      LampPattern = LampColumns;                      // restore lamp states
      if (QuerySwitch(24)) {                          // for the lower elect hole
        ActivateTimer(2000, 1, BK_GiveMultiballs);    // come back in 2 seconds to check again
        ActivateSolenoid(0, 8);}                      // release ball from lower lock
      else {                                          // no ball in lower lock
        if (QuerySwitch(41)) {                        // ball in the upper lock?
          ActivateTimer(2000, 1, BK_GiveMultiballs);  // come back in 2 seconds to check again
          ActivateSolenoid(0, 7);}                    // release ball from upper lock
        else {
          DropWait[4] = false;}}}}}

void BK_Jackpot(byte State) {
  static byte PrevState = 0;
  switch (State) {
  case 0:
    if (PrevState == 1) {
      RemoveBlinkLamp(24);}
    if (PrevState == 2) {
      RemoveBlinkLamp(21);
      RemoveBlinkLamp(40);
      RemoveBlinkLamp(42);}
    PrevState = 0;
    break;
  case 1:
    if (!PrevState) {
      AddBlinkLamp(24, 300);
      PrevState = 1;}
    break;
  case 2:
    if (PrevState == 1) {
      AddBlinkLamp(21, 300);
      AddBlinkLamp(40, 300);
      AddBlinkLamp(42, 300);
      RemoveBlinkLamp(24);
      PrevState = 2;}
    break;
  case 3:
    if (PrevState == 2) {
      Points[Player] += 250000 * (game_settings[MultiballJackpot] + 1);
      AddBlinkLamp(24, 300);
      RemoveBlinkLamp(21);
      RemoveBlinkLamp(40);
      RemoveBlinkLamp(42);
      PrevState = 1;}
    break;}}

void ClearLocks(byte Event) {
  UNUSED(Event);
  byte Count = 0;
  for (byte i=0; i<3; i++) {                          // count balls in lock
    if (QuerySwitch(41+i)) {
      Count++;}}
  if (Count) {                                        // balls in lock?
    ActivateSolenoid(0, 7);
    Count--;
    InLock = 0;
    ActivateTimer(1000, 0, ClearLocks);}}

void BK_CycleSwordLights(byte State) {                // do sword light effects - start with State = 1 and stop with State = 0
  const byte SwordLamps[26][3] = {{1,0b00000000,0b00000000},{1,0b00000001,0b00000000},{1,0b00000011,0b00000000},{1,0b00000111,0b11110000},{1,0b00001111,0b11110000},{1,0b00011111,0b11110000},{1,0b00111111,0b11110000},{1,0b01111111,0b11110000},{1,0b11111111,0b11110000},{1,0b11111111,0b11110001},{1,0b11111111,0b11110011},{1,0b11111111,0b11110111},{1,0b11111111,0b11111111},
      {0,0b11111111,0b11111111},{0,0b11111110,0b11111111},{0,0b11111100,0b11111111},{0,0b11111000,0b00001111},{0,0b11110000,0b00001111},{0,0b11100000,0b00001111},{0,0b11000000,0b00001111},{0,0b10000000,0b00001111},{0,0b00000000,0b00001111},{0,0b00000000,0b00001110},{0,0b00000000,0b00001100},{0,0b00000000,0b00001000},{0,0b00000000,0b00000000}};
  static byte Timer = 0;
  if ((State > 1) || ((State == 1) && !Timer)) {
    if (State == 1) {
      State++;}
    if (SwordLamps[State-2][0]) {
      TurnOnLamp(48);}
    else {
      TurnOffLamp(48);}
    LampColumns[6] = SwordLamps[State-2][1];
    LampColumns[7] = SwordLamps[State-2][2];
    if (State < 27) {
      State++;}
    else {
      State = 2;}
    Timer = ActivateTimer(100, State, BK_CycleSwordLights);}
  else {
    if (!State) {
      if (Timer) {
        KillTimer(Timer);
        Timer = 0;}
      ShowBonus();
      for (byte i=0; i<4; i++) {
        if (i<BonusMultiplier-1) {
          TurnOnLamp(61 + i);}
        else {
          TurnOffLamp(61 + i);}}}}}

void SetBonusMultiplier(byte Lamp) {                  // switch from blinking bonus multiplier lamp to permanent on
  RemoveBlinkLamp(Lamp);
  TurnOnLamp(Lamp);}

void BK_PlayFullBonusEffect(byte Step) {
  if (Step < 8) {                                     // still in flickering phase?
    if (Step & 1) {                                   // flicker lamps based on the LSB of Step
      LampPattern = LampColumns;                      // turn on lamps
      ActivateTimer(120, Step+1, BK_PlayFullBonusEffect);}
    else {
      LampPattern = NoLamps;                          // turn off all lamps
      ActivateTimer(30, Step+1, BK_PlayFullBonusEffect);}}
  else {
    ReleaseSolenoid(11);}}

void AddBonus(byte BonusPts) {
  byte OldBonusToAdd = BonusToAdd;
  if (Bonus < 49) {
    if (Bonus+BonusPts > 49) {                        // full bonus reached?
      BonusToAdd += 49 - Bonus;
      Bonus = 49;
      if (BonusMultiplier == 5) {                     // multiplier = 5?
        BK_CycleSwordLights(1);}}                     // start sword lamp animation
    else {
      BonusToAdd += BonusPts;
      Bonus += BonusPts;}
    if (BonusToAdd && !OldBonusToAdd) {
      ClearBonusLight(0);}}
  else {                                              // full bonus
    if (BonusMultiplier == 5 && Multiballs == 1) {    // multiplier = 5 and no multiball running?
      ActivateSolenoid(0, 11);
      ActivateSolenoid(800, 15);
      BK_PlayFullBonusEffect(0);}}}

void ClearBonusLight(byte Step) {
  if (Step == 10) {
    Step = 0;}
  if (Step == 9) {
    for (i=BonusLamp; i<BonusLamp+9; i++) {
      TurnOffLamp(i);}
    BonusToAdd--;}
  else {
    if (QueryLamp(BonusLamp+Step)) {
      TurnOffLamp(BonusLamp+Step);}
    else {
      BonusToAdd--;}}
  ActivateTimer(50, Step, SetBonusLight);}

void SetBonusLight(byte Step) {
  if (Step == 9) {
    if (QueryLamp(BonusLamp+9) || QueryLamp(BonusLamp+10) || QueryLamp(BonusLamp+11) || QueryLamp(BonusLamp+12)) {
      i = 0;
      while (i < 3) {
        if (QueryLamp(BonusLamp+9+i)) {
          TurnOffLamp(BonusLamp+9+i);
          TurnOnLamp(BonusLamp+10+i);
          i = 3;}
        else {
          i++;}}}
    else {
      TurnOnLamp(BonusLamp+9);}}
  else {
    TurnOnLamp(BonusLamp+Step);}
  Step++;
  if (BonusToAdd) {
    ActivateTimer(50, Step, ClearBonusLight);}}

void ShowBonus() {                                    // set lamps on bonus meter
  byte Count;
  if (Bonus < 50) {                                   // max bonus of 49K allowed
    Count = Bonus;}
  else {
    Count = 49;}
  for (i=0; i<13; i++) {                              // for all 13 lamps of the bonus meter
    if (Count >= BonusValues[i]) {                    // bonus larger or equal than the value of the actual lamp?
      if (i < 4) {
        Count -= BonusValues[i];}                     // reduce bonus accordingly
      else {
        Count -= 1;}
      TurnOnLamp(BonusLamps[i]);}                     // and turn the lamp on
    else {
      TurnOffLamp(BonusLamps[i]);}}}                  // otherwise turn it off

void BK_StartBgMusic() {
  BK_PlayBgMusic(1);
  AfterMusic = BK_ResumeBgMusic;
  AfterSound = BK_ResumeBgMusic;}

void BK_ResumeBgMusic() {
  BK_PlayBgMusic(0);}

void BK_StartJackpotMusic() {
  if (APC_settings[Volume]) {
    analogWrite(VolumePin,255-APC_settings[Volume]-game_settings[BK_MultiballVolume]);} // increase volume
  PlayMusic(70, "BK_M01.bin");
  BK_Jackpot(1);
  AfterSound = BK_ResumeBgMusic;}

void BK_PlayBgMusic(byte State) {
  static byte CurrentBgMusic;
  static byte TimerNo = 0;
  char Filename[13] = {"0_2e_001.snd"};
  switch (State) {
  case 0:                                             // Resume BgMusic
    Filename[7] = (CurrentBgMusic % 10) + 48;
    Filename[6] = (CurrentBgMusic / 10) + 48;
    PlayMusic(50, Filename);
    break;
  case 1:                                             // Start BgMusic
    CurrentBgMusic = 2;
    if (!TimerNo) {
      PlayMusic(50, Filename);
      TimerNo = ActivateTimer(30000, 2, BK_PlayBgMusic);}
    break;
  case 2:                                             // proceed to next track
    if (CurrentBgMusic < 28) {
      CurrentBgMusic++;}
    TimerNo = ActivateTimer(10000, 2, BK_PlayBgMusic);
    break;
  case 3:                                             // stop BgMusic
    if (TimerNo) {
      KillTimer(TimerNo);
      TimerNo = 0;}
    StopPlayingMusic();}}

void EndRightMagna(byte Event) {
  UNUSED(Event);
  RightAfterMagna = true;
  AddBlinkLamp(15, 150);
  if (!RightMagna[Player]) {
    RemoveBlinkLamp(9);}
  ActivateTimer(2010, 0, ResetRightAfterMagna);}      // for 2 seconds after magna time

void ResetRightAfterMagna(byte Event) {
  UNUSED(Event);
  RemoveBlinkLamp(15);
  RightAfterMagna = false;}

void EndLeftMagna(byte Event) {
  UNUSED(Event);
  LeftAfterMagna = true;
  AddBlinkLamp(16, 150);
  if (!LeftMagna[Player]) {
    RemoveBlinkLamp(10);}
  ActivateTimer(2010, 0, ResetLeftAfterMagna);}       // for 2 seconds after magna time

void ResetLeftAfterMagna(byte Event) {
  UNUSED(Event);
  RemoveBlinkLamp(16);
  LeftAfterMagna = false;}

void ResetRightMystery(byte Event) {
  UNUSED(Event);
  RemoveBlinkLamp(13);                                // stop the blinking of the mystery lamp
  RightMysteryTimer = 0;}                             // delete the right mystery timer number

void ResetLeftMystery(byte Event) {
  UNUSED(Event);
  RemoveBlinkLamp(14);                                // stop the blinking of the mystery lamp
  LeftMysteryTimer = 0;}                              // delete the left mystery timer number

void LockChaseLight(byte ChaseLamp) {                 // controls the chase light pointing to the upper lock
  static byte Timer = 0;
  switch (ChaseLamp) {                                // illuminate ChaseLamp
  case 0:                                             // deactivate chase lights
    if (Timer) {
      KillTimer(Timer);
      Timer = 0;}
    break;
  case 1:
    if (!Timer) {
      Timer = ActivateTimer(10, 40, LockChaseLight);}
    break;
  case 40:                                            // is it the first lamp?
    if (!LockedBalls[Player]) {                       // is there no locked ball?
      TurnOffLamp(42);}                               // turn off the last lamp
    TurnOffLamp(21);                                  // the middle lamp can always be switched off
    if (LockedBalls[Player] == 2) {                   // are there two locked balls?
      Timer = 0;
      TurnOnLamp(42);                                 // turn on the last two lock lamps
      TurnOnLamp(21);
      AddBlinkLamp(40, 200);}                         // and let the first one blink
    else {                                            // otherwise
      TurnOnLamp(40);                                 // turn the first lamp on
      Timer = ActivateTimer(300, 21, LockChaseLight);} // and come back to process lamp 21
    break;
  case 21:                                            // is it the middle lamp?
    TurnOffLamp(40);                                  // turn off the first
    TurnOnLamp(21);                                   // turn on the middle
    if (LockedBalls[Player]) {                        // is there a locked ball?
      TurnOnLamp(42);                                 // turn the last lamp on
      Timer = ActivateTimer(300, 40, LockChaseLight);} // and come back to process lamp 40
    else {                                            // no locked ball?
      Timer = ActivateTimer(300, 42, LockChaseLight);} // come back to process lamp 42
    break;
  case 42:                                            // is it the last lamp?
    TurnOffLamp(21);                                  // turn off the middle lamp
    TurnOnLamp(42);                                   // turn on the last lamp
    Timer = ActivateTimer(300, 40, LockChaseLight);   // and come back to process lamp 40
    break;}}

void ResetHighScores(bool change) {                   // delete the high scores file
  if (change) {                                       // if the start button has been pressed
    if (SDfound) {
      SD.remove(GameDefinition.HighScoresFileName);
      struct HighScores HScBuffer;                    // create new high score table
      HallOfFame = HScBuffer;                         // copy it to the hall of fame
      WriteLower(" SCORES DONE  ");}
    else {
      WriteLower(" SCORES NO SD ");}}
  else {
    WriteLower(" SCORES       ");}}

void BK_HandleVolumeSetting(bool change) {
  if (APC_settings[Volume]) {
    HandleNumSetting(change);
    if (!change) {
      PlayMusic(50, "BK_M01.snd");}
    analogWrite(VolumePin,255-APC_settings[Volume]-SettingsPointer[AppByte]);}}  // adjust PWM to volume setting

// Test mode

void TestMode_Enter() {
  KillAllTimers();
  ActivateSolenoid(0, 23);                            // enable flipper fingers
  ActivateSolenoid(0, 24);
  Switch_Pressed = DummyProcess;                      // Switches do nothing
  if (APC_settings[DebugMode]) {                      // deactivate serial interface in debug mode
    Serial.end();}
  if (QuerySwitch(73)) {                              // Up/Down switch pressed?
    Settings_Enter();}
  else {
    WriteUpper("  TEST  MODE  ");                     // Show Test Mode
    WriteLower("              ");
    LampPattern = NoLamps;                            // Turn off all lamps
    ActivateTimer(1000, 0, DisplayTest_Enter);}}      // Wait 1 second and proceed to Display Test

void DisplayTest_Enter(byte Event) {
  UNUSED(Event);
  Switch_Pressed = DisplayTest_EnterSw;               // Switch functions
  WriteUpper("DISPLAY TEST  ");}                      // Show 'Display Test'

void DisplayTest_EnterSw(byte Event) {                // Display Test Enter switch handler
  switch (Event) {
  case 72:                                            // Next test
    WriteUpper(" SWITCHEDGES  ");                     // Show 'Switch Edges'
    Switch_Pressed = SwitchEdges_Enter;               // Next mode
    break;
  case 3:
    WriteUpper("0000000000000000");
    WriteLower("0000000000000000");
    Switch_Pressed = DisplayTestSw;
    AppByte = ActivateTimer(1000, 32, DisplayCycle);}} // Activate timer for display pattern change}

void DisplayTestSw(byte Event) {
  if (Event == 72) {
    KillTimer(AppByte);
    Switch_Pressed = DisplayTest_EnterSw;
    *(DisplayLower) = 0;
    *(DisplayLower+16) = 0;
    *(DisplayUpper) = 0;
    *(DisplayUpper+16) = 0;
    WriteUpper("DISPLAY TEST  ");
    WriteLower("              ");}}

void DisplayCycle(byte Event) {                       // Display cycle test
  if (QuerySwitch(73)) {                              // cycle only if Up/Down switch is not pressed
    if (Event == 116) {                               // if the last character is reached
      AppByte2 = 32;}                                 // start from the beginning
    else {
      if (Event == 50) {                              // reached the gap between numbers and characters?
        AppByte2 = 66;}
      else {
        AppByte2 = Event+2;}}                         // otherwise show next character
    for (i=0; i<16; i++) {                            // use for all alpha digits
      if ((i==0) || (i==8)) {
        DisplayUpper[2*i] = LeftCredit[AppByte2];
        DisplayUpper[2*i+1] = LeftCredit[AppByte2+1];
        DisplayLower[2*i] = RightCredit[AppByte2];
        DisplayLower[2*i+1] = RightCredit[AppByte2+1];}
      else {
        DisplayUpper[2*i] = DispPattern1[AppByte2];
        DisplayUpper[2*i+1] = DispPattern1[AppByte2+1];
        DisplayLower[2*i] = DispPattern2[AppByte2];
        DisplayLower[2*i+1] = DispPattern2[AppByte2+1];}}}
  AppByte = ActivateTimer(500, AppByte2, DisplayCycle);}   // restart timer

void SwitchEdges_Enter(byte Event) {
  switch (Event) {
  case 72:
    WriteUpper("  COIL  TEST  ");                     // go to Solenoid Test Enter
    WriteLower("              ");
    Switch_Pressed = Solenoids_Enter;
    break;
  case 3:
    WriteUpper(" LATESTEDGES  ");                     // show 'Latest Edges'
    WriteLower("              ");
    Switch_Pressed = SwitchEdges;}}

void SwitchEdges(byte Event) {
  if (Event == 72) {
    Switch_Pressed = SwitchEdges_Enter;               // back to Switch edges enter
    WriteUpper(" SWITCHEDGES  ");
    WriteLower("              ");}
  else {
    for (i=1; i<24; i++) {                            // move all characters in the lower display row 4 chars to the left
      DisplayLower[i] = DisplayLower[i+8];}
    *(DisplayLower+30) = DispPattern2[32 + 2 * (Event % 10)]; // and insert the switch number to the right of the row
    *(DisplayLower+31) = DispPattern2[33 + 2 * (Event % 10)];
    *(DisplayLower+28) = DispPattern2[32 + 2 * (Event - (Event % 10)) / 10];
    *(DisplayLower+29) = DispPattern2[33 + 2 * (Event - (Event % 10)) / 10];}}

void Solenoids_Enter(byte Event) {
  switch (Event) {
  case 72:
    WriteUpper(" SINGLE LAMP  ");                     // go to next test
    Switch_Pressed = SingleLamp_Enter;
    break;
  case 3:
    WriteUpper(" FIRINGCOIL NO");
    WriteLower("              ");
    Switch_Pressed = SolenoidsTest;
    AppByte2 = 1;                                     // start with solenoid 1
    AppByte = ActivateTimer(1000, 0, FireSolenoids);}} // start after 1 second

void SolenoidsTest(byte Event) {
  if (Event == 72) {
    KillTimer(AppByte);                               // kill running timer for next solenoid
    WriteUpper("  COIL  TEST  ");                     // back to Solenoid Test Enter
    WriteLower("              ");
    Switch_Pressed = Solenoids_Enter;}}

void FireSolenoids(byte Event) {                      // cycle all solenoids
  UNUSED(Event);
  *(DisplayLower+30) = DispPattern2[32 + 2 * (AppByte2 % 10)]; // and show the actual solenoid number
  *(DisplayLower+31) = DispPattern2[33 + 2 * (AppByte2 % 10)];
  *(DisplayLower+28) = DispPattern2[32 + 2 * (AppByte2 - (AppByte2 % 10)) / 10];
  *(DisplayLower+29) = DispPattern2[33 + 2 * (AppByte2 - (AppByte2 % 10)) / 10];
  ActivateSolenoid(0, AppByte2);                      // activate the solenoid for 50ms
  if (QuerySwitch(73)) {                              // Up/Down switch pressed?
    AppByte2++;                                       // increase the solenoid counter
    if (AppByte2 == 20) {                             // maximum reached?
      AppByte2 = 1;}}                                 // then start again
  AppByte = ActivateTimer(1000, 0, FireSolenoids);}   // come back in one second

void SingleLamp_Enter(byte Event) {
  switch (Event) {
  case 72:
    WriteUpper("  ALL   LAMPS ");                     // go to next test
    Switch_Pressed = AllLamps_Enter;
    break;
  case 3:
    WriteUpper(" ACTUAL LAMP  ");
    WriteLower("              ");
    for (i=0; i<8; i++){                              // erase lamp matrix
      LampColumns[i] = 0;}
    LampPattern = LampColumns;                        // and show it
    Switch_Pressed = SingleLamp;
    AppByte2 = 1;                                     // start with lamp 1
    AppByte = ActivateTimer(1000, 0, ShowLamp);}}     // start after 1 second

void SingleLamp(byte Event) {
  if (Event == 72) {
    KillTimer(AppByte);                               // kill running timer for next solenoid
    LampPattern = NoLamps;                            // Turn off all lamps
    WriteUpper(" SINGLE LAMP  ");
    WriteLower("              ");
    Switch_Pressed = SingleLamp_Enter;}}

void ShowLamp(byte Event) {                           // cycle all lamps
  UNUSED(Event);
  if (QuerySwitch(73)) {                              // Up/Down switch pressed?
    *(DisplayLower+30) = DispPattern2[32 + 2 * (AppByte2 % 10)]; // and show the actual solenoid number
    *(DisplayLower+31) = DispPattern2[33 + 2 * (AppByte2 % 10)];
    *(DisplayLower+28) = DispPattern2[32 + 2 * (AppByte2 - (AppByte2 % 10)) / 10];
    *(DisplayLower+29) = DispPattern2[33 + 2 * (AppByte2 - (AppByte2 % 10)) / 10];
    TurnOnLamp(AppByte2);                             // turn on lamp
    if (AppByte2 > 1) {                               // and turn off the previous one
      TurnOffLamp(AppByte2-1);}
    else {
      TurnOffLamp(LampMax);}
    AppByte2++;                                       // increase the lamp counter
    if (AppByte2 == LampMax+1) {                      // maximum reached?
      AppByte2 = 1;}}                                 // then start again
  AppByte = ActivateTimer(1000, 0, ShowLamp);}        // come back in one second

void AllLamps_Enter(byte Event) {
  switch (Event) {
  case 72:
    WriteUpper(" SOUND  TEST  ");                     // go to next test
    Switch_Pressed = SoundTest_Enter;
    AppByte = 0;
    break;
  case 3:
    WriteUpper("FLASHNG LAMPS ");
    Switch_Pressed = ModeAllLamps;
    AppByte2 = 0;
    AppByte = ActivateTimer(500, 0, ShowAllLamps);}}  // start after 500ms

void ModeAllLamps(byte Event) {
  if (Event == 72) {
    KillTimer(AppByte);                               // kill running timer for next solenoid
    LampPattern = NoLamps;                            // Turn off all lamps
    WriteUpper("  ALL   LAMPS ");
    Switch_Pressed = AllLamps_Enter;}}

void ShowAllLamps(byte Event) {                       // Flash all lamps
  UNUSED(Event);
  if (AppByte2) {                                     // if all lamps are on
    LampPattern = NoLamps;                            // turn them off
    AppByte2 = 0;}
  else {                                              // or the other way around
    LampPattern = AllLamps;
    AppByte2 = 1;}
  AppByte = ActivateTimer(500, 0, ShowAllLamps);}     // come back in 500ms

void SoundTest_Enter(byte Switch) {
  switch (Switch) {
  case 72:
    analogWrite(VolumePin,255);                       // set volume to zero
    StopPlayingSound();
    ReleaseSolenoid(23);                              // disable flipper fingers
    ReleaseSolenoid(24);
    GameDefinition.AttractMode();
    break;
  case 3:
    WriteUpper("PLAYING SOUND ");
    WriteLower("            01");
    analogWrite(VolumePin,255-APC_settings[Volume]);  // set volume
    Switch_Pressed = SoundTest;
    AfterSound = NextTestSound;
    PlaySound(50, (char*) BK_TestSounds[AppByte]);}}

void NextTestSound() {
  if (QuerySwitch(73)) {                              // Up/Down switch pressed?
    AppByte++;}
  if (!BK_TestSounds[AppByte][0]) {
    AppByte = 0;}
  *(DisplayLower+30) = DispPattern2[32 + 2 * ((AppByte+1) % 10)]; // and show the next sound number
  *(DisplayLower+31) = DispPattern2[33 + 2 * ((AppByte+1) % 10)];
  *(DisplayLower+28) = DispPattern2[32 + 2 * ((AppByte+1) - ((AppByte+1) % 10)) / 10];
  *(DisplayLower+29) = DispPattern2[33 + 2 * ((AppByte+1) - ((AppByte+1) % 10)) / 10];
  PlaySound(50, (char*) BK_TestSounds[AppByte]);}

void SoundTest(byte Switch) {
  if (Switch == 3) {
    NextTestSound();}
  if (Switch == 72) {
    AfterSound = 0;
    StopPlayingMusic();
    WriteUpper(" SOUND  TEST  ");
    Switch_Pressed = SoundTest_Enter;
    AppByte = 0;}}
