# APC News and Changelog

## June 2025

### New SW Version V1.03

* Stability fix in KillTimer() -> does only affect self written code with heavy timer use
* Bugfix -> MPF boot issue fixed by changing the reply to USB command 19 (get number of modern lights) to a two bit response
* New [System Setting 12](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md#system-settings) 'Connect Startup' added. This is for MPF users who also use PinMame. With this setting enabled you can decide whether to boot Lisy/PinMame from the onboard PI or boot to 'USB control' mode by the state of the Up/Down switch. Hence, changing the 'Connect Type' setting is no longer necessary.
* Exception rules for System 6 Gorgar have been added. [Sound files for Gorgar are also available](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md)

## November 2024

### New SW Version V1.02

* The default name of music files has been changed from MUSIC.BIN to MUSIC.SND to be consistent with the APC audio files having .SND as a suffix. That means you might have to rename the MUSIC.BIN files of your SD card to MUSIC.SND
* A new option is available to replace the old (and sometimes annoying) [BG sound of old machines by MUSIC.SND](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameSound_3_7.md#adding-background-music). It can be activated with setting 7 in the [game settings](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md#game-settings-in-remote-control-mode).
* A general ball saver is available. It can be activated with setting 5 in the [game settings](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md#game-settings-in-remote-control-mode). There's [a list](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md#available-sounds) of supported machines. Look [here](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameExceptions.md#how-to-add-a-ball-saver) if it's not yet available for your machine.

## June 2024

### HW

* The package of the 74HCT273 ICs has been changed back to SOIC-20. They were TSSOP previously as the SOIC version was not available for some time

### New SW Version V1.01

* Bugfix -> Heavy use of timers could lead to system crashes
* Exception rules for System 11b Space Station have been added. [Sound files for Space Station are also available](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md)
* Exception rules for System 6 Time Warp have been added. [Sound files for Time Warp are also available](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md)
* Implementation of the new 3 ball multiball mode for [Pinbot](https://github.com/AmokSolderer/APC/blob/master/DOC/Pinbot.md) is complete

## December 2023

### HW

* C14 has been increased from 10µF to 22µF as some System11 machines had [startup issues](https://github.com/AmokSolderer/APC/blob/master/DOC/Problems.md)

### New SW Version V1.00

* General improvements for the Pinbot and Black Knight code added. Main feature is the new 3 ball multiball mode for [Pinbot](https://github.com/AmokSolderer/APC/blob/master/DOC/Pinbot.md)
* Exception rules for System 3 Disco Fever have been added. [Sound files for Disco Fever are also available](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md)
* Exception rules for System 6 Alien Poker have been added. [Sound files for Alien Poker are also available](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md)
* Bugfix -> if a sound/music file was invoked immediately after another, the first one was dropped regardless of the priority.
* Bugfix -> Controlling the original audio boards of System 3 - 6 games has been fixed.
* Support for single ball games added to BaseCode.
* Solenoid latches are updated every ms instead of on request which will revoke any accidental state change of solenoids. This can happen in heavily distorted environments when the distortion spikes are strong enough to trigger the latches. Distortion levels like these usually mean that something is wrong in your machine, e.g. a broken free-wheeling diode can cause this.
* System watchdog implemented. If the timer interrupt controlling the HW should not occur for 2s the system will restart.
* System 3 game adjustments implemented. You can now use the settings system of the APC to do the adjustments of System 3 games.
* New command StopAllBlinkLamps() implemented

### Misc

* Page for [installation frames](https://github.com/AmokSolderer/APC/blob/master/DOC/Frames.md) started. These make it easier to mount an APC in your backbox.
* PinMame Sound how-tos added to make it more clear what to do to let the APC play the original sounds

## March 2023

### New HW Version V3.1

* The packages of the LEDs have been changed to SMD.
* The fabrication files have been updated to let JLCPCB populate most of the non-SMD components also.
* An SMD adapter for SD cards has been added (P14).

![SD slot](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/SDonBoard.JPG)

The old connector for the self made adapter (P8) is still there, so you can decide which one you like more.

### New SW Version V0.31

* The ActivateSolenoid and ReleaseSolenoid commands can now handle recycle times for the 22 A-bank solenoids. Hence, you can prevent your slingshots from machine gunning without having to bend the switches.  
Check the [APC SW reference](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf) to learn how to use it.
* The Pinbot and [Black Knight](https://github.com/AmokSolderer/APC/blob/master/DOC/BlackKnight.md) game SW has been improved

### Misc

* DOCs have been reworked, especially [section 1](https://github.com/AmokSolderer/APC#documentation-contents) "Building and setting up an APC".

## August 2022

### New SW Version V0.30

* Fixed a serious bug in USBcontrol. Depending on your settings this could lead to an Error 11 (or worse) at the end of a PinMame game when the flipper fingers are being disabled.
* Bugfix in USBcontrol. Serial debug (USB) interface can now be used when the 'Connection type' is set to 'OnBoard' and the debug mode in the 'System Settings' is set to on.

## July 2022

### New SW Version V0.23

* Bugfix in the AfterMusic(Sound) handling. When the StopPlayingMusic(Sound) command was immediately followed by PlayMusic(Sound) then the first AfterMusic(Sound) event was skipped.
* The types of AfterSound and AfterMusic have been changed from void (*)() to void (*)(byte). The Music/Sound routines call them with the argument = 0 when the Music/Sound file has run out.  
The purpose of this is to enable state machines that are initially called from the Music/Sound routines with the argument = 0 and can then recall themself after each step with the argument being set accordingly.
* The maximum number of effects that can be handled by the PlayFlashSequence command has been increased from 64 to 256.
* The StrobeLights() command has been reworked to make it more comfortable. You don't have to care about the StrobeLightsTimer any more. Just call Strobelights(Time) with Time > 2 to start lamp flickering with a period of (Time * 10ms). Call Strobelights(0) to turn off the flicker.
* Support for 2x16 character displays with additional numerical displays has been added.
* SW support for the [LED_expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/LEDexpBoard.md) has been upgraded. One change is that you can now install additional LEDs. Therefore the new system setting 'No of LEDs' has been added as [setting 8](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md#system-settings). This might require resetting the system settings to the defaults, but it adds a lot of fun as you can see in [my Comet video](https://youtu.be/kLWVUdhSwfo).
* Exception rules for System 11 F-14 Tomcat have been added. Sound files for F-14 are also available (Thanks to Snux)
* A new setting 'none' has been added to the 'Backbox Lamps' setting in the system settings. This is for the ShowLampPatterns command to know which lamps not to include in lamp animations.
* The ShowLampPatterns command has been changed to support also games without any controlled lamps in the backbox. For this the Pattern array in LampPat has been changed from 7 to 8 entries. If your a game has the first column of lamps in the backbox you have to select 'column1' in the 'Backbox Lamps' seeting and add a zero after the duration. For games with the last row of lamps in the backbox change the setting to 'column8' and add a zero at the end of the pattern part of LampPat.
* Data East 2x16 displays are now supported. Therefore the entry 'Data East 2x16' has been added to the 'Display Type' setting in the system settings. As a side effect the numerical only displays move up one setting. Hence, if you have a System 3 - 9 type of display you have to re-adjust your display setting.
* Exception rules for System 9 Comet have been added. Sound files for Comet are also available

### Misc

* Due to the recent problems with the availability of the 74HCT273 at JLCPCB, I have split up the fabrication files section into two parts, one with the 74HCT273 in an [SOIC package](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_FabricationFiles_SOIC) and one with a [SSOP package](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_FabricationFiles_SSOP). Both folders contain all the required data to build a board, just the package of the 74HCT273 is different.
* The [4 Alpha + Credit display](https://github.com/AmokSolderer/APC/blob/master/DOC/Sys7Alpha.md) has been updated to support also System7 display cables with edge connectors.
* My [APC Black Knight](https://github.com/AmokSolderer/APC/blob/master/DOC/BlackKnight.md) game code has been updated.
* My Pinbot code has also improved a lot, but I don't have an own section for it yet.
* A new [Known issues](https://github.com/AmokSolderer/APC#known-issues) section has been added to point out problems and limits of the APC.

## July 2021

I got a notification from Mouser that the TDA7496 audio amplifier IC won't be produced any longer.

At the moment I don't have the time to select a new amplifier IC, change the board design and do the necessary tests. I might do it in the future if the IC is not available any more.  
That means you should try to get these ICs before you order any boards.

## June 2021

### New SW Version V0.22

* A new exception handling system has been implemented. This makes it easier to program machine specific changes while using PinMame. Take a look at the [PinMameExceptions](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMameExceptions.md) page for details
* Exception rules for System 7 Pharaoh have been added. Sound files for Pharaoh are also available (Thanks to Grangeomatic)
* Exception rules for System 6 Firepower have been added. Sound files for Firepower are also available (Thanks to Matiou)
* Support for 6 digit numerical displays (System3 - 6) has been added
* Support for System 3 - 6 sound boards as well as System 11 audio boards has been added
* The Pinbot exceptions have been changed to handle the looping music scores correctly
* Exception rules for Rollergames have been added, but there seems to be a problem with PinMame sending erroneous music commands. This happens only during a game, the music test works perfectly. This issue is still unsolved, so any help is welcome.

### Misc

* There's a new section [If things don't work](https://github.com/AmokSolderer/APC/blob/master/DOC/Problems.md) in case you encounter a problem
* The 74HCT273 ICs from Nexperia have run out of stock at JLCPCB quite some time ago and they don't seem to reorder them. Therefore the type of this IC has been changed to the Texas Instruments part.

## January 2021

The APC 3.0 board has most components populated by the board manufacturer. As this reduces the probability of soldering errors significantly, the set up and test procedures could be made a lot easier.

### New SW Version V0.21

The Base Code set up can now be done in the Game Settings, without having to do any code changes.

### Misc

The setup and testing procedures have been changed in the DOCs. Some Lisy specific documentation has been added.

## December 2020

APC 3.0 is released. See my [APC 3 video](https://www.youtube.com/watch?v=4EgOTJyxMXo) to get an impression what has changed.

### New HW Version V3.0

* The Lisy_Mini board is no longer needed to run Lisy (for PinMame or MPF) as the Raspberry Pi has been added to the APC board itself. Note that the 'Connect Type' setting must be set to 'On Board' for the on board Pi to work. This has been done to still be able to ignore the on board Pi and control the APC from the USB port if needed.
* The Hardware Extensions Interface has been upgraded to have 5V logic level also at the select pins. Furthermore the data lines can now be latched which makes it possible to control System7 audio boards without having to use additional HW.
* The board has been reworked to use SMD ICs where possible and move all components to the top side. This makes it possible to populate the board at the board manufacturers. All necessary fabrication files can be found in the [fabrication files folder](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/APC_FabricationFiles).

### New SW Version V0.20

* Display blanking for the various display types is now handled by SW which makes jumper JP1 and the resistor arrays RR2 - RR5 obsolete. This does also work for APC 2.x boards.
* The functionality of pre System11 sound boards has been modelled. This make it easier to use the PinMame sound of System 3 - 9 machines. For Systems 3 - 7 it is also possible to use the original audio boards.
* Support for System7 displays has been added.
* USBcontrol mode has been renamed to Remote Control Mode as the APC 3 can also be controlled by the on board Pi.

### Misc

* Sound files for Black Knight and Jungle Lord are available on request. They can be used with PinMame or as a basis for own games.
* A summary of the [Documentation contents](https://github.com/AmokSolderer/APC/tree/master#documentation-contents) has been added at the end of the main page to provide a better overview about the documentation.

## April 2020

PinMame is working.

![PinMame](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/PinMame.jpg)

Watch the

[Lisy, APC and PinMame](https://youtu.be/cXrh-XPqCKw)

video to see my old Pinbot under PinMame control.

There's still some work to be done, but with this setup you can do much more than just playing the original game. See my [PinMame page](https://github.com/AmokSolderer/APC/tree/master/DOC/PinMame.md) for details.

### New SW Version V0.13

The SW comes up in USBcontrol mode now instead of the BaseCode. The reason for this is that some people are using the APC with MPF, but without having any displays connected to it. This makes it quite cumbersome to change the game mode, so the default was changed accordingly.

Some changes have been implemented for PinMame support, especially concerning the sound handling.

The Black Knight game SW has been upgraded to implement more sounds and effects. Contact me if you're interested in getting the corresponding sound files.

## Sep 2019

It has required some additional changes in the SW to make the sound work completely with MPF, but now it's done.
That means the APC is all set for MPF, as you can see in the following video:

[MPF runs APC](https://www.youtube.com/watch?v=w4Po8OE5Zkw)

It's not a cineastic masterpiece (again) but it shows that everything works now. Sorry for the slight display flicker - it's just caused by my camera, in reality the displays look just fine.

### New SW Version V0.12

Some changes in the USBcontrol.ino to make the sound commands fully compliant to MPF.
You can find my MPF configs in DOC/Software/MPF. I don't know whether they qualify more as a good or a bad example, but they might be helpful anyway.

## Aug 2019

After some changes in the USB protocol the displays are now also working with MPF.
Snux has already shown it all working in his famous F-14

[F-14 game video](https://www.youtube.com/watch?time_continue=1&v=XAQ0JEPhWv4)

The only thing that hasn't been completely tested yet is the sound. It works when you're sending the necessary USB commands with a terminal program - I tried that. So from the APC point of view it should be OK.

### New SW Version V0.11

The new SW Version is V0.11 which affects only the USBcontrol.ino and changes the display numbering to be consistent with MPF.

### New HW Version V2.4

Apparently Williams changed the ground wiring of the direct switches for the special solenoids from Sys11 to Sys11a. Since Sys11a these switches have to get their ground from the pins 6 and 7 of 1J18. Unfortunately they didn't change it in the schematics where they're still marked as NC. Many thanks to Snux for pointing this out.

This required a new HW release which has both pins connected to ground and I used this opportunity to change the supply of the old volume pot to 5V. Before this you could theoretically damage your audio amplifier by turning up the digital volume control with the volume pot still connected. Even though this is unlikely and not allowed as stated in the Wiki, I wanted to eliminate any risk for damaging the HW. However the volumes will still add up when the the connector is not diconnected and you might blow you speakers (or your ears).

## July 2019

Snux has successfully made the attract mode of his 'F-14 Second Sortie' work with the APC:

[F-14 attract mode video](https://www.youtube.com/embed/XlqPyIdEk2k)

Don't be suprised about the boring display. As the new display protocol is not yet implemented in MPF the display cannot be controlled yet.

### Change in the BOM

When Snux used his LED lamp tester it became obvious that there is some 'crosstalk' between the lamp columns. That means if a lamp is turned on, it's counterpart in the previous lamp row shows a weak glow.

![LampFix](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/LampFix.jpg)

It's much less obvious with the normal light bulbs, but I wanted to fix it anyway. For this reason the values of the resistors R1 - R16 have been changed to speed up the column driver.

### New SW Version V0.10

* SW version number will now be shown during startup.
* The lamp timing has been changed slightly to suppress the 'crosstalk' between the lamp columns (see above)
* The USB display protocol has been updated as agreed with the Mini Lisy and MPF developers

## June 2019

First experiments with [Lisy Mini](https://lisy.dev/lisy_mini.html) which is used to run PinMame and control the APC via USB.

![Pic Mini Lisy](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/MiniLisy.png)

Attract mode of a Comet (System9) is already working. The display protocol still needs some work, but we're going to do that together with the MPF guys.

And I wish my backboxes were so clean and tidy.

### New HW Version V2.3

The pre Sys11 machines need an inverted blanking signal for their displays. In order to generate this signal an additional transistor Q8 and a base resistor R88 have been added. Furthermore the 'Display Segments 1' connector has been enlarged from 9 to 12 pins. System9 can use this connector directly, Sys 3-6 machines only require the Blanking_N pin and Sys7 users have to connect the comma pins additionally.
