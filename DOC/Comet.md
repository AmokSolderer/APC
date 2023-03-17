# The APC Comet

The basic game runs in PinMame as described in the [PinMame Section](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md).  
This page is about the PinMameExtensions I did for my Comet. You need to install the SW from the AmokPrivate branch on GitHub to run my special extensions. 

As you can see, my Comet is using my [4 Alpha+Credit](https://github.com/AmokSolderer/APC/blob/master/DOC/Sys7Alpha.md) displays. All extensions will also work with the original numerical displays except the custom text feature of course.  
The blue shine on the playfield is from the LED effects I added to my GI.

![APC Comet](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Comet.jpg)

This is a view inside the backbox.

![APC open Comet](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/CometLED.jpg)

You can see the [LED expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/LEDexpBoard.md) which is needed for the LED effects.

## Features

* Ball saver -> Comet's outlanes are special. Alas, this leads to lots of balls falling from the bumper area into the left outlane without any way for the player to prevent this. Therefore I added an optional ball saver to my PinMameExtensions which gives back any ball which is lost through an outlane when the last active switch before was in the bumper area.  
Watch [this video](https://youtu.be/JbgMa_pn0Lo) to see how the ball saver works.
* LED GI -> I'm using the [LED expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/LEDexpBoard.md) with some RGB-LED chain instead of the old GI lightbulbs. You can choose your GI color in the Game Settings.
* GI gameplay animations -> During normal gameplay the GI LEDs behave like the normal GI, but in some occasions when the game shows some fireworks animation with the backbox flashers the GI does the same on the playfield, which makes this effect much stronger.
* GI attract animations -> I also use the GI LEDs to play some colorful animations every now and then during attract mode. There's a video showing my [GI LEDs](https://youtu.be/kLWVUdhSwfo) in action
* Custom Text Message -> As my game has alphanumeric displays I'm showing a custom text message when the GI LEDs are showing their animations during attract mode. The text is retrieved from the file Custom.txt which has to be present on the SD card.
* Custom game music -> Comet's original music is ... basic. So to hear a song of your choice during gameplay, name it MUSIC.BIN, store it on the SD card and select 'Music bin' in the game settings.

## How to set up this game

* You need to have the audio files for this game. [Contact me](https://github.com/AmokSolderer/APC/tree/master#feedback) to get the corresponding sound file package.
* Use the 'AmokPrivate' branch of GitHub to get the additional features. The normal 'master' branch just supports Comet without any special features.
* For the GI LED animations you need to install the necessary HW of course, but the other features work without it. The same applies for the alphanumeric displays.

## Comet Game Settings

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 0 | USB watchdog | - | - | No | Bool setting - disables all solenoids when no watchdog reset command is received for 1s |
| 1 | Debug Mode | 0 | Off | X | No debugging |
| 1 |  | 1 | USB | - | Shows the received commands in the displays |
| 1 |  | 2 | Audio | - | Audio debug mode for PinMame Sounds |
| 2 | PinMame Sound | 0 | APC | X | PinMame sounds are played on the APC sound HW |
| 2 | | 1 | Board | - | PinMame sounds are played on an external audio board |
| 3 | PinMame game | - | - | 0 | Numerical setting - PinMame game number |
| 4 | Lisy Debug | - | - | 0 | Numerical setting according to the [Controlling Lisy](https://github.com/AmokSolderer/APC/blob/master/DOC/LisyDebug.md) page |
| 5 | Ball Saver | 0 | Off | X | No ball saver |
| 5 |  | 1 | Right Outlane | - | Ball saver only active for the right outlane |
| 5 |  | 2 | Left Outlane | - | Ball saver only active for the left outlane |
| 5 |  | 3 | Both Outlane | - | Ball saver active for both outlanes |
| 6 | LED GI red | - | - | 255 | Numerical setting (0-255) -> GI color setting red |
| 7 | LED GI green | - | - | 255 | Numerical setting (0-255) -> GI color setting green |
| 8 | LED GI blue | - | - | 255 | Numerical setting (0-255) -> GI color setting blue |
| 9 | BG Music | 0 | PinMame default | X | Normal Comet BG music |
| 9 |  | 1 | Music bin | - | Uses the MUSIC.bin file as BG music |
| 10 | Custom Text | - | - | No | Bool setting - Shows the custom text sometimes during attract mode |
| 11 | Restore Default | - | - | - | No setting - restores the default settings |
| 12 | Exit Settings | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |

## GI LEDs

These are the GI LEDs I've used. They match exactly into the holes for the normal lightbulbs.

![GI_LEDs](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/GI_LEDs.jpg)
