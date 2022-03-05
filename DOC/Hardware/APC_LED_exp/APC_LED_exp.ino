#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            12
#define NUMPIXELS      64
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

byte RecByte = 0;                                       // received byte
bool RecFlag;
byte LampStatus[8];                                     // current status of each lamp
byte LampMax[64][3];                                    // current max color values of each lamp
byte LampMaxSel[3] = {255, 255, 255};                   // selected max color values
byte i = 0;                                             // universal buffer
byte c = 0;                                             // universal buffer
byte Sync = 8;                                          // ms after last Sync
byte Mode = 0;                                          // Mode 0 -> lamps being lit get the ColorSelect color / Mode 1 -> lamps keep their color / Mode 2 -> lamps set in the following frame get the new color immediately
byte Command = 0;                                       // LED command currently being processed
byte CommandCount = 0;                                  // counts the bytes received by the color select command
byte TurnOn[6][8];                                      // the list of the lamps currently being turned on
byte TurnOff[6][8];                                     // the list of the lamps currently being turned off

void setup() {
  pixels.begin();
  //Serial.begin(115200);
 for (i=0;i<9;i++) {
    pinMode(i, INPUT_PULLUP);}
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(12, LOW);
  //DDRD = 0;                                             // Port D = Input
  //PORTD = 255;                                          // no pull-ups
  for (i=0;i<64;i++) {
    for (c=0;c<3;c++) {
      LampMax[i][c] = 255;}}
  RecFlag = PINB && 1;}

void loop() {
  if ((PINB && 1) != RecFlag) {                         // get byte
    RecByte = PIND;                                     // store it
    //Serial.println(RecByte);
    RecFlag = !RecFlag;
    if (Sync<8) {                                       // if the last sync happened less than 8 cycles ago
      if (Mode < 2) {
        for (c=0;c<5;c++) {                             // for 5 brightness levels
          if (TurnOn[c][Sync]) {                        // anything to do in this byte?
            byte Mask = 1;                              // initialize bitmask
            for (i=0;i<8;i++) {                         // for the 8 lamps currently being processed
              if (TurnOn[c][Sync] & Mask) {
                if (c) {
                  pixels.setPixelColor(Sync*8+i, pixels.Color(LampMax[Sync*8+i][0]/5*(5-c),LampMax[Sync*8+i][1]/5*(5-c),LampMax[Sync*8+i][2]/5*(5-c)));}
                else {                                  // for the last level turn it fully on
                  pixels.setPixelColor(Sync*8+i, pixels.Color(LampMax[Sync*8+i][0],LampMax[Sync*8+i][1],LampMax[Sync*8+i][2]));}}
              Mask = Mask << 1;}}
          TurnOn[c][Sync] = TurnOn[c+1][Sync];
          if (TurnOff[c][Sync]) {
            byte Mask = 1;                              // initialize bitmask
            for (i=0;i<8;i++) {                         // for the 8 lamps currently being processed
              if (TurnOff[c][Sync] & Mask) {
                if (c) {
                  pixels.setPixelColor(Sync*8+i, pixels.Color(LampMax[Sync*8+i][0]/5*(c),LampMax[Sync*8+i][1]/5*(c),LampMax[Sync*8+i][2]/5*(c)));}
                else {                                  // for the last level turn it completely off
                  pixels.setPixelColor(Sync*8+i,pixels.Color(0,0,0));}}
              Mask = Mask << 1;}}
          TurnOff[c][Sync] = TurnOff[c+1][Sync];}
        if (RecByte != LampStatus[Sync]) {              // any status changes?
          byte Mask = 1;                                // initialize bitmask
          for (i=0;i<8;i++) {
            if ((RecByte & Mask) && !(LampStatus[Sync] & Mask)) { // lamp turned on
              if (Mode == 0) {                          // New lit lamps get a new color
                for (c=0;c<3;c++) {                     // set max brightness of lamp to selected max value
                  LampMax[Sync*8+i][c] = LampMaxSel[c];}}
              LampStatus[Sync] |= Mask;
              TurnOn[5][Sync] |= Mask;}
            else {
              TurnOn[5][Sync] &= (255 - Mask);}
            if (!(RecByte & Mask) && (LampStatus[Sync] & Mask)) { // lamp turned off
              LampStatus[Sync] &= (255 - Mask);
              TurnOff[5][Sync] |= Mask;}
            else {
              TurnOff[5][Sync] &= (255 - Mask);}
            Mask = Mask << 1;}}
        else {
          TurnOn[5][Sync] = 0;
          TurnOff[5][Sync] = 0;}}
      else {                                            // selected lamps get the selected color immediately
        if (RecByte) {                                  // any lamps set?
          byte Mask = 1;
          for (i=0;i<8;i++) {                           // for the 8 lamps currently being processed
            if (RecByte & Mask) {                       // lamp set in RecByte?
              for (c=0;c<5;c++) {                       // and 5 brightness levels
                TurnOn[c][Sync] &= (255 - Mask);        // stop lamp from TurningOn - Off
                TurnOff[c][Sync] &= (255 - Mask);}
              for (c=0;c<3;c++) {                       // set max brightness of lamp to selected max value
                LampMax[Sync*8+i][c] = LampMaxSel[c];}
              pixels.setPixelColor(Sync*8+i, pixels.Color(LampMaxSel[0],LampMaxSel[1],LampMaxSel[2]));}
            Mask = Mask << 1;}}}
      Sync++;}                                          // increase the counter
    else {                                              // LED patterns are done
      if (CommandCount) {                               // still bytes to read for this command
        CommandCount--;
        switch (Command) {
        case 192:                                       // color select command
          LampMaxSel[2-CommandCount] = RecByte;
          break;
        case 195:                                       // set color for LED
          LampMax[RecByte][0] = LampMaxSel[0];
          LampMax[RecByte][1] = LampMaxSel[1];
          LampMax[RecByte][2] = LampMaxSel[2];
          if (LampStatus[RecByte / 8] &= 255-(1<<(RecByte % 8))) {  // LED on?
            pixels.setPixelColor(RecByte, pixels.Color(LampMaxSel[0],LampMaxSel[1],LampMaxSel[2]));}
          break;
        default:                                        // unknown command
          CommandCount = 0;}
        if (!CommandCount) {                            // command processed
          Command = 0;}}
      else {
        switch (RecByte) {                              // treat it as a command
        case 64:                                        // Mode 0 -> lamps being lit get the ColorSelect color
          Mode = 0;
          break;
        case 65:                                        // Mode 1 -> lamps keep their color
          Mode = 1;
          break;
        case 66:                                        // Mode 2 -> lamps set in the following frame get the new color immediately
          Mode = 2;
          break;
        case 170:                                       // sync command
          Sync = 0;                                     // the next four cycles (8 bytes) represent a lamp pattern
          pixels.show();                                // update the LEDs
          break;
        case 192:                                       // color select command
          Command = 192;
          CommandCount = 3;                             // 3 bytes to read for this command
          break;
        case 195:                                       // set color for LED
          Command = 195;
          CommandCount = 1;
          break;
        }}}}}
