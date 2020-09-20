# How to start

First you should know what you're dealing with, so take a look at the [APC schematics](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/APC_schematics.pdf). I have tried to design the hardware simple and straightforward, that means with a bit of hardware knowledge it shouldn't be a problem for you to understand how it works.

In the schematic the names of the connectors are given for System7 and System11, but the System7 names are also valid for System3 - 6 and the System11 names match to System9. The following picture should help you to determine where the various connectors belong. The solenoid GND connector of Sys11 games (1J13) has only 4 pins, in the picture it is marked as a black rectangle. The X symbols indicate which pins have to be removed.

![APC connectors](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Connectors.png)

However, the APC is only suited for you if you have some basic knowledge of electronics, because you can easily damage your precious pinball machine when you don't do things right. You should at least own a logic probe to do some basic tests as described in the assembly help.

## The Components

Now some good news - the APC is quite cheap. As you can see in the [Bill of Materials](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/Assembly/BOM.pdf), the price for the electronic components to build this board is about 35€ (Jan 2019). To get the final price you have to add the Arduino DUE, all the Molex connectors and the board itself. I didn't add these prices to the BOM, because they differ a lot depending on where you buy. You can get an Arduino for more than 40€ or you can buy a cheap one from China, same for the Molex connectors.

I have chosen the german electronics retailer Reichelt.de to help you find the right components (don't be confused as they have order numbers with characters). There is also a [Reichelt list](https://www.reichelt.de/my/1536313) of the components available. Almost all of this is standard TTL and some driver stuff which should be available almost everywhere. There are only two ICs that Reichelt doesn't sell: the 74HCT154 and the 74BCT760, but both of these are available at Mouser or Digikey.

## The board

All the required data to build a board is in the [Layout section](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Layout).

The first decision to make is whether to etch the board by yourself or buy one from a professional manufacturer like [JLCPCB](https://jlcpcb.com). The problem with the manufacturers is that they need at least an order of five boards to come up with a reasonable price, so please leave some feedback at the Arduino Forum, may be someone from your country has a board for sale. If you want to order by yourself, you're welcome. The manufacturer will need the APC_Gerber.zip file.

If you prefer to etch the board you'll need the .svg files. I use GIMP to print them on transparencies which I use for  exposition. There's also a APC-drl_map.pdf in the folder to show you which holes have to be drilled with which drill.

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

Sys3 - 7 games need a single wire for the display blanking. On the APC you can either use pin 12 of Display Segments 1 (1J5) or P3 which is a single pin located right next to 1J5 and labeled Blank_N. This signal must be connected to pin 4 of 1P3 (the connector which was plugged on 1J3 of your old CPU board).

For a Sys7 game you also have to connect the comma signals. The APC has them on pin 10 (comma 1+2) and 11 (comma 3+4) of 1J5 and they have to be connected to pin 2 (comma 1+2) and pin 1 (comma 3+4) of the old 1P8.

## Software preparation

To run the software you need at least the Arduino IDE with the SPI and the SdFat library installed.

You have two options to test your HW:

The first one is to use a terminal program (like Hterm) to connect your PC to the APC. Then you can use the commands of the [Lisy-API](http://docs.missionpinball.org/en/latest/hardware/lisy/protocol.html) to control the APC and do some tests. The system has to be in USBcontrol mode for this.

The second option is to use the software as it is. For this you have to change your game to the Base Code mode  which should be enough to test the board, as it is described in the Board assembly help. To activate the BaseCode keep the Advance button pressed until you're in the settings. Enter the 'System Settings' by pressing the Game Start button, then Advance to proceed to the 'Active Game' setting and Game Start to change it to 'Base Code'. Use Advance to select 'Exit Settings' and Game Start to confirm. If an SD card is connected then the settings will be stored.

Note that you have to change some settings at the beginning of BaseCode.ino to adapt it to your machine. Most settings should be self-explanatory, but BC_SolTimes[32] probably isn't. In this array the default times of all solenoids are stored in ms. For Sys11 machines the first 8 entries are representing the A coils (A/C relay in state 'A'), followed by 16 entries for the non A/C coils (9 to 24 - with 23 and 24 being the flipper finger enable switches) and the last 8 entries are for the 'C' solenoids. A 'zero' settings means that the corresponding coil can be turned on permanently. Normally this should only be done for the flipper finger enables and relays, as normal coils will burn when being turned on too long. Only use 0 if you're sure what you're doing, otherwise use a value of 50 which won't be long enough for your relays to work properly but at least you're not going to inflict any damage to your coils.

If your game has an A/C relay then you have to set the BC_SolTimes entry for your A/C relay to zero (like for any other relay of your game). Otherwise it is not allowed to permanently turn on your relay which will prevent any C bank solenoid from working. If this happens the A bank counterpart will be fired instead.

## The assembly

The assembly should be done in steps with each one being followed by a functional test. All this is described in the [Board Assembly Help](https://github.com/AmokSolderer/APC/wiki/Board-assembly-help).

## Own software

If the board works you can start to write a software for your machine, so take a look at the [APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) to get an overview of the API and use the [Game code tutorial](https://github.com/AmokSolderer/APC/wiki/Game-code-tutorial) to become familiar with it.

## Sound

In order to make the APC play sounds and music you have to prepare the files first with the AudioSave tool and put them on the SD card (see the [useful software tools](https://github.com/AmokSolderer/APC/wiki/Useful-software-tools) page for details). I haven't put any sound files on this pages, because I'm not sure about the legal situation concerning the original Williams Sounds. To avoid annoying error messages about missing sound files, the BlackKnight.ino version in this project does not feature any sound currently (except for some test sounds in the test mode which you have to provide on the SD card). That means you have to generate your own files and add the commands to your code. The Pinbot.ino on the other hand has sound commands included which is meant as an example how to use it, but without the necessary files it will of course result in the above mentioned error messages. The names of the sound files in the Pinbot code have BS_M or BS_S as a prefix depending on whether it is a sound or music file.

The APC features a digital volume control which allows you to adjust the volume in the system settings. But before you set your volume to a value different from zero, you should be sure that the volume pot is not connected (10J4 for Sys3-7 and 1J16 for Sys9-11) otherwise the volume levels will add up and you might damage your speakers.

## Settings

Press the Advance button with Up/Down in Up position to enter the settings (with Up/Down in Down position this will enter the test mode). Then press Advance to select System or Game settings and confirm with the game start button. The system settings are defined in the APC.ino and therefore the same for all games while the game settings depend on the game being selected. Both settings are stored on the SD card when changed.
