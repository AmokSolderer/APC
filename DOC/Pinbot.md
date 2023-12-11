# The APC Black Knight

This page is about the Pinbot.ino game SW which is part of the APC SW. Note that you have to have the Pinbot sound files for this game to work.  

![APC Black Knight](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Pinbot.JPG)

This is a view inside the backbox.

![APC open BK](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/APC_Pinbot.JPG)

I'm using an APC 2 prototype board for this. As I'm running the code natively on the Arduino I don't need Lisy/PinMame and therefore no Raspberry Pi.

## Features

* Ball Saver -> Pinbot is an outlane monster and when a ball drains in an outlane right after it was launched then I'm really getting furious. Hence there's an optional Ball Saver which saves any ball draining in an outlane before any flipper buttons had been pressed.
* 3 ball Multiball -> Pinbot's multiball is kind of lame IMHO. That's why there's a 3 ball multiball option in my SW. Activate the option, add a third ball and enjoy a different ruleset introducing an unlimited million Jackpot.

## 3 ball Multiball

Switching the 'Multiball' game setting to '3 ball' (and adding a third ball) switches to a different ruleset.  
Like the original Pinbot, this one will also explain it's rules if you keep it waiting a while in Attract Mode. Depending on the 'Multiball' settings it will explain the normal Pinbot rules (2 ball) or the new 3 ball rules.

The rules for the 3 ball mode are shown [here](

## How to set up this game

You need to have the audio files for this game. [Contact me](https://github.com/AmokSolderer/APC/tree/master#feedback) to get the corresponding sound file package.  

If you want to use the 3 ball Multiball you have to add a third ball apparently. Don't forget to remove it when you switch back to 3 ball mode.

## BK Game Settings

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 0 | Drop TG time | - | - | No | Bool setting - see features description |
| 1 | Replay Score | 0 | 1000000 | X | Set 1 million as replay score |
| 1 |  | 1 | 1500000 | - | Set 1.5 million as replay score |
| 1 |  | 2 | 2000000 | - | Set 2 million as replay score |
| 1 |  | 3 | 2500000 | - | Set 2.5 million as replay score |
| 2 | Multiball Jackpot | 0 | Off | X | Turn the multiball jackpot off |
| 2 |  | 1 | 500000 | - | Set 500K as multiball jackpot |
| 2 |  | 2 | 750000 | - | Set 750K as multiball jackpot |
| 2 |  | 3 | 1000000 | - | Set 1 million as multiball jackpot |
| 3 | Multiball volume | - | - | 0 | Numerical setting - range 0 - 30 / can be used to increase the volume during multiball. Does only work when the digital volume control of the APC is used and not the old volume pot |
| 4 | High Score volume | - | - | 0 | Numerical setting - range 0 - 30 / can be used to increase the volume during high score entry. Does only work when the digital volume control of the APC is used and not the old volume pot |
| 5 | Ball eject strength  | - | - | 30 | Numerical setting - range 0 - 30 / activation time of the ball ramp thrower (solenoid 6) in ms. Adjust this to avoid that the ball is bouncing back into the trunk
| 6 | Reset High | - | - | - | No setting - resets the high scores for this game |
| 7 | Restore Default | - | - | - | No setting - restores the default settings |
| 8 | Exit Settings | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |

Check the [settings page](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md#using-the-settings-menu) if you're not sure how to use these game settings.

