// Rules for the Pinbot pinball machine

bool PB_OpenVisorFlag = false;                        // visor is being opened if true
bool PB_CloseVisorFlag = false;                       // visor is being closed if true
bool PB_DropWait = false;                             // ignore drop target switches when true
bool PB_DropRamp = false;                             // ramp needs to be dropped when possible
bool PB_EnergyActive = false;                         // score energy active?
bool PB_SkillShot = false;                            // is the skill shot active?
bool PB_EjectIgnore = false;                          // ignore the hole switch while the ball is in the hole
bool PB_IgnoreLock = false;                           // ignore the lock switches to cope with switch bouncing
byte PB_BallSave = 0;                                 // prevent immediate outlane drains 0=inactive 1=active 2=triggered
byte PB_ChestMode = 0;                                // current status of the chest and visor
uint16_t PB_SolarValue = 100;                         // current solar value / 1000
byte PB_SolarValueTimer = 0;                          // number of the timer for the solar value shot
byte PB_ChestLightsTimer = 0;                         // number of the timer controlling the chest lamp sequencing
byte PB_LampSweepActive = 0;                          // determines whether the backbox lamp sweep is active
byte PB_SkillMultiplier = 0;                          // Multiplier for the skill shot value
byte PB_DropBlinkLamp = 0;                            // number of the lamp currently blinking
byte PB_LitChestLamps = 0;                            // amount of lit chest lamps
byte PB_ChestLamp[4][5];                              // status of the chest lamps for each player / one column per byte
byte PB_Chest_Status[5];                              // number of visor openings for this player / > 100 means visor is actually open
byte PB_Planet[5];                                    // reached planets for all players
byte PB_ExBallsLit[5];                                // no of lanes lit for extra ball
byte PB_EjectMode[5];                                 // current mode of the eject hole
byte PB_EnergyValue[5];                               // energy value for current player (value = byte*2000)
byte PB_LampsToLight = 2;                             // number of lamps to light when chest is hit
byte *PB_ChestPatterns;                               // pointer to the current chest lamp pattern

const unsigned int PB_SolTimes[32] = {50,30,30,70,50,200,30,30,0,0,0,0,0,0,150,150,50,0,50,50,50,50,0,0,50,150,150,150,150,150,150,150}; // Activation times for solenoids (last 8 are C bank)
const byte PB_BallSearchCoils[10] = {3,4,5,17,19,22,6,20,21,0}; // coils to fire when the ball watchdog timer runs out
const byte PB_OpenVisorSeq[137] = {26,1,29,9,15,4,16,2, 32,9,15,1,31,9,26,1,27,9, 29,2,28,9,32,2,29,7, 26,5,15,6,16,2,31,5,15,7, 26,4,27,7,29,6,28,9, 29,5,26,7,15,5,16,5,32,5, 15,4,31,5,26,7,29,5,27,11, 28,1,29,12,26,4,32,9, 15,3,31,7,16,5,27,5,15,3, 28,7,26,2,29,7,32,10 ,29,2,31,10,26,3,27,2,31,5, 15,2,28,9,16,4,15,1, 32,10,26,3,31,9,29,4,27,12, 28,2,29,10,26,2,15,7, 32,4,16,5,31,4,15,7,26,5,0};
const byte PB_MultiballSeq[69] = {16,5,15,5,26,5,29,10,26,5,15,5,16,10,15,5,26,5,29,10,26,5,15,5,16,10,15,5,26,5,29,10,7,0,26,5,15,5,16,10,15,5,26,5,29,10,26,5,15,5,16,10,15,5,26,5,29,10,26,5,15,5,16,5,15,10,8,0,0};
const byte PB_ScoreEnergySeq[7] = {31,10,31,10,31,10,0};
const byte PB_Ball_Locked[5] = {26,30,26,30,0};
const byte PB_MultiplierSeq[83] = {27,2,29,6,26,7,27,6,15,5,16,8,27,5,29,8,30,5,26,1,31,11,28,1,15,6,31,3,16,3,28,8,27,1,29,9,26,5,27,4,15,4,31,7,16,3,31,7,26,1,29,10,32,1,15,11,30,1,16,4,31,3,29,8,28,6,26,5,31,7,15,1,16,11,29,10,31,2,15,6,16,4,0};
const byte PB_ChestRows[11][5] = {{28,36,44,52,60},{28,29,30,31,32},{36,37,38,39,40},{44,45,46,47,48},{52,53,54,55,56},{60,61,62,63,64},
                                {32,40,48,56,64},{31,39,47,55,63},{30,38,46,54,62},{29,37,45,53,61},{28,36,44,52,60}};
const byte PB_ExBallLamps[4] = {49, 50, 58, 57};
const char PB_GameMusic[6][13] = {{"BS_M03.BIN"},{"BS_M05.BIN"},{"BS_M06.BIN"},{"BS_M07.BIN"},{"BS_M08.BIN"},{"BS_M09.BIN"}};
const byte PB_ACselectRelay = 14;                     // solenoid number of the A/C select relay

const struct SettingTopic PB_setList[9] = {{"DROP TG TIME  ",HandleNumSetting,0,3,30},
    {" REACH PLANET ",HandleNumSetting,0,1,9},
    {" ENERGY TIMER ",HandleNumSetting,0,1,90},
    {"MULTBALVOLUME ",PB_HandleVolumeSetting,0,0,30},
    {"  BALL  SAVER ",HandleBoolSetting,0,0,0},
    {" RESET  HIGH  ",PB_ResetHighScores,0,0,0},
    {"RESTOREDEFAULT",RestoreDefaults,0,0,0},
    {"  EXIT SETTNGS",ExitSettings,0,0,0},
    {"",NULL,0,0,0}};

                                   // Duration..11111110...22222111...33322222...43333333...44444444...55555554...66666555
                                   // Duration..65432109...43210987...21098765...09876543...87654321...65432109...43210987

const struct LampPat PB_EnergyPat[43] =  {{16,0b01000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                          {16,0b01100000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                          {16,0b01110000,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000,0b00000000},
                                          {16,0b11110000,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000,0b00000000},
                                          {16,0b11110000,0b00000000,0b00001000,0b00000010,0b00000000,0b00000000,0b00000000},
                                          {16,0b11110000,0b00000000,0b00011000,0b00001110,0b00000000,0b00000000,0b00000000},
                                          {16,0b11110000,0b00000000,0b00111000,0b00011110,0b00001001,0b00000000,0b00000000},
                                          {16,0b11110000,0b00000000,0b01111000,0b00111110,0b00011011,0b00001000,0b00000000},
                                          {16,0b11110000,0b00000001,0b11111000,0b01111110,0b00111111,0b00011000,0b00001000},
                                          {16,0b11110000,0b00000001,0b11111000,0b11111110,0b01111111,0b00111000,0b00011000},
                                          {16,0b11110000,0b00000001,0b11111000,0b11111110,0b11111111,0b01111000,0b00111000},
                                          {16,0b11110000,0b00000001,0b11111000,0b11111110,0b11111111,0b11111000,0b01111000},
                                          {16,0b11110000,0b00001101,0b11111000,0b11111110,0b11111111,0b11111000,0b11111000},
                                          {16,0b11110000,0b00001101,0b11111000,0b11111110,0b11111111,0b11111000,0b11111000},
                                          {16,0b11110000,0b00011101,0b11111000,0b11111110,0b11111111,0b11111011,0b11111000},
                                          {16,0b11110000,0b00111101,0b11111111,0b11111110,0b11111111,0b11111001,0b11111000},
                                          {16,0b11110000,0b10111111,0b11111111,0b11111110,0b11111111,0b11111101,0b11111000},
                                          {16,0b11110001,0b11111111,0b11111111,0b11111110,0b11111111,0b11111101,0b11111000},
                                          {16,0b11111011,0b11111111,0b11111111,0b11111110,0b11111111,0b11111101,0b11111000},
                                          {16,0b11111111,0b11111111,0b11111111,0b11111110,0b11111111,0b11111101,0b11111010},
                                          {16,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111101,0b11111011},
                                          {16,0b10111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111},
                                          {16,0b10011111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111},
                                          {16,0b10001111,0b11111111,0b11111111,0b11111101,0b11111111,0b11111111,0b11111111},
                                          {16,0b00001111,0b11111111,0b11111111,0b11111101,0b11111111,0b11111111,0b11111111},
                                          {16,0b00001111,0b11111111,0b11110111,0b11111101,0b11111111,0b11111111,0b11111111},
                                          {16,0b00001111,0b11111111,0b11100111,0b11110001,0b11111111,0b11111111,0b11111111},
                                          {16,0b00001111,0b11111111,0b11000111,0b11100001,0b11110110,0b11111111,0b11111111},
                                          {16,0b00001111,0b11111111,0b10000111,0b11000001,0b11100100,0b11110111,0b11111111},
                                          {16,0b00001111,0b11111110,0b00000111,0b10000001,0b11000000,0b11100111,0b11110111},
                                          {16,0b00001111,0b11111110,0b00000111,0b00000001,0b10000000,0b11000111,0b11100111},
                                          {16,0b00001111,0b11111110,0b00000111,0b00000001,0b00000000,0b10000111,0b11000111},
                                          {16,0b00001111,0b11111110,0b00000111,0b00000001,0b00000000,0b00000111,0b10000111},
                                          {16,0b00001111,0b11110010,0b00000111,0b00000001,0b00000000,0b00000111,0b00000111},
                                          {16,0b00001111,0b11110010,0b00000111,0b00000001,0b00000000,0b00000111,0b00000111},
                                          {16,0b00001111,0b11100010,0b00000111,0b00000001,0b00000000,0b00000100,0b00000111},
                                          {16,0b00001111,0b11000010,0b00000000,0b00000001,0b00000000,0b00000110,0b00000111},
                                          {16,0b00001111,0b01000000,0b00000000,0b00000001,0b00000000,0b00000010,0b00000111},
                                          {16,0b00001110,0b00000000,0b00000000,0b00000001,0b00000000,0b00000010,0b00000111},
                                          {16,0b00000100,0b00000000,0b00000000,0b00000001,0b00000000,0b00000010,0b00000111},
                                          {16,0b00000000,0b00000000,0b00000000,0b00000001,0b00000000,0b00000010,0b00000101},
                                          {16,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000010,0b00000100},
                                          {0,0,0,0,0,0,0,0}};

const struct LampPat PB_MultiballPat[20] ={{320,0b01000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                           {320,0b01100000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                           {320,0b01110000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                           {320,0b11110000,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000,0b00000000},
                                           {320,0b11110000,0b00000000,0b00001000,0b00001010,0b00001000,0b00001000,0b00001000},
                                           {320,0b11110000,0b00000000,0b00011000,0b00011010,0b00011000,0b00011000,0b00011000},
                                           {320,0b11110000,0b00000000,0b00111000,0b00111010,0b00111000,0b00111000,0b00111000},
                                           {320,0b11110000,0b00000000,0b01111000,0b01111110,0b01111000,0b01111000,0b01111000},
                                           {320,0b11110000,0b00000000,0b11111000,0b11111110,0b11111001,0b11111000,0b11111000},
                                           {320,0b11110000,0b00000001,0b11111000,0b11111110,0b11111011,0b11111000,0b11111000},
                                           {320,0b11110000,0b00000001,0b11111000,0b11111110,0b11111111,0b11111000,0b11111000},
                                           {320,0b11110000,0b00011001,0b11111000,0b11111110,0b11111111,0b11111000,0b11111000},
                                           {320,0b11110000,0b00111111,0b11111000,0b11111110,0b11111111,0b11111000,0b11111000},
                                           {320,0b11110000,0b00111111,0b11111100,0b11111110,0b11111111,0b11111100,0b11111000},
                                           {320,0b11110000,0b01111111,0b11111110,0b11111110,0b11111111,0b11111100,0b11111000},
                                           {320,0b11110000,0b11111111,0b11111111,0b11111110,0b11111111,0b11111111,0b11111011},
                                           {320,0b11110011,0b11111111,0b11111111,0b11111110,0b11111111,0b11111111,0b11111011},
                                           {320,0b11111111,0b11111111,0b11111111,0b11111110,0b11111111,0b11111111,0b11111011},
                                           {320,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111011},
                                           {0,0,0,0,0,0,0,0}};

const struct LampPat PB_AttractPat1[5] = {{150,0b00000000,0b00000000,0b00000000,0b00000000,0b00100000,0b00000000,0b00000000},
                                          {150,0b00000000,0b00000000,0b00000000,0b01110000,0b01110000,0b01110000,0b00000000},
                                          {150,0b00000000,0b00000000,0b11111000,0b11111000,0b11011000,0b11111000,0b11111000},
                                          {150,0b10000000,0b00111111,0b11111000,0b10001000,0b10001000,0b10001000,0b11111000},
                                          {0,0,0,0,0,0,0,0}};

const struct LampPat PB_AttractPat2[5] = {{150,0b00010000,0b11000000,0b00000111,0b00000110,0b00100001,0b00000110,0b00000010},
                                          {150,0b00100011,0b00000000,0b00000000,0b01110000,0b01110010,0b01110001,0b00000001},
                                          {150,0b01001100,0b00000000,0b11111000,0b11111001,0b11011100,0b11111000,0b11111000},
                                          {150,0b10000000,0b00111111,0b11111000,0b10001000,0b10001000,0b10001000,0b11111000},
                                          {0,0,0,0,0,0,0,0}};

const struct LampPat PB_AttractPat3[4] = {{150,0b00010000,0b11000000,0b00000111,0b00000110,0b00000001,0b00000110,0b00000010},
                                          {150,0b00100011,0b00000000,0b00000000,0b00000000,0b00000010,0b00000001,0b00000001},
                                          {150,0b01001100,0b00000000,0b00000000,0b00000001,0b00000100,0b00000000,0b00000000},
                                          {0,0,0,0,0,0,0,0}};

const struct LampPat PB_AttractPat4[36] ={{150,0b10000001,0b00000101,0b10000000,0b00000000,0b00000000,0b00000001,0b00000000},
                                          {150,0b10010011,0b00001101,0b11000000,0b00000100,0b00000000,0b00000011,0b00000000},
                                          {150,0b00110110,0b00011000,0b11100000,0b00000100,0b00000001,0b00000010,0b00000010},
                                          {150,0b01100100,0b00110000,0b11110000,0b00000001,0b00000011,0b00000000,0b00000011},
                                          {150,0b01001000,0b01100000,0b11111000,0b00000011,0b00000110,0b00000100,0b00000001},
                                          {150,0b10001001,0b11000011,0b01111000,0b00001010,0b00000100,0b00000100,0b00000011},
                                          {150,0b10010011,0b10000011,0b00111001,0b00001100,0b00001000,0b00000010,0b00000010},
                                          {150,0b00110110,0b00000000,0b00011011,0b00001100,0b00001001,0b00001011,0b00000000},
                                          {150,0b01100100,0b00000000,0b00001110,0b00001001,0b00001011,0b00001001,0b00001000},
                                          {150,0b01001000,0b00000000,0b00000100,0b00001011,0b00001110,0b00001011,0b00011000},
                                          {150,0b10001001,0b00000001,0b00000000,0b00000010,0b00001100,0b00001010,0b00111010},
                                          {150,0b10010011,0b00000101,0b00000000,0b00000100,0b00000000,0b00001000,0b01111011},
                                          {150,0b00110110,0b00001100,0b00000000,0b00000100,0b00000001,0b00000000,0b11111001},
                                          {150,0b01100100,0b00011000,0b00000000,0b00000001,0b00000011,0b10000000,0b11110011},
                                          {150,0b01001000,0b00110000,0b00000000,0b00000011,0b10000110,0b10000010,0b11100010},
                                          {150,0b10001001,0b01100001,0b00000000,0b10000010,0b10000100,0b10000111,0b11000000},
                                          {150,0b10010011,0b11000011,0b00000000,0b11000100,0b10000000,0b10000101,0b10000000},
                                          {150,0b00110110,0b10000010,0b00000001,0b11100100,0b10000001,0b10000011,0b00000000},
                                          {150,0b01100100,0b00000000,0b00000011,0b11110001,0b10000011,0b00000010,0b00000010},
                                          {150,0b01001000,0b00000000,0b00000110,0b11110011,0b00010110,0b00000000,0b00000011},
                                          {150,0b10001001,0b00000001,0b00000100,0b01110010,0b00010100,0b00010000,0b00000001},
                                          {150,0b10010011,0b00000001,0b00000000,0b00110100,0b00010000,0b00110000,0b00000011},
                                          {150,0b00110110,0b00000100,0b00000000,0b00010100,0b00010001,0b01110010,0b00000010},
                                          {150,0b01100100,0b00001100,0b00000000,0b00000001,0b01010011,0b01110011,0b00000000},
                                          {150,0b01001000,0b00011000,0b00000000,0b00000011,0b01100110,0b01110001,0b00000000},
                                          {150,0b10001001,0b00110001,0b00000000,0b00000010,0b01100100,0b01100011,0b00000000},
                                          {150,0b10010011,0b01100001,0b00000000,0b00000100,0b01100000,0b01000110,0b00000010},
                                          {150,0b00110110,0b11000010,0b00000000,0b00000100,0b01100001,0b00000100,0b00000011},
                                          {150,0b01100100,0b10000010,0b00000001,0b00000001,0b00100011,0b00000000,0b00000001},
                                          {150,0b01001000,0b00000000,0b00000011,0b00000011,0b00000110,0b00000000,0b00000011},
                                          {150,0b10010001,0b00000001,0b00000011,0b00000010,0b00000100,0b00000010,0b00000010},
                                          {150,0b10010011,0b00000001,0b00000110,0b00000100,0b00000000,0b00000011,0b00000000},
                                          {150,0b00110110,0b00000000,0b00000100,0b00000100,0b00000000,0b00000001,0b00000000},
                                          {150,0b00100100,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                          {150,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
                                          {0,0,0,0,0,0,0,0}};
                                      
const struct LampFlow PB_AttractFlow[5] = {{1,PB_AttractPat1},{10,PB_AttractPat2},{1,PB_AttractPat3},{2,PB_AttractPat4},{0,0}};

const byte PB_GameStartPat[163] = {10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                   10,0b00000,0b00000,0b00000,0b00000,0b00000,
                                   10,0b11111,0b11111,0b11111,0b11111,0b11111,0};

const byte PB_OpenVisorPat[163] = {100,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    50,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    50,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    50,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    30,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    30,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    14,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    22,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    14,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    17,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    11,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    14,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    11,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    11,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    7,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    11,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    7,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    8,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    4,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    4,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    4,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    4,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    4,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    4,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    4,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    4,0b00000,0b00000,0b00000,0b00000,0b00000,
                                    4,0b11111,0b11111,0b11111,0b11111,0b11111,0};

const byte PB_RandomChestPat[25] =  {15,0b10011,0b00100,0b01100,0b10101,0b00101,
                                     15,0b11100,0b00101,0b10011,0b10100,0b10010,
                                     15,0b00101,0b11010,0b10110,0b01011,0b01101,
                                     15,0b01010,0b11011,0b01001,0b01010,0b11010,0};

const byte PB_ExpandingSquares[37] = {10,0b00000,0b00000,0b00100,0b00000,0b00000,
                                    10,0b00000,0b01110,0b01110,0b01110,0b00000,
                                    10,0b11111,0b11111,0b11111,0b11111,0b11111,
                                    10,0b11111,0b11111,0b11011,0b11111,0b11111,
                                    10,0b11111,0b10001,0b10001,0b10001,0b11111,
                                    15,0b00000,0b00000,0b00000,0b00000,0b00000,0};

const byte PB_WalkingLines[199] = {15,0b01000,0b01000,0b01000,0b01000,0b01000,
                                   15,0b00100,0b00100,0b00100,0b00100,0b00100,
                                   15,0b00010,0b00010,0b00010,0b00010,0b00010,
                                   15,0b00001,0b00001,0b00001,0b00001,0b00001,
                                   15,0b00001,0b00010,0b00010,0b00100,0b01000,
                                   15,0b00001,0b00010,0b00100,0b01000,0b10000,
                                   15,0b00001,0b00110,0b11000,0b00000,0b00000,
                                   15,0b11111,0b00000,0b00000,0b00000,0b00000,
                                   15,0b00000,0b11111,0b00000,0b00000,0b00000,
                                   15,0b00000,0b00000,0b11111,0b00000,0b00000,
                                   15,0b00000,0b00000,0b00000,0b11111,0b00000,
                                   15,0b00000,0b00000,0b00000,0b00000,0b11111,                                  
                                   15,0b00000,0b00000,0b11000,0b00110,0b00001,
                                   15,0b10000,0b01000,0b00100,0b00010,0b00001,
                                   15,0b00100,0b00100,0b00010,0b00010,0b00001,
                                   15,0b00001,0b00001,0b00001,0b00001,0b00001,
                                   15,0b00010,0b00010,0b00010,0b00010,0b00010,
                                   15,0b00100,0b00100,0b00100,0b00100,0b00100,
                                   15,0b01000,0b01000,0b01000,0b01000,0b01000,
                                   15,0b10000,0b10000,0b10000,0b10000,0b10000,
                                   15,0b00100,0b00100,0b01000,0b01000,0b10000,                                   
                                   15,0b00001,0b00010,0b00100,0b01000,0b10000,
                                   15,0b00000,0b00000,0b00110,0b01100,0b10000,
                                   15,0b00000,0b00000,0b00000,0b00000,0b11111,
                                   15,0b00000,0b00000,0b00000,0b11111,0b00000,
                                   15,0b00000,0b00000,0b11111,0b00000,0b00000,
                                   15,0b00000,0b11111,0b00000,0b00000,0b00000,
                                   15,0b11111,0b00000,0b00000,0b00000,0b00000,
                                   15,0b10000,0b01100,0b00110,0b00000,0b00000,
                                   15,0b10000,0b01000,0b00100,0b00010,0b00001,
                                   15,0b10000,0b01000,0b01000,0b00100,0b00100,
                                   15,0b10000,0b10000,0b10000,0b10000,0b10000,0};

                                                      // offsets of settings in the settings array
#define PB_DropTime 0                                 // drop target down time setting
#define PB_ReachPlanet 1                              // target planet setting
#define PB_EnergyTime 2                               // energy timer setting
#define PB_MultiballVolume 3                          // volume increase for the multiball
#define PB_BallSaver 4                                // ball saver for the outlanes

const byte PB_defaults[64] = {15,6,15,0,0,0,0,0,      // game default settings
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0};

struct GameDef PB_GameDefinition = {
    PB_setList,                                       // GameSettingsList
    (byte*)PB_defaults,                               // GameDefaultsPointer
    "PB_SET.APC",                                     // GameSettingsFileName
    "PBSCORES.APC",                                   // HighScoresFileName
    PB_AttractMode,                                   // AttractMode
    PB_SolTimes};                                     // Default activation times of solenoids

void PB_init() {
  if (APC_settings[DebugMode]) {                      // activate serial interface in debug mode
    Serial.begin(115200);}
  ACselectRelay = PB_ACselectRelay;                   // assign the number of the A/C select relay
  GameDefinition = PB_GameDefinition;}                // read the game specific settings and highscores

void PB_AttractMode() {
  AfterMusic = 0;
  DispRow1 = DisplayUpper;
  DispRow2 = DisplayLower;
  digitalWrite(VolumePin,HIGH);
  Switch_Pressed = PB_AttractModeSW;
  Switch_Released = DummyProcess;
  AddBlinkLamp(1, 150);                               // blink Game Over lamp
  LampReturn = PB_AttractLampCycle;
  PB_AttractLampCycle(1);
  PB_AttractDisplayCycle(0);}

void PB_AttractDisplayCycle(byte Event) {
  PB_CheckForLockedBalls(0);
  switch (Event) {
  case 0:
    WriteUpper2("THE APC       ");
    ActivateTimer(50, 0, ScrollUpper);
    ActivateTimer(2000, 0, PB_AttractScroll);
    WriteLower2("              ");
    ActivateTimer(1400, 0, ScrollLower);
    if (NoPlayers) {                                  // was there a previous game?
      Event++;}                                       // proceed to case 1 next time
    else {                                            // no previous games since power on
      Event = 2;}                                     // skip case 1
    break;
  case 1:                                             // show points of previous game
    WriteUpper2("              ");                    // erase display
    WriteLower2("              ");
    for (byte i=1; i<=NoPlayers; i++) {                    // display the points of all active players
      ShowNumber(8*i-1, Points[i]);}
    ActivateTimer(50, 0, ScrollUpper);
    ActivateTimer(900, 0, ScrollLower);
    Event++;
    break;
  case 2:                                             // Show highscores
    WriteUpper2("1>     2>     ");
    WriteLower2("              ");
    for (byte i=0; i<3; i++) {
      *(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[i]-32)*2];
      *(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[i]-32)*2+1];
      *(DisplayUpper2+24+2*i) = DispPattern1[(HallOfFame.Initials[3+i]-32)*2];
      *(DisplayUpper2+24+2*i+1) = DispPattern1[(HallOfFame.Initials[3+i]-32)*2+1];}
    ShowNumber(23, HallOfFame.Scores[0]);
    ShowNumber(31, HallOfFame.Scores[1]);
    ActivateTimer(50, 0, ScrollUpper);
    ActivateTimer(900, 0, ScrollLower2);
    Event++;
    break;
  case 3:
    WriteUpper2("3>     4>     ");
    WriteLower2("              ");
    for (byte i=0; i<3; i++) {
      *(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2];
      *(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2+1];
      *(DisplayUpper2+24+2*i) = DispPattern1[(HallOfFame.Initials[9+i]-32)*2];
      *(DisplayUpper2+24+2*i+1) = DispPattern1[(HallOfFame.Initials[9+i]-32)*2+1];}
    ShowNumber(23, HallOfFame.Scores[2]);
    ShowNumber(31, HallOfFame.Scores[3]);
    ActivateTimer(50, 0, ScrollUpper);
    ActivateTimer(900, 0, ScrollLower2);
    Event = 0;}
  ActivateTimer(4000, Event, PB_AttractDisplayCycle);}

void PB_AttractScroll(byte Dummy) {
  UNUSED(Dummy);
  WriteUpper2("PINBOT        ");
  AddScrollUpper(0);}

void PB_AttractLampCycle(byte Event) {                // play multiple lamp pattern series
  static byte State = 1;
  if (Event) {
    State = 1;}
  if (!State) {
    ActC_BankSol(8);}                                 // light sun flasher
  PatPointer = PB_AttractFlow[State].FlowPat;         // set the pointer to the current series
  FlowRepeat = PB_AttractFlow[State].Repeat;          // set the repetitions
  State++;                                            // increase counter
  if (!PB_AttractFlow[State].Repeat) {                // repetitions of next series = 0?
    State = 0;}                                       // reset counter
  ShowLampPatterns(1);}                               // call the player

void PB_AttractModeSW(byte Select) {
  switch(Select) {
  case 3:                                             // credit button
    RemoveBlinkLamp(1);                               // stop the blinking of the game over lamp
    ShowLampPatterns(0);                              // stop lamp animations
    KillAllTimers();
    if (APC_settings[Volume]) {                       // system set to digital volume control?
      analogWrite(VolumePin,255-APC_settings[Volume]);} // adjust PWM to volume setting
    else {
      digitalWrite(VolumePin,HIGH);}                  // turn off the digital volume control
    Switch_Pressed = AddPlayerSW;
    for (byte i=0; i< 8; i++) {
      LampColumns[i] = 0;}
    LampPattern = LampColumns;
    TurnOnLamp(3);                                    // turn on Ball in Play lamp
    NoPlayers = 0;
    WriteUpper("              ");
    WriteLower("              ");
    Ball = 1;
    PB_AddPlayer();
    for (byte i=1;i<5;i++) {                          // for all players
      PB_Chest_Status[i] = 0;                         // reset the number of number of visor openings
      PB_ResetPlayersChestLamps(i);                   // reset the chest lamps
      PB_EnergyValue[i] = 25;                         // reset the energy value to 50K
      PB_ExBallsLit[i] = 0;                           // reset the lit extra balls
      PB_EjectMode[i] = 0;                            // reset the mode of the eject hole
      PB_Planet[i] = 0;}                              // reset reached planets
    PB_LitChestLamps = 0;
    InLock = 0;
    Player = 1;
    ExBalls = 0;
    Multiballs = 1;
    Bonus = 1;
    BonusMultiplier = 1;
    if (QuerySwitch(49) || QuerySwitch(50) || QuerySwitch(51)) {      // any drop target down?
      ActA_BankSol(4);}                               // reset it
    if (!QuerySwitch(44)) {                           // ramp in up state?
      ActA_BankSol(6);}                               // put it down
    ActivateSolenoid(0, 12);                          // turn off playfield GI
    PB_ChestMode = 20;                                // just play a chest pattern
    PB_ChestPatterns = (byte*)PB_GameStartPat;        // set chest lamps pattern
    PB_ChestLightHandler(100);                        // start player
    AfterSound = PB_GameStart;                        // release a new ball (2 expected balls in the trunk)
    PlaySound(51, "0_ad.snd");                        // 'Pinbot circuits activated'
    ActivateSolenoid(0, 23);                          // enable flipper fingers
    ActivateSolenoid(0, 24);
    break;
  case 8:                                             // high score reset
    digitalWrite(Blanking, LOW);                      // invoke the blanking
    break;
  case 46:
    if (PB_CloseVisorFlag) {
      PlaySound(51, "0_f3.snd");
      PB_CloseVisorFlag = false;
      ReleaseSolenoid(13);}
    break;
  case 47:
    if (PB_OpenVisorFlag) {
      PlaySound(51, "0_f3.snd");
      PB_OpenVisorFlag = false;
      ReleaseSolenoid(13);}
    break;
  case 72:
    Switch_Pressed = DummyProcess;
    RemoveBlinkLamp(1);                               // stop the blinking of the game over lamp
    BlinkScore(0);
    ShowLampPatterns(0);                              // stop lamp animations
    KillAllTimers();
    LampPattern = NoLamps;                            // Turn off all lamps
    ReleaseAllSolenoids();
    if (APC_settings[DebugMode]) {                    // deactivate serial interface in debug mode
      Serial.end();}
    if (!QuerySwitch(73)) {                           // Up/Down switch pressed?
      WriteUpper("  TEST  MODE  ");
      WriteLower("              ");
      AppByte = 0;
      ActivateTimer(1000, 0, PB_Testmode);}
    else {
      Settings_Enter();}
    break;
  }}

void PB_GameStart(byte Dummy) {
  UNUSED(Dummy);
  AfterSound = 0;
  PB_NewBall(2);
  ReleaseSolenoid(12);}                                // turn playfield GI back on

void AddPlayerSW(byte Switch) {
  if (Switch == 3) {
    PB_AddPlayer();}}

void PB_CheckForLockedBalls(byte Dummy) {             // check if balls are locked and release them
  UNUSED(Dummy);
  if (QuerySwitch(16)) {                              // for the outhole
    ActA_BankSol(1);}
  if (QuerySwitch(38)) {                              // for the single eject hole
    ActA_BankSol(3);}
  if (QuerySwitch(25) || QuerySwitch(26)) {           // for the eyes
    if (QuerySwitch(47)) {                            // visor is open
      if (QuerySwitch(25)) {                          // left eye
        ActA_BankSol(7);}                             // eject ball
      else {                                          // right eye
        ActA_BankSol(8);}}                            // eject ball
    else {                                            // visor is not open
      ActivateSolenoid(0, 13);                        // activate visor motor
      ActivateTimer(2000, 0, PB_OpenVisor);}}         // ignore the visor switch for 2 seconds
  else {                                              // no balls in lock
    if (!QuerySwitch(46)) {                               // visor not closed
      ActivateSolenoid(0, 13);                        // activate visor motor
      PlaySound(50, "0_f1.snd");
      ActivateTimer(2000, 0, PB_CloseVisor);}}}       // ignore the visor switch for 2 seconds

void PB_AddPlayer() {
  if ((NoPlayers < 4) && (Ball == 1)) {               // if actual number of players < 4
    NoPlayers++;                                      // add a player
    Points[NoPlayers] = 0;                            // delete the points of the new player
    ShowPoints(NoPlayers);}}                          // and show them

void PB_NewBall(byte Balls) {                         // release ball (Event = expected balls on ramp)
  ShowAllPoints(0);
  PlayMusic(50, "1_94.snd");                          // play non looping part of music track
  QueueNextMusic("1_94L.snd");                        // queue looping part as next music to be played}
  Bonus = 1;
  BonusMultiplier = 1;                                // reset bonus multiplier
  for (byte i=0; i<4; i++) {                               // turn off the corresponding lamps
    TurnOffLamp(9+i);}
  PB_SkillMultiplier = 0;                             // reset skill shot multiplier
  *(DisplayUpper+16) = LeftCredit[32 + 2 * Ball];     // show current ball in left credit
  //*(DisplayUpper+17) = LeftCredit[33 + 2 * Ball];
  BlinkScore(1);                                      // turn on score blinking
  PB_ClearChest();                                    // turn off chest lamps
  PB_CountLitChestLamps();                            // check how many chest lamps are lit
  if ((PB_Chest_Status[Player] > 100)) {              // > 100 means visor has to be open
    PB_Chest_Status[Player] = PB_Chest_Status[Player] - 100; // use it as a counter for opened visors
    PB_LampsToLight = 1;
    PB_ChestMode = 0;                                 // indicate an open visor
    if (!QuerySwitch(47)) {                           // visor not already open?
      PB_OpenVisorFlag = true;
      ActivateSolenoid(0, 13);}                       // open visor
    PB_EyeBlink(1);
    PB_ChestPatterns = (byte*)PB_WalkingLines;        // set chest lamps pattern
    PB_ChestLightHandler(100);}                       // start player
  else {
    if (!QuerySwitch(46)) {                           // visor not already closed?
      PlaySound(50, "0_f1.snd");
      PB_CloseVisorFlag = true;
      ActivateSolenoid(0, 13);}                       // close visor
    if (PB_LitChestLamps) {                           // chest lamps lit?
      PB_ChestMode = 12;                              // indicate that the visor can not be opened with one hit
      PB_ChestPatterns = (byte*)PB_RandomChestPat;}   // set chest lamps pattern
    else {
      PB_ChestMode = 1;}                              // indicate that the visor can be opened with one hit
    PB_ChestLightHandler(100);}                       // start chest animation
  if (PB_Chest_Status[Player] > 2) {                  // visor has been open more than 2 times
    PB_LampsToLight = 1;}                             // TODO change according to difficulty setting
  else {
    PB_LampsToLight = 2;}
  for (byte i=0; i<4; i++) {                               // restore extra ball lamps
    if (i<PB_ExBallsLit[Player]){
      TurnOnLamp(PB_ExBallLamps[i]);}
    else {
      TurnOffLamp(PB_ExBallLamps[i]);}}
  if (PB_EjectMode[Player] < 5) {
    for (byte i=0; i<PB_EjectMode[Player]; i++) {
      TurnOnLamp(13+i);}}
  else {
    for (byte i=0; i<(PB_EjectMode[Player]-5); i++) {
      TurnOnLamp(13+i);}
    AddBlinkLamp(PB_EjectMode[Player]+8, 100);}
  PB_DropBlinkLamp = 41;
  PB_CycleDropLights(1);                              // start the blinking drop target lights
  if (PB_Planet[Player] < game_settings[PB_ReachPlanet]) {  // target planet not reached yet?
    AddBlinkLamp(18+game_settings[PB_ReachPlanet],100);}    // let target planet blink
  for (byte i=0; i<9; i++) {                               // update planets
    if (PB_Planet[Player] > i) {
      TurnOnLamp(19+i);}
    else {
      TurnOffLamp(19+i);}}
  PB_GiveBall(Balls);}

void PB_GiveBall(byte Balls) {
  if (PB_SkillMultiplier < 10) {
    PB_SkillMultiplier++;
    if (PB_SkillMultiplier == 10) {
      //PlayFlashSequence((byte*) PB_MultiballSeq);     // super skill shot
    }}
  else {
    PB_SkillMultiplier = 1;}
  WriteUpper2(" VORTEX   X   ");
  WriteLower2("              ");
  ShowNumber(15, PB_SkillMultiplier);                 // show multiplier
  ShowMessage(3);
  if (game_settings[PB_BallSaver]) {                  // activate ball saver if enabled
    PB_BallSave = 1;}
  PB_SkillShot = true;                                // the first shot is a skill shot
  if (!QuerySwitch(20)) {                             // ball not yet in shooter lane?
    Switch_Released = DummyProcess;
    ActA_BankSol(2);                                  // release ball
    Switch_Pressed = PB_BallReleaseCheck;             // set switch check to enter game
    CheckReleaseTimer = ActivateTimer(5000, Balls-1, PB_CheckReleasedBall);} // start release watchdog
  else {                                              // ball already in shooter lane
    Switch_Released = PB_CheckShooterLaneSwitch;      //  wait for switch 20 to be released
    Switch_Pressed = PB_ResetBallWatchdog;}}

void PB_CheckShooterLaneSwitch(byte Switch) {
  if (Switch == 20) {                                 // shooter lane switch released?
    Switch_Released = DummyProcess;
    PlaySound(50, "1_95.snd");
    if (!BallWatchdogTimer) {
      BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}}}

void PB_ResetBallWatchdog(byte Switch) {              // handle switches during ball release
  if ((Switch > 11)&&(Switch != 17)&&(Switch != 18)&&(Switch != 19)&&(Switch != 44)&&(Switch != 46)&&(Switch != 47)) { // playfield switch activated?
    if (BallWatchdogTimer) {
      KillTimer(BallWatchdogTimer);}                  // stop watchdog
    if (PB_DropRamp&&(Switch != 45)&&(Switch != 49)&&(Switch != 50)&&(Switch != 51)) { // switch not close to the ramp?
      PB_DropRamp = false;                            // clear request
      ActA_BankSol(6);}                               // drop ramp
    if (PB_SkillShot) {                               // is this a skill shot?
      if (Switch != 20) {                             // no bouncing of the shooter lane switch?
        PB_SkillShot = false;                         // the next shot is not a skill shot any more
        byte c = 0;
        switch (Switch) {                             // was a skill shot target hit
        case 22:
          c = 20;
          PlaySound(50, "1_91.snd");
          break;
        case 23:
          c = 100;
          PlaySound(50, "0_6e_1.snd");
          break;
        case 24:
          c = 5;
          PlaySound(50, "1_91.snd");
          break;}
        PlayMusic(50, "1_01L.snd");                   // play music track
        QueueNextMusic("1_01L.snd");                  // track is looping so queue it also
        WriteUpper2(" VORTEX   X   ");
        WriteLower2("              ");
        ShowNumber(31, c * PB_SkillMultiplier * 1000);// show skill shot points
        ShowNumber(15, PB_SkillMultiplier);           // and multiplier
        ShowMessage(3);
        Points[Player] += c * 1000 * PB_SkillMultiplier;
        ShowPoints(Player);}}
    BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}
  PB_GameMain(Switch);}                               // process current switch

void PB_BallReleaseCheck(byte Switch) {               // handle switches during ball release
  if ((Switch > 11)&&(Switch != 17)&&(Switch != 18)&&(Switch != 19)&&(Switch != 44)&&(Switch != 46)&&(Switch != 47)) { // playfield switch activated?
    if (CheckReleaseTimer) {
      KillTimer(CheckReleaseTimer);
      CheckReleaseTimer = 0;}                         // stop watchdog
    Switch_Pressed = PB_ResetBallWatchdog;
    if (Switch == 20) {                               // ball is in the shooter lane
      Switch_Released = PB_CheckShooterLaneSwitch;}   // set mode to register when ball is shot
    else {
      if (!BallWatchdogTimer) {
        BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}}} // set switch mode to game
  PB_GameMain(Switch);}

void PB_CheckReleasedBall(byte Balls) {               // ball release watchdog
  CheckReleaseTimer = 0;
  BlinkScore(0);                                      // stop score blinking
  WriteUpper("WAITINGFORBALL");                       // indicate a problem
  WriteLower("              ");
  if (Balls == 10) {                                  // indicating a previous trunk error
    WriteUpper("              ");
    WriteLower("              ");
    ShowAllPoints(0);
    BlinkScore(1);                                    // turn on score blinking
    ActA_BankSol(2);}
  byte c = PB_CountBallsInTrunk();
  if (c == Balls) {                                   // expected number of balls in trunk
    WriteUpper("  BALL MISSING");
    if (QuerySwitch(16)) {                            // outhole switch still active?
      ActA_BankSol(1);}}                              // shove the ball into the trunk
  else {                                              //
    if (c == 5) {                                     // balls not settled
      WriteLower(" TRUNK  ERROR ");
      Balls = 10;}
    else {
      if ((c > Balls) || !c) {                        // more balls in trunk than expected or none at all
        WriteUpper("              ");
        WriteLower("              ");
        ShowAllPoints(0);
        BlinkScore(1);                                // turn on score blinking
        ActA_BankSol(2);}}}                           // release again
  CheckReleaseTimer = ActivateTimer(5000, Balls, PB_CheckReleasedBall);}

byte PB_CountBallsInTrunk() {
  byte Balls = 0;
  for (byte i=0; i<2; i++) {                               // check how many balls are on the ball ramp
    if (QuerySwitch(17+i)) {
      if (Balls < i) {
        return 5;}                                    // send warning
      Balls++;}}
  return Balls;}

void PB_SearchBall(byte Counter) {                    // ball watchdog timer has run out
  BallWatchdogTimer = 0;
  if (QuerySwitch(16)) {                              // ball in outhole?
    BlockOuthole = false;
    ActivateTimer(1000, 0, PB_ClearOuthole);}
  else {
    if (QuerySwitch(20)) {                            // if ball is waiting to be launched
      BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}  // restart watchdog
    else {                                            // if ball is really missing
      byte c = PB_CountBallsInTrunk();                // recount all balls
      if (c == 5) {                                   // balls have not settled yet
        WriteUpper("  BALL  STUCK ");
        BallWatchdogTimer = ActivateTimer(1000, 0, PB_SearchBall);} // and try again in 1s
      else {
        if (c == 2) {                                 // found 2 balls in trunk?
          if (BlockOuthole) {                         // is the outhole blocked
            PB_BallEnd(0);}                           // then it was probably a ball loss gone wrong
          else {
            ActivateTimer(1000, 2, PB_NewBall);}}     // otherwise try it with a new ball
        else {
          byte c2 = 0;
          for (byte i=0; i<2; i++) {                       // count balls in lock
            if (QuerySwitch(25+i)) {
              c2++;}}
          if (c2 > InLock) {                          // more locked balls found than expected?
            PB_HandleLock(0);                         // lock them
            BallWatchdogTimer = ActivateTimer(30000, 0, PB_SearchBall);}
          else {
            WriteUpper("  BALL  SEARCH");
            ActivateSolenoid(0, PB_BallSearchCoils[Counter]); // fire coil to get ball free
            if (PB_BallSearchCoils[Counter] == 5) {   // ramp raise?
              PB_DropRamp = true;}                    // set flag to drop ramp
            if (PB_BallSearchCoils[Counter] == 6) {   // ramp down?
              PB_DropRamp = false;}                   // clear flag to drop ramp
            Counter++;
            if (Counter == 9) {                       // all coils fired?
              Counter = 0;}                           // start again
            if (QuerySwitch(46) && !QuerySolenoid(13)) {  // visor closed and motor not active?
              ActivateSolenoid(0, 13);                // open it enough to deactivate switch 46
              ActivateTimer(2000, 0, PB_CloseVisor);} // and prepare to close it again
            BallWatchdogTimer = ActivateTimer(1000, Counter, PB_SearchBall);}}}}}} // come again in 1s if no switch is activated

void PB_OpenVisor(byte Dummy) {
  UNUSED(Dummy);
  PB_OpenVisorFlag = true;}                           // set flag to stop visor motor when open

void PB_CloseVisor(byte Dummy) {
  UNUSED(Dummy);
  PB_CloseVisorFlag = true;}                          // set flag to stop visor motor when closed

void PB_ClearOuthole(byte Event) {
  UNUSED(Event);
  if (QuerySwitch(16)) {                              // outhole switch still active?
    if (!BlockOuthole && !C_BankActive) {             // outhole blocked?
      BlockOuthole = true;                            // block outhole until this ball has been processed
      ActivateSolenoid(30, 1);                        // put ball in trunk
      ActivateTimer(2000, 0, PB_BallEnd);}
    else {
      ActivateTimer(2000, 0, PB_ClearOuthole);}}}     // come back in 2s if outhole is blocked

void PB_MultiballThunder2(byte Dummy) {
  UNUSED(Dummy);
  PlayMusic(50, "1_04.snd");                          // play non looping part of music track
  QueueNextMusic("1_04L.snd");}                       // queue looping part as next music to be played

void PB_MultiballThunder(byte State) {
  if (State < 8) {
    PlaySound(50, "0_d7.snd");
    State++;
    ActivateTimer(500, State, PB_MultiballThunder);}
  else if (State < 9) {
    MusicVolume = 0;
    PlaySound(50, "0_fb.snd");
    ActivateTimer(3300, 0, PB_MultiballThunder2);
    ActivateTimer(6000, 9, PB_MultiballThunder);}
  else {
    StopPlayingSound();}}

void PB_PlayEjectHoleSounds(byte Number) {
  PlaySound(50, "1_83.snd");
  Number--;
  if (Number) {
    ActivateTimer(400, Number, PB_PlayEjectHoleSounds);}
  else {
    PlaySound(50, "0_e9.snd");}}

void PB_EnergyRestoreLamps(byte Dummy) {
  UNUSED(Dummy);
  LampReturn = 0;
  AfterSound = 0;
  RestoreMusicVolume(10);
  LampPattern = LampColumns;}

void PB_GameMain(byte Switch) {
  static bool RampSound;
  byte c=0;
  byte i=0;
  switch(Switch) {
  case 1:
  case 2:
  case 7:
  case 9:
    WriteUpper(" TILT  WARNING");
    ActivateTimer(3000, 0, ShowAllPoints);
    break;
  case 3:
    if (Ball == 1) {
      PB_AddPlayer();}
    break;
  case 4:
  case 5:
  case 6:
    WriteUpper(" FREE   GAME  ");
    ActivateTimer(3000, 0, ShowAllPoints);
    break;
  case 8:                                             // high score reset
    Serial.print("PB_CycleDropLights = ");            // print address reference table
    Serial.println((unsigned int)&PB_CycleDropLights);
    Serial.print("PB_AttractDisplayCycle = ");
    Serial.println((unsigned int)&PB_AttractDisplayCycle);
    Serial.print("PB_AttractLampCycle = ");
    Serial.println((unsigned int)&PB_AttractLampCycle);
    Serial.print("PB_AttractScroll = ");
    Serial.println((unsigned int)&PB_AttractScroll);
    Serial.print("ScrollUpper = ");
    Serial.println((unsigned int)&ScrollUpper);
    Serial.print("ScrollLower = ");
    Serial.println((unsigned int)&ScrollLower);
    Serial.print("ScrollLower2 = ");
    Serial.println((unsigned int)&ScrollLower2);
    Serial.print("PB_SearchBall = ");
    Serial.println((unsigned int)&PB_SearchBall);
    Serial.print("PB_ChestLightHandler = ");
    Serial.println((unsigned int)&PB_ChestLightHandler);
    Serial.print("BlinkScore = ");
    Serial.println((unsigned int)&BlinkScore);
    Serial.print("BlinkLamps = ");
    Serial.println((unsigned int)&BlinkLamps);
    Serial.print("ShowMessage = ");
    Serial.println((unsigned int)&ShowMessage);
    Serial.print("PB_CountBonus = ");
    Serial.println((unsigned int)&PB_CountBonus);
    Serial.print("PB_AfterExBallRelease = ");
    Serial.println((unsigned int)&PB_AfterExBallRelease);
    Serial.print("PB_BallEnd = ");
    Serial.println((unsigned int)&PB_BallEnd);
    Serial.print("ReleaseSolenoid = ");
    Serial.println((unsigned int)&ReleaseSolenoid);
    Serial.print("ActSolenoid = ");
    Serial.println((unsigned int)&ActSolenoid);
    Serial.print("ShowLampPatterns = ");
    Serial.println((unsigned int)&ShowLampPatterns);
    Serial.print("StrobeLights = ");
    Serial.println((unsigned int)&StrobeLights);
    Serial.print("SelectSettings = ");
    Serial.println((unsigned int)&SelectSettings);
    Serial.print("PB_OpenVisor = ");
    Serial.println((unsigned int)&PB_OpenVisor);
    Serial.print("PB_CloseVisor = ");
    Serial.println((unsigned int)&PB_CloseVisor);
    Serial.print("PB_StartChestPattern = ");
    Serial.println((unsigned int)&PB_StartChestPattern);
    Serial.print("PB_CheckReleasedBall = ");
    Serial.println((unsigned int)&PB_CheckReleasedBall);
    Serial.print("PB_ClearOuthole = ");
    Serial.println((unsigned int)&PB_ClearOuthole);
    Serial.print("PB_NewBall = ");
    Serial.println((unsigned int)&PB_NewBall);
    Serial.print("ShowAllPoints = ");
    Serial.println((unsigned int)&ShowAllPoints);
    Serial.print("PB_HandleLock = ");
    Serial.println((unsigned int)&PB_HandleLock);
    Serial.print("PB_ClearEjectHole = ");
    Serial.println((unsigned int)&PB_ClearEjectHole);
    Serial.print("PB_HandleDropTargets = ");
    Serial.println((unsigned int)&PB_HandleDropTargets);
    Serial.print("PB_ReopenVisor = ");
    Serial.println((unsigned int)&PB_ReopenVisor);
    Serial.print("PB_ClearOutLock = ");
    Serial.println((unsigned int)&PB_ClearOutLock);
    Serial.print("DelaySolenoid = ");
    Serial.println((unsigned int)&DelaySolenoid);
    Serial.print("InLock = ");
    Serial.println((unsigned int)InLock);
    Serial.print("Multiballs = ");
    Serial.println((unsigned int)Multiballs);
    Serial.print("PB_ChestMode = ");
    Serial.println((unsigned int)PB_ChestMode);
    c=0;
    i=1;
    while (c<ActiveTimers) {                          // list all active timers
      if (TimerEvent[i]) {
        c++;
        Serial.print("T");
        Serial.print(i);
        Serial.print("= ");
        Serial.print(TimerArgument[i]);
        Serial.print(" ");
        Serial.println((unsigned int)TimerEvent[i]);}
      i++;}
    SwitchDisplay(1);
    digitalWrite(Blanking, LOW);                      // invoke the blanking
    break;
  case 10:                                            // left lane change
    PB_MoveExBallLamps(0);
    PB_BallSave = 0;                                  // ball saver is off when flippers are used
    break;
  case 11:                                            // right lane change
    PB_MoveExBallLamps(1);
    PB_BallSave = 0;                                  // ball saver is off when flippers are used
    break;
  case 12:                                            // left outlane
    PlaySound(50, "1_a5.snd");
    if (PB_BallSave) {
      PB_BallSave = 2;                                // trigger the ball saver
      AddBlinkLamp(33, 250);}
    else {
      Points[Player] += 5000;
      ShowPoints(Player);
      PB_AddBonus(3);
      if (QueryLamp(49)) {
        TurnOffLamp(49);
        PB_ExBallsLit[Player]--;
        PB_GiveExBall();}}
    break;
  case 13:                                            // left inlane
    PlaySound(50, "1_8b.snd");
    Points[Player] += 5000;
    ShowPoints(Player);
    PB_AddBonus(1);
    TurnOnLamp(18);
    if (QueryLamp(50)) {
      TurnOffLamp(50);
      PB_ExBallsLit[Player]--;
      PB_GiveExBall();}
    break;
  case 14:                                            // right inlane
    PlaySound(50, "1_8b.snd");
    Points[Player] += 5000;
    ShowPoints(Player);
    PB_AddBonus(1);
    if (QueryLamp(58)) {
      TurnOffLamp(58);
      PB_ExBallsLit[Player]--;
      PB_GiveExBall();}
    if (PB_EjectMode[Player] < 5) {
      if (PB_EjectMode[Player] == 4) {
        AddBlinkLamp(15, 100);}
      else {
        AddBlinkLamp(PB_EjectMode[Player]+13, 100);}
      PB_EjectMode[Player] = PB_EjectMode[Player] + 5;}
    break;
  case 15:                                            // right outlane
    PlaySound(50, "1_a5.snd");
    if (PB_BallSave) {
      PB_BallSave = 2;                                // trigger the ball saver
      AddBlinkLamp(33, 250);}
    else {
      Points[Player] += 5000;
      ShowPoints(Player);
      PB_AddBonus(3);
      if (QueryLamp(57)) {
        TurnOffLamp(57);
        PB_ExBallsLit[Player]--;
        PB_GiveExBall();}}
    break;
  case 16:                                            // outhole
    ActivateTimer(200, 0, PB_ClearOuthole);           // check again in 200ms
    break;
  case 19:                                            // advance planet
    if (QueryLamp(51)) {                              // special lit?
      TurnOffLamp(51);
      PB_AddExBall();}
    else if (QueryLamp(18)) {                              // advance planet lit?
      TurnOffLamp(18);
      PB_AddBonus(1);
      PB_AdvancePlanet();}
    else {
      PlaySound(50, "1_96.snd");}
    break;
  case 20:                                            // shooter lane
    if (!PB_SkillShot) {
      if (PB_SkillMultiplier < 10) {
        PB_SkillMultiplier++;
        WriteUpper2(" VORTEX   X   ");
        WriteLower2("              ");
        ShowNumber(15, PB_SkillMultiplier);           // show multiplier
        ShowMessage(3);
        if (PB_SkillMultiplier == 10) {
          // super skill shot
        }}
      PB_SkillShot = true;}                           // the first shot is a skill shot
    break;
  case 25:                                            // left eye
  case 26:                                            // right eye
    if (!PB_IgnoreLock) {
      PB_IgnoreLock = true;
      PB_AddBonus(1);
      ActivateTimer(1000, 0, PB_HandleLock);}         // handle locked balls after 1s
    break;
  case 28:                                            // chest
  case 29:
  case 30:
  case 31:
  case 32:
  case 33:
  case 34:
  case 35:
  case 36:
  case 37:
    if (PB_ChestMode) {                               // visor closed?
      PB_ChestLightHandler(0);                        // stop chest animation
      AppByte = Switch-27;                            // buffer the switch number
      if (Switch > 32) {                              // is it a row?
        AppByte = 16 - AppByte;}                      // turn the rows upside down
      if (PB_ChestMode < 11) {                        // visor can be opened with one row / column hit
        if (AppByte == PB_ChestMode) {                // correct row / column hit?
          PB_ChestMode = 0;                           // indicate an open visor
          PB_AddBonus(1);
          PB_OpenVisorProc(0);}                       // open visor
        else {                                        // incorrect row / column hit
          PB_ChestMode = Switch-17;                   // Store row / column hit
          PB_SetChestLamps(Switch-28);                // add the lamps for the hit row / column in PB_ChestLamp
          PB_ClearChest();                            // turn off chest lamps
          PB_ChestLightHandler(1);}}                  // call effect routine
      else {                                          // the cumbersome way to open the visor
        PB_ChestMode = Switch-17;                     // Store row / column hit
        PB_SetChestLamps(Switch-28);                  // add the lamps for the hit row / column in PB_ChestLamp
        if (PB_LitChestLamps == 25) {                 // complete chest lit?
          PB_OpenVisorProc(0);}                       // open visor
        else {
          PB_ClearChest();                            // turn off chest lamps
          PB_ChestLightHandler(1);}}}                 // call effect routine
    else {                                            // visor open
      PB_SetChestLamps(Switch-28);}                   // add the lamps for the hit row / column in PB_ChestLamp
    break;
  case 38:                                            // eject hole
    if (!PB_EjectIgnore) {
      PB_EjectIgnore = true;
      PB_AddBonus(1);
      if (PB_EjectMode[Player] < 5) {
        PlaySound(50, "1_a3.snd");
        Points[Player] += 2000;
        ShowPoints(Player);
        ActivateTimer(1000, 3, PB_ClearEjectHole);}
      else {
        ActivateTimer(400, 7, PB_PlayEjectHoleSounds);
        if (PB_EjectMode[Player] == 9) {
          RemoveBlinkLamp(15);
          TurnOnLamp(15);
          PB_EjectMode[Player] = 4;
          Points[Player] += Multiballs * 75000;}
        else {
          RemoveBlinkLamp(PB_EjectMode[Player] + 8);
          TurnOnLamp(PB_EjectMode[Player] + 8);
          PlayFlashSequence((byte*) PB_OpenVisorSeq); // play flasher sequence
          Points[Player] += Multiballs * (PB_EjectMode[Player] - 4) * 25000;
          ShowPoints(Player);
          PB_EjectMode[Player] = PB_EjectMode[Player] - 4;
          if (PB_EjectMode[Player] == 4) {
            PB_AddExBall();}}
        ActivateTimer(1000, 3, PB_ClearEjectHole);}}
    break;
  case 39:                                            // solar ramp exit
    uint16_t Buffer;
    PB_AddBonus(1);
    RampSound = false;
    if (PB_SolarValueTimer) {                         // solar ramp lit
      MusicVolume = 3;                                // reduce music volume
      PB_MultiballThunder(0);                         // play sound effects
      KillTimer(PB_SolarValueTimer);
      PB_SolarValueTimer = 0;
      RemoveBlinkLamp(35);
      Points[Player] += PB_SolarValue * 1000;
      ShowPoints(Player);
      ActivateTimer(2000,1,PB_EyeBlink);
      Buffer = PB_SolarValue;
      PB_SolarValue = 100;
      PB_ClearOutLock(0);}
    else {                                            // solar ramp not lit
      PlaySound(50, "1_a9.snd");
      if (BonusMultiplier < 5) {                      // increase bonus multiplier
        TurnOnLamp(8+BonusMultiplier);                // turn on the corresponding lamp
        BonusMultiplier++;}
      if (PB_SolarValue < 5000) {
        PB_SolarValue += 50;
        if (PB_SolarValue > 5000) {
          PB_SolarValue = 5000;}}
      Buffer = PB_SolarValue;}
    if (Player < 3) {
      WriteLower2("SOLAR =       ");
      ShowNumber(31, Buffer*1000);}
    else {
      WriteUpper2("SOLAR =       ");
      ShowNumber(15, Buffer*1000);}
    break;
  case 40:                                            // ramp entrance
    if (RampSound) {
      RampSound = false;
      PlaySound(50, "1_a9.snd");}
    else {
      RampSound = true;
      PlaySound(50, "1_a8.snd");}
    break;
  case 45:                                            // score energy switch
    if (PB_EnergyActive) {
      Points[Player] += PB_EnergyValue[Player] * 2000;// score energy value
      WriteUpper2(" ENERGY VALUE ");
      WriteLower2("              ");
      ShowNumber(31, PB_EnergyValue[Player]*2000);
      PlaySound(51, "0_ca.snd");
      QueueNextSound("0_46.snd");
      PlayFlashSequence((byte*) PB_ScoreEnergySeq);
      PatPointer = PB_EnergyPat;                      // set the pointer to the lamp pattern
      FlowRepeat = 7;                                 // set the repetitions
      LampReturn = PB_EnergyRestoreLamps;             // call this when the lamp pattern has run out
      ShowLampPatterns(1);                            // play the lamp pattern
      ShowMessage(3);}
    PB_HandleEnergy(0);                               // turn off energy lamp and sounds
    break;
  case 46:                                            // visor closed
    if (PB_CloseVisorFlag) {
      PlaySound(50, "0_f3.snd");
      PB_CloseVisorFlag = false;
      ReleaseSolenoid(13);
      PB_EyeBlink(0);}                                // turn off eye blinking
    break;
  case 47:                                            // visor open
    if (PB_OpenVisorFlag) {
      PlaySound(50, "0_f3.snd");
      PB_OpenVisorFlag = false;
      ReleaseSolenoid(13);}
    break;
  case 49:                                            // drop targets
  case 50:
  case 51:
    if (!PB_DropWait) {
      PB_DropWait = true;
      PB_AddBonus(1);
      Points[Player] += 1000;
      ShowPoints(Player);
      ActivateTimer(200, Switch, PB_HandleDropTargets);}
    break;
  case 56:                                            // standup targets
  case 59:
  case 60:
    PlaySound(45, "1_8d.snd");
    Points[Player] += 10;
    break;
  case 65:                                            // lower jet bumper
    ActivateSolenoid(0, 17);
    ActC_BankSol(6);
    if (PB_EnergyValue[Player] < 250) {
      PB_EnergyValue[Player]++;
      WriteUpper2(" ENERGY VALUE ");
      WriteLower2("              ");
      ShowNumber(31, PB_EnergyValue[Player] * 2000);
      ShowMessage(3);}
    break;
  case 67:                                            // left jet bumper
    ActivateSolenoid(0, 19);
    ActC_BankSol(6);
    if (PB_EnergyValue[Player] < 250) {
      PB_EnergyValue[Player]++;
      WriteUpper2(" ENERGY VALUE ");
      WriteLower2("              ");
      ShowNumber(31, PB_EnergyValue[Player] * 2000);
      ShowMessage(3);}
    break;
  case 68:                                            // left slingshot
    PB_MoveExBallLamps(0);
    ActivateSolenoid(0, 20);
    ActC_BankSol(5);
    break;
  case 69:                                            // right slingshot
    PB_MoveExBallLamps(1);
    ActivateSolenoid(0, 21);
    ActC_BankSol(5);
    break;
  case 70:                                            // upper jet bumper
    ActivateSolenoid(0, 22);
    ActC_BankSol(6);
    if (PB_EnergyValue[Player] < 250) {
      PB_EnergyValue[Player]++;
      WriteUpper2(" ENERGY VALUE ");
      WriteLower2("              ");
      ShowNumber(31, PB_EnergyValue[Player] * 2000);
      ShowMessage(3);}
    break;}}

void PB_MoveExBallLamps(byte Direction) {
  if (PB_ExBallsLit[Player]) {
    if (Direction) {
      Direction = QueryLamp(57);
      for (byte c=0; c<3; c++) {
        if (QueryLamp(PB_ExBallLamps[3-c-1])) {
          TurnOnLamp(PB_ExBallLamps[3-c]);}
        else {
          TurnOffLamp(PB_ExBallLamps[3-c]);}}
      if (Direction) {
        TurnOnLamp(49);}
      else {
        TurnOffLamp(49);}}
    else {
      Direction = QueryLamp(49);
      for (byte c=0; c<3; c++) {
        if (QueryLamp(PB_ExBallLamps[c+1])) {
          TurnOnLamp(PB_ExBallLamps[c]);}
        else {
          TurnOffLamp(PB_ExBallLamps[c]);}}
      if (Direction) {
        TurnOnLamp(57);}
      else {
        TurnOffLamp(57);}}}}

void PB_AddExBall() {
  if (PB_ExBallsLit[Player] < 4) {
    PB_ExBallsLit[Player]++;
    byte c = 0;
    while (QueryLamp(PB_ExBallLamps[c])) {
      c++;}
    TurnOnLamp(PB_ExBallLamps[c]);}}

void PB_AfterExBallRelease(byte Event) {
  if (QuerySwitch(20)) {                              // ball still in the shooting lane?
    ActivateTimer(2000, Event, PB_AfterExBallRelease);}  // come back in2s
  else {                                              // ball has been shot
    RemoveBlinkLamp(33);
    if (ExBalls) {                                    // player still has an extra balls
      TurnOnLamp(33);}}}

void PB_GiveExBall() {
  ExBalls++;
  TurnOnLamp(33);}

void PB_AddBonus(byte BonusToAdd) {
  if (Bonus < 99) {
    Bonus = Bonus + BonusToAdd;
    if (Bonus > 99) {
      Bonus = 99;}
    if (Player < 3) {
      WriteLower2("BONUS =       ");
      ShowNumber(31, Bonus*1000);
      DispRow2 = DisplayLower2;}
    else {
      WriteUpper2("BONUS =       ");
      ShowNumber(15, Bonus*1000);
      DispRow1 = DisplayUpper2;}
    ShowMessage(2);}}

void PB_ClearEjectHole(byte Solenoid) {               // activate solenoid after delay time
  PB_EjectIgnore = false;
  ActA_BankSol(Solenoid);}

void PB_StartChestPattern(byte Dummy) {
  UNUSED(Dummy);
  LampPattern = LampColumns;
  PB_ChestLightHandler(100);}

void PB_VisorOpen(byte Dummy) {                       // Play sound and music when visor has opened
  UNUSED(Dummy);
  PB_ChestLightHandler(0);                            // stop chest animation
  PB_ChestPatterns = (byte*)PB_ExpandingSquares;
  PB_StartChestPattern(0);
  RestoreMusicVolumeAfterSound(25);                   // restore music volume after sound has been played
  PlaySound(50, "0_db.snd");}                         // 'I am in your control'

void PB_BlinkGI(byte Number) {
  ActivateSolenoid(150, 12);
  Number--;
  if (Number) {
    ActivateTimer(700, Number, PB_BlinkGI);}}

void PB_OpenVisorProc(byte Dummy) {                   // measures to open the visor
  UNUSED(Dummy);
  PlaySound(50, "0_f1.snd");                          // moving visor sound
  AfterSound = PB_VisorOpen;
  MusicVolume = 4;                                    // reduce music volume
  PlayMusic(50, "1_02.snd");                          // change music
  QueueNextMusic("1_02L.snd");                        // queue looping part as next music to be played
  PB_ChestMode = 0;                                   // indicate that the visor is open
  PB_Chest_Status[Player]++;                          // increase the number of visor openings
  if (PB_Chest_Status[Player] == 2) {                 // visor opened two times?
    PB_AddExBall();}
  PB_LitChestLamps = 0;                               // reset the counter
  PB_ResetPlayersChestLamps(Player);                  // reset the stored lamps
  PB_ClearChest();                                    // turn off chest lamps
  PB_ChestPatterns = (byte*)PB_OpenVisorPat;          // request chest animation
  PB_StartChestPattern(0);                            // start the player
  PlayFlashSequence((byte*) PB_OpenVisorSeq);         // play flasher sequence
  FlowRepeat = 1;                                     // set the repetitions
  PB_CloseVisorFlag = false;
  ActivateSolenoid(4000, 11);                         // turn the backbox GI off
  ActivateTimer(700, 7, PB_BlinkGI);                  // Blink with GI
  ActivateTimer(2000, 0, PB_OpenVisor);
  ActivateTimer(3000, 1, PB_EyeBlink);
  ActivateSolenoid(0, 13);}

void PB_EyeFlash(byte State) {                        // Blink both lock flashers
  static byte Timer = 0;
  if ((State > 1) || ((State == 1) && !Timer)) {
    ActivateSolenoid(80, 10);
    ActivateSolenoid(80, 18);
    Timer = ActivateTimer(135, 2, PB_EyeFlash);}
  else {
    if (!State) {
      if (Timer) {
        KillTimer(Timer);
        Timer = 0;}
      ReleaseSolenoid (10);
      ReleaseSolenoid (18);}}}

void PB_EyeBlink(byte State) {                        // Blink lock flashers alternately
  static byte Timer = 0;
  if ((State > 1) || ((State == 1) && !Timer)) {
    if (State == 2) {
      ReleaseSolenoid (10);
      if (!QuerySwitch(25)) {
        ActivateSolenoid(0, 18);}
      State = 3;}
    else {
      ReleaseSolenoid (18);
      if (!QuerySwitch(26)) {
        ActivateSolenoid(0, 10);}
      State = 2;}
    Timer = ActivateTimer(250, State, PB_EyeBlink);}
  else {
    if (!State) {
      if (Timer) {
        KillTimer(Timer);
        Timer = 0;}
      ReleaseSolenoid (10);
      ReleaseSolenoid (18);}}}

void PB_ChestLightHandler(byte State) {               // handle chest lights timer (0 -> stop, 1 -> show hit animation, 100 -> start pattern mode)
  static byte Timer = 0;
  static uint16_t PatternCounter = 0;                 // counter for the current chest lamp pattern to be shown
  if (State) {
    if (State < 12 ) {                                // is an animation for a row / column hit requested or running?
      if (State < 6) {                                // turn on phase
        TurnOnLamp(PB_ChestRows[AppByte][State-1]);}
      else {                                          // turn off phase
        TurnOffLamp(PB_ChestRows[AppByte][State-6]);}
      State++;
      if (State < 11) {                               // not yet done
        if (State == 1 && Timer) {                    // timer already running
          KillTimer(Timer);}                          // kill old process
        Timer = ActivateTimer(100, State, PB_ChestLightHandler);} // come back with the current state set
      else {
        PB_ChestPatterns = (byte*)PB_RandomChestPat;  // set pattern
        PatternCounter = 0;
        Timer = ActivateTimer(500, 12, PB_ChestLightHandler);}} // come back in pattern mode
    else {                                            // no hit animation running
      if (State == 100 && Timer) {                    // process already running?

        return;}                                      // ignore call
      if (PB_ChestMode && (PB_ChestMode < 11)) {      // visor is closed and can be opened with one row / column hit
        PB_ChestMode++;                               // proceed to next row / column
        if (PB_ChestMode == 11) {                     // last row reached?
          PB_ChestMode = 1;}                          // start with first column
        for (byte i=0; i<5; i++) {                    // turn off previous blinking row / column
          RemoveBlinkLamp(PB_ChestRows[PB_ChestMode-1][i]);}
        for (byte i=0; i<5; i++) {                    // turn on next blinking row / column
          AddBlinkLamp(PB_ChestRows[PB_ChestMode][i], 75);}
        Timer = ActivateTimer(1000, 12, PB_ChestLightHandler);}
      else {                                          // show ChestPatterns
        byte Mask;
        byte Buffer;
        if (!PB_ChestPatterns[6*PatternCounter]) {
          PatternCounter = 0;}
        for (byte x=0; x<5; x++) {                    // for all columns
          Mask = 1;                                   // mask to access the stored lamps for this player
          Buffer = PB_ChestPatterns[6*PatternCounter+x+1]; // buffer the current column
          for (byte i=0; i<5; i++) {                  // for all rows
            if (PB_ChestLamp[Player-1][x] & Mask) {   // if the lamp is stored
              TurnOnLamp(28+8*x+i);}                  // turn it on
            else {                                    // otherwise
              if (Buffer & 1) {
                TurnOnLamp(28+8*x+i);}
              else {
                TurnOffLamp(28+8*x+i);}}              // it is controlled by the pattern
            Mask = Mask<<1;                           // adjust the mask
            Buffer = Buffer>>1;}}                     // and the buffer
        PatternCounter++;
        Timer = ActivateTimer(PB_ChestPatterns[6*(PatternCounter-1)]*10, 12, PB_ChestLightHandler);}}}
  else {
    if (!State) {
      if (PB_ChestMode && (PB_ChestMode < 11)) {
        for (byte i=0; i<5; i++) {                    // turn off blinking row / column
          RemoveBlinkLamp(PB_ChestRows[PB_ChestMode][i]);}}
      if (Timer) {
        KillTimer(Timer);
        Timer = 0;}}}}

void PB_ClearChest() {                                // turn off chest lamps
  byte x = 0;
  byte y = 0;
  for (x=0; x<5; x++) {
    for (y=0; y<5; y++) {
      TurnOffLamp(28+8*x+y);}}}

void PB_SetChestLamps(byte Switch) {                  // add the lamps for the hit row / column in PB_ChestLamp
  byte Pos = 0;
  byte Buffer;
  byte Buffer2 = PB_LampsToLight;
  if (Switch < 5) {                                   // is it a column?
    Buffer = PB_ChestLamp[Player-1][Switch];          // buffer it
    Pos = 16;                                         // start with a mask value of 10000b
    while (Pos && Buffer2) {                          // until all rows are processed or the required number of lamps has been lit
      if (!(Buffer & Pos)) {                          // if the lamp is not lit
        PlaySound(50, "1_85.snd");
        if (Buffer2 == 1) {                           // only once per target hit
          PB_AddBonus(1);}
        Buffer2--;                                    // reduce the number of lamps to be lit
        PB_LitChestLamps++;                           // increase the number of lit chest lamps
        Buffer = Buffer | Pos;}                       // set the corresponding bit in the buffer
      Pos = Pos>>1;}                                  // next row
    PB_ChestLamp[Player-1][Switch] = Buffer;          // copy the buffer to the chest lamp array for this player
    if (Buffer2 == PB_LampsToLight) {                 // Column already full
      PlaySound(50, "0_6f.snd");}}
  else {                                              // it is a row
    Buffer = 1<<(Switch-5);                           // calculate the mask from the row
    Pos = 0;                                          // start on the left of the chest
    while ((Pos < 5) && Buffer2) {                    // until all columns are processed or the required number of lamps has been lit
      if (!(PB_ChestLamp[Player-1][Pos] & Buffer)) {  // if the lamp is not lit
        PlaySound(50, "1_85.snd");
        if (Buffer2 == 1) {                           // only once per taget hit
          PB_AddBonus(1);}
        Buffer2--;                                    // reduce the number of lamps to be lit
        PB_LitChestLamps++;                           // increase the number of lit chest lamps
        PB_ChestLamp[Player-1][Pos] = PB_ChestLamp[Player-1][Pos] | Buffer;} // set the bit for this lamp in the chest lamp array for this player
      Pos++;}
    if (Buffer2 == PB_LampsToLight) {                 // row already full
      PlaySound(50, "0_6f.snd");}}}

void PB_CountLitChestLamps() {                        // count the lit chest lamps for the current player
  byte Buffer;
  PB_LitChestLamps = 0;                               // reset counter
  for (byte x=0; x<5; x++) {                          // for all rows
    Buffer = PB_ChestLamp[Player-1][x];               // buffer the current row
    for (byte i=0; i<5; i++) {                             // for all columns
      if (Buffer & 1) {                               // lamp lit?
        PB_LitChestLamps++;}                          // increase counter
      Buffer = Buffer>>1;}}}                          // shift buffer to the next lamp

void PB_ResetPlayersChestLamps(byte Player) {         // Reset the chest lamps for this player
  for (byte c=0; c<5; c++) {
    PB_ChestLamp[Player-1][c] = 0;}}

void PB_2ndLock(byte State) {
  switch(State) {
  case 0:                                             // subsequent call as AfterSound
    AfterSound = 0;
    RestoreMusicVolume(25);                           // restore music volume after sound has been played
    ReleaseSolenoid(12);                              // turn on playfield GI
    PB_EyeBlink(1);                                   // restart eye blinking
    break;
  case 1:                                             // initial call
    AfterSound = PB_2ndLock;
    MusicVolume = 3;                                  // reduce music volume
    PlayMusic(50, "1_03L.snd");
    QueueNextMusic("1_03L.snd");                      // queue looping part as next music to be played
    PlaySound(51, "0_c9.snd");                        // 'partial link-up'
    break;}}

void PB_HandleLock(byte State) {
  if (!State) {                                       // routine didn't call itself
    PB_IgnoreLock = false;
    InLock++;}
  byte c = 0;
  if (QuerySwitch(25)) {                              // count locked balls
    c++;}
  if (QuerySwitch(26)) {
    c++;}
  if (c != InLock) {                                  // not as expected?
    InLock = c;                                       // take the new count value
    ActivateTimer(200, 1, PB_HandleLock);}            // and come back to recheck
  else {                                              // number of locked balls as expected
    if (InLock) {                                     // locked ball found?
      if (PB_ChestMode) {                             // visor is supposed to be closed
        PB_ClearOutLock(1);}                          // remove balls from lock
      else {
        if (InLock == 1) {
          if (Multiballs > 1) {                       // multiball already running?
            MusicVolume = 4;                          // reduce music volume
            PlayMusic(50, "1_05.snd");
            AddBlinkLamp(35, 100);                    // start blinking of solar energy ramp
            PB_OpenVisorFlag = false;
            PB_EyeBlink(0);                           // turn off eye blinking
            PlaySound(50, "0_ae.snd");                // 'shoot for solar value'
            RestoreMusicVolumeAfterSound(25);         // restore music volume after sound has been played
            ActivateTimer(2000, 0, PB_CloseVisor);    // close visor
            ActivateSolenoid(0, 13);                  // start visor motor
            PB_SolarValueTimer = ActivateTimer(10000, 0,PB_ReopenVisor);} // 8s to score the solar value
          else {                                      // multiball not yet running
            //StopPlayingMusic();
            ActivateSolenoid(0, 12);                  // turn off playfield GI
            PB_EyeBlink(0);                           // stop eye blinking
            PlayFlashSequence((byte*) PB_Ball_Locked);
            PlayMusic(50, "1_80.snd");
            ActivateTimer(1000, 1, PB_2ndLock);
            PB_GiveBall(1);}}                         // give second ball
        else {                                        // both balls in lock
          if (Multiballs == 1) {                      // multiball not yet running?
            StopPlayingMusic();
            PB_EyeBlink(0);                           // turn off eye blinking
            ActivateSolenoid(0, 11);                  // turn off GI
            ActivateSolenoid(0, 12);
            ActivateSolenoid(0, 9);
            ActivateSolenoid(0, 10);
            ActivateSolenoid(0, 18);
            LampPattern = NoLamps;                    // Turn off all lamps
            PB_LampSweepActive = 2;
            PB_LampSweep(4);
            AfterSound = PB_Multiball;
            PB_EyeFlash(1);
            PlaySound(50, "0_b0.snd");                // 'now I see you'
            Multiballs = 2;}                          // start multiball
          else {
            PB_ClearOutLock(1);}}}}}}                 // eject 1 ball and close visor

void PB_Multiball_RestoreLamps(byte Dummy) {
  UNUSED(Dummy);
  LampReturn = 0;
  PB_EyeBlink(1);
  StrobeLights(0);
  LampPattern = LampColumns;}

void PB_Multiball(byte State) {                       // state machine for sound effects during multiball start
  switch(State){
  case 0:                                             // initial call from AfterSound
    PlaySound(50, "1_80.snd");
    AfterSound = 0;
    ActivateTimer(1200, 1, PB_Multiball);
    PB_EyeFlash(0);
    PatPointer = PB_MultiballPat;                     // set the pointer to the lamp pattern
    FlowRepeat = 1;                                   // set the repetitions
    LampReturn = PB_Multiball_RestoreLamps;           // call this when the lamp pattern has run out
    ShowLampPatterns(1);                              // play the lamp pattern
    StrobeLights(3);                                  // and strobe the lights while doing so
    ReleaseSolenoid(9);
    ReleaseSolenoid(10);
    ReleaseSolenoid(18);
    ReleaseSolenoid(12);
    PlayFlashSequence((byte*) PB_MultiballSeq);
    break;
  case 1:
    PlayMusic(50, "1_04.snd");
    QueueNextMusic("1_04L.snd");                      // queue looping part as next music to be played
    PlaySound(51, "1_80.snd");
    ActivateTimer(1000, 2, PB_Multiball);
    break;
  case 2:
    PlaySound(51, "1_80.snd");
    ActivateTimer(2400, 3, PB_Multiball);
    break;
  case 3:
    PlaySound(51, "1_80.snd");
    break;}}

void PB_LampSweep(byte Step) {
  TurnOffLamp(Step);
  if (PB_LampSweepActive) {
    if (PB_LampSweepActive > 1) {
      Step++;
      if (Step > 8) {
        Step = 8;
        PB_LampSweepActive = 1;}}
    else {
      Step--;
      if (Step < 4) {
        Step = 4;
        PB_LampSweepActive = 2;}}
    TurnOnLamp(Step);
    ActivateTimer(100, Step, PB_LampSweep);}}

void PB_ReopenVisor(byte Dummy) {                     // reopen visor if solar value ramp was not hit in time
  UNUSED(Dummy);
  if (Multiballs == 2) {
    PB_EyeBlink(1);}
  PB_SolarValueTimer = 0;
  RemoveBlinkLamp(35);
  PB_ClearOutLock(0);}

void PB_ClearOutLock(byte CloseVisor) {
  if (QuerySolenoid(13)) {                            // visor motor on?
    ActivateTimer(1100, CloseVisor, PB_ClearOutLock);} // come back later
  else {
    if (QuerySwitch(47)) {                            // visor is open
      if (QuerySwitch(25)) {                          // left eye?
        ActA_BankSol(7);}                             // eject it
      else {
        if (QuerySwitch(26)) {                        // right eye
          ActA_BankSol(8);}}                          // eject it
      if (CloseVisor) {                               // closed visor requested?
        ActivateTimer(1000, 13, DelaySolenoid);       // start visor motor in 1s
        ActivateTimer(3000, 00, PB_CloseVisor);}}     // set close flag
    else {
      ActivateTimer(1000, 0, PB_OpenVisor);
      ActivateSolenoid(0, 13);                        // activate visor motor
      ActivateTimer(2000, CloseVisor, PB_ClearOutLock);}}}

void PB_DropTargetReset(byte Counter) {
  PlaySound(51, "0_9b.snd");
  Counter--;
  if (Counter) {
    ActivateTimer(110, Counter, PB_DropTargetReset);}
  else {
    PB_CycleDropLights(1);                            // start the blinking drop target lights
    ActA_BankSol(4);}}                                // reset drop targets

void PB_EnergyReset(byte Counter) {
  PlaySound(51, "0_9b.snd");
  Counter--;
  if (Counter) {
    ActivateTimer(110, Counter, PB_EnergyReset);}
  else {
    PB_EnergyActive = false;                        // energy value off
    PB_DropRamp = true;}}                           // ramp needs to be dropped

void PB_TurnOffLamp(byte Lamp) {
  TurnOffLamp(Lamp);}

void PB_HandleEnergy(byte State) {
  static byte Timer = 0;
  static uint16_t Time;
  if ((State > 1) || ((State == 1) && !Timer)) {
    if (State == 1) {
      Time = 0;}
    uint16_t Length = 550 - (430 * Time / (1000 * game_settings[PB_EnergyTime])); // time to the next blink
    Time = Time + Length;                             // total time
    if (Time < 1000 * game_settings[PB_EnergyTime]) { // total time < selected energy time?
      TurnOnLamp(34);
      PlaySound(49, "0_a2.snd");
      ActivateTimer(Length/2, 34, PB_TurnOffLamp);
      Timer = ActivateTimer(Length, 2, PB_HandleEnergy);}
    else {                                            // energy time has run out
      RestoreMusicVolume(10);
      TurnOffLamp(34);
      Timer = 0;
      PB_EnergyReset(5);}}
  else {
    if (!State) {
      TurnOffLamp(34);
      RestoreMusicVolume(10);
      if (Timer) {
        KillTimer(Timer);}
      Timer = 0;
      PB_EnergyActive = false;                        // energy value off
      PB_DropRamp = true;}}}                          // ramp needs to be dropped

void PB_HandleDropTargets(byte Target) {
  static byte PB_DropTimer;
  static uint16_t Time;
  static bool SoundState;
  if (Target) {
    PB_DropWait = false;                              // stop ignoring drop target switches
    if (QuerySwitch(49) && QuerySwitch(50) && QuerySwitch(51)) {  // all targets down
      if (PB_DropTimer) {                             // any targets down before?
        KillTimer(PB_DropTimer);                      // turn off timer
        PB_DropTimer = 0;
        PB_DropBlinkLamp = 41;
        PB_CycleDropLights(1);                        // start the blinking drop target lights
        TurnOffLamp(17);}                             // turn off timer lamp
      Points[Player] += Multiballs * 25000;
      ActA_BankSol(4);                                // reset drop targets
      PB_AdvancePlanet();}
    else {
      if (!PB_DropTimer) {                            // first target hit
        if (Target-8 == PB_DropBlinkLamp) {           // blinking target hit?
          MusicVolume = 4;                              // reduce music volume
          PlaySound(50, "0_71.snd");
          ActivateTimer(1000, 0, PB_RaiseRamp);       // raise ramp in 1s
          PB_EnergyActive = true;                     // energy value on
          ActivateTimer(500, 1, PB_HandleEnergy);}
        Time = 0;                                     // reset drop target time
        SoundState = true;
        PB_CycleDropLights(0);                        // stop blinking of drop target lights
        PB_DropTimer = ActivateTimer(550, 0, PB_HandleDropTargets);}}}
  else {
    uint16_t Length = 550 - (430 * Time / (1000 * game_settings[PB_DropTime])); // time to the next blink
    Time = Time + Length;                             // total time
    if (Time < 1000 * game_settings[PB_DropTime]) {   // total time < selected drop target time?
      TurnOnLamp(17);
      if (!PB_EnergyActive) {
        if (SoundState) {
          SoundState = false;
          PlaySound(49, "0_a6.snd");}
        else {
          SoundState = true;
          PlaySound(49, "0_a1.snd");}}
      ActivateTimer(Length/2, 17, PB_TurnOffLamp);
      PB_DropTimer = ActivateTimer(Length, 0, PB_HandleDropTargets);}
    else {                                            // drop target time has run out
      TurnOffLamp(17);
      PB_DropBlinkLamp = 41;
      PB_DropTimer = 0;
      PB_DropTargetReset(5);}}}                       // initiate reset

void PB_RaiseRamp(byte Dummy) {
  UNUSED (Dummy);
  if (QuerySwitch(44)) {                              // ramp still down?
    ActA_BankSol(5);                                  // raise ramp
    ActivateTimer(1000, 0, PB_RaiseRamp);}}           // recheck in 1s

void PB_AdvancePlanet() {
  PB_Planet[Player]++;                                // player has reached next planet
  if (PB_Planet[Player] > 10) {                       // sun already reached before?
    PB_Planet[Player] = 10;}                          // set it back to the sun
  else {
    PlaySound(150, "BS_S07.BIN");
    if  (PB_Planet[Player] == 10) {                   //  10 = Sun
      TurnOnLamp(51);}                                // light special
    else {
      if (PB_Planet[Player] == game_settings[PB_ReachPlanet]) { // target planet reached
        TurnOnLamp(51);                               // light special
        RemoveBlinkLamp(18+game_settings[PB_ReachPlanet]);} // stop blinking
      TurnOnLamp(PB_Planet[Player]+18);}}}

//void PB_ResetDropTargets(byte Dummy) {
//  UNUSED(Dummy);
//  RemoveBlinkLamp(17);                                // stop drop target timer lamp
//  PB_DropBlinkLamp = 41;
//  PB_DropTimer = 0;
//  PB_CycleDropLights(1);                              // start the blinking drop target lights
//  ActA_BankSol(4);}                                   // reset drop targets

//void PB_EnergyOff(byte Dummy) {
//  UNUSED(Dummy);
//  RemoveBlinkLamp(34);                                // stop blinking of energy lamp
//  PB_EnergyActive = false;                            // energy value off
//  PB_DropRamp = true;}                                // ramp needs to be dropped

void PB_CycleDropLights(byte State) {                 // State = 0 -> Stop / State = 1 -> Start / State = 2 -> called by timer
  static byte Timer;
  if ((State == 2) || ((State == 1) && !Timer)) {     // State = 1 is a new start and not allowed when timer is already running
    if (PB_DropBlinkLamp) {                           // blink lamp active?
      if (PB_DropBlinkLamp == 43) {                   // last lamp blinking?
        AddBlinkLamp(41, 100);                        // start again with the first one
        RemoveBlinkLamp(43);                          // remove the current one
        PB_DropBlinkLamp = 41;}                       // reset the number of the currently blinking lamp
      else {                                          // not the last one
        AddBlinkLamp(PB_DropBlinkLamp+1, 100);        // start the next one
        RemoveBlinkLamp(PB_DropBlinkLamp);            // remove the current one
        PB_DropBlinkLamp++;}                          // increase number of currently blinking lamp
      Timer = ActivateTimer(3000, 2, PB_CycleDropLights);}}
  else {
    if (!State) {                                     // stop command
      if (Timer) {
        KillTimer(Timer);
        Timer = 0;}
      if (PB_DropBlinkLamp) {                         // blink lamp active?
        RemoveBlinkLamp(PB_DropBlinkLamp);
        PB_DropBlinkLamp = 0;}}}}

void PB_PlayGameMusic() {
  PlayRandomMusic(50, 6, (char *)PB_GameMusic);}

void PB_PlayMultiplierSequence(byte State) {
  static byte Timer = 0;
  if ((State > 1) || ((State == 1) && !Timer)) {
    PlayFlashSequence((byte*) PB_MultiplierSeq);
    Timer = ActivateTimer(2100, 2, PB_PlayMultiplierSequence);}
  else if (!State) {
    if (Timer) {
      KillTimer(Timer);
      Timer = 0;}}}

void PB_BallEnd(byte Event) {                         // ball has been kicked into trunk
  AppByte = PB_CountBallsInTrunk();
  if ((AppByte == 5)||(AppByte < 3-Multiballs-InLock)) {  // something's wrong in the trunk
    InLock = 0;
    if (Multiballs == 1) {
      for (byte i=0; i<2; i++) {                      // check how many balls are on the ball ramp
        if (QuerySwitch(25+i)) {
          InLock++;}}}
    WriteLower(" BALL   ERROR ");
    if (QuerySwitch(16)) {                            // ball still in outhole?
      ActA_BankSol(1);                                // make the coil a bit stronger
      ActivateTimer(2000, Event, PB_BallEnd);}        // and come back in 2s
    else {
      if (Event < 11) {                               // have I been here already?
        Event++;
        ActivateTimer(1000, Event, PB_BallEnd);}      // if not try again in 1s
      else {                                          // ball may be still in outhole
        BlockOuthole = false;
        Event = 0;
        PB_ClearOuthole(0);}}}
  else {                                              // amount of balls in trunk as expected
    PB_EyeBlink(0);
    if (Multiballs == 2) {                            // multiball running?
      Multiballs = 1;                                 // turn it off
      PB_LampSweepActive = 0;                         // turn off backbox lamp sweep
      ReleaseSolenoid(11);                            // turn backbox GI back on
      if (APC_settings[Volume]) {
        analogWrite(VolumePin,255-APC_settings[Volume]);} // reduce volume back to normal
      PlayMusic(50, "1_0a.snd");                      // play non looping part of music track
      QueueNextMusic("1_02L.snd");                    // queue looping part as next music to be played
      if (AppByte == 2) {                             // 2 balls detected in the trunk
        ActivateTimer(1000, 0, PB_BallEnd);}          // come back and check again
      else {
        PB_ClearOutLock(1);
        PB_ChestLightHandler(0);                      // stop chest animation
        PB_ChestMode = 1;
        PB_ClearChest();                              // turn off chest lamps
        PB_ChestLightHandler(100);                    // restart chest animation
        BlockOuthole = false;}}                       // remove outhole block
    else {                                            // no multiball running
      LockedBalls[Player] = 0;
      BlinkScore(0);                                  // stop score blinking
      PB_CycleDropLights(0);                          // stop the blinking drop target lights
      PB_ChestLightHandler(0);                        // stop chest animation
      for (byte i=0; i<5; i++) {                      // turn off blinking row / column
        RemoveBlinkLamp(PB_ChestRows[PB_ChestMode][i]);}
      PB_ClearChest();                                // turn off chest lamps
      if (!PB_ChestMode) {
        PB_Chest_Status[Player] = PB_Chest_Status[Player] + 100;} // indicate that the visor has been open
      RemoveBlinkLamp(18+game_settings[PB_ReachPlanet]);
      if (BallWatchdogTimer) {
        KillTimer(BallWatchdogTimer);
        BallWatchdogTimer = 0;}
      if (PB_EjectMode[Player] > 4) {                 // any blinking eject mode lamps?
        if (PB_EjectMode[Player] == 9) {              // turn them off
          RemoveBlinkLamp(15);}
        else {
          RemoveBlinkLamp(PB_EjectMode[Player] + 8);}}
      for (byte i=0; i<4; i++) {                      // turn off all eject mode lamps
        TurnOffLamp(13+i);}
      if (PB_BallSave == 2) {                         // ball saver has been triggered
        BlockOuthole = false;                         // remove outhole block
        ActivateTimer(2000, 0, PB_AfterExBallRelease);
        ActivateTimer(1000, AppByte, PB_NewBall);}
      else {                                          // no ball saver
        WriteUpper("              ");
        WriteLower("              ");
        WriteUpper2(" BONUS        ");
        ShowNumber(15, Bonus*1000);
        StopPlayingMusic();
        PlaySound(50, "0_2c.snd");
//        IntBuffer = Bonus * BonusMultiplier;
//        DisplayScore(2, BonusMultiplier);
//        DisplayScore(4, IntBuffer * 1000);
        ActivateTimer(200, 0, PB_CountBonus);}}}}

void PB_CountBonus(byte State) {
  static uint32_t TotalBonus;
  const byte Pattern[11] = {5,3,13,14,4,2,12,15,6,10,11};
  if (State < 11) {                                   // show bonus
    *(DisplayUpper+2*Pattern[State]) = *(DisplayUpper2+2*Pattern[State]);
    *(DisplayUpper+2*Pattern[State]+1) = *(DisplayUpper2+2*Pattern[State]+1);
    ActivateTimer(100, State+1, PB_CountBonus);}
  else if (State == 11) {
    StopPlayingSound();
    ActivateTimer(1000, 12, PB_CountBonus);}
  else if (State == 12) {
    WritePlayerDisplay((char*) "  1X   ", 1);
    PlaySound(50, "0_3e.snd");
    TotalBonus = Bonus*1000;
    DisplayScore(2, TotalBonus);
    if (BonusMultiplier > 1) {
      ActivateTimer(300, 13, PB_CountBonus);}
    else {
      ActivateTimer(1000, 20, PB_CountBonus);}}
  else if (State == 13) {
    WritePlayerDisplay((char*) "  2X   ", 1);
    PlaySound(50, "0_3e.snd");
    TotalBonus = Bonus*2000;
    DisplayScore(2, TotalBonus);
    if (BonusMultiplier > 2) {
      ActivateTimer(300, 14, PB_CountBonus);}
    else {
      ActivateTimer(1000, 20, PB_CountBonus);}}
  else if (State == 14) {
    WritePlayerDisplay((char*) "  3X   ", 1);
    PlaySound(50, "0_3e.snd");
    TotalBonus = Bonus*3000;
    DisplayScore(2, TotalBonus);
    if (BonusMultiplier > 3) {
      ActivateTimer(300, 15, PB_CountBonus);}
    else {
      ActivateTimer(1000, 20, PB_CountBonus);}}
  else if (State == 15) {
    WritePlayerDisplay((char*) "  4X   ", 1);
    PlaySound(50, "0_3e.snd");
    TotalBonus = Bonus*4000;
    DisplayScore(2, TotalBonus);
    if (BonusMultiplier > 4) {
      ActivateTimer(300, 16, PB_CountBonus);}
    else {
      ActivateTimer(1000, 20, PB_CountBonus);}}
  else if (State == 16) {
    WritePlayerDisplay((char*) "  5X   ", 1);
    PlaySound(50, "0_49.snd");
    PB_PlayMultiplierSequence(1);
    TotalBonus = Bonus*5000;
    DisplayScore(2, TotalBonus);
    ActivateTimer(1000, 20, PB_CountBonus);}
  else if (State == 20) {
    PlaySound(50, "0_5d.snd");
    DisplayScore(1, Points[Player]);
    ActivateTimer(100, 21, PB_CountBonus);}
  else if (State == 21) {
    TotalBonus = TotalBonus - 1000;
    Points[Player] = Points[Player] + 1000;
    WriteUpper("              ");
    DisplayScore(1, Points[Player]);
    DisplayScore(2, TotalBonus);
    if (TotalBonus) {
      ActivateTimer(29, 21, PB_CountBonus);}
    else {
      PlaySound(50, "0_65.snd");
      PB_PlayMultiplierSequence(0);
      ActivateTimer(1000, 22, PB_CountBonus);}}
  else {
    PB_BallEnd2();}}

void PB_BallEnd2() {
  BlockOuthole = false;                               // remove outhole block
  PlaySound(50, "1_85.snd");
  if (ExBalls) {                                      // Player has extra balls
    AddBlinkLamp(33, 250);                            // Let the extra ball lamp blink
    ExBalls--;
    ActivateTimer(1100, 0, PB_AfterExBallRelease);
    ActivateTimer(100, AppByte, PB_NewBall);}
  else {                                              // Player has no extra balls
    TurnOffLamp(51);
    if ((Points[Player] > HallOfFame.Scores[3]) && (Ball == APC_settings[NofBalls])) { // last ball & high score?
      Switch_Pressed = DummyProcess;                  // Switches do nothing
      PB_Congrats(0);}
    else {
      if ((PB_EjectMode[Player] == 4) || (PB_EjectMode[Player] == 9)) { // eject hole mode maxed out?
        PB_EjectMode[Player] = 0;}                    // reset it for the next ball
      PB_BallEnd3(0);}}}

void PB_BallEnd3(byte Dummy) {
  UNUSED(Dummy);
  LampPattern = LampColumns;
  if (Player < NoPlayers) {                           // last player?
    Player++;
    ActivateTimer(100, AppByte, PB_NewBall);}
  else {
    if (Ball < APC_settings[NofBalls]) {              // last ball?
      Player = 1;                                     // not yet
      Ball++;
      ActivateTimer(100, AppByte, PB_NewBall);}
    else {                                            // game end
      ReleaseSolenoid(23);                            // disable flipper fingers
      ReleaseSolenoid(24);
      LampPattern = NoLamps;                          // Turn off all lamps
      TurnOffLamp(3);                                 // turn off Ball in Play lamp
      GameDefinition.AttractMode();}}}

void PB_Congrats(byte Dummy) {                        // show congratulations
  UNUSED(Dummy);
  LampPattern = NoLamps;
  ActC_BankSol(1);
  AfterMusic = PB_EnterInitials2;
  if (APC_settings[Volume]) {
    analogWrite(VolumePin,255-APC_settings[Volume]-game_settings[PB_MultiballVolume]);} // increase volume
  PlayMusic(50, "BS_M01.BIN");
  ActivateSolenoid(0, 11);
  ActivateSolenoid(0, 12);
  PB_LampSweepActive = 2;
  PB_LampSweep(4);
  WriteUpper("              ");
  WriteLower("              ");
  DisplayScore(3, Points[Player]);
  DisplayScore(4, Points[Player]);
  WriteUpper2(" GREAT        ");
  ActivateTimer(50, 0, ScrollUpper);
  ActivateTimer(1400, 1, PB_ScrollCongrats);
  //ActivateTimer(3000, 1, PB_ScrollCongrats2);
  ActivateTimer(5000, Player, PB_Congrats2);}

void PB_ScrollCongrats(byte Dummy) {
  UNUSED(Dummy);
  WriteUpper2("  SCORE       ");
  AddScrollUpper(0);}

void PB_Congrats2(byte Dummy) {
  UNUSED(Dummy);
  WriteUpper2("ENTER         ");
  ActivateTimer(50, 0, ScrollUpper);
  ActivateTimer(1400, 1, PB_ScrollCongrats2);}

void PB_ScrollCongrats2(byte Dummy) {
  UNUSED(Dummy);
  WriteUpper2(" INITIALS     ");
  AddScrollUpper(0);
  ActivateTimer(3000, 1, PB_ScrollCongrats3);}

void PB_ScrollCongrats3(byte Dummy) {
  UNUSED(Dummy);
  WriteUpper2("PLAYER        ");
  *(DisplayUpper2+14) = DispPattern1[32 + 2 * Player];
  *(DisplayUpper2+15) = DispPattern1[33 + 2 * Player];
  ScrollUpper(0);
  ActivateTimer(2000, 0, PB_EnterInitials);
  ByteBuffer = 0;}

void PB_EnterInitials(byte Switch) {
  switch (Switch) {
  case 0:
    Switch_Pressed = PB_EnterInitials;
    EnterIni[0] = 'A';
    EnterIni[1] = 'A';
    EnterIni[2] = 'A';
    PB_BlinkInitial(0);
    break;
  case 3:                                             // credit button
    KillTimer(ByteBuffer2);
    ByteBuffer++;
    if (ByteBuffer > 2) {
      if (APC_settings[Volume]) {
        ByteBuffer3 = APC_settings[Volume];
        FadeOutMusic(100);}
      else {
        StopPlayingMusic();}}
    else {
      PB_BlinkInitial(1);}
    break;
  case 10:                                            // left flipper button
    if (EnterIni[ByteBuffer] == 65) {
      EnterIni[ByteBuffer] = 57;}
    else if (EnterIni[ByteBuffer] == 48) {
      EnterIni[ByteBuffer] = 32;}
    else if (EnterIni[ByteBuffer] == 32) {
      EnterIni[ByteBuffer] = 90;}
    else {
      EnterIni[ByteBuffer]--;}
    KillTimer(ByteBuffer2);
    PB_BlinkInitial(0);
    break;
  case 11:                                            // right flipper button
    if (EnterIni[ByteBuffer] == 57) {
      EnterIni[ByteBuffer] = 65;}
    else if (EnterIni[ByteBuffer] == 90) {
      EnterIni[ByteBuffer] = 32;}
    else if (EnterIni[ByteBuffer] == 32) {
      EnterIni[ByteBuffer] = 48;}
    else {
      EnterIni[ByteBuffer]++;}
    KillTimer(ByteBuffer2);
    PB_BlinkInitial(0);
    break;}}

void PB_BlinkInitial(byte State) {                    // blink actual character
  if (State) {
    *(DisplayUpper+20+4*ByteBuffer) = 0;              // show a blank
    *(DisplayUpper+21+4*ByteBuffer) = 0;
    State = 0;}
  else {
    for (byte i=0; i<3; i++) {
      *(DisplayUpper+20+4*i) = DispPattern1[(EnterIni[i]-32)*2];
      *(DisplayUpper+21+4*i) = DispPattern1[(EnterIni[i]-32)*2+1];}// restore the characters
    State = 1;}
  ByteBuffer2 = ActivateTimer(100+State*2000, State, PB_BlinkInitial);}  // and come back

void PB_EnterInitials2(byte Dummy) {
  UNUSED(Dummy);
  if (APC_settings[Volume]) {
    analogWrite(VolumePin, 255);}
  if (ByteBuffer > 2) {
    ByteBuffer2 = HandleHighScores(Points[Player]);
    WriteUpper2(" HIGH   SCORE ");
    ScrollUpper(0);
    ActivateTimer(2000, 1, PB_EnterInitials3);}
  else {
    PB_BallEnd3(1);}}

void PB_EnterInitials3(byte Dummy) {
  UNUSED(Dummy);
  WriteUpper2("TABLE  POS    ");
  *(DisplayUpper2+28) = DispPattern1[32 + 2 * (ByteBuffer2+1)];
  *(DisplayUpper2+29) = DispPattern1[33 + 2 * (ByteBuffer2+1)];
  ScrollUpper(0);
  ReleaseSolenoid(11);
  ReleaseSolenoid(12);
  PB_LampSweepActive = 0;
  ActivateTimer(2000, 1, PB_BallEnd3);}

void PB_ResetHighScores(bool change) {                // delete the high scores file
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

void PB_HandleVolumeSetting(bool change) {
  if (APC_settings[Volume]) {
    HandleNumSetting(change);
    if (!change) {
      PlayMusic(50, "BS_M04.BIN");}
    analogWrite(VolumePin,255-APC_settings[Volume]-SettingsPointer[AppByte]);}}  // adjust PWM to volume setting

void PB_Testmode(byte Select) {
  Switch_Pressed = PB_Testmode;
  switch(AppByte) {                                   // which testmode?
  case 0:                                                   // display test
    switch(Select) {                                  // switch events
    case 0:                                           // init (not triggered by switch)
      *(DisplayLower) = 0;                            // clear credit display
      *(DisplayLower+16) = 0;
      *(DisplayUpper) = 0;
      *(DisplayUpper+16) = 0;
      WriteUpper("DISPLAY TEST  ");
      WriteLower("              ");
      AppByte2 = 0;
      break;
    case 3:                                           // credit button
      WriteUpper("0000000000000000");
      WriteLower("0000000000000000");
      AppByte2 = ActivateTimer(1000, 32, PB_DisplayCycle);
      break;
    case 72:                                          // advance button
      if (AppByte2) {
        KillTimer(AppByte2);
        AppByte2 = 0;}
      else {
        AppByte++;}
      PB_Testmode(0);}
    break;
    case 1:                                                 // switch edges test
      switch(Select) {                                // switch events
      case 0:                                         // init (not triggered by switch)
        AppByte2 = 0;
        WriteUpper(" SWITCHEDGES  ");
        WriteLower("              ");
        break;
      case 72:                                        // advance button
        if (AppByte2) {
          AppByte2 = 0;}
        else {
          AppByte++;}
        PB_Testmode(0);
        break;
      case 3:                                         // credit button
        if (!AppByte2) {
          WriteUpper(" LATESTEDGES  ");
          AppByte2 = 1;
          break;} // @suppress("No break at end of case")
      default:                                        // all other switches
        for (byte i=1; i<24; i++) {                        // move all characters in the lower display row 4 chars to the left
          DisplayLower[i] = DisplayLower[i+8];}
        *(DisplayLower+30) = DispPattern2[32 + 2 * (Select % 10)]; // and insert the switch number to the right of the row
        *(DisplayLower+31) = DispPattern2[33 + 2 * (Select % 10)];
        *(DisplayLower+28) = DispPattern2[32 + 2 * (Select - (Select % 10)) / 10];
        *(DisplayLower+29) = DispPattern2[33 + 2 * (Select - (Select % 10)) / 10];}
      break;
      case 2:                                               // solenoid test
        switch(Select) {                              // switch events
        case 0:                                       // init (not triggered by switch)
          WriteUpper("  COIL  TEST  ");
          WriteLower("              ");
          AppByte2 = 0;
          break;
        case 3:
          WriteUpper(" FIRINGCOIL NO");
          AppBool = false;
          AppByte2 = ActivateTimer(1000, 1, PB_FireSolenoids);
          break;
        case 72:
          if (AppByte2) {
            KillTimer(AppByte2);
            AppByte2 = 0;}
          else {
            AppByte++;}
          PB_Testmode(0);}
        break;
        case 3:                                           // single lamp test
          switch(Select) {                            // switch events
          case 0:                                     // init (not triggered by switch)
            WriteUpper(" SINGLE LAMP  ");
            WriteLower("              ");
            AppByte2 = 0;
            for (byte i=0; i<(LampMax+1); i++){            // erase lamp matrix
              TurnOffLamp(i);}
            LampPattern = LampColumns;                // and show it
            break;
          case 3:
            WriteUpper(" ACTUAL LAMP  ");
            AppByte2 = ActivateTimer(1000, 1, PB_ShowLamp);
            break;
          case 72:
            LampPattern = NoLamps;
            if (AppByte2) {
              KillTimer(AppByte2);
              AppByte2 = 0;}
            else {
              AppByte++;}
            PB_Testmode(0);}
          break;
          case 4:                                           // all lamps test
            switch(Select) {                          // switch events
            case 0:                                   // init (not triggered by switch)
              WriteUpper("  ALL   LAMPS ");
              WriteLower("              ");
              AppByte2 = 0;
              break;
            case 3:
              WriteUpper("FLASHNG LAMPS ");
              AppByte2 = ActivateTimer(1000, 1, PB_ShowAllLamps);
              break;
            case 72:
              LampPattern = NoLamps;
              if (AppByte2) {
                KillTimer(AppByte2);
                AppByte2 = 0;}
              else {
                AppByte++;}
              PB_Testmode(0);}
            break;
            case 5:                                         // all music test
              switch(Select) {                        // switch events
              case 0:                                 // init (not triggered by switch)
                WriteUpper(" MUSIC  TEST  ");
                WriteLower("              ");
                AppByte2 = 0;
                break;
              case 3:
                WriteUpper("PLAYING MUSIC ");
                AfterMusic = PB_NextTestSound;
                AppByte2 = 1;
                PlayMusic(50, (char*) TestSounds[0]);
                break;
              case 72:
                AfterMusic = 0;
                StopPlayingMusic();
                if (AppByte2) {
                  AppByte2 = 0;}
                else {
                  AppByte++;}
                PB_Testmode(0);}
              break;
              case 6:                                       // visor test
                switch(Select) {                      // switch events
                case 0:                               // init (not triggered by switch)
                  WriteUpper(" VISOR  TEST  ");
                  WriteLower("              ");
                  AppByte2 = 0;
                  AppBool = false;
                  break;
                case 3:
                  AppByte2 = 1;
                  if (AppBool) {
                    if (QuerySwitch(46)) {
                      AppBool = false;
                      PB_Testmode(3);}
                    else {
                      WriteUpper("CLOSING VISOR ");
                      ActivateSolenoid(0, 13);}}
                  else {
                    if (QuerySwitch(47)) {
                      AppBool = true;
                      PB_Testmode(3);}
                    else {
                      WriteUpper("OPENING VISOR ");
                      ActivateSolenoid(0, 13);}}
                  break;
                case 46:
                  if (AppBool) {
                    ReleaseSolenoid(13);
                    AppBool = false;
                    WriteUpper(" VISOR CLOSED ");}
                  break;
                case 47:
                  if (!AppBool) {
                    ReleaseSolenoid(13);
                    AppBool = true;
                    WriteUpper(" VISOR  OPEN  ");}
                  break;
                case 72:
                  ReleaseSolenoid(13);
                  if (AppByte2) {
                    AppByte2 = 0;}
                  else {
                    // AppByte++;}
                    GameDefinition.AttractMode();
                    return;}
                  PB_Testmode(0);
                  break;
                }}}

void PB_ShowLamp(byte CurrentLamp) {                  // cycle all solenoids
  if (QuerySwitch(73)) {                              // Up/Down switch pressed?
    *(DisplayLower+30) = DispPattern2[32 + 2 * (CurrentLamp % 10)]; // and show the actual solenoid number
    *(DisplayLower+31) = DispPattern2[33 + 2 * (CurrentLamp % 10)];
    *(DisplayLower+28) = DispPattern2[32 + 2 * (CurrentLamp - (CurrentLamp % 10)) / 10];
    *(DisplayLower+29) = DispPattern2[33 + 2 * (CurrentLamp - (CurrentLamp % 10)) / 10];
    TurnOnLamp(CurrentLamp);                          // turn on lamp
    if (CurrentLamp > 1) {                            // and turn off the previous one
      TurnOffLamp(CurrentLamp-1);}
    else {
      TurnOffLamp(LampMax);}
    CurrentLamp++;                                    // increase the lamp counter
    if (CurrentLamp == LampMax+1) {                   // maximum reached?
      CurrentLamp = 1;}}                              // then start again
  AppByte2 = ActivateTimer(1000, CurrentLamp, PB_ShowLamp);} // come back in one second

void PB_ShowAllLamps(byte State) {                    // Flash all lamps
  if (State) {                                        // if all lamps are on
    LampPattern = NoLamps;                            // turn them off
    State = 0;}
  else {                                              // or the other way around
    LampPattern = AllLamps;
    State = 1;}
  AppByte2 = ActivateTimer(500, State, PB_ShowAllLamps);}  // come back in 500ms

void PB_FireSolenoids(byte Solenoid) {                // cycle all solenoids
  if (AppBool) {                                      // if C bank solenoid
    ActC_BankSol(Solenoid);
    *(DisplayLower+30) = DispPattern2[('C'-32)*2];    // show the C
    *(DisplayLower+31) = DispPattern2[('C'-32)*2+1];
    if (QuerySwitch(73)) {                            // Up/Down switch pressed?
      AppBool = false;
      Solenoid++;}}
  else {                                              // if A bank solenoid
    *(DisplayLower+28) = DispPattern2[32 + 2 * (Solenoid % 10)]; // show the actual solenoid number
    *(DisplayLower+29) = DispPattern2[33 + 2 * (Solenoid % 10)];
    *(DisplayLower+26) = DispPattern2[32 + 2 * (Solenoid - (Solenoid % 10)) / 10];
    *(DisplayLower+27) = DispPattern2[33 + 2 * (Solenoid - (Solenoid % 10)) / 10];

    if (Solenoid == 11 || Solenoid == 12 || Solenoid == 13 || Solenoid == 14 || Solenoid == 9 || Solenoid == 10 || Solenoid == 18) {  // is it a relay or a #1251 flasher?
      ActivateSolenoid(999, Solenoid);}               // then the duration must be specified
    else {
      ActivateSolenoid(0, Solenoid);}                 // activate the solenoid
    if (Solenoid < 9) {
      *(DisplayLower+30) = DispPattern2[('A'-32)*2];  // show the A
      *(DisplayLower+31) = DispPattern2[('A'-32)*2+1];
      if (QuerySwitch(73)) {                          // Up/Down switch pressed?
        AppBool = true;}}
    else {
      *(DisplayLower+30) = DispPattern2[(' '-32)*2];  // delete the C
      *(DisplayLower+31) = DispPattern2[(' '-32)*2+1];
      if (QuerySwitch(73)) {                          // Up/Down switch pressed?
        Solenoid++;                                   // increase the solenoid counter
        if (Solenoid > 22) {                          // maximum reached?
          Solenoid = 1;}}}}                           // then start again
  AppByte2 = ActivateTimer(1000, Solenoid, PB_FireSolenoids);}   // come back in one second

void PB_DisplayCycle(byte CharNo) {                   // Display cycle test
  if (QuerySwitch(73)) {                              // cycle only if Up/Down switch is not pressed
    if (CharNo == 116) {                              // if the last character is reached
      CharNo = 32;}                                   // start from the beginning
    else {
      if (CharNo == 50) {                             // reached the gap between numbers and characters?
        CharNo = 66;}
      else {
        CharNo = CharNo+2;}}                          // otherwise show next character
    for (byte i=0; i<16; i++) {                            // use for all alpha digits
      if ((i==0) || (i==8)) {
        DisplayUpper[2*i] = LeftCredit[CharNo];
        DisplayUpper[2*i+1] = LeftCredit[CharNo+1];
        DisplayLower[2*i] = RightCredit[CharNo];
        DisplayLower[2*i+1] = RightCredit[CharNo+1];}
      else {
        DisplayUpper[2*i] = DispPattern1[CharNo];
        DisplayUpper[2*i+1] = DispPattern1[CharNo+1];
        DisplayLower[2*i] = DispPattern2[CharNo];
        DisplayLower[2*i+1] = DispPattern2[CharNo+1];}}}
  AppByte2 = ActivateTimer(500, CharNo, PB_DisplayCycle);}   // restart timer

void PB_NextTestSound(byte Dummy) {
  UNUSED(Dummy);
  if (QuerySwitch(73)) {                              // Up/Down switch pressed?
    AppByte++;}
  if (!TestSounds[AppByte][0]) {
    AppByte = 0;}
  *(DisplayLower+30) = DispPattern2[32 + 2 * ((AppByte+1) % 10)]; // show the actual solenoid number
  *(DisplayLower+31) = DispPattern2[33 + 2 * ((AppByte+1) % 10)];
  *(DisplayLower+28) = DispPattern2[32 + 2 * ((AppByte+1) - ((AppByte+1) % 10)) / 10];
  *(DisplayLower+29) = DispPattern2[33 + 2 * ((AppByte+1) - ((AppByte+1) % 10)) / 10];
  PlayMusic(50, (char*) TestSounds[AppByte]);}
