# The APC Black Knight

This page is about the BlackKnight.ino game SW which is part of the APC SW. Note that you have to have the Black Knight sound files for this game to work.  
Some features like the High Score table are only available when my '4 Alpha+Credit' display is used.

![APC Black Knight](https://github.com/AmokSolderer/APC/blob/V00.23/DOC/PICS/BK.jpg)

## Features

* Timed Magna Saves -> When activated the Magna Saves behave like in games like Jungle Lord, for the magnets are active as long as the corresponding button is being held and you still have Magna Save Time left. Clearing a drop target bank adds one second to the Magna Save Time. The Black Knight is lacking the playfield lamps to indicate the remaining Magna Save Time, so this is done by letting the Magna Save lamps blink. The slower the blinking frequency the more Magna Save Time is left until the lamps stay on permanently when the Magna Save Time has reached 5 seconds.
* Multi Ball Jackpot -> When activated a music score is played after Multiball has started. After all balls are released a music score starts and the lamp of the lower eject hole starts blinking. A shot to this hole enables the upper lock for Jackpot. After the Jackpot has been scored, the lower hole starts blinking and the Jackpot can be enabled again.
* Improved ball release -> It doesn't happen often but regularly that a ball ejected into the plunger lane bounced back from the side rail and into the trunk again. If it got back into the trunk completely the SW would recognize this and eject the ball again. But sometimes the ball didn't really get back into the trunk. Instead it got stuck above the shooter lane feeder and there wasn't much I could do about it except of opening the coin door and operating the feeder manually to push the ball into the shooter lane. The APC Black Knight SW will recognize that the ball is stuck and operate the feeder again.
* Ball search -> The SW features a ball search mode which is started when no playfield switch has been triggered for 30 seconds. The SW will recheck the locks and the ball through and will activate certain solenoids if a ball is missing.
* High Score table -> This feature is only available if '4 Alpha+Credit' is selected as the Display Type for it doesn't make much sense to enter initials no one is able to see. However, with the correct display installed you can enter you initials by using the Magna Save buttons. 

## How to set up this game

You need to have the audio files for this game. [Contact me](https://github.com/AmokSolderer/APC/tree/master#feedback) to get the corresponding sound file package.  

If you want to use the Multiball Jackpot feature you need to pick a music track to be played during Multi Ball. Take a track of your choice, convert it to the APC sound format and rename it to BK_M01.snd.  
The same needs to be done for the High Score table feature. In this case the name of the music track has to be BK_M02.snd.

## BK Game Settings

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 0 | Timed Magna | - | - | No | Bool setting | see features description |
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
| 8 | Exit Settngs | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |

## Things to do

* The Jackpot still needs some sound effects.
* The attract mode and display effects of this game are still taylored to the '4 Alpha+Credit' display, but as long as no one is using it with a standard display I see no need to change this, wouldn't be a big deal though. However, the basics like score display and so on are already working.

