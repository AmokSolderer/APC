# Basic Preparation

## SD cards

The APC needs an SD card for it's settings as well as for the audio files.

From HW version 3.1 onwards the APC has an on-board SD-Card slot which is located below the Arduino.

![SD slot](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/SDonBoard.JPG)

If you want to let the APC generate the sound then you should reformat your SD Card befort using it, preferably with the tool provided from the SD Association (sdcard.org). This can improve the file access time of the card which is crucial for the sound system to work properly.

### Self made SD adapter

Depending on your pinbal machine the on-board SD-Card slot might be cumbersome to access. You might therefore want to built a simple adapter which can be plugged into connector P8 and can be accessed comfortably.

![SD adapter](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/SDadapter.JPG)

This adapter can be build very easily by just using a micro SD adapter and soldering a pin row beneath it. It must then be plugged into the SD card connector P8 of the APC. The orientation must be to make the adapter point away from the Arduino. Look at the photo below if you're unsure about the orientation.

Of course you cannot use the SD-Card slot and the self made adapter simultaneously, as the APC can only use one SD-Card at a time.

### Installation frames

There're [installation frames](https://github.com/AmokSolderer/APC/blob/V01.00/DOC/Frames.md) available which can be 3D printed and help you install the APC board in your backbox.

# Machine specific preparation

This page is divided in different sections, depending on which generation your pinball machine is from. 

## System 3 - 6 

### System 3 - 7 display blanking cable

Sys3 - 7 games need a single wire for the display blanking. On the APC you can either use pin 12 of Display Segments 1 (1J5) or P3 which is a single pin located right next to 1J5 and labeled Blank_N. This signal must be connected to pin 4 of 1P3 (the connector which was plugged on 1J3 of your old CPU board). For this being the only functional pin on 1P3, one easy way to do the connection is to just turn 1P3 by 90 degree and plug it on pin 12 of 1J5 on the APC. The picture below shows this setup in a System7 Jungle Lord.

![Sys7DispCable](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Sys7DispCable.JPG)

### System 7 display comma cables

The picture above does also show the two additional connections that have to be done for the commas of Sys7 displays to work. The APC has them on pin 10 (comma 1+2) and 11 (comma 3+4) of 1J5 and on pin 3 (comma 1+2) and pin 4 (comma 3+4) of 1J8. Either one of those have to be connected to pin 2 (comma 1+2) and pin 1 (comma 3+4) of the old 1P8.  
In the picture 1J8 of the APC has been used to provide the signals.

### System 7 audio cable

You can use a simple 5 wire cable to use System7 audio boards with the APC 3.0 (older APCs need additional HW). This cable needs to connect the pins 3 - 7 of the HW extensions interface (P11) of the APC 3.0 to the audio board. One easy way to do this is to connect them to the pins 12 to 8 of 1P8 (the plug belonging to 1J8 of your Sys7 CPU board).

|APC pin (P11)| 1P8 pin |
|--|--|
|3|12|
|4|11|
|5|10|
|6|9|
|7|8|

A cable like this is shown below. 

![Sys7SoundCable](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Sys7SoundCable.jpg)

To make the APC use this interface, you have to set the 'PinMame Sound' setting to 'Board'. The setting can be found in the 'Game Settings' menu in 'Remote Control' mode.

## System 9 - 11

For System 9, 11 and 11a machines the APC fits in quite well, you might have to cut some cable ties though.  
Using the old audio boards is not possible for these machines.

### Cable extensions for System11b + c

For some System11 games you'll need to extend some cables to make them fit to the APC. For System11a boards this is not necessary, but you'll have to cut some cable ties to open the cable tree a bit. If you don't want to do this you can use  cable extensions instead.

For later games featuring auxiliary power and interconnect boards there is no way to prevent this. For a System11c machine like Rollergames you have to extend the cables to the power connector 1J17, the lamp strobe connector 1J7 and one of the solenoid drivers 1J19. For the flipper drivers (second part of 1J19) it's enough to cut one cable tie. The result looks like this:

![APC_Rollergames](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Rollergames.JPG)

# Ready for testing

The next step is to [do the tests](https://github.com/AmokSolderer/APC/blob/master/DOC/InitialTests.md).
