#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            12
#define NUMPIXELS      64
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

byte RecByte = 0;																				// received byte
bool RecFlag;
bool LampStatus[64];																		// current status of each lamp
byte LampMax[64][3];																		// current max color values of each lamp
byte LampMaxSel[3] = {255, 255, 255};										// selected max color values
byte i = 0;																							// universal buffer
byte c = 0;																							// universal buffer
byte Sync = 8;																					// ms after last Sync
byte ColorSelectCount = 0;															// counts the bytes received by the color select command
bool TurnOn[6][64];																			// the list of the lamps currently being turned on
bool TurnOff[6][64];																		// the list of the lamps currently being turned off

void setup()
{
	pixels.begin();
	//Serial.begin(115200);
 for (i=0;i<9;i++) {
	  pinMode(i, INPUT_PULLUP);}
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
	digitalWrite(12, LOW);
	//DDRD = 0;																							// Port D = Input
	//PORTD = 255;																					// no pull-ups
	for (i=0;i<64;i++) {
		for (c=0;c<3;c++) {
			LampMax[i][c] = 255;}}
	RecFlag = PINB && 1;
}

void loop()
{
	if ((PINB && 1) != RecFlag) {													// get byte
		RecByte = PIND;																			// store it
		//Serial.println(RecByte);
		RecFlag = !RecFlag;
		if (Sync<8) {																				// if the last sync happened less than 4 cycles ago
			for (i=0;i<8;i++) {																// for the 16 lamps currently being processed
				for (c=0;c<5;c++) {															// and 5 brightness levels
					if (TurnOn[c][Sync*8+i]) {										// check if current lamp is in the turn-on phase
						if (c) {																		// if it hasn't reached the last level yet
							pixels.setPixelColor(Sync*8+i, pixels.Color(LampMax[Sync*8+i][0]/5*(5-c),LampMax[Sync*8+i][1]/5*(5-c),LampMax[Sync*8+i][2]/5*(5-c)));}
						else {																			// for the last level turn it fully on
							pixels.setPixelColor(Sync*8+i, pixels.Color(LampMax[Sync*8+i][0],LampMax[Sync*8+i][1],LampMax[Sync*8+i][2]));}}
					TurnOn[c][Sync*8+i] = TurnOn[c+1][Sync*8+i];
					if (TurnOff[c][Sync*8+i]) {										// check if current lamp is in the turn-off phase
						if (c) {																		// if it hasn't reached the last level yet
							pixels.setPixelColor(Sync*8+i, pixels.Color(LampMax[Sync*8+i][0]/5*(c),LampMax[Sync*8+i][1]/5*(c),LampMax[Sync*8+i][2]/5*(c)));}
						else {																			// for the last level turn it completely off
							pixels.setPixelColor(Sync*8+i,pixels.Color(0,0,0));}}
					TurnOff[c][Sync*8+i] = TurnOff[c+1][Sync*8+i];}}
			for (i=0;i<8;i++) {
				if ((RecByte & 1) && !LampStatus[Sync*8+i]) {		// Turn on lamps
					for (c=0;c<3;c++) {														// set max brightness of lamp to selected max value
						LampMax[Sync*8+i][c] = LampMaxSel[c];}
					LampStatus[Sync*8+i] = true;
					TurnOn[5][Sync*8+i] = true;}
				else {
					TurnOn[5][Sync*8+i] = false;}
				if (!(RecByte & 1) && LampStatus[Sync*8+i]) {		// Turn off lamps
					LampStatus[Sync*8+i] = false;
					TurnOff[5][Sync*8+i] = true;}
				else {
					TurnOff[5][Sync*8+i] = false;}
				RecByte = RecByte>>1;}
			Sync++;}																					// increase the counter
		else {
			if (ColorSelectCount) {														// these bytes belong to the color select command
				LampMaxSel[ColorSelectCount-1] = RecByte;
				if (ColorSelectCount == 3) {
					ColorSelectCount = 0;}
				else {
					ColorSelectCount++;}}
			else {
				switch (RecByte) {															// treat it as a command
				case 170:																				// sync command
					Sync = 0;																			// the next four cycles (8 bytes) represent the current lamp status
					pixels.show();																// update the LEDs
					break;
				case 192:																				// color select command
					ColorSelectCount = 1;													// indicate that the color select process is running
					break;
				}}}}}
