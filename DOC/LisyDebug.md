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

With WiFi set-up and the system being switched off, connect the jumpers 1 (No Autostart) and 

## Lisy Debug Mode

Lisy comes with a build-in Debug Mode which writes a debug log to the SD card of the Raspberry Pi when activated.