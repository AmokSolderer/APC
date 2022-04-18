# If things don't work

## Sound stuttering

If your sound has unregular lags and stutters sometimes then it's most probably an issue with your SD card.
 
The first thing to try is to format your card with a special SD-card formatting tool like the one from the SD Association (sdcard.org) which can improve the time your SD card needs to open a file. This can be crucial especially for pre System11 machines as their sound sequences usually consist of lot's of single sounds. Each of these sounds requires a new file on the card to be opened and if your card cannot open the files fast enough than the sounds just start to stutter.

If this doesn't work then try a different SD card. I had several SD cards of the same brand, but for some reason one of these cards just produced sound lags on my Pinbot, even though I had it formatted with the formatting tool. After I replaced the card with one of it's siblings everything worked fine.

However, some sound sequences might just be too challenging for your SD cards.  
The solution is to record the whole sequence into one single sound file and to play this with a high priority. This requires only one file to be opened for the whole sequence and the high priority blocks all subsequent sound commands from PinMame as long as the file is being played.

## Unknown Command error message

If you're using the APC with Lisy/PinMame and the 'Unknown Command' error message followed by a number pops up on your lower displays then you have a problem with your serial communication.

This usually happens when Lisy/PinMame is playing sound files from the SD card. The reason is that an SD-card needs several ms to open a file. During this time the APC SW cannot process any commands coming in on the serial interface which means they have to be stored in the receive buffer.  
The standard size of the DUE's serial buffer is 128 bytes which was sufficient for the SD-cards I did my tests with. Even though these were cheap and ordinary cards it still might be that your card is even slower.  
Of course you can try another SD-card and hope that this one is faster, but you can also increase the size of the serial buffer. The problem is that this setting is part of the Arduino libraries which makes it a bit cumbersome to find at least with the Arduino IDE.

If you're using Sloeber, the Arduino plugin for Eclipse you can simply take a look into your 'Project Explorer' and look for the file RingBuffer.h as shown below:

![Ringbuffer](https://github.com/AmokSolderer/APC/blob/V00.23/DOC/PICS/Ringbuffer.png)

For the Arduino IDE this is a bit more complicated.  
In the preferences window of your Arduino IDE you should see a link to some preferences.txt file in the lower left corner. We don't care about the file, but the path is important. It should look somehow like this:

    C:\Users\user\AppData\Local\Arduino15 

Follow the path and then proceed to 

    C:\Users\user\AppData\Local\Arduino15\packages\arduino\hardware\sam\1.6.12\cores\arduino
    
where you should find the RingBuffer.h file.

In the file there's the line

    #define SERIAL_BUFFER_SIZE 128
    
which sets the buffer size to 128 bytes.  
Set the size to 256 bytes and your 'Unknown Command' errors should be gone.

## Damaged Arduino clone

If you're using a chinese clone of the Arduino DUE which just stopped working and draws a huge current instead, then it's most probably a blown buck converter IC.  
You can check this by supplying your DUE on the bench (without the APC board) with 5V. Then check the temperature of the marked IC.

![Buck](https://github.com/AmokSolderer/APC/blob/master/DOC/PICS/Buck.jpg)

If the IC gets hot then it's damaged. The good thing is that you don't need this IC when using the DUE with the APC, so you can just remove it and your system should run fine again.
