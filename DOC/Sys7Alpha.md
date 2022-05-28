# Alphanumeric displays for System7 - 11 machines

Beginning with System7 Williams changed their display setup to 4 player displays with 7 characters each and one credit display with 4 characters. In System7 and 9 machines all of these displays were 7-segment numerical only. With System11 the basic setup was kept, but the displays for player 1 and 2 were changed to alphanumerical ones.

This display set replaces all 4 player displays by 14-segment alphanumeric ones, just the credit display stays numerical. It is made for System 7 and System9. It can however also be used in System11 machines up to Taxi (which was the first to use two 16 character alphanumeric displays AFAIK) but there the drilling holes of the board won't fit those of the original board.  

Note that this display can only be used with APC boards, it won't work with the original CPU boards.  
It works with the APC and PinMame, but will behave like an old numerical display set then, because PinMame won't ask for anything but numbers.  
That means it makes only sense for you to build this if you want to use the APC API or MPF to write the corresponding code to use it.

I have used this display in my [Black Knight game code](https://github.com/AmokSolderer/APC/blob/master/DOC/BlackKnight.md).

![APC Black Knight](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/BK.jpg)

The picture below shows 2 versions of the driver board. The lower (older) one is equipped with Molex connectors and has System 9 display cables attached. The upper one is the updated version which can also be used with System7 display cables. It can also use Molex connectors for System 9 and 11, but in this case they have not been populated.

![Sys7Alpha](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Sys7_Alpha.jpg)

The next shot shows the upper board in my System7 Black Knight with all the cables for the actual display boards connected.

![Sys7AlphaBK](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Sys7_Alpha2.jpg)

For the display to work properly you have to select 'Alpha+Credit' in the display settings.

The alphanumeric player displays use Kingbright's PSA08-11SRWA LED display components, the credit display has four SA08-11SRWA. Alas, these components are a bit too wide, but you can use a bench- or belt-grinder until they fit on the boards.

The Gerber files, BOM and schematics can be found [here](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys7Alpha).

As the display boards are only single sided there're four wire connections needed on each player board. In the picture below they're shown as red lines right below the display elemets.

![PlayerDisplay](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/PlayerDisplay.png)
