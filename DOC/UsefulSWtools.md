I have written some perl scripts to do certain APC related work.

## Audio data converter 

[AudioSave.pl](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/AudioSave.pl) is a data converter that must be used to convert WAV files to a format that can directly be handled by the Arduino. The sampling rate of this WAV file has to be 44.1KHz.  
The tool looks for a 16bit WAV mono file named Data.wav and generates a file Data.bin from it. Basically it skips the header and converts the 16bit signed values of the WAV file to 12bit unsigned for the DAC of the DUE.  
I recommend to use tools like Audacity to generate the mono WAV, but the tool can also be used to convert stereo files. For this you just have to uncomment the two `read(FH, $buf1, 1);` lines which makes the tool skip every second value. The drawback is that this will not generate a real mono file but just throw away one channel.

Rename Data.BIN and put it on the SD Card. Use only capital letters for the filename, because only then the pinball display is able to show it correctly in case an error message is shown.

## Display segment code calculator

The following tool generates the numerical definition of the display segment patterns (2 bytes for each character). In other words the numbers tell the APC which display segments to switch on for which character. In the code these numbers can be found at the beginning of APC.ino. They are stored in arrays named AlphaUpper, AlphaLower and NumLower depending on where you want this character to appear.

For normal letters and numbers there should be no need for you to bother with these definitions as they're already defined. However if you want special characters or graphical effects you have to determine the corresponding numbers. To make it easier I have generated three versions of this tool for the various occasions.

* [DisplayAlphaUpper.pl](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/DisplayAlphaUpper.pl) 
* [DisplayAlphaLower.pl](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/DisplayAlphaLower.pl)
* [DisplayNumLower.pl](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/DisplayNumLower.pl)

Let's assume you have a Black Knight 2000 which has alphanumerical displays in both rows and you want to know the pattern definition numbers for the character 'A' in the upper display row. Take a look into your pinball manual or open the code of the tool to determine which segments have to be lit for an 'A'. You can see that an 'A' consists of the segments a, b, c ,e ,f, g and m. Now execute DisplayAlphaUpper.pl in a shell and type each of these segment characters followed by _Enter_. The tool will print a simple sketch of the selected segments as well as the two definition bytes. If your 'A' is complete you should end up with 126, 4 and when you look at the third line of AlphaUpper in the APC.ino you will find these values to define an 'A'.

If you want to have the values for an 'A' in the lower row you have to use DisplayAlphaLower.pl and DisplayNumLower.pl for numbers in the lower row (like in Big Guns).

***