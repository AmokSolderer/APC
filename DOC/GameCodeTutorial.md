# How to create my own game code?

Let‘s assume you just built your own APC board and put it in your pinball machine. You have probably tested the adapted BaseCode.ino as described in [Setting up the BaseCode](https://github.com/AmokSolderer/APC/blob/master/DOC/SetUpBC.md) and now you‘re keen to write your own stuff. In this document I will generate a tutorial code sheet as an example to explain how to do so. In my case the tutorial is supposed to run in a Rollergames pinball, but you can adapt it to other machines or just read it to understand the principles of the APC API. Please use the [APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) to get an overview about the available commands and variables.

## 1 Adding a new codesheet
First of all I copy my modified BaseCode.ino and rename it to a new .ino file in the APC folder. In my case I create the file Tutorial.ino in the APC folder where APC.ino, BlackKnight.ino and so on are already present. Then I change all BC_ prefixes to those of my own game to avoid compilation errors. For the Tutorial, I change them all to TT_.
### 1.1 The GameDefinition structure
After changing all the prefixes, my GameDefinition structure looks like this:

    struct GameDef TT_GameDefinition = {
        TT_setList,                           // GameSettingsList
        (byte*)TT_defaults,                   // GameDefaultsPointer
        "TT_SET.BIN",                         // GameSettingsFileName
        "TT_SCORE.BIN",                       // HighScoresFileName
        TT_AttractMode,                       // AttractMode
        TT_SolTimes};                         // Default activation times of solenoids

Every game has its own highscore and settings file to be stored on the SD card. The two filenames  tell the system which names to use. They can be chosen freely as long as they don‘t exceed 8 characters. The other variables have to be defined as follows.
### 1.2 Changing the settings menu
TT_setList has to be a list of SettingTopic structures. They determine how the game settings menu of my Tutorial machine will look like and how these settings will be handled. As an example there is a 'God Mode' defined in the Base Code settings (whatever this might be, but it can't harm to have one). I leave it as it is, which makes my settings list look like this:

    struct SettingTopic TT_setList[5] = {{"  GOD   MODE  ",HandleBoolSetting,0,0,0}, // defines the game specific settings
		{" RESET  HIGH  ",TT_ResetHighScores,0,0,0},
		{"RESTOREDEFAULT",RestoreDefaults,0,0,0},
		{"  EXIT SETTNGS",ExitSettings,0,0,0},
		{"",NULL,0,0,0}};

As you can only turn this mode on or off, we define it as a boolean setting (for more details on settings refer to the SelSetting command in the APC_Software_Reference). 
The „Restore Defaults“ and „Exit Settings“ topics are probably self explaining and the last one filled with zeros is necessary to indicate the end of the list.  
Now that we have defined the game settings menu we also need the corresponding setting values. The APC software uses an array of 64 byte to store those values:

    byte game_settings[64];		// game settings to be stored on the SD

When a SD card is detected during startup and a file with the name specified above is found (TT_SET.BIN) then its content is read into the game_setting array. If a file could not be found the pointer on position 2 of the game definition (TT_defaults) is used to read the default values. These values are also used if you choose Restore Default in the game settings menu.  
As I want to have to non-god mode as default, I can leaves the array as it is: 

    const byte TT_defaults[64] = {0,0,0,0,0,0,0,0,		// game default settings
				      0,0,0,0,0,0,0,0,
				      0,0,0,0,0,0,0,0,
				      0,0,0,0,0,0,0,0,
				      0,0,0,0,0,0,0,0,
				      0,0,0,0,0,0,0,0,
				      0,0,0,0,0,0,0,0,
				      0,0,0,0,0,0,0,0};

The default values have to be in the same order as the elements in TT_setList.  
If you want to know more about the settings system, please refer to the APC_setList in APC.ino, since the SettingTopic structure is also used for the system settings. That means you can find all the stuff mentioned above again in the APC.ino file with some real content and APC_ as the prefix. 
The next topic of my game definition tells the system how to enter the game code. Since a normal game starts with the attract mode, the corresponding pointer is named TT_AttractMode. For starters I just uncomment the TT_AttractMode copied from the Base Code and change it to this:  

    void TT_AttractMode() {
	  DispRow1 = DisplayUpper;
	  DispRow2 = DisplayLower;
	  WriteUpper("MY NEW GAME     ");}

The last topic of my new game definition is the TT_SolTimes. This is quite important as it tells the system what is the default turn on time for each solenoid in milliseconds.  
To understand the importance of this just remember that we have different kinds of solenoids in a pinball machine and that motors, magnets and flashers are also handled by the solenoid drivers. Strictly spoken this solenoid times should be set before you even activate your solenoids for the first time because otherwise you won‘t be able to test them properly. However for the beginning you can fill all values with 30 which is not enough to really activate a motor or a magnet but at least you won‘t have any burnt coils because of the turn on times being too long.  
For my Rollergames I choose 100ms for all flashers, 999ms for motors and relays. For solenoids 30ms is a good value to start and if you want more power try it with 50ms.
### 1.3 Adding a new game
Now that we have completed the game definition, we have to make the system use it. There is a subroutine named TT_init for this where TT_GameDefinition is selected.

    void TT_init() {
	ACselectRelay =  TT_ACselectRelay;	// assign the number of the A/C select relay
	GameDefinition = TT_GameDefinition;}	// read the game specific settings and highscores

Finally the new game code must be implemented into the APC.ino program. For this you have to locate the „Active Game“ menu entry in the APC_setList. 

    {" ACTIVE GAME ",HandleTextSetting,&TxTGameSelect[0][0],0,4},

The handler indicates a text setting with the different text entries being in TXTGameSelect. Increase the last number by one, as this is the number of available settings and we're about to add one. The 4 from above means that I have 5 game entries (the system starts counting with 0). After that I change TXTGameSelect to

    char TxTGameSelect[5][17] = {{" BASE  CODE     "},{" BLACK KNIGHT   "},{"    PINBOT      "},{"  USB  CONTROL  "},{"  TUTORIAL      "}};

which means that from now on Tutorial will be listed in my „Active Game“ selection  menu and the variable APC_setting[ActiveGame] will become 4 when it is selected.  

The last step is to locate Init_System2 in APC.ino where the initialization routine of the selected game is called after the system has started. With my tutorial included the switch statement of my selected game looks like this:

	void Init_System2(byte State) {
	switch(APC_settings[ActiveGame]) {	// init calls for all valid games
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

If you start this, your pinball will first complain about missing files for highscores and game settings. After that it should show „MY FIRST GAME“ in the displays and that was it. Now it's time  to add some functionality.

## 2 Basic game functions

Until now we just have shown some text in the displays, so it's time to add some functionality.

### 2.1 Lamps

In order to use lamps we have to set the LampPattern pointer to an array which stores the state of all lamps. The array consists of lamp columns with each one storing the state of 8 lamps. That means we need 8 columns to store the state of all 64 lamps. If you don't want to do something fancy, you don't have to care about this, just be sure to add

    LampPattern = LampColumns;		// point to the standard lamp array

to your code. This will make your system use the standard lamp array which is used by all lamp related commands.

Turning on a lamp is quite simple. Just get the number of the lamp you want to turn on from your pinball's manual and use

    TurnOnLamp(LampNumber);

to do so. The corresponding command to turn it off is

    TurnOffLamp(LampNumber);

If you want to make a lamp blink use

    AddBlinkLamp(LampNumber, Period in ms);

To stop the blinking use

    RemoveBlinkLamp(LampNumber);

Note that all lamps having the same blinking period are blinking synchronously.

With all the above added, our programm should look like this:

    void TT_AttractMode() {                          // Attract Mode
	DispRow1 = DisplayUpper;
	DispRow2 = DisplayLower;
	WriteUpper("MY NEW GAME     ");
	LampPattern = LampColumns;                   // point to the standard lamp array
	TurnOnLamp(53);
	AddBlinkLamp(54, 250);}

In this example lamp 53 is turned on permanently while lamp 54 is blinking with a period of 250ms (4 times per second)

### 2.2 Switches

The APC offers two ways to use switches. First you can use

    QuerySwitch(SwitchNumber);

to get the current status of a switch. A return value of 'true' means that the switch is activated.

The second way is to use a switch handler, which is a program that is called when a switch is activated or released. For now we want to handle only switches being activated.

    Switch_Pressed = TT_TestSW;
    Switch_Released = DummyProcess;

If a switch is activated (pressed) the routine TT_TestSW will be called with the switch number as the argument. As we don't want to handle released switches, we just set the Switch_Released pointer to the DummyProcess, which does exactly nothing.

### 2.3 Solenoids

First, let's bring the drop target bank of my Rollergames to live. For this, my first switch handler looks like this:

    void TT_TutorialSW(byte SwitchNo) {
      switch (SwitchNo) {
      case 8:                                       // high score reset
        digitalWrite(Blanking, LOW);                // invoke the blanking
        break;
      case 49:
      case 50:
      case 51:
        if (QuerySwitch(49)) {
          TurnOnLamp(49);}
        if (QuerySwitch(50)) {
          TurnOnLamp(50);}
        if (QuerySwitch(51)) {
          TurnOnLamp(51);}
        if (QuerySwitch(49) && QuerySwitch(50) && QuerySwitch(51)) {
          ActA_BankSol(6);
          TurnOffLamp(49);
          TurnOffLamp(50);
          TurnOffLamp(51);}}}

Whenever a switch is activated the above handler will be called with the number as an argument. An easy way to handle the various switch is just to use a switch statement.

Don't be confused by switch number 8 as it's just to spare your lamps and displays. If you upload new software into the Arduino on the APC, it can happen that the lamps and displays are getting brighter than normal for a very short time. This wont damage them, but they're not going to like it either. To prevent this from happening just press the _High Score Reset_ button before the upload. This will invoke the blanking which causes lamps, displays and coils to be switched off.

The important part is the handling of the switches 49, 50 and 51. In the Rollergames these switches belong to the drop targets and the numbers are even matching to the corresponding lamps. In my case I'm just checking for every drop target switch whether it is active and in case that it is, the corresponding lamp is lit. If all targets are down, all lamps are turned off and the reset coil of the target bank is activated.

If your machine also has drop targets you can just change the lamp, Switch and coil numbers and try it yourself. But be careful with your fingers when dropping the targets as they're coming up immediately when all three are down.

### 2.3 Timers

Now, let's assume we want to have timed drop targets. Most machines with timed targets have a blinking lamp nearby to indicate a certain urgency. The Rollergames has none, but I just assign this task to lamp 53. This changes my switch handler to

    void TT_TutorialSW(byte SwitchNo) {
	static byte DropTimer = 0;
	switch (SwitchNo) {
	case 8:                                          // high score reset
	  digitalWrite(Blanking, LOW);                   // invoke the blanking
	  break;
	case 49:                                         // drop targets
	case 50:
	case 51:
	  if (QuerySwitch(49) && QuerySwitch(50) && QuerySwitch(51)) {	// all targets down?
	    if (DropTimer) {// timer running?
	      KillTimer(DropTimer);                      // stop timer
	      DropTimer = 0;                             // and indicate it
	      RemoveBlinkLamp(53);}                      // turn off blinking lamp
	    ActA_BankSol(6);}                            // reset drop targets
	  else {                                         // not all targets down
	    if (!DropTimer) {                            // timer not yet running?
	      AddBlinkLamp(53, 500);                     // start blinking lamp
	      DropTimer = ActivateTimer(5000, 100, TT_TutorialSW);}}	// start timer for 5s
	  break;
	case 100:                                        // timer has run out
	  DropTimer = 0;                                 // indicate it
	  RemoveBlinkLamp(53);                           // turn off blinking lamp
	  ActA_BankSol(6);                               // reset drop targets
	  break;}}

In this example I'm using a timer to reset the drop targets 5 seconds after the first target has been hit. 

    DropTimer = ActivateTimer(5000, 100, TT_TutorialSW);}}	// start timer for 5s

In this line a timer is activated with a running time of 5000ms. After this time it will call TT_TutorialSW with an argument value of 100. When this call happens the DropTimer variable is set to 0 to indicate that no timer is running which also means that all drop targets are up. Afterwards the blinking lamp 53 is turned off and the drop target bank is reset.

If all three targets have been knocked down by the player the targets have to be re-set immediately and the timer must be deactivated. In order to deactivate a timer you need it's number which is returned by the ActivateTimer command when the timer is being started. Here, this number is being stored in the DropTimer variable and the KillTimer(DropTimer) command is used to get rid of the timer.

Why to call TT_TutorialSW and not some additional routine? 

Both will work, but with an additional routine you would need a global variable to store the timer number and an additional routine. Both is not necessarily harmful, but I prefer to keep to number of global variables and routines low to make the code easier to read and to indicate what belongs together.

Of course if there's no need to kill a timer you don't have to store it's number. As an example let's try to avoid the drop targets being re-set immediately when the last target comes down. In a game this would probably knock the ball up against the glass or hamper the target reset because the ball is still above them. Instead I would like to wait for 200ms before resetting the targets. In the code you would move the corresponding ActA_BankSol(6) command to a new routine with the name TT_ResetDropTargets(byte Dummy) and just call it by 

    ActivateTimer(200, 0, TT_ResetDropTargets);

## 3 A real game base
Until now I have just explained how to control the basic hardware functions of the APC which basically should be all you need to start your own game code, but it's still a lot to do before the first ball is put into the plunger lane and drained balls are correctly handled. The BaseCode contains a basic framework with all the stuff necessary to run a game. So let's restore the original state of TT_AttractMode():

    void TT_AttractMode() {                               // Attract Mode
	DispRow1 = DisplayUpper;
	DispRow2 = DisplayLower;
	WriteUpper("MY NEW GAME     ");
	LampPattern = LampColumns;                        // point to the standard lamp array
	//TurnOnLamp(53);
	//AddBlinkLamp(54, 250);
        Switch_Pressed = TT_AttractModeSW;
	//Switch_Pressed = TT_TutorialSW;
	//Switch_Released = DummyProcess;}
	digitalWrite(VolumePin,HIGH);                    // set volume to zero
	LampPattern = NoLamps;
	AppByte2 = 0;
	LampReturn = TT_AttractLampCycle;
	ActivateTimer(1000, 0, TT_AttractLampCycle);
	TT_AttractDisplayCycle(0);}

The first of the new commands is muting the sound by setting the VolumePin to High state. Then all lamps are turned off by settings the LampPattern pointer to NoLamps. Next is setting the LampReturn pointer to TT_AttractLampCycle which is controlling the lamp animations by using the ShowLampPatterns command during Attract Mode and then calling this routine with a timer. 

### 3.1 ShowLampPatterns
The ShowLampPatterns command provides an easy way to show complex lamp animations for playfield lamps. Hence, it ignores the first 8 lamps as they're usually located in the backbox. To use it create an array of LampPat structures. They consist of a 16bit duration value which determines how long a pattern is shown in ms and the corresponding lamp pattern array which uses bytes to define the status for every lamp. A simple example can be found at the beginning of Tutorial.ino. TT_AttractPat1 starts with lamp 9 and proceeds to the next after 250ms. Basically this does the same as the single lamp test in the test mode, but only for lamps starting from 9.

    // Duration..11111110...22222111...33322222...43333333...44444444...55555554...66666555
    // Duration..65432109...43210987...21098765...09876543...87654321...65432109...43210987
           250,0b00000001,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},

The example above will light lamp 9 for 250ms before moving on to the next row. The two lines above are to help you locate the right lamps, by showing the number of the lamp below with the upper row indication the decade of the lamp number. The last row of such an array is marked by the duration being zero.  

To use ShowLampPattern, the global variable PatPointer must point to the pattern array and FlowRepeat specifies the number of repetitions to be shown. A simple program to play various sequences is shown below

    void TT_AttractLampCycle(byte Event) {              // play multiple lamp pattern series
	static byte Phase;
	if (Event == 1) {                               // initial call?
	  Phase = 0;}                                   // reset cycle phase
	PatPointer = TT_AttractFlow[Phase].FlowPat;     // set the pointer to the current series
	FlowRepeat = TT_AttractFlow[Phase].Repeat;      // set the repetitions
	Phase++;                                        // increase counter
	if (!TT_AttractFlow[Phase].Repeat) {            // repetitions of next series = 0?
		Phase = 0;}                             // reset counter
	ShowLampPatterns(1);}                           // call the player

Here an additional array TT_AttractFlow ist used to store the pattern sequence to be played and the number of repetitions. By setting the LampReturn pointer to TT_AttractLampCycle it is ensured that it will be called again when ShowLampPatterns has completed the sequence. Then it has has to set the required pointers to the next sequence and run ShowLampPatterns again. In order to be able to stop the lamp animations, you can either add a stop functionality to TT_AttractLampCycle or you can call ShowLampPatterns(0) with the zero being the signal for it to stop.

### 3.2 Attract mode display cycling
Usually the basic stuff being displayed during attract mode is the name of the machine, the points of the last game (if there was any) and the highscores.

    void TT_AttractDisplayCycle(byte Step) {
	TT_CheckForLockedBalls(0);
	switch (Step) {
	case 0:
		WriteUpper2("THE APC TUTORIAL");
		ActivateTimer(50, 0, ScrollUpper);
		WriteLower2("                ");
		ActivateTimer(1000, 0, ScrollLower2);
		if (NoPlayers) {                             // if there were no games before skip the next step
			Step++;}
		else {
			Step = 2;}
		break;
	case 1:
		WriteUpper2("                ");             // erase display
		WriteLower2("                ");
		for (i=1; i<=NoPlayers; i++) {               // display the points of all active players
			ShowNumber(8*i-1, Points[i]);}
		ActivateTimer(50, 0, ScrollUpper);
		ActivateTimer(900, 0, ScrollLower2);
		Step++;
		break;
	case 2:	                                             // Show highscores
		WriteUpper2("1>              ");
		WriteLower2("2>              ");
		for (i=0; i<3; i++) {
			*(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[i]-32)*2];
			*(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[i]-32)*2+1];
			*(DisplayLower2+8+2*i) = DispPattern2[(HallOfFame.Initials[3+i]-32)*2];
			*(DisplayLower2+8+2*i+1) = DispPattern2[(HallOfFame.Initials[3+i]-32)*2+1];}
		ShowNumber(15, HallOfFame.Scores[0]);
		ShowNumber(31, HallOfFame.Scores[1]);
		ActivateTimer(50, 0, ScrollUpper);
		ActivateTimer(900, 0, ScrollLower2);
		Step++;
		break;
	case 3:
		WriteUpper2("3>              ");
		WriteLower2("4>              ");
		for (i=0; i<3; i++) {
			*(DisplayUpper2+8+2*i) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2];
			*(DisplayUpper2+8+2*i+1) = DispPattern1[(HallOfFame.Initials[6+i]-32)*2+1];
			*(DisplayLower2+8+2*i) = DispPattern2[(HallOfFame.Initials[9+i]-32)*2];
			*(DisplayLower2+8+2*i+1) = DispPattern2[(HallOfFame.Initials[9+i]-32)*2+1];}
		ShowNumber(15, HallOfFame.Scores[2]);
		ShowNumber(31, HallOfFame.Scores[3]);
		ActivateTimer(50, 0, ScrollUpper);
		ActivateTimer(900, 0, ScrollLower2);
		Step = 0;}
	ActivateTimer(4000, Step, TT_AttractDisplayCycle);}

In order to start the display cycling, the The PB_AttractDisplayCycle subroutine has to be called once from the attract mode. After that it can be regarded as an own and independent process which calls itself all 4s to update the displays.  
As the timer number is not stored this time, the only way to stop this process is to call KillAllTimers() which kills all timers and therefore stops all processes. Normally this is not the most elegant way of process handling, but as the attract mode has quite some processes running, it makes sense to stop everything and start from scratch when leaving it.

However, using KillAllTimers has it's pitfalls when dealing with processes keeping track of their timers by themselves. Most of these processes use static variables to store the number of the timer they're currently using. If the timer is killed without the process being notified the static variable will still hold the value of a timer which doesn't exist any more. If for some reason the process is now ordered to stop, it might try to kill it's timer. If you're lucky this will just lead to an Error 11, which means that you have tried to kill a timer which is not in use. But if the corresponding timer is now being used by another process, then it will be killed which can lead to all kinds of problems that are really hard to track. For this reason you should be aware of which process is using timers in which phase of the game and you should take an Error 11 very seriously when it occurs as it means that you have lost track of your timers. An Error 11 will freeze the game in debug mode, when the 'Debug Mode' setting is set to 'no' the error will just be shown in the displays for some seconds, but the game will continue.

### 3.3 Attract mode switch handler

Now you know how to produce some light and display animations in the attract mode, but how to leave it? For this we need a switch handler. In void TT_AttractMode() of this Tutorial we have set

    Switch_Pressed = TT_AttractModeSW;

which means that this routine is being called when a switch is activated. The corresponding target routine looks like this:

    void TT_AttractModeSW(byte Button) {              // Attract Mode switch behaviour
	switch (Button) {
	case 8:                                       // high score reset
		digitalWrite(Blanking, LOW);          // invoke the blanking
		break;
	case TT_OutholeSwitch:                        // outhole
		ActivateTimer(200, 0, TT_CheckForLockedBalls); // check again in 200ms
		break;
	case 72:                                      // Service Mode
		BlinkScore(0);                        // stop score blinking
		ShowLampPatterns(0);                  // stop lamp animations
    KillAllTimers();
		BallWatchdogTimer = 0;
		CheckReleaseTimer = 0;
    LampPattern = NoLamps;                            // Turn off all lamps
    ReleaseAllSolenoids();
    if (!QuerySwitch(73)) {														// Up/Down switch pressed?
      WriteUpper("  TEST  MODE    ");
      WriteLower("                ");
      AppByte = 0;
      ActivateTimer(1000, 0, TT_Testmode);}
    else {
      Settings_Enter();}
		break;
	case 3:                                       // start game
		TT_InitGame();}}

This program only reacts to four switches. Switch 8 is the High Score Reset switch (at least for Sys7 and higher) and it activates the blanking signal when the switch is being pressed, which will turn off the displays, lights and all solenoids.

This feature is for protecting the lamps and displays of your pinball machine while reprogramming the Arduino. Basically you can reprogram your System 'on the fly', but when the reset kicks in it can happen that the lamps and displays light up shortly. This wont damage them, but they wont like it either and therefore I recommend to press 'High Score Reset' before initiating the software upload. Then everything is deactivated and comes up again when the new software is uploaded.

The next switch is activated when a ball arrives in the outhole, which is then shoved into the ball through.

Switch 72 is the Advance Button which will initiate the test or settings mode depending on the position of switch 73 Up/Down. Note that BlinkScore and ShowLampPatterns are called before the KillAllTimers command is being executed. This is related to what I've written in the previous section. Also be sure to call ReleaseAllSolenoids after using KillAllTimers, because it will also kill the release timers for the solenoids. After that the state of switch 73 is queried and depending on the result the Test Mode or the Game Settings are being called.

The next switch (3) is the game start button. It will call the InitGame routine which will do all the steps required to initialize a game and put a ball into the plunger lane. There's some quite complex stuff happening here, but for the moment it's enough for you to know that when the game is running and a switch is being activated the TT_GameMain routine is called. As you can see I have already inserted our drop target handling from the previous sections to the switches 49, 50 and 51 (they're for the Rollergames and probably different for your machine). That means you should be able to launch a ball and shoot the drop targets.

I hope you got a rough overview about how the APC software interface works. If you're interested in using it, please post your questions to one of the forums mentioned on the front project page. I'm willing to expand this documentation when there is a need, but at the moment most people seem to be interested in using the APC with PinMame and MPF, so I focus on this for the time being.
