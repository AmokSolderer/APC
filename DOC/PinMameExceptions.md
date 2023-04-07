# PinMameExceptions

PinMameExceptions are a simple but powerful tool to change the behaviour of your game even though it is controlled by PinMame. Some exceptions are inevitable to make the APC generate the sound for your machine correctly. A description for this is on the [PinMame howto](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame_howto.md) page.

In this section we'll focus on exceptions that change a game. This can be by just fixing flaws in the original game SW or adding new features.

## Doing exceptions for the magna save of the Jungle Lord

Jungle Lord features timed magna saves which means that the magnets are just activated for as long as the magna save buttons are pressed.
If we let PinMame handle this, then the message of the pressed button is forwarded from the APC to PinMame who will calculate an answer and in return send a command to turn on the magna save coil. Even though the reaction times are quite short it still feels a little sluggish sometimes and we want the APC to handle this directly.

As the whole process is triggered by the magna save button, the SwitchActCommand case is the right place to add our code:

    case SwitchActCommand:                              // activated switches
      if (Command == 49) {                              // right magnet button
        if (QueryLamp(8) && QueryLamp(2)) {             // right magnet and ball in play lamp lit?
          ActivateSolenoid(0, 22);}}                    // activate right magnet
      else if (Command == 50) {                         // left magnet button
        if (QueryLamp(39) && QueryLamp(2)) {            // left magnet and ball in play lamp lit?
          ActivateSolenoid(0, 21);}}                    // activate leftmagnet
      return(0);                                        // all switches are reported to PinMame

If the right magna save button is pressed, we check whether lamp 8 is lit which means that the player has activated the magna save feature. We also check for the 'Ball in Play' lamp 2 to prevent the magnet from being activated without a game running. If these conditions are met, the magna save coil 22 is activated.
The left magna save is treated accordingly.

The return value is important for all exceptions which do not affect the sound, because it determines whether the main program additionally tries to process this command afterwards (return(0)) or whether it just omitts any further processing. In our case returning a 1 would mean that we wouldn't tell PinMame about the magnet button being activated which would make no sense as PinMame has to count the seconds the magna save is on and turn off the magna save lamps if the player has none left.

For timed magna saves we have to do the same for the magna save buttons being released, we just don't have to check for any lit lamps:

    case SwitchRelCommand:                              // deactivated switches
      if (Command == 49){                               // right magnet button
        ReleaseSolenoid(22);}                           // turn off right magnet
      else if (Command == 50) {                         // left magnet button
        ReleaseSolenoid(21);}                           // turn off left magnet
      return(0);                                        // all switches are reported to PinMame

## Improving the System7 ball release

The previous examples were just to improve the emulation of a machine, which is necessary to make your game work correct with PinMame. However, you are not limited to this, but you can also use this to apply changes to a game.

As an example I want to fix one nasty problem that affected both of my System7 machines.
It didn't happen often but regularly that a ball ejected into the plunger lane bounced back from the side rail and into the trunk again. If it got back into the trunk completely the SW would recognize this and eject the ball again. But sometimes the ball didn't really get back into the trunk. Instead it got stuck above the shooter lane feeder and there wasn't much I could do about it except of opening the coin door and operating the feeder manually to push the ball into the shooter lane. I really wonder how these games survived in the pubs with a bug like this.

With the APC we can fix this.
There is a switch in the shooter lane, so all we have to do is start a timer when the solenoid of the shooter lane feeder is activated. If the shooter lane switch is triggered within 3 seconds, we know the ball has been ejected successfully and we can just kill the timer. But if the timer runs out, we know the ball is probably stuck and we just operate the feeder again.

How does this look in code? First we need a handling routine for our timer. As we're going to need this for most if not all System7 machines, I've generated a generic timer handler for our problem that can be used by all machines:

    void EX_BallRelease(byte State) {                     // repeat ball eject in case the ball got stuck
      static byte Timer;                                  // stores the timer number
      switch (State) {                                    // determines what to do
      case 0:                                             // kill the timer
        if (Timer) {                                      // if a timer is active
          KillTimer(Timer);                               // kill it
          Timer = 0;}                                     // and set Timer = 0 to indicate that no timer is active
        break;
      case 1:                                             // start a timer
        if (!Timer) {                                     // if no timer is active
          Timer = ActivateTimer(3000, 2, EX_BallRelease);}  // start one for 3s with 2 as the argument
        break;
      case 2:                                             // timer has run out
        ActivateSolenoid(40, EX_EjectSolenoid);           // activate the shooter lane feeder again
        Timer = ActivateTimer(3000, 2, EX_BallRelease);}} // and restart the timer

This routine shall be called with a State argument which determines what it has to do.
If State is 0 then the timer is not needed any more and must be deactivated. In our case this would happen if the ball has triggered the shooter lane switch.
State = 1 means that a timer shall be started. The number of this timer is being stored in the Timer variable and the Timer is configured to call our handler again after 3s. When it does so it'll pass a 2 as an argument which will indicate that our handling routine has been called by the timer.
Hence, our handler knows that the timer has run out when it's called with State = 2 which means that the ball is probably stuck. So it activates the shooter lane feeder again and restarts the timer in case the ball gets stuck again.
For more information about timers read the [APC SW reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) and the [Basic Game Functions](https://github.com/AmokSolderer/APC/blob/master/DOC/GameCodeTutorial.md#2-basic-game-functions) tutorial.

With this timer handler in place we just have to tell our exception handler to use it. First of all it needs to know which coil is for the shooter lane feeder. In case of the Jungle Lord this is solenoid number 2 which we already defined before in EX_Init by setting EX_EjectSolenoid = 2.
The rest is easy. We just have to create an exception for EX_EjectSolenoid being activated

    case SolenoidActCommand:                            // activated solenoids
      if (Command == EX_EjectSolenoid){                 // ball eject coil
        if (QueryLamp(2)) {                             // ball in play lamp lit?
          EX_BallRelease(1);}}                          // start ball release timer
      return(0);                                        // solenoid will be activated

If the shooter lane feeder is activated, we first check whether a game is running by probing the 'Ball in Play' lamp 2. This is because we don't want our fix to be accidentially triggered in test mode. If the conditions are met we call our timer handler EX_BallRelease with an argument of 1, as this will start a timer. Note that we return a value of 0 as we don't want to block the activation of this solenoid.
Last but not least we have to switch off the timer when the shooter lane switch (43) is triggered. This is done by calling EX_BallRelease with an argument of 0.

    case SwitchActCommand:                              // activated switches
      if (Command == 43) {                              // ball successfully ejected
        EX_BallRelease(0);}                             // stop ball release timer

These were just a few simple examples of what you could do with exception handling, but with this you could even do significant changes of the rules without having to program the whole game by yourself.

Refer to PinMameExceptions.ino to see all the above mentioned changes in place.
For more information about using the APC API read the [APC SW reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) and the [Basic Game Functions](https://github.com/AmokSolderer/APC/blob/master/DOC/GameCodeTutorial.md#2-basic-game-functions) tutorial.
Watch my [Jungle Lord video](https://www.youtube.com/watch?v=bbfhH_-gMfE) so see an example. The corresponding code can be found in PinMameExceptions.ino in the AmokPrivate branch on Github.

## How to add a ball saver

I have a Comet which made me really angry when it drowned my balls in the outlanes before I even had a chance to touch them with a flipper. My first countermeasure was to remove the plumb bob tilt. By this I could nudge the ball out of the left outlane back into play (this is due to the special left outlane of the Comet). But as I had an APC board installed I could do it in a more subtle way eventually. My [Comet video](https://youtu.be/JbgMa_pn0Lo) makes more clear what I mean.

The code for this ball saver is quite simple. These are the complete PinMameExceptions of my Comet:

    byte EX_Comet(byte Type, byte Command) {
      static byte LastSwitch;                             // stores the number of the last activated switch
      static byte BlindPinmame;                           // hide switches from PinMame while active
      static byte Timer = 0;
      switch(Type) {
      case SoundCommandCh1:                               // sound commands for channel 1
        if (!Command || Command > 253) {                  // sound command 0x00 and 0xff -> stop sound
          AfterMusic = 0;
          StopPlayingMusic();
          StopPlayingSound();}
        else if (Command == 11) { }                       // ignore sound command 0x0b
        else if (Command == 47) {                         // play BG music
          PlayMusic(50, "0_2f.snd");
          QueueNextMusic("0_2f.snd");}                    // track is looping so queue it also
        else {                                            // handle standard sound
          if (Command == 9) {
            MusicVolume = 4;}                             // reduce music volume
          if (Command == 241) {
            RestoreMusicVolumeAfterSound(25);}            // and restore it
          char FileName[9] = "0_00.snd";
          if (USB_GenerateFilename(1, Command, FileName)) { // create filename and check whether file is present
            PlaySound(51, (char*) FileName);}}
        return(0);                                        // no exception rule found for this type so proceed as normal
      case SwitchActCommand:                              // activated switches
        if (BlindPinmame) {                               // hide switches from PinMame
          if (Command == 31 || Command == 32 || Command == 44) {
            return(1);}                                   // hide these switches from PinMame
          if (Command == 45) {                            // outhole switch?
            if (Timer) {                                  // timer still running?
              KillTimer(Timer);                           // stop it
              Timer = 0;}
            ActivateSolenoid(40, 1);                      // kick ball into plunger lane
            RemoveBlinkLamp(60);                          // stop blinking the extra ball lamps
            RemoveBlinkLamp(5);
            if (BlindPinmame > 1) {                       // extra ball lamps were on before?
              TurnOnLamp(60);
              TurnOnLamp(5);}
            BlindPinmame = 0;                             // and don't fool PinMame any longer
            return(1);}}                                  // hide this switch from PinMame
        else {                                            // normal mode
          if ((Command == 44 && (game_settings[USB_Option1] & 2)) || (Command == 31 && (game_settings[USB_Option1] & 1))) { // Ball Saver for outlanes active?
            if ((LastSwitch > 32) && (LastSwitch < 43)) { // ball dropped from the bumpers directly into the outlane?
              if (QueryLamp(60)) {                        // extra ball lamp lit?
                BlindPinmame = 2;}
              else {                                      // extra ball lamp not lit
                BlindPinmame = 1;}
              AddBlinkLamp(60, 150);                      // blink extra ball lamps
              AddBlinkLamp(5, 150);
              Timer = ActivateTimer(5000, 1, EX_Comet2);  // wait 5s for the ball to reach the outhole
              return(1);}}                                // hide this switch from PinMame
          else {                                          // switches are reported normally
            LastSwitch = Command;}}
        return(0);
      case 50:                                            // timer of ball saver has run out
        Timer = 0;
        if (BlindPinmame) {                               // ball saver still active?
          RemoveBlinkLamp(60);                            // stop blinking the extra ball lamps
          RemoveBlinkLamp(5);
          if (BlindPinmame > 1) {                         // extra ball lamps were on before?
            TurnOnLamp(60);
            TurnOnLamp(5);}
          BlindPinmame = 0;}                              // and don't fool PinMame any longer
        return(0);
      default:
        return(0);}}                                      // switch will also be reported to PinMame.

    void EX_Comet2(byte Dummy) {                          // to be called by timer from EX_Comet
      UNUSED(Dummy);
      EX_Comet(50, 0);}

The first SoundCommandCh1 exceptions are necessary for the sound of the Comet to work properly and have nothing to do with ball saver which starts with the SwitchActCommand exception.

Normally the BlindPinmame variable is zero which means the ball saver is passive and all switches are reported to PinMame. All the exception does in this 'normal mode' is to store the number of each activated switch in the LastSwitch variable.
If switch 31 or 44 (outlanes) is triggered and the corresponding ball saver is activated in the settings, the LastSwitch variable is checked. If the last switch belongs to the bumper area (switches 32 - 43) the ball saver becomes active by blinking the extra ball lamps and changing the value of BlindPinmame to 1 or 2 depending on whether the extra ball lamp was on or off before. A return(1) means that the activation of the outlane switch is not reported to PinMame.
This is also valid for any subsequent activation of the outlane switches (31, 32, 44) as long as BlindPinmame is different from zero.
If Switch 45 is triggered (outhole) then solenoid 1 is activated (shooter lane feeder), the state of the extra ball lamps is restored and BlindPinmame is cleared to deactivate the ball saver.
This is basically enough, the old game SW might wonder why the ball suddenly pops up in the shooter lane, but who cares as long as the gameplay resumes normally.

However, due to the special left outlane design of the Comet, it is possible to bounce the ball from the outlane back into play. For our exception this would mean that BlindPinmame would be set when the outlane switch is triggered and would stay active until the ball finally reaches the outhole. As the ball has been bouncing back it didn't reach the outhole when it was meant to which means that the ball saver stays active until the next regular ball drain happens.
To prevent this from happening a timer is started when the ball saver is activated and if the ball doesn't reach the outhole within 5 seconds then it deactivates itself.

You can find this code in the PinMameExceptions of the AmokPrivate branch in GitHub.
