# APC solenoid expansion board

This page is only a placeholder as

THIS BOARD NOT YET FINISHED









































This board can be connected to the Hardware Extensions Interface of the APC and increases the number of controlled solenoids by 8. 

![SolExpBoard](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/SolExpBoard.jpg)

Currently the use of only one board is supported by the SW, but this could be changed if needed.

This board must be activated by setting the 'Sol Exp Board' setting in the 'System Settings' menu to 'yes'.  
Solenoids connected to the pins 1 to 8 of the P2 connector of the expander board are then mapped to the APC solenoid numbers 26 - 33.

## Hardware

The hardware just consists of a latch and some power MOSFET drivers. The latch is controlled by the Sel7 signal of the APC and is automatically reset when the blanking signal is active. The board requires an additional ground connection for the solenoid currents.

The schematic, Gerber (and drilling) are located in the [APC_Solenoid_exp](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_Solenoid_exp) folder.

