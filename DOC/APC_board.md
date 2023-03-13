# The APC board

First you should know what you're dealing with, so take a look at the [APC schematics](https://github.com/AmokSolderer/APC/blob/master/DOC/Hardware/APC_schematics.pdf). I have tried to design the hardware simple and straightforward, that means with a bit of hardware knowledge it shouldn't be a problem for you to understand how it works.

In the schematics the names of the connectors are given for System7 and System11, but the System7 names are also valid for System3 - 6 and the System11 names match to System9. The following picture should help you to determine where the various connectors belong. The direction of the Molex connectors is printed on the boards and also visible in the picture below: the friction lock belongs to the side where the additional thin line is drawn.  
The solenoid GND connector of Sys11 games (1J13) has only 4 pins, in the picture it is marked as a black rectangle. The X symbols indicate which pins have to be removed.

![APC connectors](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/PICS/APC_Connectors.png)

However, the APC is only suited for you if you have some basic knowledge of electronics, because you can easily damage your precious pinball machine when you don't do things right.

## Order a board

It has become increasingly difficult to get all the required parts. Especially the 74HCT273 seem to be a problem and it sometimes happens that they're only available in a certain package. I have therefore split up the fabrication files section into two parts, one with the 74HCT273 in an [SOIC package](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_FabricationFiles_SOIC) and one with a [SSOP package](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/APC_FabricationFiles_SSOP). Both folders contain all the required data to build a board, just the package of the 74HCT273 is different.

I recommend to use [JLCPCB](https://jlcpcb.com) as your board manufacturer, because the assembly files are tailored to their specifications. The problem with the manufacturers is that they need at least an order of five boards to come up with a reasonable price, so please leave some feedback at the Arduino Forum, may be someone from your country has a board for sale.  
If you want to place an order, the manufacturer will need the APC_Gerber.zip file to build the boards. You also have to select 'SMT Assembly' and provide the APC_BOM.csv and APC-top-pos.csv files to make them populate the parts.

## The Components

Most of the components can be populated by the board manufacturer. They can do all SMD and most of the other stuff as well. Hence, there're only a few components left for you to assemble. You can find a list of these components with their respective order number from [Mouser](http://www.mouser.com) and [Reichelt](http://www.reichelt.de) in the [Bill of Materials](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/Hardware/Assembly/APC_BOMselfSolder.pdf)

The german electronics retailer Reichelt doesn't sell all required components, but some (especially connectors) are much cheaper compared to Mouser, so for people in Europe it might still make sense to order them separately.

### TDA7496 availability

I got a notification from Mouser that the TDA7496 audio amplifier IC won't be produced any longer.

At the moment I don't have the time to select a new amplifier IC, change the board design and do the necessary tests. I might do it in the future if the IC is not available any more.  
That means you should try to get these ICs before you order any boards.

## The assembly

As the boards have the component names printed at the corresponding locations, you can just use the [Bill of Materials](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/Hardware/Assembly/APC_BOMselfSolder.pdf) to identify the  right component to put there.  
The resistor network RR8 has to be populated in the correct orientation. There's a marking for pin 1 printed on the APC boards. On the resistor networks pin 1 is usually marked with a dot.

## Getting your board started

Plug the Arduino DUE on your APC board, but don't populate the Pi yet. I recommend to do the basic tests before assembling the Pi.
The next step is to [Upload the SW](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/Upload_SW.md). I'd do this before you put the APC board into your pinball machine, because if this works you know, that your 5V supply has no short and is working properly.
