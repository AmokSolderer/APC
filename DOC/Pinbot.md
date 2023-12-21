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

The the 3 ball mode keeps Pinbot's basic rules, which means that opening the visor, extra balls and so on work exactly as before. The  main differences are:

* The skill shot multiplier (Vortex) does not increase with every new launch but stays 1
* When the visor has been opened and two balls have been locked in the eyes, the visor closes and a third ball is put into play. This ball must be shot into the eject hole left of the visor to start multiball
* As long as 3 ball multiball is running, the ramp is lit for jackpots. The first jackpot is worth 200,000 points, the second 400,000 and so on. At 1,000,000 points it won't increase any more, but can be scored as long as 3 balls are in play.
* During 3 ball multiball, the eyes and the eject hole hold balls for an adjustable hold time (10s by default)
* If a ball goes from the ramp to the shooter lane, no jackpots can be scored until the ball has been launched
* After one ball has drained, the visor won't close and the remaining two balls can be locked in the eyes again to re-lite the eject hole.
* If a second ball drains, the visor closes and needs to be re-opened with the chest lamps

Like the original Pinbot, the APC SW also explains the rules if the machine is in attract mode for a while. Depending on which multiball mode is selected in the game settings, it will explain the original 2 ball or the 3 ball rules as shown [here](https://youtu.be/IGrUnbhkijU).

## How to set up this game

You need to have the audio files for this game. [Contact me](https://github.com/AmokSolderer/APC/tree/master#feedback) to get the corresponding sound file package.

If you want to use the 3 ball Multiball you have to add a third ball apparently. Don't forget to remove it when you switch back to 2 ball mode.

You might have to adjust the 'Eject Strength' setting. It should be selected for the outhole kicker to be strong enough to put one ball into the trunk, but not two. The default value should be OK for most machines, but if your outhole kicker is somewhat special you might have to change this setting.  
The reason for this is that Pinbot's ball through is lacking a switch to detect a third ball. Therefore it's difficult to detect a double drain of the last two balls. The SW tries to detect whether one or two balls are in the outhole by limiting the strength of the outhole kicker. If the first kick was not successfull, the second one is done much stronger and the SW assumes this to be a double drain.

## Pinbot Game Settings

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

