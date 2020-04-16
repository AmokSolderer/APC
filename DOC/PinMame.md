# The APC and PinMame
Bringing PinMame and the APC together was only possible with the help of the Lisy project. In the APC case we're using the Lisy_Mini board, but Lisy offers much more especially for Bally and Gottlieb fans. Please visit the [Lisy homepage](https://lisy.dev) for more.

We have started our PinMame experiments with my old trusty Pinbot which is running quite well as you can see in the following video:

Pinbot video

PinMame support is still in it's early stages. As you can see the Pinbot is already working quite well, but it's not yet perfect. The main challenges when adding support for more machines are sound related. This has to do with the way we did the sound implementation which offers a lot more than just playing the original sounds and music, but requires a bit more work to do so. 

## The Sound

Our current setup is not using the Raspberry Pi to generate the PinMame sounds. Instead all sound and music files have to be created once and stored on the SD card of the APC. Of course this requires some work, but we think the benefits are worth it.
First of all, the Pi uses the Unix version of PinMame which has severe sound quality issues while the audio quality of the APC is very good.
But there're some more advantages in having all sound files on the SD card. You can use the same files for PinMame, MPF or when programming the APC natively. This may not sound like a big deal, but it gives you the opportunity to run your game in PinMame and introduce certain changes by using the [APC API](https://github.com/AmokSolderer/APC/blob/master/DOC/Software/APC_SW_reference.pdf)
.
A good example is a local fellow who wants his System 3 'Disco Fever' to play songs like 'Saturday Night Fever' additionally to the normal sounds of the machine. We still have to add PinMame support for System 3, but once we have it adding the music should be an easy task. The commands of the APC API can be used to start the music when the game starts and to stop it when it ends. You could even identify certain trigger points to switch to another song and so on.
To make this possible, it is necessary that PinMame and the APC can play sounds simultaneously and to avoid an additional HW sound mixer which mixes both sound channels together, we decided to let the APC do the complete sound handling with PinMame just telling him which sound to play when.

The drawback of this is that you have to extract the music files from PinMame. Furthermore we have to understand how the audio boards work and emulate their behaviour. The first task is easy assuming we find a place to store these files somewhere in the internet, because then this work must only be done once for each game.
For System 3 - 9 games I expect this to be quite easy, as they have a very limited sound performance. You could use the Audio Debug Mode which is explained later to find out which sounds to extract.
As start you can find our Pinbot sound files HERE. The quality of the sounds is good, but I personally find the PinMame sounds when opening and closing the visor quite lousy. So, if someone finds the time to record these sounds from the audio board, you are welcome to send them to me and I'm going to add them. As we have them on the SD card we can just replace them as we like.

Understanding the audio boards is a similar story. Again I expect this to be easy for games prior to System 11, but it took me roughly a week before I was satified with the sounds of my Pinbot. Of course I started from scratch, so it's going to be easier for the next one doing this, but System 11 features various audio boards and I don't know how much they differ from the Pinbot's.
For this reason I'm going to maintain a list here where all PinMame relevant information about the relevant systems are listed.

### System 11



