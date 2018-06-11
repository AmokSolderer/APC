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
byte LockLightsTimer = 0;                             // number of the timer for the lock chase light
byte CurrentLockLight = 0;                            // number of the lock light being illuminated at the moment
byte TimedRightMagnaTimer = 0;												// number of the timer for the right timed magna save
byte TimedLeftMagnaTimer = 0;													// number of the timer for the left timed magna save
byte BonusToAdd = 0;																	// bonus points to be added by AddBonus()
byte BonusCountTime = 0;															// counter to reduce the waiting time between the bonus counts
const byte BonusLamp = 48;														// first lamp of the bonus ladder
bool LowerExBall[5];                                  // extra ball in turnaround pending?
bool UpperExBall[5];                                  // extra ball in left ramp pending?
bool PlayersExBalls[5];                               // did the player already get an extra ball?
byte DropTimer[4];                                    // timer for all drop target banks
byte DropHits[16];                                    // counts how often the target banks have been cleared
bool DropWait[5];																			// indicates that a waiting time for this drop target bank is active before it it being processed
byte BallSearchCoils[11] = {1,8,10,9,2,3,4,5,7,19,15}; // coils to fire when the ball watchdog timer runs out
const unsigned int BK_SolTimes[24] = {30,50,50,50,50,10,50,50,1999,1999,5,5,5,5,999,999,50,50,50,5,5,5,0,0}; // Activation times for solenoids
const byte BK_defaults[64] = {0,0,0,0,0,0,0,0,		 		// game default settings
											  			0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0};

const byte TimedMagna = 0;                            // Timed Magna saves?

struct SettingTopic BK_setList[5] = {{" TIMED  MAGNA ",HandleBoolSetting,0,0,0},
			{" RESET  HIGH  ",ResetHighScores,0,0,0},
      {"RESTOREDEFAULT",RestoreDefaults,0,0,0},
      {"  EXIT SETTNGS",ExitSettings,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

                              	 // Duration080910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364
const struct LampPat AttractPat1[52] = {{30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														           	{30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1},
														            {30,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1},
														            {30,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1},
														            {30,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1},
														            {30,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1},
														            {30,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1},
														            {30,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1},
														            {30,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1},
														            {30,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1},
														            {30,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
														            {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0},
														            {30,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
														            {30,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
														            {30,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0},
														            {30,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
														            {30,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
														            {30,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
														            {30,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
														            {30,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
														            {30,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														            {30,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
														             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

const struct LampPat AttractPat2[4] = {{250,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0},
																		 	 {250,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,1,0},
																			 {250,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1},
																			   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

const struct LampPat AttractPat3[6] = {{90,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0},
														           {90,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0},
														           {90,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
														           {90,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,1},
														           {90,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,1,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,0},
																				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

const struct LampPat AttractPat4[52] = {{30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
                                        {30,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
                                        {30,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
                                        {30,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
                                        {30,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
                                        {30,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0},
                                        {30,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
                                        {30,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                        {30,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1},
                                        {30,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1},
                                        {30,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1},
                                        {30,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1},
                                        {30,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1},
                                        {30,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1},
                                        {30,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1},
                                        {30,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
                                 // Duration080910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364
const struct LampPat AttractPat5[17] = {{50,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
																				{50,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1},
																				{50,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
																				{50,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
																				{50,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
																				{50,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
																				{50,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
																				{50,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
																				{50,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
																				{50,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
																				{50,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
																				{50,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {50,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {50,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                        {50,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
                                        {50,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
																				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

const struct LampFlow AttractFlow[6] = {{3,AttractPat1},{10,AttractPat2},{3,AttractPat4},{10,AttractPat3},{10,AttractPat5},{0,0}};
const byte BonusLamps[13] = {60,59,58,57,56,55,54,53,52,51,50,49,48}; // numbers of the bonus lamps
const byte BonusValues[13] = {40,30,20,10,9,8,7,6,5,4,3,2,1}; // values of the bonus lamps
const byte FirstMultLamp = 61;
const byte DropTargets[4] = {25, 29, 33, 37};
const byte DropSolenoid = 2;
const byte DropLamp = 17;

const struct GameDef BK_GameDefinition = {
	BK_setList,																					// GameSettingsList
	(byte*)BK_defaults,																	// GameDefaultsPointer
	"BK_SET.BIN",																				// GameSettingsFileName
	"BKSCORES.BIN",																			// HighScoresFileName
	BK_AttractMode,																			// AttractMode
	BK_SolTimes};																				// Default activation times of solenoids

void BK_init() {
	GameDefinition = BK_GameDefinition;}								// read the game specific settings and highscores

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
  if (Switch[20]) {                                   // for the outhole
    ActivateSolenoid(0, 1);}
  if (Switch[24]) {                                   // for the lower eject hole
    ActivateSolenoid(0, 8);}
  if (Switch[41]) {                                   // for the multiball lock
    ActivateSolenoid(0, 7);}}

void AttractLampCycle(byte Event) {                   // play multiple lamp pattern series
  PatPointer = AttractFlow[AppByte2].FlowPat;         // set the pointer to the current series
  FlowRepeat = AttractFlow[AppByte2].Repeat;          // set the repetitions
  if (AppByte2 == 2) {
    StrobeLightsTimer = ActivateTimer(30, 0, StrobeLights);}
  else {
    if (StrobeLightsTimer) {
      KillTimer(StrobeLightsTimer);
      StrobeLightsTimer = 0;}}
  AppByte2++;                                         // increase counter
  if (!AttractFlow[AppByte2].Repeat) {                // repetitions of next series = 0?
    AppByte2 = 0;}                                    // reset counter
  ShowLampPatterns(0);}                               // call the player

void AttractScroll(byte Dummy) {
  WriteUpper2("SPEZIAL       ");
  AddScrollUpper(1);}

void AttractDisplayCycle(byte Event) {
  CheckForLockedBalls(0);
  switch (Event) {
    case 0:
      WriteUpper2(" FRANKS       ");
      ActivateTimer(50, 1, ScrollUpper);
      ActivateTimer(900, 1, AttractScroll);
      WriteLower2(" BLACK KNIGHT ");
      ActivateTimer(1400, 1, ScrollLower);
      if (NoPlayers) {
        Event++;}
      else {
        Event = 2;}
      break;
    case 1:
    	WriteUpper2("              ");                   // erase display
    	WriteLower2("              ");
    	  for (i=1; i<=NoPlayers; i++) {                  // display the points of all active players
    	  	ShowNumber(8*i-1, Points[i]);}
    	ActivateTimer(50, 1, ScrollUpper);
    	ActivateTimer(900, 1, ScrollLower);
      Event++;
      break;
    case 2:																						// Show highscores
      AddBlinkLamp(6, 150);                           // blink Highest Score lamp
      WriteUpper2("1>            ");
      WriteLower2("2>            ");
      for (i=0; i<3; i++) {
        *(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[i]-32)*2];
        *(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[i]-32)*2+1];
        *(DisplayLower2+8+2*i) = DispPattern2[(HallOfFame.Initials[3+i]-32)*2];
        *(DisplayLower2+8+2*i+1) = DispPattern2[(HallOfFame.Initials[3+i]-32)*2+1];}
      ShowNumber(15, HallOfFame.Scores[0]);
      ShowNumber(31, HallOfFame.Scores[1]);
      ActivateTimer(50, 1, ScrollUpper);
      ActivateTimer(900, 1, ScrollLower);
      Event++;
      break;
    case 3:
      RemoveBlinkLamp(6);                             // stop blinking of Highest Score lamp
      WriteUpper2("3>            ");
      WriteLower2("4>            ");
      for (i=0; i<3; i++) {
        *(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2];
        *(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2+1];
        *(DisplayLower2+8+2*i) = DispPattern2[(HallOfFame.Initials[9+i]-32)*2];
        *(DisplayLower2+8+2*i+1) = DispPattern2[(HallOfFame.Initials[9+i]-32)*2+1];}
      ShowNumber(15, HallOfFame.Scores[2]);
      ShowNumber(31, HallOfFame.Scores[3]);
      ActivateTimer(50, 1, ScrollUpper);
      ActivateTimer(900, 1, ScrollLower);
      Event = 0;}
    ActivateTimer(4000, Event, AttractDisplayCycle);}

void AttractModeSW(byte Event) {                      // Attract Mode switch behaviour
  switch (Event) {
  case 20:                                            // outhole
    ActivateTimer(200, 0, CheckForLockedBalls);       // check again in 200ms
    break;
  case 72:                                            // Service Mode
    StrobeLightsTimer = 0;
    RightMysteryTimer = 0;
    LeftMysteryTimer = 0;
    LockLightsTimer = 0;
    BlinkScoreTimer = 0;
    BallWatchdogTimer = 0;
    TimedRightMagnaTimer = 0;
    TimedLeftMagnaTimer = 0;
    CheckReleaseTimer = 0;
    RemoveBlinkLamp(4);                               // stop blinking of Game Over lamp
    RemoveBlinkLamp(6);                               // stop blinking of Highest Score lamp
    TestMode_Enter();
    break;
  case 3:																							// start game
  	if (CountBallsInTrunk() == 3 || (CountBallsInTrunk() == 2 && Switch[45])) { // Ball missing?
			StrobeLightsTimer = 0;
			RemoveBlinkLamp(4);
			RemoveBlinkLamp(6);
			Switch_Pressed = DummyProcess;                  // Switches do nothing
			KillAllTimers();                                // stop the lamp cycling
			for (i=0; i< LampMax+1; i++) {
				Lamp[i] = false;}
			LampPattern = Lamp;
			Lamp[2] = true;                                 // turn on Ball in Play lamp
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
	for (Dummy=0; Dummy <4; Dummy++) {									// Dummy counts through the drop target banks
		for (i=0; i<3;i++) {															// for each target
			if (Switch[DropTargets[Dummy]+i]) {							// target down?
				ActivateSolenoid(0, DropSolenoid+Dummy);			// reset
				break;}}}}																		// quit for next bank

void AddPlayer() {
  if ((NoPlayers < 4) && (Ball == 1)) {               // if actual number of players < 4
    NoPlayers++;                                      // add a player
    Points[NoPlayers] = 0;                            // delete the points of the new player
    ShowPoints(NoPlayers);}}                          // and show them

void NewBall(byte Balls) {                            // release ball (Event = expected balls on ramp)
  ShowAllPoints(0);
  ShowBonus();
  *(DisplayUpper+16) = LeftCredit[32 + 2 * Ball]; 	// show current ball in left credit
  //*(DisplayUpper+17) = LeftCredit[33 + 2 * Ball];
  if (!LockLightsTimer) {
    LockLightsTimer = ActivateTimer(1000, 40, LockChaseLight);}
  if (!BlinkScoreTimer) {
    BlinkScoreTimer = ActivateTimer(1000, 1, BlinkScore);}
  Lamp[36] = true;                                  	// bumper light on
  for (i=0; i<4; i++) {                             	// restore the drop target lamps of the current player
    for (AppByte=0; AppByte < 3; AppByte++) {       	// for all drop target banks
      if (DropHits[(4*(Player-1))+i] > AppByte) {
        Lamp[DropTargets[i]+AppByte] = true;}
      else {
        Lamp[DropTargets[i]+AppByte] = false;}}}
  if (LowerExBall[Player]) {                        	// restore extra ball lamps
    Lamp[23] = true;}
  else {
    Lamp[23] = false;}
  if (UpperExBall[Player]) {
    Lamp[41] = true;}
  else {
    Lamp[41] = false;}
	if (game_settings[TimedMagna]) {              			// if timed magna save mode
		if (LeftMagna[Player]) {
			if (LeftMagna[Player] == 5) {										// 5 seconds reached?
				RemoveBlinkLamp(10);													// turn off blinking lamp
				Lamp[10] = true;}															// and switch it on permanently
			else {
				AddBlinkLamp(10, 150*LeftMagna[Player]);}}
		else {
			RemoveBlinkLamp(10);														// turn off blinking lamp
			Lamp[10] = false;}															// and switch it off permanently
		if (RightMagna[Player]) {
			if (RightMagna[Player] == 5) {									// 5 seconds reached?
				RemoveBlinkLamp(9);														// turn off blinking lamp
				Lamp[9] = true;}															// and switch it on permanently
			else {
				AddBlinkLamp(9, 150*RightMagna[Player]);}}
		else {
			RemoveBlinkLamp(9);															// turn off blinking lamp
			Lamp[9] = false;}}															// and switch it off permanently
	else {
		if (LeftMagna[Player]) {
			Lamp[10] = true;}
		else {
			Lamp[10] = false;}
		if (RightMagna[Player]) {
			Lamp[9] = true;}
		else {
			Lamp[9] = false;}}
  Switch_Released = CheckShooterLaneSwitch;
  if (!Switch[45]) {
    ActivateSolenoid(0, 6);                          	// release ball
    Switch_Pressed = BallReleaseCheck;                          // set switch check to enter game
    CheckReleaseTimer = ActivateTimer(5000, Balls-1, CheckReleasedBall);} // start release watchdog
  else {
    Switch_Pressed = ResetBallWatchdog;}}

void CheckShooterLaneSwitch(byte Switch) {
  if (Switch == 45) {                                 // shooter lane switch released?
    Switch_Released = DummyProcess;
    if (!BallWatchdogTimer) {
      BallWatchdogTimer = ActivateTimer(30000, 0, SearchBall);}}}

void BallReleaseCheck(byte Switch) {                  // handle switches during ball release
	if ((Switch > 10)&&(Switch != 17)&&(Switch != 18)&&(Switch != 19)&&(Switch != 46)) { // playfield switch activated?
		if (CheckReleaseTimer) {
			KillTimer(CheckReleaseTimer);
			CheckReleaseTimer = 0;}                   			// stop watchdog
		Switch_Pressed = ResetBallWatchdog;
		if (Switch == 45) {																// ball is in the shooter lane
			Switch_Released = CheckShooterLaneSwitch;}				// set mode to register when ball is shot
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

void SearchBall(byte Counter) {												// ball watchdog timer has run out
  BallWatchdogTimer = 0;
  if (Switch[20]) {
    BlockOuthole = false;
    ActivateTimer(1000, 0, ClearOuthole);}
  else {
    if (Switch[45]) {																	// if ball is waiting to be launched
      BallWatchdogTimer = ActivateTimer(30000, 0, SearchBall);}	// restart watchdog
    else {  																					// if ball is really missing
      AppByte = CountBallsInTrunk();									// recount all balls
      if (AppByte == 3) {															// found 3 balls in trunk?
      	if (BlockOuthole) {														// is the outhole blocked
      		BallEnd(0);}																// then it was probably a ball loss gone wrong
      	else {
      		ActivateTimer(1000, 3, NewBall);}}					// otherwise try it with a new ball
      else {
      	AppByte2 = 0;
      	for (i=0; i<3; i++) {                         // count balls in lock
      		if (Switch[41+i]) {
      			if (AppByte2 < i) {
      				AppByte = 5;}                           // set warning flag
      			AppByte2++;}}
      	if (AppByte == 5) {														// balls have not settled yet
      		WriteUpper("  BALL  STUCK ");
      		BallWatchdogTimer = ActivateTimer(1000, 0, SearchBall);} // and try again in 1s
      	else {
      		if (AppByte2 > InLock) {										// more locked balls found than expected?
      			HandleLock(0);														// lock them
      			BallWatchdogTimer = ActivateTimer(30000, 0, SearchBall);}
      		else {
      			WriteUpper("  BALL  SEARCH");
      			ActivateSolenoid(0, BallSearchCoils[Counter]); // fire coil to get ball free
      			Counter++;
      			if (Counter == 10) {											// all coils fired?
      				Counter = 0;}														// start again
      			BallWatchdogTimer = ActivateTimer(1000, Counter, SearchBall);}}}}}} // come again in 1s if no switch is activated

byte CountBallsInTrunk() {
  byte Balls = 0;
  for (i=0; i<3; i++) {                               // check how many balls are on the ball ramp
    if (Switch[17+i]) {
      if (Balls < i) {
        return 5;}                                    // send warning
      Balls++;}}
  return Balls;}

void CheckReleasedBall(byte Balls) {                  // ball release watchdog
  if (BlinkScoreTimer) {															// stop blinking to show messages
    KillTimer(BlinkScoreTimer);
    BlinkScoreTimer = 0;}
  WriteUpper("WAITINGFORBALL");                       // indicate a problem
  WriteLower("              ");
  if (Balls == 10) {																	// indicating a previous trunk error
    WriteUpper("              ");
    WriteLower("              ");
    ShowAllPoints(0);
    BlinkScoreTimer = ActivateTimer(1000, 1, BlinkScore);
    ActivateSolenoid(0, 6);}
  AppByte = CountBallsInTrunk();
  if (AppByte == Balls) {                             // expected number of balls in trunk
    WriteUpper("  BALL MISSING");
    if (Switch[20]) {                                 // outhole switch still active?
      ActivateSolenoid(0, 1);}}												// shove the ball into the trunk
  else {																							//
    if (AppByte == 5) {																// balls not settled
      WriteLower(" TRUNK  ERROR ");
      Balls = 10;}
    else {
      if (AppByte > Balls) {													// more balls in trunk than expected
        WriteUpper("              ");
        WriteLower("              ");
        ShowAllPoints(0);
        BlinkScoreTimer = ActivateTimer(1000, 1, BlinkScore);
        ActivateSolenoid(0, 6);}}}                    // release again
  CheckReleaseTimer = ActivateTimer(5000, Balls, CheckReleasedBall);}

void TimedRightMagna(byte dummy) {										// runs every second as long as button is pressed
	if (Switch[9] && RightMagna[Player]) {							// button still pressed and magna seconds left?
		RightMagna[Player]--;															// reduce magna save seconds
		AddBlinkLamp(9, 150*RightMagna[Player]);					// adjust blinking rhythm of lamp
		TimedRightMagnaTimer = ActivateTimer(1000, 0, TimedRightMagna);}// and come back in 1s
	else {																							// magna save not to be continued
		ReleaseSolenoid(9);																// turn off magnet
		if (!TimedLeftMagnaTimer) {
			Switch_Released = DummyProcess;}
		EndRightMagna(0);
		TimedRightMagnaTimer = 0;}}

void TimedLeftMagna(byte dummy) {											// runs every second as long as button is pressed
	if (Switch[10] && LeftMagna[Player]) {							// button still pressed and magna seconds left?
		LeftMagna[Player]--;															// reduce magna save seconds
		AddBlinkLamp(10, 150*LeftMagna[Player]);					// adjust blinking rhythm of lamp
		TimedLeftMagnaTimer = ActivateTimer(1000, 0, TimedLeftMagna);}// and come back in 1s
	else {																							// magna save not to be continued
		ReleaseSolenoid(10);															// turn off magnet
		if (!TimedRightMagnaTimer) {
			Switch_Released = DummyProcess;}
		EndLeftMagna(0);
		TimedLeftMagnaTimer = 0;}}

void TimedMagnaSW(byte Switch) {											// magna save button released
	if (Switch==9 && TimedRightMagnaTimer) {						// was it the right switch and is the magna save active?
		KillTimer(TimedRightMagnaTimer);
		TimedRightMagnaTimer = 0;
		ReleaseSolenoid(9);																// turn off magnet
		EndRightMagna(0);}
	if (Switch==10 && TimedLeftMagnaTimer) {						// was it the left switch and is the magna save active?
		KillTimer(TimedLeftMagnaTimer);
		TimedLeftMagnaTimer = 0;
		ReleaseSolenoid(10);															// turn off magnet
		EndLeftMagna(0);}
	if (!TimedRightMagnaTimer && !TimedLeftMagnaTimer) { // no magna save active
		Switch_Released = DummyProcess;}}									// ignore released switches

void GameMain(byte Event) {                           // game switch events
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
  case 9:                                             // right magna save button
    if (RightMagna[Player]) {                         // right magna save available?
    	if (!game_settings[TimedMagna]) {								// no timed magna save
    		AddBlinkLamp(9, 150);
    		RightMagna[Player] = 0;                   		// not any more
    		ActivateTimer(4990, 0, EndRightMagna);
    		ActivateSolenoid(5000, 9);}                  	// use it for 5 seconds
    	else {																					// timed magna active
    		Switch_Released = TimedMagnaSW;
    		RightMagna[Player]--;													// reduce magna save seconds
    		AddBlinkLamp(9, 150*RightMagna[Player]);
    		ActivateSolenoid(6000, 9);										// switch on magnet
    		TimedRightMagnaTimer = ActivateTimer(1000, 0, TimedRightMagna);}}		// check again in 1s
    break;
  case 10:                                            // left magna save button
    if (LeftMagna[Player]) {                          // left magna save available?
    	if (!game_settings[TimedMagna]) {								// no timed magna save
    		AddBlinkLamp(10, 150);
    		LeftMagna[Player] = 0;                   			// not any more
    		ActivateTimer(4990, 0, EndLeftMagna);
    		ActivateSolenoid(5000, 10);}                  // use it for 5 seconds
    	else {																					// timed magna active
    		Switch_Released = TimedMagnaSW;
    		LeftMagna[Player]--;													// reduce magna save seconds
    		AddBlinkLamp(10, 150*LeftMagna[Player]);
    		ActivateSolenoid(6000, 10);										// switch on magnet
    		TimedLeftMagnaTimer = ActivateTimer(1000, 0, TimedLeftMagna);}} // check again in 1s
    break;
  case 11:                                            // left outlane
  case 12:                                            // right outlane
    Points[Player] += Multiballs * 5000;              // add 5000 points
    ShowPoints(Player);                               // and show the score
    AddBonus(2);
    if (LastChance) {
      LastChance = false;
      LastChanceOver = true;                          // dont give a last chance again
      LastChanceActive = true;                        // give a last chance ball
      AddBlinkLamp(11, 150);                          // let the last chance lamps blink
      AddBlinkLamp(12, 150);}
    break;
  case 13:                                            // spinner
    if (RightMysteryTimer) {
      Points[Player] += Multiballs * 2500;}           // add 5000 points
    else {
      Points[Player] += Multiballs * 100;}            // add 100 points
    ShowPoints(Player);                               // and show the score
    break;
  case 14:                                            // right ramp
    if (LeftMysteryTimer) {                           // left mystery active?
      ByteBuffer = ((byte)(TimerValue[LeftMysteryTimer])/2.56);
      if (ByteBuffer < 20) {
        Points[Player] += Multiballs * 20000;}        // at least 20000 points
      else {
        Points[Player] += Multiballs * ByteBuffer * 1000;} // or more
      KillTimer(LeftMysteryTimer);                    // turn off left mystery
      LeftMysteryTimer = 0;
      RemoveBlinkLamp(14);}                           // stop the blinking of the mystery lamp
    else {
      Points[Player] += Multiballs * 500;}            // add 500 points
    ShowPoints(Player);                               // and show the score
    break;
  case 15:                                            // right inlane
    if (RightMysteryTimer) {                          // right mystery timer already running?
      KillTimer(RightMysteryTimer);}                  // stop it
    else {
      AddBlinkLamp(13, 250);}                         // let the mystery lamp blink
    if (RightAfterMagna) {                            // if it's right after using the right magna save
      AddBonus(5);
      Points[Player] += Multiballs * 10000;}          // add 10000 points
    else {
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
      AddBonus(5);
      Points[Player] += Multiballs * 10000;}          // add 10000 points
    else {
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
      AddBlinkLamp(FirstMultLamp+BonusMultiplier-2, 250);
      ActivateTimer(2000, FirstMultLamp+BonusMultiplier-2, SetBonusMultiplier);}
    if (LowerExBall[Player]) {
      Lamp[23] = false;
      Lamp[47] = true;
      Lamp[1] = true;
      LowerExBall[Player] = false;
      ExBalls++;}
    break;
  case 24:                                            // lower eject hole
    Points[Player] += Multiballs * 5000;
    ShowPoints(Player);
    if (Lamp[24]) {                                   // lower lock lit?
      LockedBalls[Player]++;
      StartMultiball();}
    else {
      ActivateTimer(2000, 8, DelaySolenoid);}         // eject ball
    break;
  case 25:
  case 26:                                            // lower left drop targets
  case 27:
    if (!DropWait[0]) {
      DropWait[0] = true;
      Points[Player] += Multiballs * 1000;
      ShowPoints(Player);
      ActivateTimer(200, 0, HandleDropTargets);}
    break;
  case 29:
  case 30:                                            // lower right drop targets
  case 31:
    if (!DropWait[1]) {
      DropWait[1] = true;
      Points[Player] += Multiballs * 1000;
      ShowPoints(Player);
      ActivateTimer(200, 1, HandleDropTargets);}
    break;
  case 33:
  case 34:                                            // upper left drop targets
  case 35:
    if (!DropWait[2]) {
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
    if (UpperExBall[Player]) {
      Lamp[41] = false;
      Lamp[47] = true;
      Lamp[1] = true;
      UpperExBall[Player] = false;
      ExBalls++;}
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
    LockLightsTimer = 0;
    BlinkScoreTimer = 0;
    RightMysteryTimer = 0;
    LeftMysteryTimer = 0;
    BallWatchdogTimer = 0;
    TimedRightMagnaTimer = 0;
    TimedLeftMagnaTimer = 0;
    CheckReleaseTimer = 0;
    TestMode_Enter();
    break;
}}

void ClearOuthole(byte Event) {
  if (Switch[20]) {                                   // outhole switch still active?
    if (!BlockOuthole) {															// outhole blocked?
      BlockOuthole = true;														// block outhole until this ball has been processed
      ActivateSolenoid(0, 1);                         // put ball in trunk
      if (LastChanceActive && InLock) {								// saved by last chance
        BlockOuthole = false;													// remove outhole block
        Lamp[24] = false;
        RemoveBlinkLamp(11);                          // stop the blinking of the last chance lamps
        RemoveBlinkLamp(12);
        InLock--;                                     // decrease number of balls in lock
        LastChanceActive = false;                     // deactivate last chance protection
        ActivateSolenoid(0, 7);                       // eject 1 ball from the lock
        LockedBalls[Player]--;                        // decrease number of locked balls
        if (LockedBalls[Player]) {                    // if player had two locked balls
          RemoveBlinkLamp(40);                        // remove the blinking lock arrow
          LockLightsTimer = ActivateTimer(1000, 40, LockChaseLight);}} // and restart the chase light
      // ActivateTimer(1000, InLock, LastChanceLock);}
      else {
        ActivateTimer(2000, 0, BallEnd);}}
    else {
      ActivateTimer(2000, 0, ClearOuthole);}}}				// come back in 2s if outhole is blocked

void LastChanceLock(byte Event) {
  if (Event > 1) {
    DropWait[4] = true;
    ActivateSolenoid(0, 7);}}

void BallEnd(byte Event) {
  AppByte = CountBallsInTrunk();
  if ((AppByte == 5)||(AppByte < 4-Multiballs-InLock)) {
    InLock = 0;
    if (Multiballs == 1) {
      for (i=0; i<3; i++) {                         	// check how many balls are on the ball ramp
        if (Switch[41+i]) {
          InLock++;}}}
    WriteLower(" BALL   ERROR ");
    if (Event < 11) {                                	// have I been here already?
      Event++;
      ActivateTimer(1000, Event, BallEnd);}          	// if not try again in 1s
    else {                                          	// ball may be still in outhole
      BlockOuthole = false;
      Event = 0;
      ClearOuthole(0);}}
  else {
    switch (Multiballs) {
    case 3:                                           // goto 2 ball multiball
      Multiballs = 2;
      AddBlinkLamp(28, 500);
      RemoveBlinkLamp(32);
      if (AppByte != 1) {                             // not 1 ball in trunk
        ActivateTimer(1000, 0, BallEnd);}             // check again later
      else {
        BlockOuthole = false;}												// remove outhole block
      break;
    case 2:                                           // end multiball
      Multiballs = 1;
      RemoveBlinkLamp(28);
      if (AppByte == 3) {                             // 3 balls in trunk?
        ActivateTimer(1000, 0, BallEnd);}
      else {
        BlockOuthole = false;}												// remove outhole block
      break;
    case 1:                                           // end of ball
      if (AppByte + InLock != 3) {
      	WriteUpper(" COUNT  ERROR ");
      	InLock = 0;
      	for (i=0; i<3; i++) {                         // check how many balls are on the ball ramp
      	  if (Switch[41+i]) {
      	    InLock++;}}
      	ActivateTimer(1000, 0, BallEnd);}
      else {
      	LockedBalls[Player] = 0;
        Lamp[24] = false;                             // unlight lower lock
        if (LockLightsTimer) {
          KillTimer(LockLightsTimer);
          LockLightsTimer = 0;}
        else {
          RemoveBlinkLamp(40);}
        if (BlinkScoreTimer) {
          KillTimer(BlinkScoreTimer);
          BlinkScoreTimer = 0;}
        LastChance = false;                           // deactivate last chance
        Lamp[11] = false;
        Lamp[12] = false;
        LastChanceOver = false;
        LastChanceActive = false;
//        Lamp[FirstMultLamp] = false;
//        Lamp[FirstMultLamp+1] = false;
//        Lamp[FirstMultLamp+2] = false;
//        Lamp[FirstMultLamp+3] = false;
        BonusToAdd = Bonus;
        BonusCountTime = 20;
        CountBonus(AppByte);}}}}

void CountBonus(byte Balls) {
	AppByte = Bonus % 10;
	if (AppByte) {
		Lamp[BonusLamp + AppByte - 1] = false;}
	else {
		AppByte = Bonus / 10;
		Lamp[BonusLamp + 8 + AppByte] = false;
    if (AppByte > 1) {
      Lamp[BonusLamp + 7 + AppByte] = true;}
		for (i=48; i<57; i++) {
			Lamp[i] = true;}}
	Points[Player] += 1000;
	ShowPoints(Player);
	Bonus--;
	if (Bonus) {
    if (BonusCountTime) {
      BonusCountTime--;}
		ActivateTimer(20+20*BonusCountTime, Balls, CountBonus);}
	else {
    if (BonusMultiplier > 1) {
      Lamp[FirstMultLamp+BonusMultiplier-2] = false;
      Bonus = BonusToAdd;
      ShowBonus();
      BonusMultiplier--;
      ActivateTimer(20+20*BonusCountTime, Balls, CountBonus);}
    else {
      BonusToAdd = 0;
      Bonus = 1;
      BallEnd2(Balls);}}}

void BallEnd2(byte Balls) {
  if (BallWatchdogTimer) {
    KillTimer(BallWatchdogTimer);
    BallWatchdogTimer = 0;}
  if (game_settings[TimedMagna]) {              			// if timed magna save mode
		Switch_Released = DummyProcess;               		// reset the released switch mode
		if (TimedRightMagnaTimer) {                 			// if the left magnet is currently active
			ReleaseSolenoid(9);                      				// turn it off
			KillTimer(TimedRightMagnaTimer);          			// and kill its timer
			TimedRightMagnaTimer = 0;}
		if (TimedLeftMagnaTimer) {                  			// same as above
			ReleaseSolenoid(10);
			KillTimer(TimedLeftMagnaTimer);
			TimedLeftMagnaTimer = 0;}
		RemoveBlinkLamp(9);                         			// turn off the magna save lamps
		RemoveBlinkLamp(10);}
	if (ExBalls) {                                			// Player has extra balls
    AddBlinkLamp(47, 250);                      			// Let the extra ball lamp blink
    AddBlinkLamp(1, 250);                       			// also in the backbox
    ExBalls--;
    ActivateTimer(2000, 0, AfterExBallRelease);
    ActivateTimer(1000, AppByte, NewBall);
    BlockOuthole = false;}														// remove outhole block
  else {                                        			// Player has no extra balls
    if ((Points[Player] > HallOfFame.Scores[3]) && (Ball == APC_settings[NofBalls])) { // last ball & high score?
      Switch_Pressed = DummyProcess;                                // Switches do nothing
      CheckHighScore(Player);}
    else {
      BallEnd3(Balls);}}}

void BallEnd3(byte Balls) {
 	BlockOuthole = false;																// remove outhole block
 	Lamp[24] = false;                         					// lower lock lamp off
 	if (Player < NoPlayers) {                 					// last player?
 		Player++;
 		ActivateTimer(1000, Balls, NewBall);}
 	else {
 		if (Ball < APC_settings[NofBalls]) {
 			Player = 1;
 			Ball++;
 			ActivateTimer(1000, Balls, NewBall);}
 		else {																						// game end
 			Lamp[36] = false;                     					// bumper light off
 			ReleaseSolenoid(23);                  					// disable flipper fingers
 			ReleaseSolenoid(24);
 			//digitalWrite(SpcSolEnable, LOW);      				// disable special solenoids
 			CheckForLockedBalls(0);
      Lamp[2] = false;                      					// turn off Ball in Play lamp
 			GameDefinition.AttractMode();}}}

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
	case 3:                                           	// credit button
		KillTimer(ByteBuffer2);
		ByteBuffer++;
		if (ByteBuffer > 2) {
			HandleHighScores(Points[Player]);
			WriteUpper(" HIGH   SCORE ");
			WriteLower("TABLE  POS    ");
			*(DisplayLower+28) = DispPattern2[32 + 2 * (ByteBuffer2+1)];
			*(DisplayLower+29) = DispPattern2[33 + 2 * (ByteBuffer2+1)];
			LampPattern = Lamp;
			ActivateSolenoid(0, 23);                      	// enable flipper fingers
			ActivateSolenoid(0, 24);
			ActivateTimer(2000, 0, BallEnd3);}
		else {
			BlinkInitial(1);}
		break;
	case 9:                                           	// right magna save button
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
	case 10:                                          	// left magna save button
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

void HandleHighScores(unsigned int Score) {
	ByteBuffer2 = 0;
	while (HallOfFame.Scores[ByteBuffer2] > Score) {  	// check to which position of the highscore list it belongs
		ByteBuffer2++;}
	for (i=3; i>ByteBuffer2; i--) {                   	// move all lower highscores down
		HallOfFame.Scores[i] = HallOfFame.Scores[i-1];}
	for (i=9; i>ByteBuffer2*3; i--) {
		HallOfFame.Initials[i+2] = HallOfFame.Initials[i-1];}
	HallOfFame.Scores[ByteBuffer2] = Score;           	// and include the new highscore to the list
	for (i=0; i<3; i++) {
		HallOfFame.Initials[ByteBuffer2*3+i] = EnterIni[i];} // copy initials
	if (SDfound) {
		SD.remove(GameDefinition.HighScoresFileName);
		File HighScore = SD.open(GameDefinition.HighScoresFileName,FILE_WRITE);  // open the highscore file on the SD card
		HighScore.write((byte*) &HallOfFame, sizeof HallOfFame); // and write the HallOfFame structure
		HighScore.close();}
	else {
		WriteUpper("  NO   SD CARD");
		delay(2000);}
}

void AfterExBallRelease(byte Event) {
  if (Switch[45]) {                                   // ball still in the shooting lane?
    ActivateTimer(2000, Event, AfterExBallRelease);}  // come back in2s
  else {                                              // ball has been shot
    RemoveBlinkLamp(1);
    RemoveBlinkLamp(47);
    if (ExBalls) {                                    // player still has an extra balls
      Lamp[1] = true;
      Lamp[47] = true;}}}

void HandleLock(byte Event) {
  AppBool = false;
  AppByte = 0;
  for (i=0; i<3; i++) {                               // count balls in lock
    if (Switch[41+i]) {
      if (AppByte < i) {
        AppBool = true;}                              // set warning flag
      AppByte++;}}
  if (AppBool) {
    ActivateTimer(1000, 0, HandleLock);}              // try again in 1s
  else {
    DropWait[4] = false;}                             // clear ignore flag
  if (Multiballs > 1) {                             	// multiball running
    ClearLocks(0);}                                 	// eject balls
  else {                                            	// no multiball running
    if (AppByte > InLock) {                           // more than before?
      Points[Player] += 5000;
      ShowPoints(Player);
      InLock++;
      LockedBalls[Player]++;                          // increase number of locked balls
      if (LockedBalls[Player] == 3) {                 // 3 locked balls?
        StartMultiball();}
      else {                                          // less than 3 locked balls?
        if ((Ball == APC_settings[NofBalls]) && !LastChanceOver) { // last ball and no last chance yet?
          LastChance = true;                          // activate last chance
          Lamp[11] = true;
          Lamp[12] = true;}
        if (InLock > LockedBalls[Player]) {           // more balls in lock than player has?
          InLock--;                                   // eject 1 ball
          ActivateSolenoid(0, 7);}
        else {
          Lamp[24] = true;                            // light lower lock
          if (BallWatchdogTimer) {
            KillTimer(BallWatchdogTimer);
            BallWatchdogTimer = 0;}
          NewBall(3-InLock);}}}}}                     // release new ball

void HandleDropTargets(byte Event) {
  DropWait[Event] = false;
  if (Switch[DropTargets[Event]] && (Switch[DropTargets[Event]+1]) && (Switch[DropTargets[Event]+2])) { // all drop targets hit?
    AddBonus(3);                                      // add 3K bonus
    if (DropTimer[Event]) {
      KillTimer(DropTimer[Event]);}                   // kill the target bank timer
    ResetDropTargets(Event);                          // and reset the targets
    if ((Event==1) || (Event==3)) {                   // enable magna save if right target banks were hit
    	if (!game_settings[TimedMagna]) {								// timed magna feature off
    		Lamp[9] = true;
    		RightMagna[Player] = 1;}
    	else {																					// timed magna feature on
    		if (RightMagna[Player] < 5) {									// less than 5 magna seconds
    			RightMagna[Player]++;												// add one
    			if (RightMagna[Player] == 5) {							// 5 seconds reached?
    				RemoveBlinkLamp(9);												// turn off blinking lamp
    				Lamp[9] = true;}													// and switch it on permanently
    			else {																			// 5 seconds not reached?
    				AddBlinkLamp(9, 150*RightMagna[Player]);}}}} // adapt blinking frequency
    else {																						// enable magna save if left target banks were hit
    	if (!game_settings[TimedMagna]) {								// timed magna feature off
    		Lamp[10] = true;
    		LeftMagna[Player] = 1;}
    	else {																					// timed magna feature on
     		if (LeftMagna[Player] < 5) {									// less than 5 magna seconds
     			LeftMagna[Player]++;												// add one
     			if (LeftMagna[Player] == 5) {								// 5 seconds reached?
     				RemoveBlinkLamp(10);											// turn off blinking lamp
     				Lamp[10] = true;}													// and switch it on permanently
     			else {																			// 5 seconds not reached?
     				AddBlinkLamp(10, 150*LeftMagna[Player]);}}}} // adapt blinking frequency
    if (DropHits[(Player-1)*4+Event] < 3) {           // target bank cleared less than 3 times?
      Lamp[DropTargets[Event]+DropHits[(Player-1)*4+Event]] = true; // turn on the appropriate arrow lamp
      DropHits[(Player-1)*4+Event]++;                 // and increase the counter for this target bank
      if (DropHits[(Player-1)*4+Event] == 3) {        // target bank cleared 3 times?
        if (PlayersExBalls[Player]) {                 // if the player did already get an extra ball
          if ((DropHits[(Player-1)*4] == 3) && (DropHits[(Player-1)*4+1] == 3) && (DropHits[(Player-1)*4+2] == 3) && (DropHits[(Player-1)*4+3] == 3)) {
            for (i=0; i<4; i++) {                     // for all drop target banks
              DropHits[(Player-1)*4+i] = 0;           // clear their hits
              Lamp[DropTargets[i]] = false;           // and their arrow lamps
              Lamp[DropTargets[i]+1] = false;
              Lamp[DropTargets[i]+2] = false;}
            Lamp[23] = true;                          // turn on the lower extra ball lamp
            LowerExBall[Player] = true;}}                     // and enable the lower extra ball
        else {                                        // if the player didn't get an extra ball yet
          for (AppByte=0 ; AppByte < 2; AppByte++) {
            if ((DropHits[(Player-1)*4+AppByte*2] == 3) && (DropHits[(Player-1)*4+AppByte*2+1] == 3)) {
              for (i=0; i<2; i++) {                   // for all drop target banks
                DropHits[(Player-1)*4+AppByte*2+i] = 0; // clear their hits
                Lamp[DropTargets[AppByte*2+i]] = false; // and their arrow lamps
                Lamp[DropTargets[AppByte*2+i]+1] = false;
                Lamp[DropTargets[AppByte*2+i]+2] = false;}
              Lamp[41] = true;                        // turn on the upper extra ball lamp
              PlayersExBalls[Player] = true;
              UpperExBall[Player] = true;}}}}}}
  else {                                              // not all targets cleared
    if (Switch[DropTargets[Event]] || (Switch[DropTargets[Event]+1]) || (Switch[DropTargets[Event]+2])) { // any target down? (or false alarm)
      if (!DropTimer[Event]) {                        // no timer running for this bank already?
        DropTimer[Event] = ActivateTimer(3000, Event, BlinkFaster); // start one
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
  DropWait[Event] = false;}

void DelaySolenoid(byte Event) {                      // activate solenoid after delay time
  ActivateSolenoid(0, Event);}

void StartMultiball() {
  WriteUpper2(" MULTI  BALL  ");                     	// switch display to alternate buffer
  DispRow1 = DisplayUpper2;
  if (LastChance) {                                   // last chance avtive?
    LastChance = false;                               // deactivate it
    Lamp[11] = false;
    Lamp[12] = false;}
  ActivateSolenoid(1000, 15);                         // ring the bell
  Lamp[24] = false;                                   // unlight lower lock
  if (LockedBalls[Player] == 3) {                     // 2 or 3 ball multiball?
    Multiballs = 3;
    AddBlinkLamp(32, 500);                            // let triple points lamp blink
    RemoveBlinkLamp(40);                              // turn off the blinking of the first lock arrow
    LockLightsTimer = ActivateTimer(1000, 40, LockChaseLight);}
  else {
    Multiballs = 2;
    AddBlinkLamp(28, 500);}                           // let double points lamp blink
  LockedBalls[Player] = 0;
  ActivateTimer(3000, 1, SwitchDisplay);              // switch display back to main buffer in 3 seconds
  if (Switch[24]) {                                   // for the lower elect hole
    ActivateTimer(2000, 8, DelaySolenoid);}
  ActivateTimer(3100, 0, ClearLocks);}                // clear out balls after that

void SwitchDisplay(byte Event) {                      // switch between different display buffers
  if (Event == 1) {
    DispRow1 = DisplayUpper;
    DispRow2 = DisplayLower;}
  else {
    DispRow1 = DisplayUpper2;
    DispRow2 = DisplayLower2;}}

void ClearLocks(byte Event) {
  AppByte = 0;
  for (i=0; i<3; i++) {                               // count balls in lock
    if (Switch[41+i]) {
      AppByte++;}}
  if (AppByte) {                                      // balls in lock?
    ActivateSolenoid(0, 7);
    AppByte--;
    InLock = 0;
    ActivateTimer(1000, 0, ClearLocks);}}

void SetBonusMultiplier(byte Event) {                 // switch from blinking bonus multiplier lamp to permanent on
  RemoveBlinkLamp(Event);
  Lamp[Event] = true;}

void AddBonus(byte BonusPts) {
	byte OldBonusToAdd = BonusToAdd;
	if (Bonus < 49) {
		if (Bonus+BonusPts > 49) {
			BonusToAdd += 49 - Bonus;
			Bonus = 49;}
		else {
			BonusToAdd += BonusPts;
			Bonus += BonusPts;}
		if (BonusToAdd && !OldBonusToAdd) {
			ClearBonusLight(0);}}}

void ClearBonusLight(byte Step) {
	if (Step == 10) {
		Step = 0;}
	if (Step == 9) {
		for (i=BonusLamp; i<BonusLamp+9; i++) {
			Lamp[i]  = false;}
		BonusToAdd--;}
	else {
		if (Lamp[BonusLamp+Step]) {
			Lamp[BonusLamp+Step] = false;}
		else {
			BonusToAdd--;}}
	ActivateTimer(50, Step, SetBonusLight);}

void SetBonusLight(byte Step) {
	if (Step == 9) {
		if (Lamp[BonusLamp+9] || Lamp[BonusLamp+10] || Lamp[BonusLamp+11] || Lamp[BonusLamp+12]) {
			i = 0;
			while (i < 3) {
				if (Lamp[BonusLamp+9+i]) {
					Lamp[BonusLamp+9+i] = false;
					Lamp[BonusLamp+10+i] = true;
					i = 3;}
				else {
					i++;}}}
		else {
			Lamp[BonusLamp+9] = true;}}
	else {
		Lamp[BonusLamp+Step] = true;}
	Step++;
	if (BonusToAdd) {
		ActivateTimer(50, Step, ClearBonusLight);}}

void ShowBonus() {                                    // set lamps on bonus meter
  if (Bonus < 50) {                                   // max bonus of 49K allowed
    AppByte = Bonus;}
  else {
    AppByte = 49;}
  for (i=0; i<13; i++) {                              // for all 13 lamps of the bonus meter
    if (AppByte >= BonusValues[i]) {                  // bonus larger or equal than the value of the actual lamp?
      if (i < 4) {
        AppByte -= BonusValues[i];}                   // reduce bonus accordingly
      else {
        AppByte -= 1;}
      Lamp[BonusLamps[i]] = true;}                    // and turn the lamp on
    else {
      Lamp[BonusLamps[i]] = false;}}}                 // otherwise turn it off

void EndRightMagna(byte Event) {
  RightAfterMagna = true;
  AddBlinkLamp(15, 150);
  if (!RightMagna[Player]) {
  	RemoveBlinkLamp(9);}
  ActivateTimer(2010, 0, ResetRightAfterMagna);}      // for 2 seconds after magna time

void ResetRightAfterMagna(byte Event) {
  RemoveBlinkLamp(15);
  RightAfterMagna = false;}

void EndLeftMagna(byte Event) {
  LeftAfterMagna = true;
  AddBlinkLamp(16, 150);
  if (!LeftMagna[Player]) {
  	RemoveBlinkLamp(10);}
  ActivateTimer(2010, 0, ResetLeftAfterMagna);}       // for 2 seconds after magna time

void ResetLeftAfterMagna(byte Event) {
  RemoveBlinkLamp(16);
  LeftAfterMagna = false;}

void ResetRightMystery(byte Event) {
  RemoveBlinkLamp(13);                                // stop the blinking of the mystery lamp
  RightMysteryTimer = 0;}                             // delete the right mystery timer number

void ResetLeftMystery(byte Event) {
  RemoveBlinkLamp(14);                                // stop the blinking of the mystery lamp
  LeftMysteryTimer = 0;}                              // delete the left mystery timer number

void LockChaseLight(byte ChaseLamp) {                 // controls the chase light pointing to the upper lock
  LockLightsTimer = 0;
  AppByte = LockedBalls[Player];
  switch (ChaseLamp) {                                // illuminate ChaseLamp
    case 40:                                          // is it the first lamp?
      if (!AppByte) {                                 // is there no locked ball?
        Lamp[42] = false;}                            // turn off the last lamp
      Lamp[21] = false;                               // the middle lamp can always be switched off
      if (AppByte == 2) {                             // are there two locked balls?
        Lamp[42] = true;                              // turn on the last two lock lamps
        Lamp[21] = true;
        AddBlinkLamp(40, 500);}                       // and let the first one blink
      else {                                          // otherwise
        Lamp[40] = true;                              // turn the first lamp on
        LockLightsTimer = ActivateTimer(1000, 21, LockChaseLight);} // and come back in one second to process lamp 21
      break;
    case 21:                                          // is it the middle lamp?
      Lamp[40] = false;                               // turn off the first
      Lamp[21] = true;                                // turn on the middle
      if (AppByte) {                                  // is there a locked ball?
        Lamp[42] = true;                              // turn the last lamp on
        LockLightsTimer = ActivateTimer(1000, 40, LockChaseLight);} // and come back in one second to process lamp 40
      else {                                          // no locked ball?
        LockLightsTimer = ActivateTimer(1000, 42, LockChaseLight);} // come back in one second to process lamp 42
      break;
    case 42:                                          // is it the last lamp?
      Lamp[21] = false;                               // turn off the middle lamp
      Lamp[42] = true;                                // turn on the last lamp
      LockLightsTimer = ActivateTimer(1000, 40, LockChaseLight); // and come back in one second to process lamp 40
      break;}}

void ResetHighScores(bool change) {										// delete the high scores file
	if (change) {																				// if the start button has been pressed
		if (SDfound) {
			SD.remove(GameDefinition.HighScoresFileName);
			struct HighScores HScBuffer;										// create new high score table
			HallOfFame = HScBuffer;													// copy it to the hall of fame
			WriteLower(" SCORES DONE  ");}
		else {
			WriteLower(" SCORES NO SD ");}}
	else {
		WriteLower(" SCORES       ");}}

// Test mode

void TestMode_Enter() {
  KillAllTimers();
  ActivateSolenoid(0, 23);                            // enable flipper fingers
  ActivateSolenoid(0, 24);
  Switch_Pressed = DummyProcess;                                // Switches do nothing
  if (!digitalRead(UpDown)) {
    Settings_Enter();}
  else {
    WriteUpper("  TEST  MODE  ");                     // Show Test Mode
    WriteLower("              ");
    LampPattern = NoLamps;                            // Turn off all lamps
    ActivateTimer(1000, 0, DisplayTest_Enter);}}      // Wait 1 second and proceed to Display Test

void DisplayTest_Enter(byte Event) {
  Switch_Pressed = DisplayTest_EnterSw;                         // Switch functions
  WriteUpper("DISPLAY TEST  ");}                    	// Show 'Display Test'

void DisplayTest_EnterSw(byte Event) {                // Display Test Enter switch handler
  switch (Event) {
  case 72:                                            // Next test
    WriteUpper(" SWITCHEDGES  ");                   	// Show 'Switch Edges'
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
  if (!digitalRead(UpDown)) {                         // cycle only if Up/Down switch is not pressed
    if (Event == 116) {                             	// if the last character is reached
      AppByte2 = 32;}                                 // start from the beginning
    else {
      if (Event == 50) {                            	// reached the gap between numbers and characters?
        AppByte2 = 66;}
      else {
        AppByte2 = Event+2;}}                       	// otherwise show next character
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
      WriteUpper("  COIL  TEST  ");                 	// go to Solenoid Test Enter
      WriteLower("              ");
      Switch_Pressed = Solenoids_Enter;
      break;
    case 3:
      WriteUpper(" LATESTEDGES  ");                 	// show 'Latest Edges'
      WriteLower("              ");
      Switch_Pressed = SwitchEdges;}}

void SwitchEdges(byte Event) {
  if (Event == 72) {
    Switch_Pressed = SwitchEdges_Enter;               // back to Switch edges enter
    WriteUpper(" SWITCHEDGES  ");
    WriteLower("              ");}
  else {
    ConvertToBCD(Event);                              // convert the switch number to BCD
    for (i=1; i<24; i++) {                            // move all characters in the lower display row 4 chars to the left
      DisplayLower[i] = DisplayLower[i+8];}
    *(DisplayLower+28) = DispPattern2[32 + 2 * ByteBuffer2]; // and insert the two result digits to the right of the row
    *(DisplayLower+29) = DispPattern2[33 + 2 * ByteBuffer2];
    *(DisplayLower+30) = DispPattern2[32 + 2 * ByteBuffer];
    *(DisplayLower+31) = DispPattern2[33 + 2 * ByteBuffer];}}

void Solenoids_Enter(byte Event) {
  switch (Event) {
    case 72:
      WriteUpper(" SINGLE LAMP  ");                 	// go to next test
      Switch_Pressed = SingleLamp_Enter;
      break;
    case 3:
      WriteUpper(" FIRINGCOIL NO");
      WriteLower("              ");
        Switch_Pressed = SolenoidsTest;
        AppByte2 = 1;                                 // start with solenoid 1
        AppByte = ActivateTimer(1000, 0, FireSolenoids);}} // start after 1 second

void SolenoidsTest(byte Event) {
  if (Event == 72) {
    KillTimer(AppByte);                               // kill running timer for next solenoid
    WriteUpper("  COIL  TEST  ");                   	// back to Solenoid Test Enter
    WriteLower("              ");
    Switch_Pressed = Solenoids_Enter;}}

void FireSolenoids(byte Event) {                      // cycle all solenoids
  ConvertToBCD(AppByte2);                             // convert the actual solenoid number
  *(DisplayLower+28) = DispPattern2[32 + 2 * ByteBuffer2]; // and show it
  *(DisplayLower+29) = DispPattern2[33 + 2 * ByteBuffer2];
  *(DisplayLower+30) = DispPattern2[32 + 2 * ByteBuffer];
  *(DisplayLower+31) = DispPattern2[33 + 2 * ByteBuffer];
  ActivateSolenoid(0, AppByte2);                     	// activate the solenoid for 50ms
  if (!digitalRead(UpDown)) {
    AppByte2++;                                       // increase the solenoid counter
  if (AppByte2 == 20) {                               // maximum reached?
    AppByte2 = 1;}}                                   // then start again
  AppByte = ActivateTimer(1000, 0, FireSolenoids);}   // come back in one second

void SingleLamp_Enter(byte Event) {
  switch (Event) {
    case 72:
      WriteUpper("  ALL   LAMPS ");                 	// go to next test
      Switch_Pressed = AllLamps_Enter;
      break;
    case 3:
      WriteUpper(" ACTUAL LAMP  ");
      WriteLower("              ");
      for (i=0; i<(LampMax+1); i++){                  // erase lamp matrix
        Lamp[i] = false;}
      LampPattern = Lamp;                             // and show it
      Switch_Pressed = SingleLamp;
      AppByte2 = 1;                                 	// start with lamp 1
      AppByte = ActivateTimer(1000, 0, ShowLamp);}} 	// start after 1 second

void SingleLamp(byte Event) {
  if (Event == 72) {
    KillTimer(AppByte);                               // kill running timer for next solenoid
    LampPattern = NoLamps;                            // Turn off all lamps
    WriteUpper(" SINGLE LAMP  ");
    WriteLower("              ");
    Switch_Pressed = SingleLamp_Enter;}}

void ShowLamp(byte Event) {                           // cycle all solenoids
  if (!digitalRead(UpDown)) {
    ConvertToBCD(AppByte2);                           // convert the actual solenoid number
    *(DisplayLower+28) = DispPattern2[32 + 2 * ByteBuffer2]; // and show it
    *(DisplayLower+29) = DispPattern2[33 + 2 * ByteBuffer2];
    *(DisplayLower+30) = DispPattern2[32 + 2 * ByteBuffer];
    *(DisplayLower+31) = DispPattern2[33 + 2 * ByteBuffer];
    Lamp[AppByte2] = true;                            // turn on lamp
    if (AppByte2 > 1) {                               // and turn off the previous one
      Lamp[AppByte2-1] = false;}
    else {
      Lamp[LampMax] = false;}
    AppByte2++;                                       // increase the lamp counter
    if (AppByte2 == LampMax+1) {                      // maximum reached?
      AppByte2 = 1;}}                                 // then start again
  AppByte = ActivateTimer(1000, 0, ShowLamp);}        // come back in one second

void AllLamps_Enter(byte Event) {
  switch (Event) {
    case 72:
    	WriteUpper(" SOUND  TEST  ");                 	// go to next test
      Switch_Pressed = SoundTest_Enter;
      AppByte = 0;
      break;
    case 3:
      WriteUpper("FLASHNG LAMPS ");
      Switch_Pressed = ModeAllLamps;
      AppByte2 = 0;
      AppByte = ActivateTimer(500, 0, ShowAllLamps);}} // start after 500ms

void ModeAllLamps(byte Event) {
  if (Event == 72) {
    KillTimer(AppByte);                               // kill running timer for next solenoid
    LampPattern = NoLamps;                            // Turn off all lamps
    WriteUpper("  ALL   LAMPS ");
    Switch_Pressed = AllLamps_Enter;}}

void ShowAllLamps(byte Event) {                       // Flash all lamps
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
      ReleaseSolenoid(23);                            // disable flipper fingers
      ReleaseSolenoid(24);
      GameDefinition.AttractMode();
      break;
    case 3:
      WriteUpper("PLAYING SOUND ");
      WriteLower("            01");
      Switch_Pressed = SoundTest;
      AfterMusic = NextTestSound;
      PlayMusic((char*) TestSounds[AppByte]);}}

void NextTestSound() {
  if (!digitalRead(UpDown)) {
	  AppByte++;}
	if (!TestSounds[AppByte][0]) {
		AppByte = 0;}
  ConvertToBCD(AppByte+1);                           	// convert the actual solenoid number
  *(DisplayLower+28) = DispPattern2[32 + 2 * ByteBuffer2]; // and show it
  *(DisplayLower+29) = DispPattern2[33 + 2 * ByteBuffer2];
  *(DisplayLower+30) = DispPattern2[32 + 2 * ByteBuffer];
  *(DisplayLower+31) = DispPattern2[33 + 2 * ByteBuffer];
	PlayMusic((char*) TestSounds[AppByte]);}

void SoundTest(byte Switch) {
	if (Switch == 3) {
		NextTestSound();}
	if (Switch == 72) {
    AfterMusic = 0;
		StopPlayingMusic();
		WriteUpper(" SOUND  TEST  ");
    Switch_Pressed = SoundTest_Enter;
    AppByte = 0;}}

