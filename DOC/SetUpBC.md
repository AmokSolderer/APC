# Setting up the Base Code

To switch to the Base Code you have to select the Base Code in the System Settings (Setting 1 'Active Game' should be set to 0 'Base Code'). Look at the [Settings page](https://github.com/AmokSolderer/APC/blob/master/DOC/Settings.md) to learn how to do this.

In order for the Base Code to work properly, you have to set it up with the properties of your machine. Usually you would do this in the code itself, but for the Base Code this has been implemented in the 'Game Settings'.  
To get there press the Advance button while running the Base Code, then enter the 'Game Settings' which are shown below:

| Number | Text  | Item Nr | Item Text | Default | Comment |
|--|--|--|--|--|--|
| 0 | Outhole Switch | - | - | 9 | Numerical setting - Number of the Outhole Switch |
| 1 | Ball Thru 1 | - | - | 13 | Numerical setting - Number of the 1st Ball Through Switch |
| 2 | Ball Thru 2 | - | - | 12 | Numerical setting - Number of the 2nd Ball Through Switch, if there is any |
| 3 | Ball Thru 3 | - | - | 11 | Numerical setting - Number of the 3rd Ball Through Switch, if there is any |
| 4 | Plunger Ln Sw | - | - | 20 | Numerical setting - Number of the Plunger Lane Switch |
| 5 | AC Sel Relay | - | - | 12 | Numerical setting - Number of the A/C relay solenoid, set to zero for pre System11 games |
| 6 | Outhole Kicker | - | - | 1 | Numerical setting - Number of the Outhole Kicker solenoid |
| 7 | Shooter Ln Feed | - | - | 2 | Numerical setting - Number of the Shooter Lane Feeder solenoid |
| 8 | Instald Balls | - | - | 3 | Numerical setting - Number of balls installed |
| 9 | RestoreDefault | - | - | - | No setting - restores the default settings |
| 10 | Exit Settngs | - | - | - | No setting - exits the settings mode and writes the new setting to an SD card if present |

You have to get the required values from the manual of your pinball machine. Alas, the names of the switches and solenoids have changed over the years.  
The default values are valid for a System11c Rollergames and I have added the corresponding manual pages below to make it easier for you to identify the right parts.

For single ball games set the 'Installed Balls' setting to 1. The shooter lane feeder should be set to your Outhole Kicker solenoid.

![RollergamesSwitches](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/RG_Sw.JPG)

![RollergamesSolenoids](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/RG_Sol.JPG)
