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
The amount of used LEDs can vary between 1 and 192. You must therefore specify this number in the 'No of LEDs' setting.

The normal TurnOnLamp/TurnOffLamp commands are also used to control LEDs. By default (LED Color Mode = 0) the brightness of an LED being turned on or off will gradually change in 5 steps with each step taking 20ms. Hence, the whole turn on/off takes 100ms which looks similar to a lightbulb.

The APC API features some special commands which work only with LEDs and not with normal matrix lamps. These commands are:

### LEDsetColor(byte Red, byte Green, byte Blue)

Sets the LED default color to the given red, green and blue values.  
How the LEDs are affected by this new set color depends on the currently selected LED color mode.

### LEDchangeColor(byte LED)

The color of the specified LED changes to the LEDsetColor. If the LED is already lit, it'll change the color immediately.  
If Color Mode 1 or 5 is selected then this command is the only way to change the color of an LED.

However, this method is not suited to change the color of several LEDs at once. Use LED color modes for this purpose.

### LEDsetColorMode(byte Mode)

This command selects a new Color Mode.

The LED_exp board features five color modes which can be selected by this command:

| Mode number | How it works |
|--|--|
| 0 | LEDs being lit get the LEDsetColor. LEDs which are already lit are not affected. LEDs are smoothly turned on and off like a bulb |
| 1 | LEDs being lit keep their color. If you want to change it use LEDchangeColor. LEDs are smoothly turned on and off like a bulb |
| 2 | LEDs being set in the pattern change their color to the LEDsetColor and are turned on|
| 3 | LEDs being set in the pattern change their color to the LEDsetColor, but are not turned on|
| 4 | Same as mode 0, but the LEDs turn on and off instantly|
| 5 | Same as mode 1, but the LEDs turn on and off instantly|

The default mode is 0.

### LEDshowPatterns(byte State)

This command shows a sequence of LED effects including color changes.  
In order to use this, you need to define an array of bytes of the following format:

* Length of each pattern (in bytes)

After that you need to specify the following for each pattern you want to show:

* Duration of the pattern to be shows (in 10th of ms)
* Color of the LEDs being set in the following pattern (3 bytes -> Red, Green, Blue)
* As many LED pattern bytes as stated in the first topic of this array. Every bit defines the state of one LED.

You can define up to 255 patterns like this.  
The definition ends with the duration being set to zero.

The following example is taken from the PinMameExceptions.ino of the AmokPrivate branch and makes the 32 GI-LEDs of my Comet do an animation.

                                      //LED number..00000000....11111110....22222111....33322222
                                //Duration Color....87654321....65432109....43210987....21098765
    const byte GI_Pattern[74] = {4, 1, 255, 0, 0, 0b01001001, 0b10010010, 0b00100100, 0b01001001,
                                    1, 0, 255, 0, 0b10010010, 0b00100100, 0b01001001, 0b10010010,
                                   20, 0, 0, 255, 0b00100100, 0b01001001, 0b10010010, 0b00100100,
                                    1, 0, 0, 255, 0b01001001, 0b10010010, 0b00100100, 0b01001001,
                                    1, 255, 0, 0, 0b10010010, 0b00100100, 0b01001001, 0b10010010,
                                   20, 0, 255, 0, 0b00100100, 0b01001001, 0b10010010, 0b00100100,
                                     1,0, 255, 0, 0b01001001, 0b10010010, 0b00100100, 0b01001001,
                                    1, 0, 0, 255, 0b10010010, 0b00100100, 0b01001001, 0b10010010,
                                   20, 255, 0, 0, 0b00100100, 0b01001001, 0b10010010, 0b00100100, 0};

The first byte determines that every LED pattern consists of 4 bytes which makes 32 LEDs.  
The next byte sets the duration for the following pattern to 1 which means 10ms.  
Now come 3 bytes for the color of the following pattern. In this case it's 255,0,0 which is pure red.  
Next are the 4 bytes of the pattern. The first LED is represented by the LSB of the first byte and so on. The commented lines above the array show the way.

The first pattern switches the color to red and lights every third LED. The next switches to green and also turns on every third LED but shifted by one position. The third pattern lights the remaining LEDs in blue. This all happens as fast as possible (Duration = 1). What we get is red, green and blue LEDs alternating.  
The next duration is set to 20 which means the current state is shown for 200ms.

The next 3 patterns do the same as before, but everything shifted by one LED which is also shown for 200ms. The last 3 patterns are also shifted by one position.  
The final zero signals the end of the animation.  
There's a pointer variable LEDreturn. Point it to a subroutine you want to be executed (with an argument of 0) when the animation has finished, otherwise set it to zero. 

To start this animation the variable LEDpointer needs to be set to the array and LEDshowPatterns(1) needs to be called. 

        LEDreturn = EX_AttractLEDeffects2;
        LEDpointer = GI_Pattern;
        LEDshowPatterns(1);

As this is supposed to be a permanent animation, the LEDreturn pointer is set to some code which restarts it several times.

### The LEDpattern pointer

LEDpattern needs to point to the LED pattern to be shown. Normally this pointer is set to LEDstatus which is what TurnOnLamp/TurnOffLamp are working with. The LEDshowPatterns command is using this pointer to show the animations and you have to set it back to LEDstatus after you're done with your animation and you want to use TurnOnLamp/TurnOffLamp to control your LEDs again. The advantage is that the state of your LEDs is not affected by the 
LEDshowPatterns command, you can even use TurnOnLamp/TurnOffLamp while an animation is running.  
However, it's not enough to set the pointer, but you have to tell the LED handler to re-process your LEDs every time you've changed LEDpattern and want to make the change become effective. The command for that is

    LEDhandling(9, X);

With X being the length in bytes of the pattern you want to be shown.  
For example, the Comet code in AmokPrivate uses

        LEDpattern = PME_GIon;                        // set GI pattern
        LEDhandling(9, 4);                            // apply changes
        
To turn on the lamps that are meant to be lit as GI. In this case PME_GIon just consists of 4 bytes defining the states of the LEDs (like in the array used by LEDshowPatterns).

### Own LED commands

For complex or repetitive animations it is also possible to add individual LED commands to the SW of the Exp_board and let it handle them by itself.

At the end of APC_LED_exp.ino there's a switch statement which handles all commands of the LED_ExpBoard.

      else {                                          // different command
        switch (RecByte) {
    
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
As stated above the LEDs are updated once in every refresh cycle. Therefore it makes sense to run our LED player also just once per refresh cycle. I have marked the right position in the code with a corresponding comment which can be found in the first line of my example player. It is part of the sync command which runs every 20ms.

      if (OwnCommands) {                          // a good place to let an own command run once per refresh cycle
        if (OwnCommands & 1) {                    // check which command is meant
          if (!(OwnCommandStep % 5)) {            // only be active every 5th refresh cycle
            byte Step = OwnCommandStep / 5;       // calculate the current step
            for (byte i=0;i<6;i++) {              // pattern has 6 fading grades
              if (Step+i < 12) {                  // it's for 12 LEDs
                pixels.setPixelColor(Step+i,OwnPattern[i][0],OwnPattern[i][1],OwnPattern[i][2]);}
              else {
                pixels.setPixelColor(Step+i-12,OwnPattern[i][0],OwnPattern[i][1],OwnPattern[i][2]);}}}
          OwnCommandStep++;
          if (OwnCommandStep > 59) {
            OwnCommandStep = 0;}}}

The first whether OwnCommands is different from zero. That means if any of the bits in OwnCommands is set then the following code is executed.
At first the player checks the LSB of OwnCommands and proceeds if it's set. In order not to play the pattern too fast the player checks next if OwnCommandStep can be divided by 5 without rest and skips the other 4 cycles. The desired effect is some kind of green radar animation for an LED ring with 12 LEDs. Basically it's always the same pattern going round and round, a bright spot which fades in 6 steps.

    const byte OwnPattern[6][3] = {{0,0,0},{0,50,0},{0,100,0},{0,150,0},{0,200,0},{0,250,0}};

That means for the player that it has to write always the same LED data, but the start LED changes from cycle to cycle. Therefore OwnCommandStep divided by 5 is also the internal counter of this command which determines the actual LED to start from. The pixels.set command writes the color values in the corresponding LEDs. This data is then sent to the LEDs on the next Sync.  
At the end OwnCommandStep is increased by one and after 60 cycles it starts all over again.

Now that we have added the command to the LED_ExpBoard SW, we still have to issue it from the program running on the APC board itself. This can be done with the LEDhandling command. For our single byte command this would be

    LEDhandling(6, 100);                              // write 100 to the command buffer
    LEDhandling(7, 0);                                // send the buffer to the LED_ExpBoard
    
The first line adds our command (number 100) to the command buffer and the second line sends 1 byte of this buffer. If your command has more than one byte they all need to be send with LEDhandling(6,byte) and executed with LEDhandling(7, 0).
Let's use LEDsetColor as an example. This command has 4 bytes (command and 3 color values) which leads to

    void LEDsetColor(byte Red, byte Green, byte Blue) {   // set a new color
      LEDhandling(6, 192);
      LEDhandling(6, Red);
      LEDhandling(6, Green);
      LEDhandling(6, Blue);
      LEDhandling(7, 0);}
  
