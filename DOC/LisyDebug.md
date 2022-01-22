# Controlling Lisy

This page covers just the APC relevant basics. In some cases it might be necessary to read the corresponding chapters in the [Lisy manual](https://lisy.dev/documentation.html).

The basic mode of Lisy is selected by the Lisy_Jumpers P12 which are depicted below.

![Lisy Jumper](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/LisyJumper.png)

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

## Remote debug Mode

Lisy can also be controlled remotely via WiFi. In this mode it'll just print all information directly into your shell, including the debug log. Hence, you don't have to bother with removing the SD card any more, since all the required information is already on your screen.

If you have set-up the WiFi configuration of your Lisy system, you can use the Remote Debug Mode as follows:  
With your pinball machine switched off, connect the jumpers 1 (No Autostart) and 3 (Debug Mode).  
After you've turned on the power, wait for the yellow LED to light up. Now Lisy has booted and is waiting for instructions.  
First you have to obtain the IP adress of Lisy in your WiFi network. Now open a shell on your PC and connect to Lisy via ssh: 
 
    ssh pi@IP-adress

Log into the system by using lisy80 as the password.  
Now it depends whether you're using the Pi on an APC 3 or on a Lisy_Mini board. In case of the APC 3 you have to use

    ./run_lisy_apc
    
to start PinMame and with a Lisy_Mini board it's

    ./run_lisy_mini
    
### Remote mode and Lisy updates

The run_lisy_apc and run_lisy_mini commands are not automatically updated with your Lisy system when the update is done via Lisy Control. That means you have to change these scripts manually. Let's use the run_lisy_apc script as an example. Normally it looks as follows:

    sudo ./lisy/xpinmame.vid_lisy -nosound -skip_disclaimer -skip_gameinfo -nvram_directory /pinmame/nvram -rp /boot/lisy/lisy_m/roms lisy_apc
    #sudo /usr/local/bin/lisy -nosound -skip_disclaimer -skip_gameinfo -nvram_directory /pinmame/nvram -rp /boot/lisy/lisy_m/roms lisy_apc
    
To activate the Lisy update you just have to move the comment sign from the second to the first line and you're done.  
Note that you have to issue an rw command first in order to set Lisy into read/write mode.
