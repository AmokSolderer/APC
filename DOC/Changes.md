# Changelog of the Arduino Pinball Controller

## Aug 2019

After some changes in the USB protocol the displays are now also working with MPF. That means everything is working and it just must be tested in a real game. I guess Snux is the first to try ...

### New SW Version V0.11

The new SW Version is V0.11 which affected only the USBcontrol.ino and changed the display numbering to be consistent with MPF.

Snux?

### New HW Version V2.4

Apparently Williams changed the ground wiring of the direct switches for the special solenoids from Sys11 to Sys11a. Since Sys11a these switches have to get their ground from the pins 6 and 7 of 1J18. Unfortunately they didn't change it in the schematics where they're still marked as NC. Many thanks to Snux for pointing this out.

This required a new HW release which has both pins connected to ground and I used this opportunity to change the supply of the old volume pot to 5V. Before this you could theoretically damage your audio amplifier by turning up the digital volume control with the volume pot still connected. Even though this is unlikely and not allowed as stated in the Wiki, I wanted to eliminate any risk for damaging the HW. However the volumes will still add up when the the connector is not diconnected and you might blow you speakers (or your ears).

## July 2019

Snux has successfully made the attract mode of his 'F-14 Second Sortie' work with the APC:

[F-14 attract mode video](https://www.youtube.com/embed/XlqPyIdEk2k)

As long as the new display protocol is not implemented in MPF the display cannot be used.

### Change in the BOM

When Snux used his LED lamp tester it became obvious that there is some 'crosstalk' between the lamp columns. That means if a lamp is turned on it's counterpart in the previous lamp row shows a weak glow.

![LampFix](https://github.com/AmokSolderer/APC/blob/AmokWork/DOC/PICS/LampFix.jpg)

It's hardly visible with the normal light bulbs, but I wanted to fix it anyway. For this reason the value of the resistors R1 - R16 has been changed to speed up the column driver.

### New SW Version V0.10

* SW version number will now be shown during startup.
* The lamp timing has been changed slightly to suppress the 'crosstalk' between the lamp columns (see above)
* The USB display protocol has been updated as agreed with the Mini Lisy and MPF developers

## June 2019

First experiments with [Lisy Mini](https://lisy.dev/lisy_mini.html) which is used to run PinMame and control the APC via USB.

![Pic Mini Lisy](https://github.com/AmokSolderer/APC/blob/AmokWork/DOC/PICS/MiniLisy.png)

Attract mode of a Comet (System9) is already working. The display protocol still needs some work, but we're going to do that together with the MPF guys.
