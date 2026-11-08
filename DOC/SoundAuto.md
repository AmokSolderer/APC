# Automatic sound file generation

## Automatic extraction from PinMame

In PinMame, press F4 to enter the 'Sound Command Mode' then press DEL for the Manual / Command Mode.

Now enter the audio command you want PinMame to start with, for audio board 0 this would be 000000000000 and 000000000100 for audio board 1. Press F6 to start the automatic extraction. PinMame will go through the audio command one by one and generate .wav files of them.  
This should generate most of the sound files, but there're limits especially for System11. The music files for example are sometimes truncated, so you'd have to extract them manually. This is not a big deal because there're just a few of them, usually starting with the audio command 000000000101.  
Another problem with early System11 is that PinMame cannot find all sounds for audio board 1. You could try to guide it by using additional start commands like 000000000160, but it's easier to just leave it for now and if you get an error message like '1_85.snd not found' during your first games then you know that starting another extraction with 000000000180 should do the trick.

## Automatic renaming

Files generated with this method get names which include the audio command number and the machine name (e.g. 0x00A5-fire_l3.wav). However, the APC needs the names in the format [single digit board prefix]_[sound number in hex].wav, so the correct name for this sound would be 0_a5.wav. With Linux you can just use the rename command which should be part of most distributions and is very powerful. Windows users would have to install some bulk renaming tool.  
On our case we need to use

    rename -v -n 's/0x00//' *         to remove the '0x00' at the start of the filename
    rename -v -n 's/-fire_l3//' *     to get rid of the rom name
    rename -v -n 'y/A-Z/a-z/' *       to replace upper case by lower case letters
    rename -v -n 's/^/0_/' *          to add the board prefix '0_'

The actions will be done for all files in the current directory. Note that the -v -n options will issue a dry run, so the tool will just show you the new names without actually doing anything.

## Automatic processing with Audacity

Basically you have to do the same steps as described in the [manual way of sound file preparation](https://github.com/AmokSolderer/APC/blob/master/DOC/SoundManual.md), but you'd use a macro to process all files in your working directory. An example macro for processing callouts would look like this:

    Select:End="0,15" Mode="Set" RelativeTo="ProjectStart" Start="0"
    Delete:
    Select:End="0" Mode="Set" RelativeTo="ProjectEnd" Start="0,05"
    Delete:
    SelectAll:
    Normalize:ApplyGain="1" PeakLevel="0" RemoveDcOffset="1" StereoIndependent="0"
    Amplify:Ratio="1,9952623"
    ExportWav:
    
This will delete 150ms at the beginning, as the automatic extraction sometimes adds a few ms of noise to the start of a sound and 50ms at the end. Then it normalizes the sound to a peak level of 0dB and amplifies even 6dB more. The latter will cause the signal to be distorted, but it's still OK as it will just affect the peaks and callouts need to be quite loud to be still audible when the BG music plays.

These are just example values I used for one specific machine. You generally have to find out for yourself what the best relation in terms of volume is for the BG music, the callouts and the general sound effects.

## Sampling Rate

Windows users will additionally have to set the sampling rate to the standard 44.1KHz, as the Windows PinMame uses 48KHz by default. This cannot be batched processed in Audacity, so you'd have to use a tool like [OCENAUDIO](https://www.ocenaudio.com) for this.
