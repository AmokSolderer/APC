# APC LED expansion board

The purpose of this board is to control a WS2812 based RGB LED strip with the APC.

## Hardware

The hardware is quite simple and mainly consists of an Arduino Mini and a bit of logic to connect it to the HW extension interface of the APC.

![Pic_Led_Exp](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/LED_ExpBoard.jpg)

The schematic, Gerber (and drilling) as well as an SVG file are located in the [APC_LED_exp](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_LED_exp) folder.

## Software

The SW is located in the same folder as the HW.

## Examples

### GI LEDs

One relatively simple change which has a huge effect is to replace the light bulbs of the GI with LEDs. The following video shows how this looks in [my Comet](https://youtu.be/kLWVUdhSwfo).

![APC Comet LED](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/CometLED.jpg)

The corresponding code can be found in the AmokPrivate branch of this repository.

## Usage

Depending on the 'LED lamps' setting the LED strip is used as a replacement for the controlled lamps (1-64) or the LEDs are added to the normal lamps which means the first LED corresponds to lamp number 65. Hence, if you choose 'playfield' in the 'LED lamps' setting in the 'System Settings' menu of the APC, the normal controlled lamps will be switched off and the LED strip will be used instead. The first LED of the strip will then work as lamp 1 and so on. If you choose 'additional' the normal lamp matrix will stay active and the LEDs will get the numbers 64+X.  
In the 'additional' LED mode the amount of used LEDs can vary between 1 and 192. You must therefore specify this number in the 'No of LEDs' setting.

The more LEDs you connect, the longer it takes to update them all. The minimum refresh cycle time for one LED would be 8ms and for 192 LEDs it's 38ms. For the default number of 64 LEDs the refresh cycle time is 20ms.

The normal TurnOnLamp/TurnOffLamp commands are also used to control LEDs. When the status of an LED changes, it's brightness will change gradually in 5 steps with each step taking one cycle time. For the default number of LEDs (64) the whole turn on/off takes 100ms which looks similar to a lightbulb.

The APC API features some special commands which work only with LEDs and not with normal matrix lamps.  
Note that the LED_exp board can only handle one command per refresh cycle (normally 20ms) and the command buffer can just store 8 commands, so don't send too many command at a time.

### LEDsetColor(byte Red, byte Green, byte Blue)

Sets the LED default color to the given red, green and blue values.  
How the LEDs are affected by this new set color depends on the currently selected LED color mode.

### LEDchangeColor(byte LED)

The color of the specified LED changes to the LEDsetColor. If the LED is already lit, it'll change the color immediately.  
If mode 1 is selected then this command is the only way to change the color of an LED.

As only one LED command can be sent per refresh cycle, this method is not suited to change the color of several LEDs at once. Use LED color modes for this purpose.

### LEDsetColorMode(byte Mode)

This command selects a new Color Mode.

The LED_exp board features five color modes which can be selected by this command:

| Mode number | How it works |
|--|--|
| 0 | LEDs being lit get the LEDsetColor. LEDs which are already lit are not affected |
| 1 | LEDs being lit keep their color. If you want to change it use LEDchangeColor |
| 2 | LEDs being set in the pattern change their color to the LEDsetColor and are turned on|
| 3 | LEDs being set in the pattern change their color to the LEDsetColor, but are not turned on|
| 4 | LEDs are frozen, the pattern has no effect|

The default mode is 0.

In most cases the modes 0 and 1, TurnOnLamp/TurnOffLamp and LEDchangeColor should be sufficient to control your LEDs. However, for certain effects or for changing the color of several LEDs at once, some additional modes might come in handy.  
To be able to use these modes you'll need some basic understanding of how the communication between the APC and the LED_Exp board works.  
The APC is permanently transmitting LED patterns to the Exp_board. The length of these patterns depends on the number of LEDs set in the System Settings and the LEDpattern pointer determines which pattern is used. The default pattern is LEDstatus which is an internal array of the LEDhandling routine.  
In all of these patterns every LED is represented by one bit and the LED Color Mode determines how to handle those.

For the color modes 0 and 1 every bit in the pattern determines which LED is lit and which is not. Normally the internal LEDstatus array is used for this. This is why LEDpattern is pointing to LEDstatus after the LED handling is initialized by LEDinit() and it's also the reason for TurnOnLamp/TurnOffLamp to set/clear the corresponding bits in this pattern.  
The color modes 2 and 3 are different, as a set bit means that the color of the corresponding LED is changed to the LEDsetColor while a zero means that the LED is not affected at all. Hence, you can not turn off an LED in one of these modes.  
Mode 2 will additionally turn on the selected LEDs while mode 3 will just change their color.  
Mode 4 freezes the status of the LEDs.

LED patterns are only changed at the end of the refresh cycle when the LED commands are being to the LED_Exp board as well. That means they're synchronized and you can change the LEDsetColor and the pattern at the same time without the risk of the new color affecting the LEDs selected in the previous pattern.  
Let's assume you have 32 LEDs in your system and you want to change every third LED to a new color. A quick way to do this is to switch to LEDsetColorMode 2 or 3 and then do 

                   //LED number..00000000....11111110....22222111....33322222
                     // Color....12345678....65432109....43210987....21098765
    const byte ColorPattern = {0b01001001, 0b10010010, 0b00100100, 0b01001001};
    
    LEDpattern = ColorPattern;
    LEDsetColor(Red, Green, Blue);
    
Note that the TurnOnLamp and TurnOffLamp commands have no effect as long as LEDpattern does not point to LEDstatus.

The ShowLEDpatterns command is another example of how color effects can be done. It's similar to the ShowLampPatterns command and changes the color of LEDs according to two arrays. Here's a simple example for 64 LEDs:

                          //LED number..00000000...11111110...22222111...33322222...43333333...44444444...55555554...66666555
                            // Color....12345678...65432109...43210987...21098765...09876543...87654321...65432109...43210987
    const byte LEDflow[88] = {255,0,0,0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
                              200,50,0,0b00000001,0b00000000,0b00000001,0b00000000,0b00000001,0b00000001,0b00000000,0b00000000,
                              150,100,0,0b00000010,0b00000000,0b00000010,0b00000000,0b00000010,0b00000010,0b00000000,0b00000000,
                              100,150,0,0b00000100,0b00000000,0b00000100,0b00000000,0b00000100,0b00000100,0b00000000,0b00000000,
                              50,200,0,0b00001000,0b00000000,0b00001000,0b00000000,0b00001000,0b00001000,0b00000000,0b00000000,
                              0,250,50,0b00010000,0b00000000,0b00010000,0b00000000,0b00010000,0b00010000,0b00000000,0b00000000,
                              0,250,100,0b00100000,0b00000000,0b00100000,0b00000000,0b00100000,0b00100000,0b00000000,0b00000000,
                              0,250,150,0b01000000,0b00000000,0b01000000,0b00000000,0b01000000,0b01000000,0b00000000,0b00000000};
    const uint16_t LEDdur[9] = {1000,1000,1000,1000,1000,1000,1000,1000,0};

The first three bytes of the LEDflow array determine the color of the LEDs selected in the following pattern. The LEDdur specifies how long each row of LEDflow is shown before the next row is processed.  
A zero in LEDdur stops ShowLEDpatterns. If LEDreturn is different from zero then the routine it's pointing to is called.  
Depending on the active Color Mode the effect of this command would be quite different, so be sure to set the correct mode beforehand. Also set LEDpointer to point to your LED pattern and LEDpatDuration to your duration array.  
A typical call for this command would look like this:

    LEDsetColorMode(2);                               // select correct color mode
    LEDpointer = LEDflow;                             // set pointer to the pattern array
    LEDpatDuration = LEDdur;                          // set pointer to the duration array
    LEDreturn = 0;                                    // do not call a routine when the pattern runs out
    ShowLEDpatterns(1);                               // use 1 as an argument to start and 0 to stop it

### Own LED commands

For complex or repetitive animations it is also possible to add individual LED commands to the SW of the Exp_board and let it handle them by itself.

At the end of APC_LED_exp.ino there's a switch statement which handles all commands of the LED_ExpBoard.

    switch (RecByte) {                              // treat it as a command
    
To add a new command, just look for an unused command number and create a case for it.

I've already added command 100 as an example. It consists of a simple player which repeatedly plays a predefined pattern on the first 12 LEDs.  
Let's see how it works:

    case 100:                                       // execute OwnCommand
      OwnCommands |= 1;                             // activate OwnCommand number 1
      break;
      
When command 100 is received by the LED_ExpBoard, all it does is to set the LSB in the OwnCommands byte which is a switch for the player to work. It will work as long as command 101 is received.

    case 101:
      OwnCommands &= 254;                           // deactivate OwnCommand number 1
      for (byte i=0;i<12;i++) {                     // for all affected LEDs
        pixels.setPixelColor(i, pixels.Color(0,0,0)); // turn them off
        LampStatus[i / 8] &= 255-(1<<(i % 8));}     // and change the status to off
      break;
      
This command clears the LSB in OwnCommands to signal the player to stop. Furthermore it switches off all LEDs and also changes their status in LampStatus to zero.

Until now we just have a bit in OwnCommands which determines whether our command is active or not. The next step is to add a player which reads this bit and plays some LED animation when it is set.  
As stated above the LEDs are updated once in every refresh cycle. Therefore it makes sense to run our LED player also just once per refresh cycle. I have marked the right position in the code with a corresponding comment which can be found in the first line of my example player.

    if (!Sync && OwnCommands) {                       // a good place to let an own command run once per refresh cycle
      if (OwnCommands & 1) {                          // check which command is meant
        if (!(OwnCommandStep % 5)) {                  // only be active every 5th refresh cycle
          byte Step = OwnCommandStep / 5;             // calculate the current step
          for (byte i=0;i<6;i++) {                    // pattern has 6 fading grades
            if (Step+i < 12) {                        // it's for 12 LEDs
              pixels.setPixelColor(Step+i,OwnPattern[i][0],OwnPattern[i][1],OwnPattern[i][2]);}
            else {
              pixels.setPixelColor(Step+i-12,OwnPattern[i][0],OwnPattern[i][1],OwnPattern[i][2]);}}}
        OwnCommandStep++;
        if (OwnCommandStep > 59) {
          OwnCommandStep = 0;}}}
          
The first line waits for the Sync counter becoming zero and OwnCommands being different from zero. That means if any of the bits in OwnCommands is set then the following code is executed after each Sync which is once per refresh cycle.  
At first the player checks the LSB of OwnCommands and proceeds if it's set. In order not to play the pattern too fast the player checks next if OwnCommandStep can be divided by 5 without rest and skips the other 4 cycles. The desired effect is some kind of green radar animation for an LED ring with 12 LEDs. Basically it's always the same pattern going round and round, a bright spot which fades in 6 steps.

    const byte OwnPattern[6][3] = {{0,0,0},{0,50,0},{0,100,0},{0,150,0},{0,200,0},{0,250,0}};

That means for the player that it has to write always the same LED data, but the start LED changes from cycle to cycle. Therefore OwnCommandStep divided by 5 is also the internal counter of this command which determines the actual LED to start from. The pixels.set command writes the color values in the corresponding LEDs. This data is then sent to the LEDs on the next Sync.  
At the end OwnCommandStep is increased by one and after 60 cycles it starts all over again.

Now that we have added the command to the LED_ExpBoard SW, we still have to issue it from the program running on the APC board itself. This can be done with the LEDhandling command. For our single byte command this would be

    LEDhandling(6, 100);                              // write 100 to the command buffer
    LEDhandling(7, 1);                                // send 1 byte to the LED_ExpBoard
    
The first line adds our command (number 100) to the command buffer and the second line sends 1 byte of this buffer. If your command has more than one byte they all need to be send with LEDhandling(6,byte) and executed with LEDhandling(7, number of bytes).  
Let's use LEDsetColor as an example. This command has 4 bytes (command and 3 color values) which leads to

    void LEDsetColor(byte Red, byte Green, byte Blue) {   // set a new color
      LEDhandling(6, 192);
      LEDhandling(6, Red);
      LEDhandling(6, Green);
      LEDhandling(6, Blue);
      LEDhandling(7, 4);}
  
