# System 11

The first System11 machines had two independent audio circuits, each containing a processor and sound EPROMS. One of these circuits was located on the main CPU board while the other was on a seperate audio board.  
In case of the Pinbot, PinMame distinguishes these boards by using two audio prefixes which can be selected by 00 and 01 in PinMame's 'Sound Command Mode'. Both of these prefixes offer different sound commands. Our current status of what we know about these sound commands is listed below. There're still lots of gaps, but we didn't want to spend too much effort in investigating these boards. Again, we see this more as a community task and there're probably plenty of people out there with a lot more knowledge about this stuff than we have.

So if you have additional information to fill these gaps please tell us and we're going to include it.

|Prefix|Command / Hex|Comment|
|--|--|--|
|00|00|Stop Sound|
|01|00|Stop Music|
|01|01 - 08|Looped music tracks, some of them have an intro before the looping part begins|
|01|40 - 42|Transistions and endings for looped music|
|01|7f|Stop sounds of prefix 01|
|01|6X|Music volume setting 0x60 is full volume 0x64 is almost nothing|

Prefix 01 plays music and sounds simultaneously. It looks like sound numbers below 0x80 are for music and the rest is for sounds. As the APC has only two independent sound channels, only the music of prefix 01 is played on the APC's music channel while the sounds are being forwarded to the sound channel.

Somewhere in System11B all audio circuitry was removed from the CPU board and the functionality was added to the audio board. For these games only the prefix 01 is used by PinMame.

## How to make it work

So much for the theory. Let's proceed to the [Howto page](https://github.com/AmokSolderer/APC/tree/master/DOC/PinMame_howto.md).
