# APC LED expansion board

The purpose of this board is to control a WS2812 based RGB LED strip with the APC.

Depending on the 'LED lamps' setting the LED strip is used as a replacement for the controlled lamps (1-64) or the LEDs are added to the normal lamps which means the first LED corresponds to lamp number 65. Hence, if you choose 'playfield' in the 'LED lamps' setting in the 'System Settings' menu of the APC, the normal controlled lamps will be switched off and the LED strip will be used instead. The first LED of the strip will then work as lamp 1 and so on. If you choose 'additional' the normal lamp matrix will stay active and the LEDs will get the numbers 64+X.

The normal TurnOnLamp/TurnOffLamp commands are also used to control LEDs. When the status of an LED changes it's brightness will change gradually in 5 steps over 100ms which is similar to a lightbulb.

The APC API features some special commands which work only with LEDs and not with normal matrix lamps.  
Note that the LED_exp board can only handle one command per lamp cycle (normally 20ms) and the command buffer can just store 8 commands, so don't send too many command at a time.

### LEDsetColor(byte Red, byte Green, byte Blue)

Sets the LED default color to the given red, green and blue values.  
How the LEDs are affected by this new set color depends on the currently selected LED color mode.

### LEDsetColorMode(byte Mode)

Up to now the LED_exp board features three color modes which can be selected by this command:

| Mode number | How it works |
|--|--|
| 0 | LEDs being turned on get the LEDsetColor. LEDs which are already lit are not affected |
| 1 | LEDs keep their color. If you want to change it use LEDchangeColor |
| 2 | All lit LEDs change their color to the LEDsetColor |

The default mode is 0.

### LEDchangeColor(byte LED)

The color of the specified LED changes to the LEDsetColor. If the LED is already lit, it'll change the color immediately.  
If mode 1 is selected then this command is the only way to change the color of an LED.

## Hardware

The hardware is quite simple and mainly consists of an Arduino Mini and a bit of logic to connect it to the HW extension interface of the APC.

![Pic_Led_Exp](https://github.com/AmokSolderer/APC/blob/V00.23/DOC/PICS/LED_ExpBoard.jpg)

The schematic, Gerber (and drilling) as well as an SVG file are located in the [APC_LED_exp](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_LED_exp) folder.

## Software

The SW is located in the same folder as the HW.


