# Run a game

So you've done the tests and you have the Base Code running. If you've also [adjusted the Base Code](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/SetUpBC.md) to your machine then you can already press the Game Start button and play a very basic game.

After you've had enough fun with the Base Code you have the following options:

* Use Lisy/PinMame to run the original game code on a Raspberry Pi
* Create your game in MPF and let it control the APC
* Do your own game code in C and use the APC SW framework

## PinMame

The APC by itself cannot run the original Williams EPROM software, but you can use PinMame to emulate the pinball's hardware. By this you can run the old game SW on the APC. The easiest way to do this is to plug a Raspberry Pi on the APC board and install Lisy on it.  
Here's what to do:

1. Enter the [System Settings](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/Settings.md#system-settings)  
1.1. Select 'Remote Control' as the Active Game  
1.2. Check that 'On Board' is selected in the 'Connect Type' setting  
2. Enter the [Game Settings of the Remote Control Mode](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/Settings.md#game-settings-in-remote-control-mode)  
2.1. Select the corresponding [PinMame game number](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/lisyminigames.csv) in the Game Settings  
2.2. If you want to let the APC generate the audio then the Game Setting 'PinMame Sound' has to be APC, 'Board' means that an old audio board is used which is possible for System 3 - 7 machines. System 7 machines need an additional cable for this to work  
2.3. Exit the settings and switch off your machine  
3. Go to the [Lisy project page](https://lisy.dev/apc.html) and download a Lisy image file. Note that only the Raspberry Pi Zero, Zero W, 3A+ and 3B+ models are currently supported by Lisy.  
3.1. Install the image file on an SD card and put it in your Raspberry Pi  
3.2. Install the Pi on your APC board  
4. Turn on your pinball machine  
4.1. The 'Booting Lisy' message should appear in your displays  
4.2. After a while the yellow on board LED will be lit and the name of the selected game will appear in the displays as well as a countdown  
4.3. The green LED will be lit and the game should start

Most Williams games go into 'Factory Settings' mode when started for the first time. That means if the game doesn't start, use the Advance and Up/Down Buttons to navigate and quit the Williams settings. Note that as you're now in the original Williams settings you can navigate them as usual with one exception:  
You must not keep Advance pressed for more than 1 second with Up/Down in up position as this will trigger the APC settings. If you want to browse the Williams Settings quickly, just do it backwards with Up/Down in down position.

If you feel that your game is not running at the correct speed, you can change PinMame's emulation speed. To do this you have to remove the SD card from your Raspberry Pi and access the file  
boot/lisy/lisy_m/cfg/lisyminigames.csv  
There's a throttle value specified for each game. Changing this value to a lower value will make the game run faster and vice versa.


### PinMame sound

When you're not using an old audio board but you want the APC to do the audio instead, then you need to have the necessary sound files. Check the table on the [PinMame Sound](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/PinMame.md) page to see whether your game is already supported and the audio files are available. If your game is listet here, you can simply request the files and put them on the SD card on the APC board (not the one of the Pi). That's it, have fun.  
If your game is not yet supported or you want to change the sounds (or even rules) then read the [PinMame howto](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/PinMame_howto.md).

## MPF

If you want to do your own rules without having to program in C, the [Mission Pinball Framework](http://missionpinball.org/) might be your method of choice. It can run on a PC which then controls the APC via USB. You'd have to select 'Remote Control' as the Active Game and  'Connect Type' to 'USB' in the [System Settings](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/Settings.md#system-settings).  
We can also let Lisy run MPF which would then work without a PC, but we'd do this only on request.

Here you can find my basic [MPF setup](https://github.com/AmokSolderer/APC/tree/master/DOC/Software/MPF)

## C code

If you're familiar with C you can also program the APC directly. This SW would then run on the Arduino itself with no need for an additional PC or Raspberry Pi.  
For this the APC software offers an [API](https://github.com/AmokSolderer/APC/tree/master/DOC/Software/APC_SW_reference.pdf) providing the necessary commands to control a pinball machine. It's still a lot of effort to program a game completely from scratch, but you could even run your game in PinMame and only use the API to do changes or extensions to the original rules.

The APC software itself consists of two parts: the operating system APC.ino and the game specific code. The former controls the hardware and offers an application interface (API) for the game specific software to use. For an overview of the available API variables and commands please take a look at the
[APC software reference](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/Software/APC_SW_reference.pdf).

I have written game codes for my Black Knight and Pinbot. They are still not final, but good enough to have fun with and to use as a reference when writing own code. Additionally there's a [Base Code](https://github.com/AmokSolderer/APC/blob/V00.31/BaseCode.ino) which should serve as a starting point for you to do your own game. It contains the very basics of a pinball game and it can be easily adapted to your machine. As a startup guide how to start writing game code I have written a short [tutorial](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/GameCodeTutorial.md).

Please note that I have equipped my Black Knight with a [special alphanumerical display](https://github.com/AmokSolderer/APC/blob/V00.31/DOC/Sys7Alpha.md) for pre System11 machines and that advanced APC commands like scrolling are currently not usable with numerical only displays. This is because I think that these displays are not suited for homebrew machines. If you do all the work needed to do your own game code, you'd for sure want to have a display with letters, otherwise you wouldn't be able to even have a decent high score list. Additionally it would be quite cumbersome to debug some game software without the display being able to show letters. Therfore I recommend to use an early System11 display which has at least one row with alphanumeric displays (or build my [System7Alpha](https://github.com/AmokSolderer/APC/tree/master/DOC/Hardware/Sys7Alpha)). However, the basic software support is implemented, which means you can use the old displays without any restrictions you just have to do a bit more coding to get all the features. And if you just want to use them with PinMame to replace your old boards these displays will work perfectly well.
