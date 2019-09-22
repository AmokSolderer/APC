# The Arduino Pinball Controller
![APC 2.0](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC.JPG)
## Overview

The APC is a freely programmable controller for Williams pinball machines. It uses an Arduino Due and contains all the circuitry required to run a Williams pinball machine from System 3 to System 11c (and the compatible Data East machines):

* Display interface for all corresponding display types
* 24 Solenoid drivers
* Lamp matrix drivers for 64 lamps
* Switch matrix drivers for 64 switches + 8 non matrix switches
* SD card interface (to store audio files, highscores and settings)
* Audio amplifier with two channels (to play music and sound effects simultaneously / can be changed to single channel stereo)
* Hardware extensions interface (the pinout is compatible to the Sound Overlay Solenoid Board used in games like Whirlwind, but it is upgraded a bit to support all kind of self made hardware)

To summarize the above it can be said that the APC does replace all CPU, power driver and sound related boards, at a price of well below 100€ per board. A more detailed calculation of the costs can be found in the [APC wiki](https://github.com/AmokSolderer/APC/wiki/Home).

At the moment the APC cannot run the original Williams EPROM software, but we're working on a solution to conntect the APC via USB to PinMame. However, this board is mainly meant to be a platform for people who want to program their own rules and features. Therefore the APC software offers an API providing the necessary commands to make this a lot easier, but you still have to code the game specific part by yourself. But the new USB control mode is not only meant for PinMame, but also for the [Mission Pinball Framework](http://missionpinball.org/), which would be an alternative to coding.

To see the APC in action take a look at my [Black Knight game video](https://youtu.be/N5ipyHBKzgs)

The following picture shows the APC 2.0 in a Black Knight with the LED Expansion board connected to the hardware extensions interface.

![Arduino Pinball Controller](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/BKopen.JPG)

A short video with this setup can be found here : 

[Hardware extensions interface video](https://youtu.be/8BnVTpKq-2Y)

The LED expansion board is a self made board to control WS2812 based LED-Strips with the APC. In this case it's used just as an example. For more information about this board please refer to the [hardware section](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware).

Basically the hardware extensions interface is an 8 bit bus with several enable signals which can be used for all kinds of additional hardware.

### Hardware

The APC hardware status is mature. I have version 2.0 in use for more than a year now without anything to complain about. Beginning of march 2019 I built a version 2.1 board which spared some components and had some slight layout changes. The version in the [hardware section](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware) is 2.3 which has the Display Segments 1 connector enlarged to 12 pins to ease the use of System7 and System9 displays.

I have tested the APC in a Black Knight (System 7), a Pinbot (System 11a) and a Rollergames (System 11c), but I checked the connector pinouts and it should work with System 3,4,6 and 9 as well as the compatible Data East machines also.

In the Pinbot I had to cut some cable ties and open up the wiring harness a bit in order to make the cable connectors reach their corresponding counterparts on the board. This is necessary as the APC is much smaller than the original circuit boards and the location of the connectors differs from System to System.

![Pic Pinbot](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Pinbot.JPG)

Later System11 machines with an auxiliary power and interconnect board need 3 cable extensions, see the preparation section in [the wiki](https://github.com/AmokSolderer/APC/wiki) for details.

### Software

The software consists of two parts: the operating system APC.ino and the game specific code. The former controls the hardware and offers an application interface (API) for the game specific software to use. For an overview of the available API variables and commands please take a look at the
[APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf).

I have written game codes for my Black Knight and Pinbot. They are still not final, but good enough to have fun with and to use as a reference when writing own code. Additionally there's a Base Code which should serve as a starting point for you to do your own game. It contains the very basics of a pinball game and it can be easily adapted to your machine. As a startup guide how to start writing game code I have written a short tutorial in the [wiki](https://github.com/AmokSolderer/APC/wiki) section.

Please note that I have equipped my Black Knight with a [special kind of display](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys7Alpha) and that advanced APC commands like scrolling are currently not usable with pre System11 displays. This is because I think that these displays are not suited for homebrew machines. If you do all the work needed to do your own game code, you'd for sure want to have a display with letters, otherwise you wouldn't be able to even have a decent high score list. Additionally it would be quite cumbersome to debug some game software without the display being able to show letters. Therfore I recommend to use an early System11 display which has at least one row with alphanumeric displays (or build my System7Alpha). However, the basic software support is implemented, which means you can use the old displays without any restrictions you just have to do a bit more coding to get all the features. And if you just want to use them with PinMame to replace your old boards these displays will work perfectly well as soon as the PinMame support is completely done.

To make it easier to start doing your own game I did a Base Code which provides basic pinball funtionality and should work in all machines with only small adaptions.

## Current Status (September 2019)

The following table gives an overview about the various system generations the APC can be used with and if at least one machine of each generation has been confirmed to work with it. Additionally you can see whether PinMame or [MPF](http://missionpinball.org/) have been tested with at least one machine of this generation and whether some special preparation like additional cables are required. Details about these cables can be found [here](https://github.com/AmokSolderer/APC/wiki#cable-extensions).

| System | Tested  | PinMame support | MPF support | Comment |
|--|--|--|--|--|
|3| Not yet | Not yet | Yes| Needs an additional wire |
|4| Not yet | Not yet | Yes| Needs an additional wire |
|6| Not yet | Not yet | Yes | Needs an additional wire |
|7| Yes | Not yet | Yes | Needs three additional wires |
|9| Yes | soon | Yes |  |
|11| Yes | Not yet | Yes | Some cable ties have to be cut and the wiring harness opened a bit |
|11a| Yes | Not yet | Yes |  |
|11b| Not yet | Not yet | Yes |  |
|11c| Yes | Not yet | Yes | The wires of three connectors must be extended |

## Changes / What's new?

A history of the recent changes can be found in the [Changelog](https://github.com/AmokSolderer/APC/tree/master/DOC/Changes.md)

## Why this page?

Basically this is a private fun project for me and some friends and it was never meant to go public. However, after a while I realized that the APC offers much more potential than I could handle as some kind of 'one man army'. I've done the necessary basics, which is the hardware and the software to run it, but I don't think I'm the right guy to invent new rules or add the right amount of polish to the game software that makes for a great game. And I'm definitely not the right guy to dig into the PinMame code and write some kind of driver to make it control the APC which could then be used as a general cheap and simple replacement board (which would be quite cool though). Therefore I hope to establish a small community of people who can do that and just need a base to start from.
 
## Feedback

Feedback is very important for me, because if there is none I must assume that nobody is interested in this project and that I can stop to bother with additional documentation or selecting a board manufacturer. And albeit I have tried my best to help you to become familiar with this project, there will be questions and I am willing to do some support. Like I said, it's a hobby so don't expect 24/7 availability, but I'll do my best to help.

**So if you're interested please leave some feedback in the [Flippertreff forum (german)](https://www.flippertreff.de/start/forum/topic/11356-arduino-pinball-controller/) or in the  - [Pinside Form (english)](https://pinside.com/pinball/forum/topic/arduino-pinball-controller#post-4898318).**

## How to get a board?

I'm sorry, but I'm not going to sell them. However, I have tried to put the required information for building a board and using the software to the [APC wiki](https://github.com/AmokSolderer/APC/wiki/Home)
