# Initial tests

## Preparation

For these tests you need to have all components except of the Raspberry Pi populated and the APC SW installed.  
For the audio test take your sound file of choice, convert it with the [Audio Converter](https://github.com/AmokSolderer/APC/blob/master/DOC/UsefulSWtools.md), rename it to MUSIC.BIN and copy it to an SD card which needs to be connected to P8 of the APC board.

## Switches and Displays

Put your board into your pinball machine, but only plug the connectors for Logic Power, switches and display: 

| Connector name | Number for System 3 - 7 | Number for System 9 + 11 |
|--|--|--|
| Logic Power | 1J2 | 1J17 |
| Switch Drivers | 2J2 | 1J8 |
| Switch Inputs | 2J3 | 1J10 |
| Diag_sw | 1J4 | 1J14 |
| Display Strobes 1 | 1J7 | 1J1 |
| Display Strobes 2 | 1J6 | 1J2 |
| Display Segments 1 | 1J5 | 1J3 |
| Display Segments 2 | - | 1J22 |

Now power up your pinball machine. After some status messages your display should show 'USB CONTROL'. If you just see two brightly illuminated digits then turn off your machine immediately, because in this case your display strobing doesn't work.  
If you have a pre System11 machine with numerical displays or a System11 2x16 digit one you have to adjust the display setting to make it work correctly. A description how to do it is in the [settings section](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md).

Even if your display works from the start you should enter the settings and browse through them to do a basic test of your switch circuitry.

Turn off your machine when the test is done.

## Solenoids

This step will test the solenoids. For this you have to connect the following connectors:

| Connector name | Number for System 3 - 7 | Number for System 9 + 11 |
|--|--|--|
| Solenoid GND | 2J10 | 1J13 |
| Solenoid Drivers 1 | 2J11 | 1J11 |
| Solenoid Drivers 2 | 2J9 | 1J12 |
| Special Sol Drivers | 2J12 | 1J19 |

If you hear the sound of an activating solenoid when turning on your machine then turn it off immediately, as it means that one of your solenoid drivers is stuck.  
If no solenoid is activated you can enter the 'System Settings' and set the 'Active Game' to 'Base Code'.  
Depending on your machine it might be possible that a solenoid is being periodically activated after you left the settings. This is not a problem as long as the solenoid is not turned on permanently and will probably be resolved after you have [set up the Base Code](https://github.com/AmokSolderer/APC/blob/master/DOC/SetUpBC.md) to your machine.

Turn off your machine when the test is done.

## Lamps and Audio

Now it's time for the lamps and the audio part.

| Connector name | Number for System 3 - 7 | Number for System 9 + 11 |
|--|--|--|
| Speakers | 10J2 | depends on sound config |
| Lamp B+ | 2J4 | 1J4 |
| Lamp GND | 2J6 | 1J5 |
| Lamp Strobes | 2J5 | 1J7 |
| Lamp Rows | 2J7 | 1J6 |

Turn on your machine. As you have an SD card plugged into the APC, the previous setting should have been stored and the Base Code should come up automatically. The attract mode of the base code has the lamps cycling from 1 to 64 which should be visible now.

## Using the Test Mode

Press the Advance button with Up/Down being in the down position to enter the test mode of the Base Code.  
The name of the current test is shown in the Display. For numerical displays the number of the test is shown in the Player 1 display. You can enter the test by pressing the game start button or proceed to the next test by pressing Advance again. The display, coil and single lamp tests are cycling automatically. This cycling can be stopped by the Up/Down button being in Down position.  
The following table shows the avaiable tests and their number which is shown for numerical displays.

| Test number | Test name |
|--|--|
| 1 | Display Test |
| 2 | Switch Edges |
| 3 | Coil Test |
| 4 | Single Lamp |
| 5 | All Lamps |
| 6 | Music Test |


