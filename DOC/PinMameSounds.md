# Instructions for extracting sound files from pinball ROMs

There are several ways to extract sounds from pinball ROMs:
1) 'F5' method using [PINMAME and M1 music extractor](https://www.vpforums.org/index.php?app=tutorials&article=54)  
2) 'F6' method using [PINMAME with patch by lucky1](https://vpuniverse.com/forums/topic/4489-pinmame-altsound-editor/)

Method 2) seems preferable because it delivers a list of files automatically, but does not work for everybody (OS Win7, Win10)
Method 1) has the drawback that the F5 key is not reliably polled and thus requires good nerves for many files...

Both methods have in common, that both music and sound board of the pinball control system are triggered with machine specific commands (eg. 0x0101 means play song1 on musicboard, 0x0001 means play sound1 on soundboard) . With method 1) a number of files will be available in the 'wave' folder of the pinmame folder (eg. 'rdkn0001.wav').  These files are sampled with 48kHz which is the default pinmame sample rate and which needs to be converted to the APC samplerate of 44.1kHz later.

Before that, renaming and postprocessing of the files is required. This could be performed with an automatic renaming tool ( eg. Bulk Rename Utility https://www.bulkrenameutility.co.uk). The sound files need to be changed to '00_XX.wav' with XX in hexadecimal format, and the music files to '01_XX.wav' accordingly.

The next step is postprocessing in an audio tool like e.g. audacity (http://www.audacity.com). With audacity nearly every operation can be batch processed. This is very handy to change the relative volume of music files or/and sound files with a single mouse click.
It also can be used to automize a time series of 'F5' exported soundfiles. Eg. if the 'F5' method is used to record the sounds 0x00-0x0F into a single sound file, the sounds can be threshold-level detected by audacity and saved automatically.
This can be achieved by inserting the following commands into the macro queue:

1) Mark all
2) Highpass filter 10Hz /6dB
3) noisefinder (-)28db, 0.1 , 0.1 , 0  ( dB value might need adaption, if resulting selection is unsatisfactory)

After executing the macro, check if the text markers are set correctly. Then use CTRL-Shift-L to export single files defined by the text markers (use prefix '0_').

The next recommended step is to mix the stereo track down to mono (use Tracks->mix->downmix to mono in audacity) and change the volume (either use 'normalize' oder 'amplify' function in audacity). Experiment a bit with the amplification, because the synthetically generated peak values (by pinmame) are sometimes misleading. Try it out on the real machine.

After this step, you can either convert each file to 44.1kHz manually in audacity (for this there is no macro function yet ). Or use a different tool like OCENAUDIO (https://www.ocenaudio.com) which can batch convert sample rates. With OCENAUDIO you can pull a folder in the left column of the program screen, mark all with CTRL-A and use the right mouse button to 'change samplerate' and set this to 44100Hz. After this, close all files and confirm 'save files' with ok and wait for automatic conversion.

The last step is to convert the 16Bit WAV files to the 12Bit APC format. This can be done with a PERL script which takes all (mono) wav files within the current directory and converts them to *.snd files. This script can be executed with a perl interpreter, e.g. for windows available from http://strawberryperl.com

The resulting snd Files need to be copied to the APC SD-CARD. Please use a SD-card formatter utility (eg https://www.sdcardformatter.com/) and not the operating system function.  FAT file systems do not sort or index files so open does a sequential search for the file. Opening a file normally takes about 15ms but can take up to 50ms. In addition to that, the SD card controller might be doing some 'housekeeping', further prolonging access times. As a result, a sound file may not be found by the APC, even if it is available on the SD card (with a larger number of files stored on it). 

Finally there is an additional bonus to all the work required to extract and convert sound files: each sound or music can be exchanged or extended easily. E.g by adding two lines of code to the USBcontrol.ino , custom music can be added to the original pinball music.

    if ((USB_SerialBuffer[1] == 2)|| (USB_SerialBuffer[1] == 58)) {  // main theme
      USB_SerialBuffer[1]=random(200,204+1);}                        // cycle between five songs randomly
    if (USB_SerialBuffer[1] == 200) { USB_SerialBuffer[1]=58;}       // include main theme also
          // C9, CA, CB,CC = custom songs


