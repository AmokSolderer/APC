# The Arduino Pinball Controller

Before I explain what the APC is, let's first clarify what it is not.
 
The APC is no commercial drop-in replacement board. It is intended for people with some basic knowledge of programming and electronics who want to expand the possibilities of their pinball machine.

As this is a private project with no commercial interest, you may use the APC at your own risk and I'm not liable for any damage that might occur to your machine while using it.  
Some special machines might even not work with the APC or require additional HW. Please check the [Known issues](https://github.com/AmokSolderer/APC#known-issues) section for details.

## Overview

The APC is a freely programmable controller for Williams pinball machines. It uses an Arduino Due and contains all the circuitry required to run a Williams pinball machine from System 3 to System 11c (and the compatible Data East machines):

* Display interface for all corresponding display types
* 24 Solenoid drivers
* Lamp matrix drivers for 64 lamps
* Switch matrix drivers for 64 switches + 8 non matrix switches
* SD card interface (to store audio files, highscores and settings)
* Audio amplifier with two channels (to play music and sound effects simultaneously / can be changed to single channel stereo)

To summarize the above it can be said that the APC does replace all CPU, power driver and sound related boards at a price of well below 100€ per board. 

![APC 2.0](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC.JPG)

The picture shows an APC 3.1 board with the new on-board SD-Card slot below the Arduino.  
See my [APC 3 video](https://www.youtube.com/watch?v=4EgOTJyxMXo) to get an impression what this board can do.

There's a table of contents of the available documentation at the end of this page.

### Special features

* Hardware extensions interface. The pinout is compatible to the Sound Overlay Solenoid Board used in games like Whirlwind, but it is upgraded a bit to support all kind of self made hardware
* A socket for a Raspberry Pi which can be used to run PinMame on the APC board. With this you can run the original ROM code on your APC.
* [PinMameExceptions](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameExceptions.md) are game specific code blocks that act like a man-in-the-middle between the game HW and PinMame. That means even if your game is running in PinMame you can still introduce some changes. Take a look at my [Jungle Lord video](https://www.youtube.com/watch?v=bbfhH_-gMfE) or my [Comet ball saver](https://youtu.be/JbgMa_pn0Lo) to see how a few lines of code can change a game dramatically.

Typical examples for the use of PinMameExceptions are:

* Pre System11 games usually use just one audio channel, so you could add a background music track. Wouldn't it be great if your Disco Fever would play old Disco music in the BG?
* Add HW toys like a shaker motor or flashers and the necessary rules to drive them. If your game has no free solenoid drivers left, then just add the [Solenoid expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/SolExpBoard.md) to get 8 more. You can also use the [LED expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/LEDexpBoard.md) to control RGB-LED stripes or just connect your own individual HW.
* Add a ball saver. Ball savers are quite easy to realize. All you have to do is not to tell PinMame that the ball has reached the outhole, but to kick it into the plunger lane instead. Watch my [Comet video](https://youtu.be/JbgMa_pn0Lo) to see how it can be done with just a few [lines of code](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame_howto.md#How-to-add-a-ball-saver).

The picture below shows an APC prototype in my Pinbot.

![Pic Pinbot](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Pinbot.JPG)

To see the APC in an early stage you might want take a look at my [Black Knight game video](https://youtu.be/N5ipyHBKzgs)

## Game Software

### PinMame

You can use [Lisy](https://lisy.dev/apc.html) to run PinMame on an APC board. This spares you the effort to do any game Software as you can run the old ROM code.
For System 3 - 7 machines the APC can be used with the old original soundboards. That means in this case it's a plug & play solution.
You might want the APC to generate the audio anyway, either because you don't have an audio board, you want to do your own sounds or you have a System 9 or 11 game. In this case then it's going to require some work to set up your game with PinMame. Take a look at the [PinMame Sound page](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md) to see what I mean.

With PinMame running for your game you can use the [PinMameExceptions](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameExceptions.md) functionality of the APC to change the rules of your game even though it's running under PinMame control.  
Watch my [Jungle Lord video](https://www.youtube.com/watch?v=bbfhH_-gMfE) so see an example. The corresponding code can be found in PinMameExceptions.ino in the AmokPrivate branch on Github.  
Another example is the ball saver I [added to my Comet](https://youtu.be/JbgMa_pn0Lo).

### MPF

For those who want to do their own game rules without having to use C, the APC does support the [Mission Pinball Framework](http://missionpinball.org/). It can run on a PC which then controls the APC via USB.

I did a short video with some basic [MPF setup](https://github.com/AmokSolderer/APC/tree/master/DOC/Software/MPF) to test that everything is working:

[MPF runs APC](https://www.youtube.com/watch?v=w4Po8OE5Zkw)

### C code

If you're familiar with C you can also program the APC directly. This SW would then run on the Arduino itself with no need for an additional PC or Raspberry Pi.  
For this the APC software offers an [API](https://github.com/AmokSolderer/APC/tree/master/DOC/Software/APC_SW_reference.pdf) providing the necessary commands to control a pinball machine. It's still a lot of effort to program a game completely from scratch, but you could even run your game in PinMame and only use the API to do changes or extensions to the original rules.

You don't necessarily have to do the code from scratch. There is a [Base Code](https://github.com/AmokSolderer/APC/blob/master/BaseCode.ino) which covers the basics of a pinball machine and can be used as a starting point.  
Furthermore there're the above mentioned [PinMameExceptions](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameExceptions.md) which can change a game dramatically with just a few lines of code.

## Hardware

The APC hardware status is mature. I have version 2.0 in use since January 2018 without anything to complain about. Since then the HW has basically stayed the same, just some features have been added and the board has been prepared for automatic assembly.
You can find the schematics, BOM and layout data in the [hardware section](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware). There have been some minor HW updates which are explained in the [Changelog](https://github.com/AmokSolderer/APC/tree/master/DOC/Changes.md).

The APC features an HW extensions interface which is an 8 bit bus with several enable signals. At the moment there're two extension board available, but you could use it for all kinds of additional hardware.

The first board is an LED expansion board to control WS2812 based LED-Strips with the APC. For more information about this board please refer to the [APC LED expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/LEDexpBoard.md) section.

The following picture shows the APC in a Comet with the LED Expansion board connected to the hardware extensions interface.

![APC Comet LED](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/CometLED.jpg)

A short video with this setup can be found here : 

[GI LEDs](https://youtu.be/kLWVUdhSwfo)

The second board is a [driver for 8 additional solenoids](https://github.com/AmokSolderer/APC/blob/master/DOC/SolExpBoard.md). The current version is mainly suited for use with an own power supply (e.g. for 24V shaker motors).

I use [special alphanumerical displays](https://github.com/AmokSolderer/APC/blob/master/DOC/Sys7Alpha.md) in my Black Knight which can also be found in the HW section as well as an LED replacement for the original System7 numerical displays.

## Current Status (March 2023)

The following table gives an overview about the various system generations the APC can be used with and if at least one machine of each generation has been confirmed to work with it. Additionally you can see whether PinMame or [MPF](http://missionpinball.org/) have been tested with at least one machine of this generation and whether some special preparation like additional cables are required. Details about these cables can be found [here](https://github.com/AmokSolderer/APC/blob/master/DOC/HowToStart.md#cable-extensions)

The PinMame support is still under development and even if a generation is basically supported you still need the audio files for your particular game. A list of available audio files and a description how to extract your own files can be found on the [PinMame page](https://github.com/AmokSolderer/APC/tree/master/DOC/PinMame.md).

| Williams System | Tested  | PinMame support | MPF support | Comment |
|--|--|--|--|--|
|3| Yes | Yes | Yes|  |
|4| Yes | Yes | Yes|  |
|6| Yes | Yes | Yes |  |
|7| Yes | Yes | Yes | Needs two additional wires |
|9| Yes | Yes | Yes |  |
|11| Yes | Yes | Yes |  |
|11a| Yes | Yes | Yes | Some cable ties have to be cut and the wiring harness opened a bit |
|11b| Not yet | Not yet | Yes |  |
|11c| Yes | Yes | Yes | The wires of three connectors must be extended |

The following Data East MPUs are almost identical to their counterparts from Williams but DE used 2.1 audio boards. Check the [known issues](https://github.com/AmokSolderer/APC#known-issues) for details

| Data East Version | Tested  | PinMame support | MPF support | Comment |
|--|--|--|--|--|
|1| Not yet | Yes | Yes | audio needs a cable adapter and is only mono |
|2| Not yet | Yes | Yes | audio needs a cable adapter and is only mono |
|3| Yes | Yes | Yes | Only non DMD machines / audio needs a cable adapter and is only mono / probably some [cable extensions](https://pinside.com/pinball/forum/topic/arduino-pinball-controller/page/18#post-6846714) are needed |

## Changes / What's new?

A history of the recent changes can be found in the [Changelog](https://github.com/AmokSolderer/APC/tree/master/DOC/Changes.md)

## Known issues

### Stereo audio

Some games like Jokerz! or Data East games starting from MPU version 1 have stereo or even 2.1 audio boards. These are currently not supported by the APC SW.  
From the HW point of view, the APC is capable of generating two independent sound channels which could also be used for stereo. But up to now they're used to generate separate channels for sounds and music which are combined in the output amplifier.  
In order to use these machines with the APC you'd have to build a cable adapter and the audio would only be mono.

### Stepper motors

Games like Jokerz! or Riverboat Gambler use stepper motors to drive a wheel in the backbox and the amount of steps being generated determine where the wheel stops.  
Alas, there's a problem with Lisy/PinMame not sending all steps to the APC. This means it is not possible for the APC to determine at which position the wheel is supposed to stop.  
Note that this is not a problem of the APC itself, but occurs only when Lisy/PinMame is used. If you program your game code in C there won't be any problems.

### Rollergames music problems

The Rollergames implementation in PinMame has a bug in the music control. For some reason a new music track is started every few seconds. Most of the time it's the same track that's already running. I have therefore implemented a workaround in the PinMameExceptions of the Rollergames which prevents that the running music track can be restarted. This improves the situation somehow, but more sophisticated PinMameExceptions are needed to really fix this.  
Up to now only Rollergames seems to be affected, other machines using the same audio boards are working fine.

## Feedback

Feedback is very important for me, because if there is none I must assume that nobody is interested in this project and that I can stop to bother with additional documentation. And albeit I have tried my best to help you to become familiar with this project, there will be questions and I am willing to do some support. Like I said, it's a hobby so don't expect 24/7 availability, but I'll do my best to help.

**So if you're interested please leave some feedback in the [Flippertreff forum (german)](https://www.flippertreff.de/start/forum/topic/11356-arduino-pinball-controller/) or in the  - [Pinside Form (english)](https://pinside.com/pinball/forum/topic/arduino-pinball-controller#post-4898318).**

## How to get startet?

I'm sorry, but I'm not going to sell any boards. Check whether anyone in the above mentioned forums has a spare board for sale. If this fails you can still order populated board in China.  
If you're interested in using an APC, then be sure to follow the instructions given in the first part of the documentation listet below. They will guide you through the process of getting and setting up an APC board.

## Documentation contents

1. Building and setting up an APC  
1.1. [The APC board](https://github.com/AmokSolderer/APC/blob/master/DOC/APC_board.md) - How to get one and how to set it up  
1.2. [Upload the SW](https://github.com/AmokSolderer/APC/blob/master/DOC/Upload_SW.md) - How to program the Arduino DUE  
1.3. [Basic Preparation](https://github.com/AmokSolderer/APC/blob/master/DOC/Prepare.md) - Required cables and stuff  
1.4. [Do the tests](https://github.com/AmokSolderer/APC/blob/master/DOC/InitialTests.md) - Test your board  
1.5. [Run a game](https://github.com/AmokSolderer/APC/blob/master/DOC/RunGame.md) - All the ways to get a game running  

2. References  
2.1. [Setting up the BaseCode](https://github.com/AmokSolderer/APC/blob/master/DOC/SetUpBC.md) - Do the BaseCode setup to get a very simple game  
2.2. [Useful software tools](https://github.com/AmokSolderer/APC/blob/master/DOC/UsefulSWtools.md) - Simple tools for audio data conversion and so on  
2.3. [APC settings](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md) - a table of the settings and how to use them  
2.4. [APC schematics](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/APC_schematics.pdf) - just in case you want to know what you're about to build  
2.5. [If things don't work](https://github.com/AmokSolderer/APC/blob/master/DOC/Problems.md) - If you have a problem, check here first

3. Writing my own game code - You want to program your own game in C? Then read this.  
3.1. [Game code tutorial](https://github.com/AmokSolderer/APC/blob/master/DOC/GameCodeTutorial.md) - A step by step introduction to writing your own game code  
3.2. [APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) - All commands of the API in a nutshell  

4. Running PinMame  
4.1. [Lisy Homepage](https://lisy.dev/apc.html) - Location of the Lisy SW download and more  
4.2. [PinMame Sound](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md) - shows the current status of the APC and Lisy running PinMame  
4.3. [PinMame Sound howto](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame_howto.md) - If your game is not yet supported, you can learn here how to change that  
4.4. [PinMameExceptions](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameExceptions.md) - change your game, but let PinMame do the main work  
4.5. [PinMame game numbers](https://github.com/AmokSolderer/APC/blob/master/DOC/lisyminigames.csv) - list of the PinMame game numbers  
4.6. [Controlling Lisy](https://github.com/AmokSolderer/APC/blob/master/DOC/LisyDebug.md) - updating Lisy and using the debug mode  
4.7. [Instructions for extracting sound files](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameSounds.md) - automatic extraction of sound files and the use of Audacity in more detail (by Mokopin)

5. Using MPF  
5.1. [MPF runs APC](https://www.youtube.com/watch?v=w4Po8OE5Zkw) - see my first humble MPF steps in this video  
5.2. [MPF setup](https://github.com/AmokSolderer/APC/tree/master/DOC/Software/MPF) - my MPF test config files
5.3. Lisy runs MPF

6. Additional APC hardware - boards and other stuff that might come in handy with the APC  
6.1. [APC LED expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/LEDexpBoard.md) - a board to control WS2812 based LED strips with the APC  
6.2. [APC solenoid expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/SolExpBoard.md) - to control additional features  
6.3. [System 7 alphanumeric display](https://github.com/AmokSolderer/APC/blob/master/DOC/Sys7Alpha.md) - to have alphanumerical displays in a pre System 11 machine  
6.4. [Installation frames](https://github.com/AmokSolderer/APC/blob/V01.00/DOC/Frames.md) - To mount the APC in your backbox  

7. Additional non APC hardware - just some stuff I designed over the years. Can also be used without the APC  
7.1. [System 7 LED display](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys7_Display) - an LED replacement display for System 7, purely numerical  
7.2. [System 11a LED display](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys11a_Display) - an LED replacement display for System 11a

8. APC games - Complete games running natively on the APC or games that run in PinMame but have been changed  
8.1. [Black Knight](https://github.com/AmokSolderer/APC/blob/master/DOC/BlackKnight.md) - Complete game code with some additional features  
8.2. [Pin Bot](https://github.com/AmokSolderer/APC/blob/master/DOC/Pinbot.md) - Complete game code with some additional features  
8.3. [Comet](https://github.com/AmokSolderer/APC/blob/master/DOC/Comet.md) - Some extensions to the original game code running in PinMame

9. Videos - For the generation Youtube  
9.1. [What's new in APC3](https://youtu.be/4EgOTJyxMXo) - Gives an overview about what the APC is and what version 3 can do  
9.2. [Lisy, APC and PinMame](https://youtu.be/cXrh-XPqCKw) - Shows the use of PinMame in an early stage  
9.3. [PinMameExceptions](https://youtu.be/bbfhH_-gMfE) - What is a PinMameException and how does it work?  
9.4. [Adding a ball saver with PinMameExceptions](https://youtu.be/JbgMa_pn0Lo) - Another example of how to use PinMameExceptions  
9.5. [APC MPF](https://youtu.be/w4Po8OE5Zkw) - A simple demonstration of the APC being controlled by MPF  
9.6. [GI LEDs](https://youtu.be/kLWVUdhSwfo) - Shows the LED_exp board in action  
9.7. [Arduino Pinball Controller hardware extensions interface](https://youtu.be/8BnVTpKq-2Y) - Also shows the use of the LED_exp board, but in an early SW stage  
9.8. [APC Black Knight demo](https://youtu.be/N5ipyHBKzgs) - My first APC video. Not really up to date any more, but it shows the basics  
9.9. [How to use numerical displays](https://youtu.be/2A5Tt9FQ2as) - How to navigate the menus when you can't read any text  
9.10. [How to adjust the display setting to a 2x16 alphanumerical display](https://youtu.be/XqPWbm-HWM8) - How to find the right setting to make these displays work
