# APC LED expansion board

The purpose of this board is to control a WS2812 based RGB LED strip with the APC.

Up to now the SW of this board is designed to use the LED strip as a replacement for the controlled lamps. Hence, if you activate the LEDs by changing the 'LED lamps' setting in the 'System Settings' menu of the APC, the normal controlled lamps will be switched off and the LED strip will be used instead. The first LED of the strip will work as lamp 1 and so on. When the status of a lamp changes, the brightness of the corresponding LED will change gradually in 5 steps over 100ms which is similar to a lightbulb.

## Hardware

The hardware is quite simple and mainly consists of an Arduino Mini and a bit of logic to connect it to the HW extension interface of the APC.

![Pic_Led_Exp](https://github.com/AmokSolderer/APC/blob/V00.23/DOC/PICS/LED_ExpBoard.jpg)

The schematic, Gerber (and drilling) as well as an SVG file are located in the [APC_LED_exp](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_LED_exp) folder.

## Software

The SW is located in the same folder as the HW.