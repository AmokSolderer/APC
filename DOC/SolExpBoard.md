# APC solenoid expansion board (for use with loads having an additional power supply)

This board can be connected to the Hardware Extensions Interface of the APC and increases the number of controlled solenoids by 8.  
Note that this is a simple board without ground separation. This means you can only use this board with an own power supply. A typical application would be a shaker motor which usually needs 24V and therefore comes with an own 24V power supply. Connect the ground of this power supply to the Sol_GND connector of the Solenoid Expansion Board and not to the Solenoid Ground connector of the APC.

The prototype of the Solenoid Expansion Board looks like this. The final board looks slightly different.

![SolExpBoard](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/SolExpBoard.jpg)

Currently the use of only one board is supported by the SW, but this could be changed if needed.

This board must be activated by setting the 'Sol Exp Board' setting in the 'System Settings' menu to 'yes'.  
Solenoids connected to the pins 1 to 8 of the P2 connector of the expander board are then mapped to the APC solenoid numbers 26 - 33.

## Hardware

The hardware just consists of a latch and some power MOSFET drivers. The latch is controlled by the Sel7 signal of the APC and is automatically reset when the blanking signal is active. The board is only suited for use with an own power supply. Connect the ground of this supply to P3 (Sol_GND).

The schematic, Gerber (and drilling) are located in the [APC_Solenoid_exp](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_Solenoid_exp) folder.

# APC solenoid expansion board with separated grounds (for use with loads using the pinball's power supply)

Still to come.