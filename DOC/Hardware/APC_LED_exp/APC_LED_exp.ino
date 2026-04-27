#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            12                             // change to 16 for Arduino Nano

byte RecByte = 0;                                     // received byte
bool RecFlag;
byte LampStatus[24];                                  // current status of each lamp
byte LampMax[192][3];                                 // current max color values of each lamp
byte LampMaxSel[3] = {200, 200, 200};                 // selected max color values
byte Mode = 0;                                        // Mode 0 -> lamps being lit get the ColorSelect color / Mode 1 -> lamps keep their color / Mode 2 -> lamps set in the following frame get the new color immediately
byte Command = 0;                                     // LED command currently being processed
byte CommandCount = 0;                                // counts the bytes received by the color select command
byte OwnCommands = 0;                                 // indicate active own LED commands
byte OwnCommandStep = 0;                              // needed for own LED commands
byte NumOfLEDbytes = 8;                               // stores the length of the transferred LED pattern
byte TurnOn[6][24];                                   // the list of the lamps currently being turned on
byte TurnOff[6][24];                                  // the list of the lamps currently being turned off
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NumOfLEDbytes*8, PIN, NEO_GRB + NEO_KHZ800);
const byte OwnPattern[6][3] = {{0,0,0},{0,50,0},{0,100,0},{0,150,0},{0,200,0},{0,250,0}};

void setup() {
  pixels.begin();
  for (byte i=0;i<9;i++) {
    pinMode(i, INPUT_PULLUP);}
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  for (byte i=0;i<64;i++) {
    for (byte c=0;c<3;c++) {
      LampMax[i][c] = 255;}}
  RecFlag = PINB && 1;}

void loop() {
  if ((PINB & 1) != RecFlag) {                        // get byte
    RecByte = PIND;                                   // store it
    RecFlag = !RecFlag;
    digitalWrite(13, RecFlag);
    if (CommandCount) {                               // still bytes to read for this command
      CommandCount--;
      if (Command < 25) {                             // LEDstatus command?
        if (Command) {                                // zero is not a valid command
          if (Mode < 2) {                             // LEDs fade on and off
            byte Buffer = Command - 1;
            if (RecByte != LampStatus[Buffer]) {      // any status changes?
              byte Mask = 1;                          // initialize bitmask
              for (byte i=0;i<8;i++) {
                if ((RecByte & Mask) && !(LampStatus[Buffer] & Mask)) { // lamp turned on
                  if (Mode == 0) {                    // New lit lamps get a new color
                    for (byte c=0;c<3;c++) {          // set max brightness of lamp to selected max value
                      LampMax[Buffer*8+i][c] = LampMaxSel[c];}}
                  LampStatus[Buffer] |= Mask;
                  TurnOn[5][Buffer] |= Mask;}
                else {
                  TurnOn[5][Buffer] &= (255 - Mask);}
                if (!(RecByte & Mask) && (LampStatus[Buffer] & Mask)) { // lamp turned off
                  LampStatus[Buffer] &= (255 - Mask);
                  TurnOff[5][Buffer] |= Mask;}
                else {
                  TurnOff[5][Buffer] &= (255 - Mask);}
                Mask = Mask << 1;}}}
          else if (Mode < 4) {                        // selected lamps get the selected color immediately
            if (RecByte) {                            // any lamps set?
              byte Mask = 1;
              byte Buffer = Command - 1;
              for (byte i=0;i<8;i++) {                // for the 8 lamps currently being processed
                if (RecByte & Mask) {                 // lamp set in RecByte?
                  if (Mode == 2)  {                   // selected LEDs are also turned on
                    LampStatus[Buffer] |= Mask;       // turn on lamp
                    pixels.setPixelColor(Buffer*8+i, pixels.Color(LampMaxSel[0],LampMaxSel[1],LampMaxSel[2]));}
                  else {                              // Mode = 3
                    if (LampStatus[Buffer] & Mask) {  // LED on?
                      pixels.setPixelColor(Buffer*8+i, pixels.Color(LampMaxSel[0],LampMaxSel[1],LampMaxSel[2]));}}
                  for (byte c=0;c<3;c++) {            // set max brightness of lamp to selected max value
                    LampMax[Buffer*8+i][c] = LampMaxSel[c];}}
                Mask = Mask << 1;}}}}}
      else {
        switch (Command) {
        case 192:                                     // color select command
          LampMaxSel[2-CommandCount] = RecByte;
          break;
        case 193:                                     // configure number of LED bytes
          NumOfLEDbytes = RecByte;
          pixels.updateLength(NumOfLEDbytes*8);
          break;
        case 195:                                     // set color for LED
          LampMax[RecByte][0] = LampMaxSel[0];
          LampMax[RecByte][1] = LampMaxSel[1];
          LampMax[RecByte][2] = LampMaxSel[2];
          if (LampStatus[RecByte / 8] &= 255-(1<<(RecByte % 8))) {  // LED on?
            pixels.setPixelColor(RecByte, pixels.Color(LampMaxSel[0],LampMaxSel[1],LampMaxSel[2]));}
          break;
        default:                                      // unknown command
          CommandCount = 0;}
        if (!CommandCount) {                          // command processed
          Command = 0;}}}
    else {                                            // ready for next command
      if (RecByte < 25) {                             // LEDstatus command?
        if (RecByte) {                                // zero is not a valid command
          Command = RecByte;                          // store command
          CommandCount = 1;}}                         // wait for  one argument
      else {                                          // different command
        switch (RecByte) {
        case 64:                                      // Mode 0 -> lamps being lit get the ColorSelect color
          Mode = 0;
          break;
        case 65:                                      // Mode 1 -> lamps keep their color
          Mode = 1;
          break;
        case 66:                                      // Mode 2 -> lamps set in the following frame get the new color immediately
          Mode = 2;
          break;
        case 67:                                      // Mode 3 -> only the color of the LEDs is changed, but they're not turned on
          Mode = 3;
          break;
        case 68:                                      // Mode 4 -> LED state is frozen
          Mode = 4;
          break;
        case 100:                                     // execute OwnCommand
          OwnCommands |= 1;                           // activate OwnCommand number 1
          break;
        case 101:
          OwnCommands &= 254;                         // deactivate OwnCommand number 1
          for (byte i=0;i<12;i++) {                   // for all affected LEDs
            pixels.setPixelColor(i, pixels.Color(0,0,0)); // turn them off
            LampStatus[i / 8] &= 255-(1<<(i % 8));}   // and change the status to off
          break;
        case 170:                                     // sync command
          if (Mode < 2) {                             // LEDs fade on and off
            for (byte x=0;x<NumOfLEDbytes;x++) {      // for all LEDbytes
              for (byte c=0;c<5;c++) {                // for 5 brightness levels
                if (TurnOn[c][x]) {                   // anything to do in this byte?
                  byte Mask = 1;                      // initialize bitmask
                  for (byte i=0;i<8;i++) {            // for the 8 lamps currently being processed
                    if (TurnOn[c][x] & Mask) {
                      if (c) {
                        pixels.setPixelColor(x*8+i, pixels.Color(LampMax[x*8+i][0]/5*(5-c),LampMax[x*8+i][1]/5*(5-c),LampMax[x*8+i][2]/5*(5-c)));}
                      else {                          // for the last level turn it fully on
                        pixels.setPixelColor(x*8+i, pixels.Color(LampMax[x*8+i][0],LampMax[x*8+i][1],LampMax[x*8+i][2]));}}
                    Mask = Mask << 1;}}
                TurnOn[c][x] = TurnOn[c+1][x];
                if (TurnOff[c][x]) {
                  byte Mask = 1;                      // initialize bitmask
                  for (byte i=0;i<8;i++) {            // for the 8 lamps currently being processed
                    if (TurnOff[c][x] & Mask) {
                      if (c) {
                        pixels.setPixelColor(x*8+i, pixels.Color(LampMax[x*8+i][0]/5*(c),LampMax[x*8+i][1]/5*(c),LampMax[x*8+i][2]/5*(c)));}
                      else {                          // for the last level turn it completely off
                        pixels.setPixelColor(x*8+i,pixels.Color(0,0,0));}}
                    Mask = Mask << 1;}}
                TurnOff[c][x] = TurnOff[c+1][x];}}}
          if (OwnCommands) {                          // a good place to let an own command run once per refresh cycle
            if (OwnCommands & 1) {                    // check which command is meant
              if (!(OwnCommandStep % 5)) {            // only be active every 5th refresh cycle
                byte Step = OwnCommandStep / 5;       // calculate the current step
                for (byte i=0;i<6;i++) {              // pattern has 6 fading grades
                  if (Step+i < 12) {                  // it's for 12 LEDs
                    pixels.setPixelColor(Step+i,OwnPattern[i][0],OwnPattern[i][1],OwnPattern[i][2]);}
                  else {
                    pixels.setPixelColor(Step+i-12,OwnPattern[i][0],OwnPattern[i][1],OwnPattern[i][2]);}}}
              OwnCommandStep++;
              if (OwnCommandStep > 59) {
                OwnCommandStep = 0;}}}
          pixels.show();                              // update the LEDs
          break;
        case 192:                                     // color select command
          Command = 192;
          CommandCount = 3;                           // 3 bytes to read for this command
          break;
        case 193:                                     // configure number of LED bytes
          Command = 193;
          CommandCount = 1;
          break;
        case 195:                                     // set color for LED
          Command = 195;
          CommandCount = 1;
          break;
        case 196:                                     // turn off all LEDs
          for (byte i=0;i<NumOfLEDbytes*8;i++) {      // for all LEDs
            pixels.setPixelColor(i, pixels.Color(0,0,0));} // turn them off
          for (byte i=0;i<NumOfLEDbytes;i++) {        // for all LED bytes
            for (byte x=0;x<6;x++) {                  // disable fading
              TurnOn[x][i] = 0;
              TurnOff[x][i] = 0;}
            LampStatus[i] = 0;}                       // change the status to off
          pixels.show();                              // update the LEDs
          break;
        }}}}}
