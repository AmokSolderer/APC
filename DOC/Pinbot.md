# The APC Pinbot

This page is about the Pinbot.ino game SW which is part of the APC SW. Note that you have to have the Pinbot sound files for this game to work.  

![APC Pinbot](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Pinbot.JPG)

This is a view inside the backbox.

![APC open PB](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Pinbot.JPG)

I'm using an APC 2 prototype board for this. As I'm running the code natively on the Arduino I don't need Lisy/PinMame and therefore no Raspberry Pi.

## Features

* Ball Saver -> Pinbot is an outlane monster and when a ball drains in an outlane right after it was launched then I'm really getting furious. Hence there's an optional Ball Saver which saves any ball draining in an outlane before any flipper buttons had been pressed.
* 3 ball Multiball -> Pinbot's multiball is kind of lame IMHO. That's why there's a 3 ball multiball option in my SW. Activate the option, add a third ball and enjoy a different ruleset introducing an unlimited million Jackpot.

## 3 ball Multiball

Switching the 'Multiball' game setting to '3 ball' (and adding a third ball) switches to a different ruleset.  
Like the original Pinbot, this one will also explain it's rules if you keep it waiting a while in Attract Mode. Depending on the 'Multiball' settings it will explain the normal Pinbot rules (2 ball) or the new 3 ball rules.

The rules for the 3 ball mode are shown [here](https://youtu.be/IGrUnbhkijU).

## How to set up this game

You need to have the audio files for this game. [Contact me](https://github.com/AmokSolderer/APC/tree/master#feedback) to get the corresponding sound file package.  

If you want to use the 3 ball Multiball you have to add a third ball apparently. Don't forget to remove it when you switch back to 3 ball mode.

## BK Game Settings

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 0 | Drop TG Time | - | - | 15 | Down time of the drop targets |
| 1 | Reach Planet | 0 | Pluto | - | Planet to reach for Extra Ball |
| 1 | | 1 | Neptune | - |  |
| 1 | | 2 | Uranus | - |  |
| 1 | | 3 | Saturn | - |  |
| 1 | | 4 | Jupiter | - |  |
| 1 | | 5 | Mars | - |  |
| 1 | | 6 | Earth | X |  |
| 1 | | 7 | Venus | - |  |
| 1 | | 8 | Mercury | - |  |
| 2 | Energy Timer | - | - | 15 | Time to collect energy |
| 3 | Multiball | 0 | 2 Ball | - | 2 ball multiball (Normal Pinbot) gameplay - see features description|
| 3 | | 1 | 3 Ball | - | 3 ball multiball gameplay |
| 4 | Ball Saver | - | - | No | Bool setting - see features description |
| 5 | Eject Strength | - | - | 30 | Strength of the outhole kicker (debug option)|
| 6 | Hold Time | - | - | 10 | Time to hold balls in locks during 3 ball multiball |
| 7 | Reset High | - | - | - | No setting - resets the high scores for this game |
| 8 | Restore Default | - | - | - | No setting - restores the default settings |
| 9 | Exit Settings | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |

Check the [settings page](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md#using-the-settings-menu) if you're not sure how to use these game settings.

