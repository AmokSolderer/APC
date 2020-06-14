# APC settings

## System settings

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 1 | Display Type | 1 | 4 Alpha+Credit | X | 4x 7 digit alphanumeric + credit |
| 1 |  | 2 | Sys11 Pinbot | - | 2x 7 digit alphanumeric + 2x 7 digit numeric + credit |
| 1 |  | 3 | Sys11 F-14 | - | 2x 7 digit alphanumeric + 2x 7 digit numeric |
| 1 |  | 4 | Sys11 BK2K | - | 2x 16 digit alphanumeric |
| 1 |  | 5 | Sys11 Taxi | - | 2x 16 digit alphanumeric + 1x 7 digit numeric|
| 1 |  | 6 | Sys11 Riverboat | - | 2x 16 digit alphanumeric + 2x 7 digit numeric|
| 1 |  | 7 | 123456123456 | - | 4x 6 digit numeric + credit|
| 1 |  | 8 | 12345671234567 | - | 4x 7 digit numeric + credit|
| 2 | Active Game | 1 | Base Code | - | The very basics of a game SW |
| 2 |  | 2 | Black Knight | - | My own Black Knight game SW |
| 2 |  | 3 | Pinbot | - | My own Pinbot game SW |
| 2 |  | 4 | USB Control | X | For controlling the APC via I2C or USB |
| 2 |  | 5 | Tutorial | - | The corresponding game SW to the game code tutorial Wiki|
| 3 | No of balls | - | - | 3 | Numerical setting - range 1 -5 |
| 4 | Free game | - | - | Yes | Bool setting |
| 5 | Connect Type | 1 | Off | - | No remote control during USBcontrol mode |
| 5 |  | 2 | On board | X | APC can be controlled by the Pi on board |
| 5 |  | 3 | USB | - | APC can be controlled via USB |
| 6 | Dim inserts | - | - | No | Bool setting - brightness of playfield lamps is set to 50% when on |
| 7 | Speaker volume | - | - | 0 | Numerical setting - range 1 - 255 / must be set to 0 when volume pot is connected at 10J4 / 1J16 |
| 8 | LED lamps | 1 | No LEDs | X | The APC_LED_exp board is not used |
| 8 |  | 2 | Playfld only | - | The APC_LED_exp board is only used for the lamps 9 - 64 |
| 8 |  | 3 | PlayfldBackbox | - | The APC_LED_exp board is used for all lamps |
| 9 | Debug Mode | - | - | No | Bool setting - Active debug mode will show the number of active timers in the credit display and will stop the game on error |
| 10 | RestoreDefault | - | - | - | No setting - restores the default settings |
| 11 | Exit Settngs | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |

## Game settings in USBcontrol mode

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 1 | USB watchdog | - | - | No | Bool settings - disables all solenoids when no watchdog reset command is received for 1s |
| 2 | Debug Mode | 1 | Off | X | No debugging |
| 2 |  | 2 | USB | - | Shows the received USB/I2C commands in the displays |
| 2 |  | 3 | Audio | - | Audio debug mode for PinMame Sounds |
| 3 | Lisy Mode | 1 | PinMame | X | Start PinMame |
| 3 | | 2 | MPF | - | Start MPF |
| 3 | | 3 | Control | - | Start LISYcontrol |
| 3 | | 4 | Debug | - | Start Lisy with debug |
| 4 | PinMame game | - | - | 0 | Numerical setting - PinMame game number |
| 5 | Debug Display | - | - | No | Enable Lisy Display debugging |
| 6 | Debug Switch | - | - | No | Enable Lisy Switch debugging |
| 7 | Debug Lamp | - | - | No | Enable Lisy Lamp debugging |
| 8 | Debug coil | - | - | No | Enable Lisy Solenoid debugging |
| 9 | Debug Sound | - | - | No | Enable Lisy Sound debugging |
| 10 | PinMame Sound | 1 | APC | X | PinMame sounds are played on the APC sound HW |
| 10 | | 2 | Board | - | PinMame sounds are played on the external audio board |
| 11 | RestoreDefault | - | - | - | No setting - restores the default settings |
| 12 | Exit Settngs | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |
