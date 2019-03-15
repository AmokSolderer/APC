# The Arduino Pinball Controller
![APC 2.0](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC.JPG)
## Overview

The APC is a controller for Williams pinball machines. It uses an Arduino Due and contains all the circuitry required to run a Williams pinball machine from System 6 to System 11c (and the compatible Data East machines):

* Display interface for all corresponding display types
* 24 Solenoid drivers
* Lamp matrix drivers for 64 lamps
* Switch matrix drivers for 64 switches + 8 non matrix switches
* SD card interface (to store audio files, highscores and settings)
* Audio amplifier with two channels (to play music and sound effects simultaneously / can be changed to single channel stereo)
* Hardware extensions interface (the pinout is compatible to the Sound Overlay Solenoid Board used in games like Whirlwind, but it is upgraded a bit to support all kind of self made hardware)

To summarize the above it can be said that the APC does replace all CPU, power driver and sound related boards, but you still need the power supply board and the interconnect boards used by several System 11 games.

To see the APC in action take a look at my [Black Knight game video](https://youtu.be/N5ipyHBKzgs)

The following picture shows the APC 2.0 in a Black Knight with the LED Expansion board connected to the hardware extensions interface.

![Arduino Pinball Controller](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/BKopen.JPG)

A short video with this setup can be found here : 

[Hardware extensions interface video](https://youtu.be/8BnVTpKq-2Y)

The LED expansion board is a self made board to control WS2812 based LED-Strips with the APC. In this case it's used just as an example. For more information about this board please refer to the [hardware section](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware) in the WIKI.

Basically the hardware extensions interface is an 8 bit bus with several enable signals which can be used for all kinds of additional hardware.

## Current Status (March 2019)

### Hardware

The APC hardware status is mature. I have version 2.0 in use for more than a year now without anything to complain about. Beginning of march 2019 I built a version 2.1 board which spared some components and had some slight layout changes. The version in the [hardware section](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware) is 2.2 which has just one capacitor changed to SMD and the corresponding line removed.

I have tested the APC in a Black Knight (System 7) and a Pinbot (System 11a), but I checked the connector pinouts and it should work with System 6, System 9 and the compatible Data East machines also.

In the Pinbot I had to cut some cable ties and open up the wiring harness a bit in order to make the cable connectors reach their corresponding counterparts on the board. This is necessary as the APC is much smaller than the original circuit boards and the location of the connectors differs from System6 to System11.

![Pic Pinbot](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Pinbot.JPG)

### Software

The software consists of two parts: the operating system APC.ino and the game specific code. The former controls the hardware and offers an application interface (API) for the game specific software to use. For an overview of the available API variables and commands please take a look at the
[APC software reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf).

I have written game codes for my Black Knight and Pinbot. They are still not final, but good enough to have fun with and to use as a reference when writing own code. As a startup guide how to start writing game code I have written a short tutorial in the [wiki](https://github.com/AmokSolderer/APC/wiki) section.

### To Do

My next step is to put the APC in a Rollergames. This will require three cables to be extended a bit, since the auxiliary boards of this System11c machine makes the wiring quite inflexible and these three cable are just too short.

However apart from writing new game code and generating sound samples the following software improvements are pending:

* Implementation of a better error handler
* Support for more displays (The displays of the Rollergames require some changes to the software)
* Support for PinMame to run the original Eprom Software on a PC and use the APC just as an interface to the machine. With this, the APC could also be used as a replacement for old boards.

## Why this page?

Basically this is a private fun project for me and some friends and it was never meant to go public. However, after a while I realized that the APC offers much more potential than I could handle. I've done the necessary basics, which is the hardware and the software to run it. Now it's time for people thinking about new games and adding some polish to the software. Therefore I hope to establish a small community of people who can do that and just need a base to start from.
 
## Feedback

Feedback is very important for me, because if there is none I must assume that nobody is interested in this project and that I can stop to bother with additional documentation or selecting a board manufacturer. And albeit I have tried my best to help you to become familiar with this project, there will be questions and I am willing to do some support. Like I said, it's a hobby so don't expect 24/7 availability, but I'll do my best to help.

**So if you're interested please leave a post in the Arduino Forum (link coming soon).**

## How to get a board?

I'm sorry, but I'm not going to sell them. However, I have tried to put the required information for building a board and using the software to the [APC wiki](https://github.com/AmokSolderer/APC/wiki/Home)
