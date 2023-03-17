# Upload the Software

## Installing the Arduino IDE

The APC uses an Arduino DUE which needs to be programmed with the APC Software.  
Go to the [Arduino DUE](https://docs.arduino.cc/hardware/due) page and use the 'Quickstart Guide' to learn how to do this.
 
The next step is to install the SPI and the SdFat libraries. There is a tutorial on the [Arduino page](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-installing-a-library) about that.

## Download the APC software from GitHub

If you don't want to use Git to access the GitHub repository, you can download a ZIP package instead.  
To do that go to the main [GitHub page](https://github.com/AmokSolderer/APC) and click the green <> Code button at the top right of the page. A small pull down menu appears with 'Download ZIP' as the lowest entry. Click on it to download the complete repository.  
Unzip the file and rename the APC-master folder to APC. The folder must contain all .ino files and the Sound.h from the APC repository.

## Uploading the APC software

Enter the APC folder and open the Arduino.ino sketch in the Arduino IDE. The other .ino files should open up as seperate register cards.

I recommend to initially program the Arduino before installing the APC into your pinball machine. The APC's power consumption is quite low so the USB connection to yor PC is enough to supply it.  
Connect the 'Programming Port' of the DUE to your PC and press 'Upload' in your Arduino IDE. The SW should be compiled and uploaded automatically.

You're now ready for the [basic preparation](https://github.com/AmokSolderer/APC/blob/master/DOC/Prepare.md) that might be necessary for your machine to work with the APC.

## Updating the SW

Once you have the APC installed in your machine, you don't have to remove it to upload a new SW version. However, I'd recommend to press the 'High Score Reset' button before uploading as this disables the lamps, coils and displays.  
After the upload is completed, the APC will restart and come up with the new SW. If you're using a Raspberry Pi with Lisy you might have to restart your machine anyway, to make the Pi and the Arduino sync again.
