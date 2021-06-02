# The Arduino Pinball Controller

Before I explain what the APC is, let's first clarify what it is not.
 
The APC is no commercial drop-in replacement board. It is intended for people with some basic knowledge of programming and electronics who want to expand the possibilities of their pinball machine. That means it's going to require some work to set up your game even if you don't want to change anything, but run the original rules with PinMame. Take a look at the [PinMame page](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md) to see what I mean.  
As this is a private project with no commercial interest, you may use the APC at your own risk and I'm not liable for any damage that might occur to your machine while using it.  
Some special machines might even not work with the APC or require additional HW. Jokerz! for example has a unique audio board which uses seperate audio channels for the cabinet and the backbox speakers. The APC HW is capable of generating two independent sound channels, but you need at least DC-blocks (capacitors) when connecting it to the speakers.

## Overview

The APC is a freely programmable controller for Williams pinball machines. It uses an Arduino Due and contains all the circuitry required to run a Williams pinball machine from System 3 to System 11c (and the compatible Data East machines):

* Display interface for all corresponding display types
* 24 Solenoid drivers
* Lamp matrix drivers for 64 lamps
* Switch matrix drivers for 64 switches + 8 non matrix switches
* SD card interface (to store audio files, highscores and settings)
* Audio amplifier with two channels (to play music and sound effects simultaneously / can be changed to single channel stereo)
* Hardware extensions interface (the pinout is compatible to the Sound Overlay Solenoid Board used in games like Whirlwind, but it is upgraded a bit to support all kind of self made hardware)
* A socket for a Raspberry Pi which can be used to run PinMame

To summarize the above it can be said that the APC does replace all CPU, power driver and sound related boards at a price of well below 100€ per board. 

![APC 2.0](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC.JPG)
The picture shows an APC 3.0 board configured for System 9 + 11 machines as the connectors for System 3 - 7 are not populated.  
See my [APC 3 video](https://www.youtube.com/watch?v=4EgOTJyxMXo) to get an impression what has changed compared to the previous versions.

The picture below shows an APC prototype in my Pinbot.

![Pic Pinbot](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Pinbot.JPG)

To see the APC in action take a look at my [Black Knight game video](https://youtu.be/N5ipyHBKzgs)

## Software

### PinMame

The APC by itself cannot run the original Williams EPROM software, but you can use PinMame to emulate the pinball's hardware and run the old game SW on the APC. The easiest way to do this is to plug a Raspberry Pi on the APC board and install Lisy on it. The image file for the Pi's SD card and additional information about Lisy can be found on the [Lisy project page](https://lisy.dev). Note that only the Raspberry Pi Zero, Zero W, 3A+ and 3B+ models are currently supported by Lisy.  
There's also a Lisy_Mini board available. The Raspberry Pi is then located on this board which can be conntected to the APC via USB. For more information about how to setup PinMame please have a look at my [PinMame page](https://github.com/AmokSolderer/APC/tree/master/DOC/PinMame.md).

### MPF

The APC is mainly meant to be a platform for people who want to program their own rules and features.  
One way of doing this is to use the [Mission Pinball Framework](http://missionpinball.org/). It can run on a PC which then controls the APC via USB. For later it is also planned to let Lisy run MPF which would then work without a PC.

I did a short video with some basic [MPF setup](https://github.com/AmokSolderer/APC/tree/master/DOC/Software/MPF) to test that everything is working:

[MPF runs APC](https://www.youtube.com/watch?v=w4Po8OE5Zkw)

### C code

If you're familiar with C you can also program the APC directly. This SW would then run on the Arduino itself with no need for an additional PC or Raspberry Pi.  
For this the APC software offers an [API](https://github.com/AmokSolderer/APC/tree/master/DOC/Software/APC_SW_reference.pdf) providing the necessary commands to control a pinball machine. It's still a lot of effort to program a game completely from scratch, but you could still run your game in PinMame and only use the API to do changes or extensions to the original rules.

The APC software itself consists of two parts: the operating system APC.ino and the game specific code. The former controls the hardware and offers an application interface (API) for the game specific software to use. For an overview of the available API variables and commands please take a look at the
[APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf).

I have written game codes for my Black Knight and Pinbot. They are still not final, but good enough to have fun with and to use as a reference when writing own code. Additionally there's a Base Code which should serve as a starting point for you to do your own game. It contains the very basics of a pinball game and it can be easily adapted to your machine. As a startup guide how to start writing game code I have written a short [tutorial](https://github.com/AmokSolderer/APC/blob/master/DOC/GameCodeTutorial.md).

Please note that I have equipped my Black Knight with a [special kind of display](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys7Alpha) and that advanced APC commands like scrolling are currently not usable with pre System11 displays. This is because I think that these displays are not suited for homebrew machines. If you do all the work needed to do your own game code, you'd for sure want to have a display with letters, otherwise you wouldn't be able to even have a decent high score list. Additionally it would be quite cumbersome to debug some game software without the display being able to show letters. Therfore I recommend to use an early System11 display which has at least one row with alphanumeric displays (or build my [System7Alpha](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys7Alpha)). However, the basic software support is implemented, which means you can use the old displays without any restrictions you just have to do a bit more coding to get all the features. And if you just want to use them with PinMame to replace your old boards these displays will work perfectly well as soon as the PinMame support is completely done.

## Hardware

The APC hardware status is mature. I have version 2.0 in use since January 2018 without anything to complain about. You can find the schematics, BOM and layout data in the [hardware section](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware). There have been some minor HW updates which are explained in the [Changelog](https://github.com/AmokSolderer/APC/tree/master/DOC/Changes.md).

The APC features an HW extensions interface which is an 8 bit bus with several enable signals. At the moment there're two extension board available, but you could use it for all kinds of additional hardware.

The first board is an LED expansion board to control WS2812 based LED-Strips with the APC. For more information about this board please refer to the [hardware section](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware).

The following picture shows the APC 2.0 in a Black Knight with the LED Expansion board connected to the hardware extensions interface.

![Arduino Pinball Controller](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/BKopen.JPG)

A short video with this setup can be found here : 

[Hardware extensions interface video](https://youtu.be/8BnVTpKq-2Y)

The second board is a driver for 8 additional solenoids. The current version is only suited for use with an own power supply (e.g. for 24V shaker motors).

I use special alphanumerical displays in my Black Knight which can also be found in the HW section as well as an LED replacement for the original System7 numerical displays.

## Current Status (June 2021)

The following table gives an overview about the various system generations the APC can be used with and if at least one machine of each generation has been confirmed to work with it. Additionally you can see whether PinMame or [MPF](http://missionpinball.org/) have been tested with at least one machine of this generation and whether some special preparation like additional cables are required. Details about these cables can be found [here](https://github.com/AmokSolderer/APC/blob/master/DOC/HowToStart.md#cable-extensions)

The PinMame support is still under development and even if a generation is basically supported you still need the audio files for your particular game. A list of available audio files and a description how to extract your own files can be found on the [PinMame page](https://github.com/AmokSolderer/APC/tree/master/DOC/PinMame.md).

| System | Tested  | PinMame support | MPF support | Comment |
|--|--|--|--|--|
|3| Not yet | Not yet | Yes|  |
|4| Yes | Not yet | Yes|  |
|6| Yes | Yes | Yes |  |
|7| Yes | Yes | Yes | Needs two additional wires |
|9| Yes | soon | Yes |  |
|11| Yes | Yes | Yes |  |
|11a| Yes | Yes | Yes | Some cable ties have to be cut and the wiring harness opened a bit |
|11b| Not yet | Not yet | Yes |  |
|11c| Yes | Yes | Yes | The wires of three connectors must be extended |

## Changes / What's new?

A history of the recent changes can be found in the [Changelog](https://github.com/AmokSolderer/APC/tree/master/DOC/Changes.md)

## Feedback

Feedback is very important for me, because if there is none I must assume that nobody is interested in this project and that I can stop to bother with additional documentation. And albeit I have tried my best to help you to become familiar with this project, there will be questions and I am willing to do some support. Like I said, it's a hobby so don't expect 24/7 availability, but I'll do my best to help.

**So if you're interested please leave some feedback in the [Flippertreff forum (german)](https://www.flippertreff.de/start/forum/topic/11356-arduino-pinball-controller/) or in the  - [Pinside Form (english)](https://pinside.com/pinball/forum/topic/arduino-pinball-controller#post-4898318).**

## How to get a board?

I'm sorry, but I'm not going to sell them. You can order boards from [JLCPCB](https://www.jlcpcb.com), but before you do so I'd recommend to read the documentation below.

## Documentation contents

1 Building and setting up an APC  
1.1 [How to start](https://github.com/AmokSolderer/APC/blob/master/DOC/HowToStart.md) - If you want to start using the APC you should read this page first.  
1.2 [Useful software tools](https://github.com/AmokSolderer/APC/blob/master/DOC/UsefulSWtools.md) - Simple tools for audio data conversion and so on  
1.3 [APC settings](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md) - a table of the settings and how to use them  
1.4 [Initial Tests](https://github.com/AmokSolderer/APC/blob/master/DOC/InitialTests.md) - how to start it up for the first time and use the Test Mode of the Base Code  
1.5 [Base Code setup](https://github.com/AmokSolderer/APC/blob/master/DOC/SetUpBC.md) - set up the Base Code for your machine  
1.6 [APC schematics](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/APC_schematics.pdf) - just in case you want to know what you're about to build  
1.7 [If things don't work](https://github.com/AmokSolderer/APC/blob/master/DOC/Problems.md) - If you have a problem, check here first

2 Writing my own game code - You want to program your own game in C? Then read this.  
2.1 [Game code tutorial](https://github.com/AmokSolderer/APC/blob/master/DOC/GameCodeTutorial.md)  
2.2 [APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) - All commands of the API in a nutshell  

3 Running PinMame  
3.1 [Lisy Homepage](https://lisy.dev/apc.html) - Location of the Lisy SW download and more  
3.2 [The APC and PinMame](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md) - shows the current status of the APC and Lisy running PinMame  
3.3 [PinMame howto](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame_howto.md) - If your game is not yet supported, you can learn here how to change that  
3.4 [PinMame game numbers](https://github.com/AmokSolderer/APC/blob/master/DOC/lisyminigames.csv) - list of the PinMame game numbers  
3.5 [Controlling Lisy](https://github.com/AmokSolderer/APC/blob/master/DOC/LisyDebug.md) - updating Lisy and using the debug mode  
3.6 [Instructions for extracting sound files](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameSounds.md) - automatic extraction of sound files and the use of Audacity in more detail (by Mokopin)

4 Using MPF  
4.1 [MPF runs APC](https://www.youtube.com/watch?v=w4Po8OE5Zkw) - see my first humble MPF steps in this video  
4.2 [MPF setup](https://github.com/AmokSolderer/APC/tree/master/DOC/Software/MPF) - my MPF test config files
4.3 Lisy runs MPF

5 Additional APC hardware - boards that might come in handy with the APC  
5.1 [APC LED expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/LEDexpBoard.md) - a board to control WS2812 based LED strips with the APC  
5.2 [APC solenoid expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/SolExpBoard.md) - to control additional features  
5.3 [System 7 alphanumeric display](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys7Alpha) - to have alphanumerical displays in a pre System 11 machine  

6 Additional non APC hardware - just some stuff I designed over the years. Can also be used without the APC  
6.1 [System 7 LED display](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys7_Display) - an LED replacement display for System 7, purely numerical  
6.2 [System 11a LED display](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys11a_Display) - an LED replacement display for System 11a
