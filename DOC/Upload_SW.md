# Upload the Software

## Installing the Arduino IDE

The APC uses an Arduino DUE which needs to be programmed with the APC Software.  
Go to the [Arduino DUE](https://docs.arduino.cc/hardware/due) page and use the 'Quickstart Guide' to learn how to do this.
 
The next step is to install the SPI and the SdFat libraries. There is a tutorial on the [Arduino page](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-installing-a-library) about that.

## Uploading the APC software

Be sure to have all .ino files and the Sound.h from the APC repository in your working directory. The name of this directory has to be APC.  
Open the Arduino.ino sketch in the Arduino IDE. The other .ino files should open up as seperate register cards.

I recommend to initially program the Arduino before installing the APC into your pinball machine. The APC's power consumption is quite low so the USB connection to yor PC is enough to supply it.  
Connect the 'Programming Port' of the DUE to your PC and press 'Upload' in your Arduino IDE. The SW should be compiled and uploaded automatically.

## Updating the SW

Once you have the APC installed in your machine, you don't have to remove it to upload a new SW version. However, I'd recommend to press the 'High Score Reset' button before uploading as this disables the lamps, coils and displays.  
After the upload is completed, the APC will restart and come up with the new SW. If you're using a Raspberry Pi with Lisy you might have to restart your machine anyway, to make the Pi and the Arduino sync again.
