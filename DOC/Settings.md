# APC settings

## Selecting the correct display

The preselected display setting is set for early System11 displays. That means if you have a pre System11 (numbers only) display or a Black Knight 2000 (BK2K / 2 x 16 alphanumeric) you have to adjust the display setting first. In order to get there blindly, the display setting is the very first one.  
To get there you have to keep the Advance key pressed for more than 1s with the Up/Down key being in up position. Wait 4s and then press the Game Start button slowly but repeatedly until you can read the display contents. Pre Sys11 displays cannot show any text, so for the 6 digit displays (Sys3 - 6) a single 7 is shown on the player 4 display. For 7 digit displays (Sys7 and 9) it's an 8.  
After the correct display is selected, press Up/Down to get it down and Game Start to go back one setting to 'Exit Settings'. Now release the Up/Down button and press Game Start again to save the settings and return to the attract mode.

The complete re-initialization of the display is done when you EXIT the system settings. That means your lower row might still look a bit odd even though you've selected the correct display until you exit the settings.

I did two videos to show what I've described above:

[How to use numerical displays](https://www.youtube.com/watch?v=2A5Tt9FQ2as)

[Adjusting the display settings for 2x16 segment System11 displays](https://www.youtube.com/watch?v=XqPWbm-HWM8)

Note that the menus have changed since these videos have been made. So use them only to see how it works in general, but use the tables below as a reference for the settings.

## Using the settings menu

To enter the APC settings you have to press the Advance button at the coin door with the Up/Down switch in 'up' position. If you're in Remote Control mode you have to keep it pressed for more than 1s.

After that you have to select whether you want to enter the system or game settings.

* The System Settings are valid for all selected active games
* The Game Settings are game specific. They change depending on which 'Active Game' is selected in the System Settings. This is not identical to the game you might want to run in PinMame, but you have to set your 'Active Game' to [Remote Control mode](https://github.com/AmokSolderer/APC/blob/master/DOC/RunGame.md#pinmame) in order to run PinMame.

Games with a credit display show 00 for System Settings and 01 for Game Settings in the left two digits. The number of the setting currently being shown appears in the right two digits of the credit display. This is important for pre System11 displays as they can show only numbers and no menu text. 

For pre Sys11 displays all text settings are represented by their item number as shown in the tables. For example, if a pre Sys11 display is selected and the credit display shows 00 01 you have selected the System Settings and the 'Active Game' setting is currently being shown. An early System11 display (upper display row alphanumerical and lower row numerical) will still try to show the name of the currently selected game as a text which is not perfect, but good enough. A pre System11 display will show the item number instead and if there is a 3 stated in the player 4 display then you're in Remote Control mode.

In settings mode use Advance to select the setting and the Game Start button to change it's value. If the Up/Down button is in down position then you're moving backwards.

If an SD card is present the settings are stored when you choose 'Exit Settings'.

## Using PinMame settings

While you're running PinMame you can adjust the original Williams System 4 - 11 adjustments as usual with one exception:  
You must not keep Advance pressed for more than 1 second with Up/Down in up position as this will trigger the APC settings. If you want to browse the Williams Settings quickly, just do it backwards with Up/Down in down position.

### System 3 game adjustments

Doing game adjustments on a System 3 game is done with DIP switches. The procedure is quite cumbersome and error prone which is why we're using the APC's settings menu instead.  
The following description works only for System 3 games being controlled by Lisy/PinMame, for all other generations the adjustment process hasn't changed from the original.

Keep the Advance switch pressed for more than one second with the Up/Down switch in up position. This will end PinMame and enter the settings menu of the APC. The content of the 18 System 3 adjustments is now stored in the 'Game Settings' 46 to 63 as shown in the table below (Game Settings in Remote Control mode).  
Enter the game settings and do your changes. When you're done, just select menu entry 65 (Exit Settings) and press the start button. After a few seconds PinMame will restart with the new settings applied.

## System Settings

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 0 | Display Type | 0 | 4 Alpha+Credit | X | 4x 7 digit alphanumeric + credit |
| 0 |  | 1 | Sys11 Pinbot | - | 2x 7 digit alphanumeric + 2x 7 digit numeric + credit |
| 0 |  | 2 | Sys11 F-14 | - | 2x 7 digit alphanumeric + 2x 7 digit numeric |
| 0 |  | 3 | Sys11 BK2K | - | 2x 16 digit alphanumeric (inverted segments)|
| 0 |  | 4 | Sys11 Taxi | - | 2x 16 digit alphanumeric + 1x 7 digit numeric (non inverted segments)|
| 0 |  | 5 | Sys11 Riverboat | - | 2x 16 digit alphanumeric + 2x 7 digit numeric (inverted segments)|
| 0 |  | 6 | Data East 2x16 | - | 2x 16 digit alphanumeric (non inverted segments)|
| 0 |  | 7 | 7 | - | 4x 6 digit numeric + credit (System 3 - 6)|
| 0 |  | 8 | 8 | - | 4x 7 digit numeric + credit (System 7 + 9)|
| 1 | Active Game | 0 | Base Code | - | The very basics of a game SW |
| 1 |  | 1 | Black Knight | - | My own Black Knight game SW |
| 1 |  | 2 | Pinbot | - | My own Pinbot game SW |
| 1 |  | 3 | Remote Control | X | For controlling the APC remotely via USB or an onboard Raspberry Pi|
| 1 |  | 4 | Tutorial | - | The corresponding game SW to the game code tutorial Wiki|
| 2 | No of balls | - | - | 3 | Numerical setting - range 1 -5 |
| 3 | Free game | - | - | Yes | Bool setting |
| 4 | Connect Type | 0| Off | - | No remote control during Remote Control mode |
| 4 |  | 1 | On board | - | APC is controlled by the Pi on board |
| 4 |  | 2 | USB | X | APC is controlled via USB |
| 5 | Dim inserts | - | - | No | Bool setting - brightness of playfield lamps is set to 50% when on |
| 6 | Speaker volume | - | - | 0 | Numerical setting - range 1 - 255 / must be set to 0 when volume pot is connected at 10J4 / 1J16 |
| 7 | LED lamps | 0 | No LEDs | X | The APC_LED_exp board is not used |
| 7 |  | 1 | Additional | - | The APC_LED_exp board is used for the lamps 65+ |
| 7 |  | 2 | Playfld only | - | The APC_LED_exp board is only used for the lamps 9 - 64 |
| 7 |  | 3 | PlayfldBackbox | - | The APC_LED_exp board is used for the lamps 1 - 64 |
| 8 | No of LEDs | - | - | 64 | Numerical setting - range 1 - 192 / The length of the LED stripe. Setting is only effective when 'Additional' is selected as 'LED lamps' setting|
| 9 | Sol Exp Board | - | - | No | Bool setting - will use the solenoid expander board for solenoids 26 - 33 if set
| 10 | Debug Mode | - | - | No | Bool setting - Active debug mode will show the number of active timers in the credit display and will stop the game on error |
| 11 | Backbox Lamps | 0| Column 1 | X | Backbox lamps are in lamp column 1 |
| 11 |  | 1 | Column 8 | - | Backbox lamps are in lamp column 8 |
| 11 |  | 2 | None | - | game has no controlled backbox lamps |
| 12 | Connect Startup | - | - | No | Bool setting - If active, then the connection type is not set by setting 4 (Connect Type) but by the state of the Up/Down switch during startup (and when leaving the settings). Switch in Up state selects the onboard Pi, otherwise the USB programming port |
| 13 | Restore Default | - | - | - | No setting - restores the default settings |
| 14 | Exit Settings | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |

## Game Settings in Remote Control mode

These game settings are only visible if 'Remote Control' is selected as the 'Active Game' in the 'System Settings'.

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 0 | USB watchdog | - | - | No | Bool settings - disables all solenoids when no watchdog reset command is received for 1s |
| 1 | Debug Mode | 0 | Off | X | No debugging |
| 1 |  | 1 | USB | - | Shows the received commands in the displays |
| 1 |  | 2 | Audio | - | Audio debug mode for PinMame Sounds |
| 2 | PinMame Sound | 0 | APC | X | PinMame sounds are played on the APC sound HW |
| 2 | | 1 | Board | - | PinMame sounds are played on an external audio board |
| 3 | PinMame game | - | - | 0 | Numerical setting - [PinMame game number](https://github.com/AmokSolderer/APC/blob/master/DOC/lisyminigames.csv) |
| 4 | Lisy Debug | - | - | 0 | Numerical setting according to the [Controlling Lisy](https://github.com/AmokSolderer/APC/blob/master/DOC/LisyDebug.md) page |
| 5 | Ball Saver | 0 | Off | X | The optional ball saver is not active |
| 5 |  | 1 | On | - | The ball saver is active for all drained balls |
| 6 | Ball Saver Time | - | - | 20 | Numerical setting - range 5 - 250 / Active time of the Ball Saver |
| 7 | BG Music | 0 | PinMame default | X | Normal BG music |
| 7 |  | 1 | Music snd | - | Uses the MUSIC.SND file as BG music |
| 8 - 45 | Setting Unused | - | – | - | They behave like boolean settings, but they have no effect |
| 46 | System3 Set 1 | - | - | - | Use this to change the 1st setting of system 3 games |
| 47 | System3 Set 2 | - | - | - | Use this to change the 2nd setting of system 3 games |
| 48 | System3 Set 3 | - | - | - | Use this to change the 3rd setting of system 3 games |
| 49 | System3 Set 4 | - | - | - | Use this to change the 4th setting of system 3 games |
| 50 | System3 Set 5 | - | - | - | Use this to change the 5th setting of system 3 games |
| 51 | System3 Set 6 | - | - | - | Use this to change the 6th setting of system 3 games |
| 52 | System3 Set 7 | - | - | - | Use this to change the 7th setting of system 3 games |
| 53 | System3 Set 8 | - | - | - | Use this to change the 8th setting of system 3 games |
| 54 | System3 Set 9 | - | - | - | Use this to change the 9th setting of system 3 games |
| 55 | System3 Set 10 | - | - | - | Use this to change the 10th setting of system 3 games |
| 56 | System3 Set 11 | - | - | - | Use this to change the 11th setting of system 3 games |
| 57 | System3 Set 12 | - | - | - | Use this to change the 12th setting of system 3 games |
| 58 | System3 Set 13 | - | - | - | Use this to change the 13th setting of system 3 games |
| 59 | System3 Set 14 | - | - | - | Use this to change the 14th setting of system 3 games |
| 60 | System3 Set 15 | - | - | - | Use this to change the 15th setting of system 3 games |
| 61 | System3 Set 16 | - | - | - | Use this to change the 16th setting of system 3 games |
| 62 | System3 Set 17 | - | - | - | Use this to change the 17th setting of system 3 games |
| 63 | System3 Set 18 | - | - | - | Use this to change the 18th setting of system 3 games |
| 64 | Restore Default | - | - | - | No setting - restores the default settings |
| 65 | Exit Settings | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |
