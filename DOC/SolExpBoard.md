# APC solenoid expansion board (without ground separation)

This board can be connected to the Hardware Extensions Interface of the APC and increases the number of controlled solenoids by 8.  
Note that this is a simple board without ground separation. This means you can only use this board for smaller load currents or for loads with an own power supply. A typical application would be a shaker motor which usually needs 24V and therefore comes with an own 24V power supply. Connect the ground of this power supply to the Sol_GND connector of the Solenoid Expansion Board and not to the Solenoid Ground connector of the APC.

The prototype of the Solenoid Expansion Board looks like this. The final board looks slightly different.

![SolExpBoard](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/SolExpBoard.JPG)

Currently the use of only one board is supported by the SW, but this could be changed if needed.

This board must be activated by setting the 'Sol Exp Board' setting in the 'System Settings' menu to 'yes'.  
Solenoids connected to the pins 1 to 8 of the P2 connector of the expander board are then mapped to the APC solenoid numbers 26 - 33.

### Use in Whirlwind

The System11b Whirlwind uses a Sound Overlay Solenoid Board to have 5 additional solenoid drivers. You may use the APC solenoid expansion board instead, but be sure to activate the 'Sol Exp Board' setting in the 'System Settings' menu before you start PinMame.  
The reason is that the Sound Overlay Solenoid Board inverts all solenoid signals, but the APC solenoid expansion board doesn't. Hence PinMame sends inverted commands for the 5 additional solenoid drivers and the APC needs to know that the APC solenoid expansion board is used to correct this.

## Hardware

The hardware just consists of a latch and some power MOSFET drivers. The latch is controlled by the Sel7 signal of the APC and is automatically reset when the blanking signal is active. The board is only suited for use with an own power supply. Connect the ground of this supply to P3 (Sol_GND).

The schematic, Gerber (and drilling) are located in the [APC_Solenoid_exp](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_Solenoid_exp) folder.

# APC solenoid expansion board 2 (with ground separation)

This board can be connected to the Hardware Extensions Interface of the APC and increases the number of controlled solenoids by 8.  
Other than the board above, this one has a ground separation. That means the signal ground (coming from the APC) is separated from the load ground. Hence this board should be used if you want to control additional solenoids which share the normal solenoid ground of your pinball machine.  
In this case connect the Sol_GND ground (J2 on this board) to the Solenoid Ground connector of the APC (sys7_2J10/sys11_1J13). The signal ground is provided by the Hardware Extensions Interface of the APC.

![SolExpBoard2](https://github.com/AmokSolderer/APC/blob/V01.04/DOC/PICS/SolExpBoard2.png)

## Hardware

The hardware is very similar to the Solenoid Expansion board described above, but every solenoid driver features an additional ground shifting stage. It consists of an ULN2803 providing a control current to the signal ground and a PNP transistor using this current to provide a second current to the power ground. This current is used by a 4.7K resistor to generate the gate source voltage for the power MOSFET.  
Connect the Sol_GND (J2) of this board to the ground of your load.

The schematic, Gerber (and drilling) are located in the [APC_Solenoid_exp2](https://github.com/AmokSolderer/APC/tree/V01.04/DOC/Hardware/APC_Solenoid_exp2) folder.
