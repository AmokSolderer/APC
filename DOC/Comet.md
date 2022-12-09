# The APC Comet

The basic game runs in PinMame as described in the [PinMame Section](https://github.com/AmokSolderer/APC/blob/master/DOC/PinMame.md).  
This page is about the PinMameExtensions I did for my Comet. You need to install the SW from the AmokPrivate branch on GitHub to run my special extensions. 

As you can see, my Comet is using my [4 Alpha+Credit](https://github.com/AmokSolderer/APC/blob/master/DOC/Sys7Alpha.md) displays. All extensions will also work with the original numerical displays except the custom text feature of course.  
The blue shine on the playfield is from the LED effects I added to my GI.

![APC Comet](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/PICS/Comet.jpg)

This is a view inside the backbox.

![APC open Comet](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/PICS/CometLED.jpg)

You can see the [LED expansion board](https://github.com/AmokSolderer/APC/blob/master/DOC/LEDexpBoard.md) which is needed for the LED effects.

## Features

* Ball saver -> Comet's outlanes are special. Alas, this leads to lots of balls falling from the bumper area into the left outlane without any way for the player to prevent this. Therefore I added an optional ball saver to my PinMameExtensions which gives back any ball which is lost through an outlane when the last active switch before was in the bumper area.  
Watch [this video](https://youtu.be/JbgMa_pn0Lo) to see how the ball saver works.


## How to set up this game

You need to have the audio files for this game. [Contact me](https://github.com/AmokSolderer/APC/tree/master#feedback) to get the corresponding sound file package.  

If you want to use the Multiball Jackpot feature you need to pick a music track to be played during Multi Ball. Take a track of your choice, convert it to the APC sound format and rename it to BK_M01.snd.  
The same needs to be done for the High Score table feature. In this case the name of the music track has to be BK_M02.snd.

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

## Things to do

* The Jackpot still needs some sound effects.
* The attract mode and display effects of this game are still taylored to the '4 Alpha+Credit' display, but as long as no one is using it with a standard display I see no need to change this, wouldn't be a big deal though. However, the basics like score display and so on are already working.

