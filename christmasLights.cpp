/***************************************************************************
 *                                                                         *
 *   Mini library for particular Christmas lights project                  *
 *   (c) 2012 Jan Prihoda                                                  *
 *                                                                         *
 *                                                                         *
 ***************************************************************************
 *                                                                         * 
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU License.                                  *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU License V2 for more details.                                        *
 *                                                                         *
 ***************************************************************************/

#include "Arduino.h"
#include "christmasLights.h"

volatile byte pixelMap[12][12] = {
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 01, index 00
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 02, index 01
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 03, index 02
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 04, index 03
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 05, index 04
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 06, index 05
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 07, index 06
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 08, index 07
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 09, index 08
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 10, index 09
      {0,0,0,0,0,0,0,0,0,0,0,0}, //column 11, index 10
      {0,0,0,0,0,0,0,0,0,0,0,0}  //column 12, index 11
    };

void christmasLights::writeZero(int mcuPin) {

  digitalWrite(mcuPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(mcuPin, LOW);
  delayMicroseconds(15);
  // tCycle = 28 us  (total) 
}

void christmasLights::writeOne(int mcuPin) {

  digitalWrite(mcuPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(mcuPin, LOW);
  delayMicroseconds(2);
  digitalWrite(mcuPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(mcuPin, LOW);
  delayMicroseconds(2);
  // tCycle = 28 us  (total) 
}

void christmasLights::endOfSequence() {
  //28 * 5 us, based on datasheet: 3.5x tCycle - 5.5 tCycle 
  delayMicroseconds(112); //140       //112
}

void christmasLights::gsDataLatchSequence() {
  //28 * 10 us, based on datasheet: minimum 8.0x tCycle
  delayMicroseconds(280);  
}

void christmasLights::setLed(int mcuPin, byte out0, byte out1, byte out2) {

  //Start sequence + 0x3A 
  writeZero(mcuPin);
  writeZero(mcuPin);
  writeOne(mcuPin);
  writeOne(mcuPin);
  writeOne(mcuPin);
  writeZero(mcuPin);
  writeOne(mcuPin);
  writeZero(mcuPin);

  //RGB (OUT0-2) values
  writeData(mcuPin,out0);
  writeData(mcuPin,out1);
  writeData(mcuPin,out2);

  endOfSequence();
  
}

void christmasLights::writeData(int mcuPin, byte data) {
  for (byte i = 0; i<8; i++) {
    if(data & B10000000) {
      writeOne(mcuPin);
    } else { 
      writeZero(mcuPin);
    }
    data <<= 1;
  }
}

// Write sequence for all LEDs in stripe
void christmasLights::setStripe(int stripeNumber) {
  // 03 -> 14, 10 -> 15, 11 -> 16
  // Because setLed needs real pin assignment (0 in array is 2 on mcu - first free pin)   
  int mcuPin = stripeNumber + 2; 
  switch (mcuPin) {
    case 3: mcuPin = 14; break;
    case 10: mcuPin = 15; break;
    case 11: mcuPin = 16; break;
    default: break; 
  }
 
  setLed(mcuPin, pixelMap[stripeNumber][0], pixelMap[stripeNumber][1], pixelMap[stripeNumber][2]);
  setLed(mcuPin, pixelMap[stripeNumber][3], pixelMap[stripeNumber][4], pixelMap[stripeNumber][5]);
  setLed(mcuPin, pixelMap[stripeNumber][6], pixelMap[stripeNumber][7], pixelMap[stripeNumber][8]);
  setLed(mcuPin, pixelMap[stripeNumber][9], pixelMap[stripeNumber][10], pixelMap[stripeNumber][11]);
}

void christmasLights::setFullRow(int row, byte grayScale) {

  for(int i = 0; i < 12; i++) {
    pixelMap[i][row] = grayScale;
    setStripe(i);
    }
}

void christmasLights::setFullColumn(int column, byte grayScale) {

  for(int i = 0; i < 12; i++) {
    pixelMap[column][i] = grayScale;  
  }
  setStripe(column);
}

void christmasLights::setFullPattern(byte grayScale) {

  for(int i = 0; i < 12; i++) {
    for(int j = 0; j < 12; j++) {
      pixelMap[i][j] = grayScale;
    } 
    setStripe(i);
  }
}

void christmasLights::setPixel(int column, int row, byte grayScale) {

  pixelMap[column][row] = grayScale;
  setStripe(column);
}

void christmasLights::setRow(int row, byte led1, byte led2, byte led3, byte led4, byte led5, byte led6, byte led7, byte led8, byte led9, byte led10, byte led11, byte led12 ) {

  pixelMap[0][row] = led1;  
  pixelMap[1][row] = led2;
  pixelMap[2][row] = led3;
  pixelMap[3][row] = led4;
  pixelMap[4][row] = led5;
  pixelMap[5][row] = led6;
  pixelMap[6][row] = led7;
  pixelMap[7][row] = led8;
  pixelMap[8][row] = led9;
  pixelMap[9][row] = led10;
  pixelMap[10][row] = led11;
  pixelMap[11][row] = led12;
  for(int i = 0; i < 12; i++) {
    setStripe(i);
  }
}

void christmasLights::setColumn(int column, byte led1, byte led2, byte led3, byte led4, byte led5, byte led6, byte led7, byte led8, byte led9, byte led10, byte led11, byte led12 ) {

  pixelMap[column][0] = led1;  
  pixelMap[column][1] = led2;
  pixelMap[column][2] = led3;
  pixelMap[column][3] = led4;
  pixelMap[column][4] = led5;
  pixelMap[column][5] = led6;
  pixelMap[column][6] = led7;
  pixelMap[column][7] = led8;
  pixelMap[column][8] = led9;
  pixelMap[column][9] = led10;
  pixelMap[column][10] = led11;
  pixelMap[column][11] = led12;
  setStripe(column);
}

void christmasLights::setPattern() {
  
  for(int i = 0; i < 12; i++) {
    setStripe(i);    
  }
}

void christmasLights::writePixelMap(int column, int row, byte grayScale) {
  
  pixelMap[column][row] = grayScale;
}
