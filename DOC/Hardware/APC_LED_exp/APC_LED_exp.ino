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
byte Sync = 8;                                          // ms after last Sync
byte Mode = 0;                                          // Mode 0 -> lamps being lit get the ColorSelect color / Mode 1 -> lamps keep their color / Mode 2 -> lamps set in the following frame get the new color immediately
byte Command = 0;                                       // LED command currently being processed
byte CommandCount = 0;                                  // counts the bytes received by the color select command
byte OwnCommands = 0;                                   // indicate active own LED commands
byte OwnCommandStep = 0;                                // needed for own LED commands
byte TurnOn[6][8];                                      // the list of the lamps currently being turned on
byte TurnOff[6][8];                                     // the list of the lamps currently being turned off
const byte OwnPattern[6][3] = {{0,0,0},{0,50,0},{0,100,0},{0,150,0},{0,200,0},{0,250,0}};

void setup() {
  pixels.begin();
  for (byte i=0;i<9;i++) {
    pinMode(i, INPUT_PULLUP);}
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(12, LOW);
  for (byte i=0;i<64;i++) {
    for (byte c=0;c<3;c++) {
      LampMax[i][c] = 255;}}
  RecFlag = PINB && 1;}

void loop() {
  if ((PINB && 1) != RecFlag) {                         // get byte
    RecByte = PIND;                                     // store it
    RecFlag = !RecFlag;
    if (Sync<8) {                                       // if the last sync happened less than 8 cycles ago
      if (Mode < 2) {                                   // LEDs fade on and off
        for (byte c=0;c<5;c++) {                             // for 5 brightness levels
          if (TurnOn[c][Sync]) {                        // anything to do in this byte?
            byte Mask = 1;                              // initialize bitmask
            for (byte i=0;i<8;i++) {                    // for the 8 lamps currently being processed
              if (TurnOn[c][Sync] & Mask) {
                if (c) {
                  pixels.setPixelColor(Sync*8+i, pixels.Color(LampMax[Sync*8+i][0]/5*(5-c),LampMax[Sync*8+i][1]/5*(5-c),LampMax[Sync*8+i][2]/5*(5-c)));}
                else {                                  // for the last level turn it fully on
                  pixels.setPixelColor(Sync*8+i, pixels.Color(LampMax[Sync*8+i][0],LampMax[Sync*8+i][1],LampMax[Sync*8+i][2]));}}
              Mask = Mask << 1;}}
          TurnOn[c][Sync] = TurnOn[c+1][Sync];
          if (TurnOff[c][Sync]) {
            byte Mask = 1;                              // initialize bitmask
            for (byte i=0;i<8;i++) {                    // for the 8 lamps currently being processed
              if (TurnOff[c][Sync] & Mask) {
                if (c) {
                  pixels.setPixelColor(Sync*8+i, pixels.Color(LampMax[Sync*8+i][0]/5*(c),LampMax[Sync*8+i][1]/5*(c),LampMax[Sync*8+i][2]/5*(c)));}
                else {                                  // for the last level turn it completely off
                  pixels.setPixelColor(Sync*8+i,pixels.Color(0,0,0));}}
              Mask = Mask << 1;}}
          TurnOff[c][Sync] = TurnOff[c+1][Sync];}
        if (RecByte != LampStatus[Sync]) {              // any status changes?
          byte Mask = 1;                                // initialize bitmask
          for (byte i=0;i<8;i++) {
            if ((RecByte & Mask) && !(LampStatus[Sync] & Mask)) { // lamp turned on
              if (Mode == 0) {                          // New lit lamps get a new color
                for (byte c=0;c<3;c++) {                // set max brightness of lamp to selected max value
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
      else {                                            // Mode > 1
        if (Mode < 4) {                                 // selected lamps get the selected color immediately
          if (RecByte) {                                // any lamps set?
            byte Mask = 1;
            for (byte i=0;i<8;i++) {                    // for the 8 lamps currently being processed
              if (RecByte & Mask) {                     // lamp set in RecByte?
                if (Mode == 2)  {                       // selected LEDs are also turned on
                  for (byte c=0;c<5;c++) {              // and 5 brightness levels
                    TurnOn[c][Sync] &= (255 - Mask);    // stop lamp from TurningOn - Off
                    TurnOff[c][Sync] &= (255 - Mask);}
                  LampStatus[Sync] |= Mask;             // turn on lamp
                  pixels.setPixelColor(Sync*8+i, pixels.Color(LampMaxSel[0],LampMaxSel[1],LampMaxSel[2]));}
                else {                                  // Mode = 3
                  if (LampStatus[Sync] & Mask) {        // LED on?
                    pixels.setPixelColor(Sync*8+i, pixels.Color(LampMaxSel[0],LampMaxSel[1],LampMaxSel[2]));}}
                for (byte c=0;c<3;c++) {                // set max brightness of lamp to selected max value
                  LampMax[Sync*8+i][c] = LampMaxSel[c];}}
              Mask = Mask << 1;}}}}
      if (!Sync && OwnCommands) {                       // a good place to let an own command run once per refresh cycle
        if (OwnCommands & 1) {                          // check which command is meant
          if (!(OwnCommandStep % 5)) {                  // only be active every 5th refresh cycle
            byte Step = OwnCommandStep / 5;             // calculate the current step
            for (byte i=0;i<6;i++) {                    // pattern has 6 fading grades
              if (Step+i < 12) {                        // it's for 12 LEDs
                pixels.setPixelColor(Step+i,OwnPattern[i][0],OwnPattern[i][1],OwnPattern[i][2]);}
              else {
                pixels.setPixelColor(Step+i-12,OwnPattern[i][0],OwnPattern[i][1],OwnPattern[i][2]);}}}
          OwnCommandStep++;
          if (OwnCommandStep > 59) {
            OwnCommandStep = 0;}}}
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
        case 67:                                        // Mode 3 -> only the color of the LEDs is changed, but they're not turned on
          Mode = 3;
          break;
        case 68:                                        // Mode 4 -> LED state is frozen
          Mode = 4;
          break;
        case 100:
          OwnCommands |= 1;                             // activate OwnCommand number 1
          break;
        case 101:
          OwnCommands &= 254;                           // deactivate OwnCommand number 1
          for (byte i=0;i<12;i++) {                     // for all affected LEDs
            pixels.setPixelColor(i, pixels.Color(0,0,0)); // turn them off
            LampStatus[i / 8] &= 255-(1<<(i % 8));}     // and change the status to off
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
