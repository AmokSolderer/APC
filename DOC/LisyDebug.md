# Controlling Lisy

This page covers just the APC relevant basics. In some cases it might be necessary to read the corresponding chapters in the [Lisy manual](https://lisy.dev/documentation.html).

The basic mode of Lisy is selected by the Lisy_Jumpers P12 which are depicted below.

![Lisy Jumper](https://github.com/AmokSolderer/APC/blob/V00.21/DOC/PICS/LisyJumper.png)

|Jumper|Function|Comment|
|--|--|--|
|1|No Autostart|Lisy won't start PinMame after booting up|
|2|Hotspot Mode|Lisy will start a WiFi hotspot instead of using the local Wifi|
|3|Debug Mode|Writes a debug log|
|4|Lisy Control|Starts the Lisy Webserver|
|5|Not used||
|6|Not used||

## Updating Lisy

If you have a WiFi capable Raspberry Pi you can use the incremental update function for minor updates instead of having to write a new Lisy-image to the SD card. Take a look at the 'Wireless config' section in the Lisy manual to learn how to set-up the WiFi.

With WiFi set-up and the system being switched off, connect the jumper 4 (Lisy Control) and power up your machine.  
After booting up, Lisy will try to connect to your WiFi. If the connection is successful, the obtained IP address is shown in the pinball displays.  
Enter this IP address into your browser to connect to the LisyControl webpage where you can initiate an update.

## Lisy Debug Mode

Lisy comes with a build-in Debug Mode which writes a debug log to the SD card of the Raspberry Pi when activated.  
Before you start the Debug mode you have to specify what kind of debug information you want to have logged. To do this, enter the 'Game Settings' of the 'Remote Control' Mode and change the 'Lisy Debug' setting accordingly.  
The value for 'Lisy Debug' can be derived from the following table.

|Debug Option|Value|
|--|--|
|Displays|1|
|Switches|2|
|Lamps|4|
|Solenoids|8|
|Sound|16|

If you want to log more than one option you have to add the respective values. For example, if you want to log the events for switches and solenoids you have to select 10 here.  
Don't forget to select 'Exit Settings' to store your changes to the APC's SD card.

To start the Debug Mode, turn off your machine and set Lisy jumper 3 before turning it back on. The system will now log the selected debug information, but this doesn't mean that it is immediately stored on Lisy's SD card. You should therefore not just turn your pinball machine off after your debug session, but press the Shutdown Switch SW1 which will initiate a controlled shutdown of Lisy. After that you can turn off your machine and remove the SD card of the Raspberry Pi.

The log file is located in the /lisy/lisy_m/debug folder.
