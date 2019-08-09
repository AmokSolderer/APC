# Changelog of the Ardiono Pinball Controller

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

[![F-14 attract mode video](https://img.youtube.com/embed/XlqPyIdEk2k/0.jpg)](https://www.youtube.com/embed/XlqPyIdEk2k "F-14 attract mode video")

[F-14 attract mode video](https://www.youtube.com/embed/XlqPyIdEk2k)

We still have to work on the display protocol, so the display is not used yet.
