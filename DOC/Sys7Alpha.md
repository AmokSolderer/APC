# Alphanumeric display for pre System7 - 11 machines

Beginning with System7 Williams changed their display setup to 4 player displays with 7 characters each and one credit display with 4 characters. In System7 and 9 machines all of these displays were 7-segment numerical only. With System11 the basic setup was kept, but the displays for player 1 and 2 were changed to alphanumerical ones.

This display set replaces all 4 player displays by 14-segment alphanumeric ones, just the credit displays stays numerical. It is made for System 7 and System9. It can however also be used in System11 machines up to Taxi (which fast the first to use two 16 character alphanumeric displays AFAIK) but there the drilling holes of the board won't fit those of the original board.  

Note that this display can only be used with APC boards, it won't work with the original CPU boards.  
It works with the APC and PinMame, but will behave like an old numerical displays then. That means it makes only sense for you to build this if you want to use the APC API or MPF to write the corresponding code.

The picture below shows 2 versions of the driver board. The lower (older) one is equipped with Molex connectors and has System 9 display cables attached. The upper one is the updated version which can also be used with System7 display cables. It can also use Molex connectors for System 9 and 11, but in this case they have not been populated.

![Sys7Alpha](https://github.com/AmokSolderer/APC/blob/V00.23/DOC/PICS/Sys7_Alpha.jpg)

The second shot shows the upper board in my System7 Black Knight with all the cables for the actual display board connected.

![Sys7AlphaBK](https://github.com/AmokSolderer/APC/blob/V00.23/DOC/PICS/Sys7_Alpha2.jpg)

For the display to work properly you have to select 'Alpha+Credit' in the display settings.

The Gerber files, BOM and schematics can be found [here](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys7Alpha).