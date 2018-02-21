// Rules for the Pinbot pinball machine

const unsigned int PB_SolTimes[24] = {30,30,30,50,30,800,30,30,500,500,999,999,999,999,500,500,50,500,50,50,50,50,0,0}; // Activation times for solenoids

struct SettingTopic PB_setList[4] = {{"DROP TG TIME  ",HandleNumSetting,0,3,10},
       {"RESTOREDEFAULT",RestoreDefaults,0,0,0},
       {"  EXIT SETTNGS",ExitSettings,0,0,0},
       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

const byte PB_defaults[64] = {5,0,0,0,0,0,0,0,		 		// game default settings
											  			0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0,
															0,0,0,0,0,0,0,0};

const struct GameDef PB_GameDefinition = {
	PB_setList,																					// GameSettingsList
	(byte*)PB_defaults,																	// GameDefaultsPointer
	"PB_SET.APC",																				// GameSettingsFileName
	"PBSCORES.APC",																			// HighScoresFileName
	PB_AttractMode,																			// AttractMode
	PB_SolTimes};																				// Default activation times of solenoids

void PB_AttractMode() {

}
