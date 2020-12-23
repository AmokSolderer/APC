# How to start

First you should know what you're dealing with, so take a look at the [APC schematics](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/APC_schematics.pdf). I have tried to design the hardware simple and straightforward, that means with a bit of hardware knowledge it shouldn't be a problem for you to understand how it works.

In the schematic the names of the connectors are given for System7 and System11, but the System7 names are also valid for System3 - 6 and the System11 names match to System9. The following picture should help you to determine where the various connectors belong. The solenoid GND connector of Sys11 games (1J13) has only 4 pins, in the picture it is marked as a black rectangle. The X symbols indicate which pins have to be removed.

![APC connectors](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Connectors.png)

However, the APC is only suited for you if you have some basic knowledge of electronics, because you can easily damage your precious pinball machine when you don't do things right. You should at least own a logic probe to do some basic tests if things don't work.

## The board

All the required data to build a board is in the [fabrication files section](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_FabricationFiles).

I recommend to use [JLCPCB](https://jlcpcb.com) as your board manufacturer, because the assembly files are tailored to their specifications. The problem with the manufacturers is that they need at least an order of five boards to come up with a reasonable price, so please leave some feedback at the Arduino Forum, may be someone from your country has a board for sale. If you want to order by yourself, you're welcome. The manufacturer will need the APC_Gerber.zip file to build the boards and the APC_BOM.csv and APC-top-pos.csv to populate the SMD parts.

## The Components

As most of the components are SMD and have already been populated by the board manufacturer there's only a few components left for you to assemble. You can find a list of these components with their respective order number from [Mouser](http://www.mouser.com) and [Reichelt](http://www.reichelt.de) in the [Bill of Materials](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC_BOMnonSMD.pdf)

The german electronics retailer Reichelt doesn't sell all required components, but some (especially connectors) are much cheaper compared to Mouser, so for people in Europe it might still make sense to order them separately. 

## Preparation

### SD card adapter

For using SD cards with the APC an adapter is needed. It can be build very easily by just using a micro SD adapter and soldering a pin row beneath it.

![SD adapter](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/SDadapter.JPG)

This adapter must then be plugged into the SD card connector P8 of the APC. The orientation must be to make the adapter point away from the Arduino. Look at the photos of the APC inside of my machines if you're unsure about the orientation.

### Cable extensions 

For some System11 games you'll need to extend some cables to make them fit to the APC. For System11a boards this is not necessary, but you'll have to cut some cable ties to open the cable tree a bit. If you don't want to do this you can use  cable extensions instead.

For later games featuring auxiliary power and interconnect boards there is no way to prevent this. For a System11c machine like Rollergames you have to extend the cables to the power connector 1J17, the lamp strobe connector 1J7 and one of the solenoid drivers 1J19. For the flipper drivers (second part of 1J19) it's enough to cut one cable tie. The result looks like this:

![APC_Rollergames](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Rollergames.JPG)

### Additional cable connections

Sys3 - 7 games need a single wire for the display blanking. On the APC you can either use pin 12 of Display Segments 1 (1J5) or P3 which is a single pin located right next to 1J5 and labeled Blank_N. This signal must be connected to pin 4 of 1P3 (the connector which was plugged on 1J3 of your old CPU board). For this being the only functional pin on 1P3, one easy way to do the connection is to just turn 1P3 by 90 degree and plug it on pin 12 of 1J5 on the APC. The picture below shows this setup in a System7 Jungle Lord.

![Sys7DispCable](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Sys7DispCable.JPG)

The picture does also show the two additional connections that have to be done for the commas of Sys7 displays to work. The APC has them on pin 10 (comma 1+2) and 11 (comma 3+4) of 1J5 and on pin 3 (comma 1+2) and pin 4 (comma 3+4) of 1J8. Either one of those have to be connected to pin 2 (comma 1+2) and pin 1 (comma 3+4) of the old 1P8.  
In the picture 1J8 of the APC has been used to provide the signals.

You can use a simple 5 wire cable to use System7 audio boards with the APC 3.0 (older APCs need additional HW). This cable needs to connect the pins 3 - 7 of the HW extensions interface (P11) of the APC 3.0 to the audio board. One easy way to do this is to connect them to the pins 12 to 8 of 1P8 (the plug belonging to 1J8 of your Sys7 CPU board).  
A cable like this is shown below. 

![Sys7SoundCable](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Sys7SoundCable.jpg)

To make the APC use this interface, you have to set the 'PinMame Sound' setting to 'Board'. The setting can be found in the 'Game Settings' menu in 'Remote Control' mode.

## Software preparation

To run the software you need at least the Arduino IDE with the SPI and the SdFat libraries installed.  
In the IDE you have to select 'Arduino DUE (Programming Port)' as your target board. Be sure to have all .ino files and the Sound.h from the APC repository in your working directory.

## Sound

In order to make the APC play sounds and music you have to prepare the files first with the AudioSave tool and put them on the SD card (see the [useful software tools](https://github.com/AmokSolderer/APC/wiki/Useful-software-tools) page for details).  
We're still looking for someone to put the sound files on a webserver, but for the time being you have to contact me to get the necessary audio files to run the games. A list of available sound file packages can be found on the [PinMame section](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md).  
The same is valid for my Black Knight game SW which also uses the PinMame sound files for Black Knight plus a liite extra.

The APC features a digital volume control which allows you to adjust the volume in the system settings. But before you set your volume to a value different from zero, you should be sure that the volume pot is not connected (10J4 for Sys3-7 and 1J16 for Sys9-11) otherwise the volume levels will add up and you might damage your speakers.  
I'd recommend not to use 10J4 at all, but only the digital volume control.

## Settings

A list of the settings and a brief description of how to use them can be found on the [settings page](https://github.com/AmokSolderer/APC/blob/V00.14/DOC/Settings.md).

## The assembly

As the boards have the component names printed at the corresponding locations, you can just use the [Bill of Materials](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/APC_BOMnonSMD.pdf) to identify the  right component to put there.

## Getting your board started

Plug the Arduino DUE on your APC board, but don't populate the Pi yet. I recommend to do the basic tests before assembling the Pi.
The next step is to install the Arduino SW. I'd do this before you put the APC board into your pinball machine, because if this works you know, that your 5V supply has no short and is working properly.

When this is done you can put your board into your pinball machine, but only plug the connectors for Logic Power, switches and display: 

| Connector name | Number for System 3 - 7 | Number for System 9 + 11 |
|--|--|--|
| Logic Power | 1J2 | 1J17 |
| Switch Drivers | 2J2 | 1J8 |
| Switch Inputs | 2J3 | 1J10 |
| Diag_sw | 1J4 | 1J14 |
| Display Strobes 1 | 1J7 | 1J1 |
| Display Strobes 2 | 1J6 | 1J2 |
| Display Segments 1 | 1J5 | 1J3 |
| Display Segments 2 | - | 1J22 |

Now power up your pinball machine. After some startus messages your display should show 'USB CONTROL'. If you just see two brightly illuminated digits then turn off your machine immediately, because in this case your display strobing doesn't work.  
If you have a pre System11 machine with numerical displays or a System11 2x16 digit one you have to adjust the display setting to make it work correctly. A description how to do it is in the [settings section](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md).

Even if your display works from the start you should enter the settings and browse through them to do a basic test of your switch circuitry.

If displays and switches are working you can proceed to the lamps. So turn off your machine and connect the following connectors:

| Connector name | Number for System 3 - 7 | Number for System 9 + 11 |
|--|--|--|
| Lamp B+ | 2J4 | 1J4 |
| Lamp GND | 2J6 | 1J5 |
| Lamp Strobes | 2J5 | 1J7 |
| Lamp Rows | 2J7 | 1J6 |

Now would also be a good time to connect an SD-card to P8. Then turn on your machine. If any lamps should be lit quite brightly then turn it back off as your lamp strobing is faulty.  
If no lamps are lit you can enter the system settings and change the 'Active Game' to 'Base Code'. After leaving the settings the attract mode of the base code should start which cycles through the controlled lamps.

The last step is about the solenoid related connectors, so turn off your machine and connect the following:

| Connector name | Number for System 3 - 7 | Number for System 9 + 11 |
|--|--|--|
| Solenoid GND | 2J10 | 1J13 |
| Solenoid Drivers 1 | 2J11 | 1J11 |
| Solenoid Drivers 2 | 2J9 | 1J12 |
| Special Sol Drivers | 2J12 | 1J19 |

If you hear the sound of an activating solenoid when turning on your machine then turn it off immediately, as it means that one of your solenoid drivers is stuck.


In order to test your HW you have two options:  
The first one is to use a terminal program (like Hterm) to connect your PC to the APC. Then you can use the commands of the [Lisy-API](http://docs.missionpinball.org/en/latest/hardware/lisy/protocol.html) to control the APC and do some tests. For this the system has to be in Remote Control mode and the communications have to be set to USB which are the default settings.
The second option is to use the software as it is. For this you have to change your game to the Base Code mode  which should be enough to test the board, as it is described in the Board assembly help. To activate the BaseCode keep the Advance button pressed until you're in the settings. Enter the 'System Settings' by pressing the Game Start button, then Advance to proceed to the 'Active Game' setting and Game Start to change it to 'Base Code'. Use Advance to select 'Exit Settings' and Game Start to confirm. If an SD card is connected then the settings will be stored.  
A list of the avaiable settings can be found [here](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md).

Note that you have to change some settings at the beginning of BaseCode.ino to adapt it to your machine. Most settings should be self-explanatory, but BC_SolTimes[32] probably isn't. In this array the default times of all solenoids are stored in ms. For Sys11 machines the first 8 entries are representing the A coils (A/C relay in state 'A'), followed by 16 entries for the non A/C coils (9 to 24 - with 23 and 24 being the flipper finger enable switches) and the last 8 entries are for the 'C' solenoids. A 'zero' settings means that the corresponding coil can be turned on permanently. Normally this should only be done for the flipper finger enables and relays, as normal coils will burn when being turned on too long. Only use 0 if you're sure what you're doing, otherwise use a value of 50 which won't be long enough for your relays to work properly but at least you're not going to inflict any damage to your coils.

If your game has an A/C relay then you have to set the BC_SolTimes entry for your A/C relay to zero (like for any other relay of your game). Otherwise it is not allowed to permanently turn on your relay which will prevent any C bank solenoid from working. If this happens the A bank counterpart will be fired instead.

## Own software

With your board being up and running you have the following options to get a working game:

* write your own game SW in C. For this please take a look at the [APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) to get an overview of the API and use the [Game code tutorial](https://github.com/AmokSolderer/APC/wiki/Game-code-tutorial) to become familiar with it.

* setup your game in MPF which has a lot of documentation on its [homepage](http://missionpinball.org/). 

* use the PinMame emulator to run the original game SW. More information about this can be found on the [PinMame section](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md).


