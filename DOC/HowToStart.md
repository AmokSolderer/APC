# How to start

First you should know what you're dealing with, so take a look at the [APC schematics](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/APC_schematics.pdf). I have tried to design the hardware simple and straightforward, that means with a bit of hardware knowledge it shouldn't be a problem for you to understand how it works.

In the schematics the names of the connectors are given for System7 and System11, but the System7 names are also valid for System3 - 6 and the System11 names match to System9. The following picture should help you to determine where the various connectors belong. The solenoid GND connector of Sys11 games (1J13) has only 4 pins, in the picture it is marked as a black rectangle. The X symbols indicate which pins have to be removed.

![APC connectors](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Connectors.png)

However, the APC is only suited for you if you have some basic knowledge of electronics, because you can easily damage your precious pinball machine when you don't do things right. You should at least own a logic probe to do some basic tests if things don't work.

## The board

All the required data to build a board is in the [fabrication files section](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_FabricationFiles).

I recommend to use [JLCPCB](https://jlcpcb.com) as your board manufacturer, because the assembly files are tailored to their specifications. The problem with the manufacturers is that they need at least an order of five boards to come up with a reasonable price, so please leave some feedback at the Arduino Forum, may be someone from your country has a board for sale.  
If you want to order by yourself, you're welcome. The manufacturer will need the APC_Gerber.zip file to build the boards. You also have to select 'SMT Assembly' and provide the APC_BOM.csv and APC-top-pos.csv files to make them populate the SMD parts.

Note that the board preview of JLCPCB doesn't work with the APC boards. This also means that you have to enter the board dimensions manually to get a quote. The dimensions for the APC board are 200mmx237mm.

## The Components

As most of the components are SMD and have already been populated by the board manufacturer there's only a few components left for you to assemble. You can find a list of these components with their respective order number from [Mouser](http://www.mouser.com) and [Reichelt](http://www.reichelt.de) in the [Bill of Materials](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC_BOMnonSMD.pdf)

The german electronics retailer Reichelt doesn't sell all required components, but some (especially connectors) are much cheaper compared to Mouser, so for people in Europe it might still make sense to order them separately.

### TDA7496 availability

I got a notification from Mouser that the TDA7496 audio amplifier IC won't be produced any longer.

At the moment I don't have the time to select a new amplifier IC, change the board design and do the necessary tests. I might do it in the future if the IC is not available any more.  
That means you should try to get these ICs before you order any boards.

## Preparation

### SD card adapter

For using SD cards with the APC an adapter is needed. It can be build very easily by just using a micro SD adapter and soldering a pin row beneath it.

![SD adapter](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/SDadapter.JPG)

This adapter must then be plugged into the SD card connector P8 of the APC. The orientation must be to make the adapter point away from the Arduino. Look at the photos of the APC inside of my machines if you're unsure about the orientation.

### Cable extensions for System11b + c

For some System11 games you'll need to extend some cables to make them fit to the APC. For System11a boards this is not necessary, but you'll have to cut some cable ties to open the cable tree a bit. If you don't want to do this you can use  cable extensions instead.

For later games featuring auxiliary power and interconnect boards there is no way to prevent this. For a System11c machine like Rollergames you have to extend the cables to the power connector 1J17, the lamp strobe connector 1J7 and one of the solenoid drivers 1J19. For the flipper drivers (second part of 1J19) it's enough to cut one cable tie. The result looks like this:

![APC_Rollergames](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Rollergames.JPG)

### System7 display cables

Sys3 - 7 games need a single wire for the display blanking. On the APC you can either use pin 12 of Display Segments 1 (1J5) or P3 which is a single pin located right next to 1J5 and labeled Blank_N. This signal must be connected to pin 4 of 1P3 (the connector which was plugged on 1J3 of your old CPU board). For this being the only functional pin on 1P3, one easy way to do the connection is to just turn 1P3 by 90 degree and plug it on pin 12 of 1J5 on the APC. The picture below shows this setup in a System7 Jungle Lord.

![Sys7DispCable](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Sys7DispCable.JPG)

The picture does also show the two additional connections that have to be done for the commas of Sys7 displays to work. The APC has them on pin 10 (comma 1+2) and 11 (comma 3+4) of 1J5 and on pin 3 (comma 1+2) and pin 4 (comma 3+4) of 1J8. Either one of those have to be connected to pin 2 (comma 1+2) and pin 1 (comma 3+4) of the old 1P8.  
In the picture 1J8 of the APC has been used to provide the signals.

### System 7 audio cable

You can use a simple 5 wire cable to use System7 audio boards with the APC 3.0 (older APCs need additional HW). This cable needs to connect the pins 3 - 7 of the HW extensions interface (P11) of the APC 3.0 to the audio board. One easy way to do this is to connect them to the pins 12 to 8 of 1P8 (the plug belonging to 1J8 of your Sys7 CPU board).  
A cable like this is shown below. 

![Sys7SoundCable](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Sys7SoundCable.jpg)

To make the APC use this interface, you have to set the 'PinMame Sound' setting to 'Board'. The setting can be found in the 'Game Settings' menu in 'Remote Control' mode.

## Software preparation

To run the software you need at least the Arduino IDE with the SPI and the SdFat libraries installed.  
In the IDE you have to select 'Arduino DUE (Programming Port)' as your target board. Be sure to have all .ino files and the Sound.h from the APC repository in your working directory. The name of this directory has to be APC.

## Sound

In order to make the APC play sounds and music you have to prepare the files first with the AudioSave tool and put them on the SD card (see the [useful software tools](https://github.com/AmokSolderer/APC/blob/master/DOC/UsefulSWtools.md) page for details).  
We're still looking for someone to put the sound files on a webserver, but for the time being you have to contact me to get the necessary audio files to run the games. A list of available sound file packages can be found on the [PinMame section](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md).  
The same is valid for my Black Knight game SW which also uses the PinMame sound files for Black Knight plus a little extra.

The APC features a digital volume control which allows you to adjust the volume in the system settings. But before you set your volume to a value different from zero, you should be sure that the volume pot is not connected (10J4 for Sys3-7 and 1J16 for Sys9-11) otherwise the volume levels will add up and you might damage your speakers.  
I'd recommend not to use 10J4 at all, but only the digital volume control.

## Settings

A list of the settings and a brief description of how to use them can be found on the [settings page](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md).

## The assembly

As the boards have the component names printed at the corresponding locations, you can just use the [Bill of Materials](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC_BOMnonSMD.pdf) to identify the  right component to put there.  
Pay attention to the polarity of the electrolytic capacitors C2, C9 and C11. The rectangular pad does always belong to the plus pin of the cap.  
The resistor networks RR1, RR7 and RR8 have to be populated in the correct orientation also. There's a marking for pin 1 printed on the APC boards. On the resistor networks pin 1 is usually marked with a dot.

## Getting your board started

Plug the Arduino DUE on your APC board, but don't populate the Pi yet. I recommend to do the basic tests before assembling the Pi.
The next step is to install the Arduino SW. I'd do this before you put the APC board into your pinball machine, because if this works you know, that your 5V supply has no short and is working properly.

Now you're ready for the [initial tests](https://github.com/AmokSolderer/APC/blob/master/DOC/InitialTests.md).

## Own software

With your board being up and running you have the following options to get a working game:

* write your own game SW in C. For this please take a look at the [APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) to get an overview of the API and use the [Game code tutorial](https://github.com/AmokSolderer/APC/blob/master/DOC/GameCodeTutorial.md) to become familiar with it.

* setup your game in MPF which has a lot of documentation on its [homepage](http://missionpinball.org/). 

* use the PinMame emulator to run the original game SW. More information about this can be found on the [PinMame section](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md).


