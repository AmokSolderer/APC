# APC specialties

Usually the APC handles the pinball hardware quite straightforward. That means the lamp, switch and solenoid numbers used by the APC are the same as printed in the manual of your pinball machine, no matter whether you use MPF, Lisy/PinMame or write your own C code.  
However, there're some specialties which are handled in this section.

## Special switches

Williams assigned numbers only to Switches which belong to the switch matrix.  
The APC assigns numbers > 64 to the rest of them, so they can be used as normal switches:

| APC switch number | Williams equivalent |
|--|--|
| 65 | Activation switch for special solenoid 1 |
| 66 | Activation switch for special solenoid 2 |
| 67 | Activation switch for special solenoid 3 |
| 68 | Activation switch for special solenoid 4 |
| 69 | Activation switch for special solenoid 5 |
| 70 | Activation switch for special solenoid 6 |
| 71 | Memory Protect switch |
| 72 | Advance switch |
| 73 | Up/Down switch (active when the switch is in Up position) |

Additional information about these switches is provided below:

### Special solenoids and their switch numbers

Williams machines from System3 to System 11a have so called special solenoids. Each of these solenoids is connected to a special switch which will fire the solenoid immediately without any involvement of the CPU. This means that those special switches are not part of the switch matrix (which is CPU controlled). They are handled by a dedicated logic, are not visible for the CPU and therefore have no numbers.  
If your switch matrix has a switch for a special solenoid anyway, then it's because the CPU needs to know when it is triggered (e.g. for scoring). It's a second switch additional to the special solenoid switch. For example: in case of a pop bumper, the plastic disc to trigger the bumper is usually connected to the special solenoid switch and the scoring switch is triggered by the bumper ring being pulled down by the solenoid.  
This means that you have to select the special solenoid switch as the activation switch for your pop bumper, because the scoring switch is only activated when the solenoid is already moving.

The APC handles special solenoid switches as normal switches, but their numbers start with 65 which is the trigger switch for special solenoid 1.  

Williams dropped the special solenoid switches with the start of System 11b. After that the activation switches were part of the switch matrix and controlled by the CPU.

### The Advance, Memory Protect and Up/Down switch

Similar to the special solenoid switches, the Advance, Memory Protect and Up/Down switch are also not part of the switch matrix. The APC assignes the numbers > 70 to them. Note that the Up/Down switch is low active, which means that it's active when NOT pressed.

## The flipper relay

In the original Williams HW the flipper relay switches both flipper fingers at once and has no solenoid number.  
The APC doesn't use a relay but switching transistors. There's one for the left and one for the right flippers, so it is possible to activate the sides individually.  
Their solenoid numbers are 23 and 24, but any (de-)activation command sent to solenoid 25 will affect both flipper fingers.
