# If things don't work

## Sound stuttering

If your sound has unregular lags and stutters sometimes then it's most probably an issue with your SD card.
 
The first thing to try is to format your card with a special SD-card formatting tool like the one from the SD Association (sdcard.org) which can improve the time your SD card needs to open a new file. This can be crucial especially for pre System11 machines as their sound sequences usually consist of lot's of single sounds. Each of these sounds requires a new file on the card to be opened and if your card cannot open the files fast enough than the sounds just start to stutter.

If this doesn't work then try a different SD card. I had several SD cards of the same brand, but for some reason one of these cards just produced sound lags on my Pinbot, even though I had it formatted with the formatting tool. After I replaced the card with one of it's siblings everything worked fine.

However, some sound sequences might just be too challenging for your SD cards.  
The solution is to record the whole sequence into one single sound file and to play this with a high priority. By this only one file has to be opened for the whle sequence and the high priority block all subsequent sound commands from PinMame as long as the file is being played.

## Damaged Arduino clone

If you're using a clones of the Arduino DUE and it just stopped working and draws a huge current instead then it's most probably a blown buck converter IC.  
You can check this by supplying your DUE on the bench (without the APC board) with 5V. Then check the temperature of the marked IC.
![Buck](https://github.com/AmokSolderer/APC/blob/V00.22/DOC/PICS/buck.jpg)
If the IC gets hot then it's damaged. The good thing is that you don't need this IC when using the DUE with the APC, so you can just remove it and your system should run fine again.
