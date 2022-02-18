# APC News and Changelog

## TBD

### New SW Version V0.23

* Bugfix in the AfterMusic(Sound) handling. When the StopPlayingMusic(Sound) command was immediately followed by PlayMusic(Sound) then the first AfterMusic(Sound) event was skipped.

* The types of AfterSound and AfterMusic have been changed from void (*)() to void (*)(byte). The Music/Sound routines call them with the argument = 0 when the Music/Sound file has run out.  
The purpose of this is to enable state machines that are initially called from the Music/Sound routines with the argument = 0 and can then recall themself after each step with the argument being set accordingly.

* The maximum number of effects that can be handled by the PlayFlashSequence command has been increased from 64 to 256.

* The StrobeLights() command has been reworked to make it more comfortable. You don't have to care about the StrobeLightsTimer any more. Just call Strobelights(Time) with Time > 2 to start lamp flickering with a period of (Time * 10ms). Call Strobelights(0) to turn off the flicker.

* Support for 2x16 character displays with additional numerical displays has been added.

* SW support for the [LED_expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/LEDexpBoard.md) has been upgraded.

* Exception rules for System 11 F-14 Tomcat have been added. Sound files for F-14 are also available (Thanks to Snux)

* A new setting 'none' has been added to the 'Backbox Lamps' setting in the system settings. This is for the ShowLampPatterns command to know which lamps not to include in lamp animations.

* The ShowLampPatterns command has been changed to support also games without any controlled lamps in the backbox. For this the Pattern array in LampPat has been changed from 7 to 8 entries. If your a game has the first column of lamps in the backbox you have to select 'column1' in the 'Backbox Lamps' seeting and add a zero after the duration. For games with the last row of lamps in the backbox change the setting to 'column8' and add a zero at the end of the pattern part of LampPat.

### Misc

* The [4 Alpha + Credit display](https://github.com/AmokSolderer/APC/blob/V00.23/DOC/Sys7Alpha.md) has been updated to support also System7 display cables with edge connectors.

* My [APC Black Knight](https://github.com/AmokSolderer/APC/blob/V00.23/DOC/BlackKnight.md) game code has been updated.

## July 2021

I got a notification from Mouser that the TDA7496 audio amplifier IC won't be produced any longer.

At the moment I don't have the time to select a new amplifier IC, change the board design and do the necessary tests. I might do it in the future if the IC is not available any more.  
That means you should try to get these ICs before you order any boards.

## June 2021

### New SW Version V0.22

* A new exception handling system has been implemented. This makes it easier to program machine specific changes while using PinMame. Take a look at the [PinMame How-to](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame_howto.md#programming-exceptions) page for details

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
