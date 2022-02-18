# APC settings

## Selecting the correct display

The preselected display setting is set for early System11 displays. That means if you have a pre System11 (numbers only) display or a Black Knight 2000 (BK2K / 2 x 16 alphanumeric) you have to adjust the display setting first. In order to get there blindly, the display setting is the very first one.  
To get there you have to keep the Advance key pressed for more than 1s with the Up/Down key being in up position. Wait 4s and then press the Game Start button slowly but repeatedly until you can read the display contents. Pre Sys11 displays cannot show any text, so for the 6 digit displays (Sys3 - 6) a single 6 is shown on the player 4 display. For 7 digit displays (Sys7 and 9) it's a 7.  
After the correct display is selected, press Up/Down to get it down and Game Start to go back one setting to 'Exit Settings'. Now release the Up/Down button and press Game Start again to save the settings and return to the attract mode.

I did two videos to show what I've described above:

[How to use numerical displays](https://www.youtube.com/watch?v=2A5Tt9FQ2as)

[Adjusting the display settings for 2x16 segment System11 displays](https://www.youtube.com/watch?v=XqPWbm-HWM8)

## Using the settings menu

To enter the APC settings you have to press the Advance button at the coin door. If you're in Remote Control mode you have to keep it pressed for more than 1s.

In settings mode use Advance to select the setting and the Game Start button to change it's value. If the Up/Down button is in down position then you're moving backwards.

If an SD card is present the settings are stored when you choose 'Exit Settings'.

The APC settings are splitted in two parts.

* The System Settings are valid for all selected active games
* The Game Settings are game specific. They change depending on which 'Active Game' is selected in the System Settings.

Games with a credit display show 00 for System Settings and 01 for Game Settings in the left two digits. The number of the setting currently being shown appears in the right two digits of the credit display. This is important for pre System11 displays as they can show only numbers and no menu text. 

For pre Sys11 displays all text settings are represented by their item number as shown in the tables. For example, if a pre Sys11 display is selected and the credit display shows 00 01 you have selected the System Settings and the 'Active Game' setting is currently being shown. An early System11 display (upper display row alphanumerical and lower row numerical) will still try to show the name of the currently selected game as a text which is not perfect, but good enough. A pre System11 display will show the item number instead and if there is a 3 stated in the player 4 display then you're in Remote Control mode.

## System Settings

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 0 | Display Type | 0 | 4 Alpha+Credit | X | 4x 7 digit alphanumeric + credit |
| 0 |  | 1 | Sys11 Pinbot | - | 2x 7 digit alphanumeric + 2x 7 digit numeric + credit |
| 0 |  | 2 | Sys11 F-14 | - | 2x 7 digit alphanumeric + 2x 7 digit numeric |
| 0 |  | 3 | Sys11 BK2K | - | 2x 16 digit alphanumeric |
| 0 |  | 4 | Sys11 Taxi | - | 2x 16 digit alphanumeric + 1x 7 digit numeric|
| 0 |  | 5 | Sys11 Riverboat | - | 2x 16 digit alphanumeric + 2x 7 digit numeric|
| 0 |  | 6 | 6 | - | 4x 6 digit numeric + credit|
| 0 |  | 7 | 7 | - | 4x 7 digit numeric + credit|
| 1 | Active Game | 0 | Base Code | - | The very basics of a game SW |
| 1 |  | 1 | Black Knight | - | My own Black Knight game SW |
| 1 |  | 2 | Pinbot | - | My own Pinbot game SW |
| 1 |  | 3 | Remote Control | X | For controlling the APC remotely via USB or an onboard Raspberry Pi|
| 1 |  | 4 | Tutorial | - | The corresponding game SW to the game code tutorial Wiki|
| 2 | No of balls | - | - | 3 | Numerical setting - range 1 -5 |
| 3 | Free game | - | - | Yes | Bool setting |
| 4 | Connect Type | 0| Off | - | No remote control during Remote Control mode |
| 4 |  | 1 | On board | X | APC is controlled by the Pi on board |
| 4 |  | 2 | USB | - | APC is controlled via USB |
| 5 | Dim inserts | - | - | No | Bool setting - brightness of playfield lamps is set to 50% when on |
| 6 | Speaker volume | - | - | 0 | Numerical setting - range 1 - 255 / must be set to 0 when volume pot is connected at 10J4 / 1J16 |
| 7 | LED lamps | 0 | No LEDs | X | The APC_LED_exp board is not used |
| 7 |  | 1 | Additional | - | The APC_LED_exp board is used for the lamps 65+ |
| 7 |  | 2 | Playfld only | - | The APC_LED_exp board is only used for the lamps 9 - 64 |
| 7 |  | 3 | PlayfldBackbox | - | The APC_LED_exp board is used for the lamps 1 - 64 |
| 8 | Sol Exp Board | - | - | No | Bool setting - will use the solenoid expander board for solenoids 26 - 33 if set
| 9 | Debug Mode | - | - | No | Bool setting - Active debug mode will show the number of active timers in the credit display and will stop the game on error |
| 10 | Backbox Lamps | 0| Column 1 | X | Backbox lamps are in lamp column 1 |
| 10 |  | 1 | Column 8 | - | Backbox lamps are in lamp column 8 |
| 10 |  | 2 | None | - | game has no controlled backbox lamps |
| 11 | Restore Default | - | - | - | No setting - restores the default settings |
| 12 | Exit Settngs | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |

## Game Settings in Remote Control mode

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 0 | USB watchdog | - | - | No | Bool settings - disables all solenoids when no watchdog reset command is received for 1s |
| 1 | Debug Mode | 0 | Off | X | No debugging |
| 1 |  | 1 | USB | - | Shows the received commands in the displays |
| 1 |  | 2 | Audio | - | Audio debug mode for PinMame Sounds |
| 2 | PinMame Sound | 0 | APC | X | PinMame sounds are played on the APC sound HW |
| 2 | | 1 | Board | - | PinMame sounds are played on an external audio board |
| 3 | PinMame game | - | - | 0 | Numerical setting - PinMame game number |
| 4 | Lisy Debug | - | - | 0 | Numerical setting according to the [Controlling Lisy](https://github.com/AmokSolderer/APC/blob/master/DOC/LisyDebug.md) page |
| 5 | RestoreDefault | - | - | - | No setting - restores the default settings |
| 6 | Exit Settngs | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |
