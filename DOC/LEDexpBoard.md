# APC LED expansion board

The purpose of this board is to control a WS2812 based RGB LED strip with the APC.

## Hardware

The hardware is quite simple and mainly consists of an Arduino Mini and a bit of logic to connect it to the HW extension interface of the APC.

![Pic_Led_Exp](https://github.com/AmokSolderer/APC/blob/V00.23/DOC/PICS/LED_ExpBoard.jpg)

The schematic, Gerber (and drilling) as well as an SVG file are located in the [APC_LED_exp](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_LED_exp) folder.

## Software

The SW is located in the same folder as the HW.

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
Mode 4 freezes the status of the LEDs. This is useful for the time needed to change the pattern (refresh cycle) to avoid that LEDs from the wrong pattern are affected accidentally.

If you just want to change the color of all lit LEDs then you can simply change the color mode to 2 or 3 and back, but if additional LEDs are lit that must not be affected then it get's a bit more tricky.  
The following code shows a handler that uses color mode 3 to change the color according to LEDcolorPattern. In this case the pattern selects the LEDs 1-8 (65-72 for 'additional' LEDs) in a system with 64 LEDs to the color previously determined by LEDsetColor:

    byte LEDcolorPattern[8] = {0b11111111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000};

    void TT_SelectBulkLEDcolor(byte State) {              // Change the color of several LEDs without turning them on
      switch (State) {
      case 1:                                             // step 1
        LEDsetColorMode(4);                               // freeze LED states
        ActivateTimer(20, 2, TT_SelectBulkLEDcolor);      // wait 20ms for the refresh cycle to end
        break;
      case 2:                                             // step 2
        LEDsetColorMode(3);                               // LEDs will change their color without being turned on
        LEDpattern = LEDcolorPattern;                     // apply pattern to specify which LEDs are affected
        ActivateTimer(20, 3, TT_SelectBulkLEDcolor);      // wait 20ms for the refresh cycle to end
        break;
      case 3:                                             // step 3
        LEDsetColorMode(4);                               // freeze LED states
        ActivateTimer(20, 4, TT_SelectBulkLEDcolor);      // wait 20ms for the refresh cycle to end
        break;
      case 4:
        LEDsetColorMode(1);                               // LEDs keep their color
        LEDinit();                                        // switch back to normal lamp pattern
        break;}}

To be sure that both of your patterns don't interfere with each other you have to switch to color mode 4 and wait for one refresh cycle before changing the pattern. After LEDcolorPattern is selected you also need to wait for one cycle for the whole pattern to be transferred to the LED_Exp board. Then you also need to freeze the LEDs for one cycle before you can switch the pattern back to default.

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

For complex or repetitive animations it is also possible to add individual LED commands to the SW of the Exp_board and let the it handle them by itself.
