/***************************************************************************
 *                                                                         *
 *   Mini library for particular Christmas lights                          *
 *   project based on TLC59731                                             *
 *   (c) 2015 Jan Prihoda                                                  *
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
#include <math.h>

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

// Animations


// Light falls down with gaus curve
void christmasLights::allLightsFallDown(int count, int delayMs) {

 for (int c = 0; c < count; c++) {   
    byte led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12 = 0;
  
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  
  delay(delayMs);
  
  led1 = 16;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);
  
  led2 = led1;
  led1 = led1 * 2; //32
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led3 = led2;
  led2 = led1;
  led1 = led1 * 2; //64
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led4 = led3;
  led3 = led2;
  led2 = led1;
  led1 = led1 * 2; //128
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led5 = led4;
  led4 = led3;
  led3 = led2;
  led2 = led1;
  led1 = (led1 * 2) - 1; //(255)
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led6 = led5;
  led5 = led4;
  led4 = led3;
  led3 = led2;
  led2 = led1;
  led1 = (led1 + 1) / 2; //128
    setColumn(0, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);
  delay(delayMs);

  led6 = led5;
  led5 = led4;
  led4 = led3;
  led3 = led2;
  led2 = led1;
  led1 = (led1 + 1) / 2; //64
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led7 = led6;
  led6 = led5;
  led5 = led4;
  led4 = led3;
  led3 = led2;
  led2 = led1;
  led1 = (led1 + 1) / 2; //32
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led8 = led7;
  led7 = led6;
  led6 = led5;
  led5 = led4;
  led4 = led3;
  led3 = led2;
  led2 = led1;
  led1 = (led1 + 1) / 2; //16
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led9 = led8;
  led8 = led7;
  led7 = led6;
  led6 = led5;
  led5 = led4;
  led4 = led3;
  led3 = led2;
  led2 = led1;
  led1 = 0;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led10 = led9;
  led9 = led8;
  led8 = led7;
  led7 = led6;
  led6 = led5;
  led5 = led4;
  led4 = led3;
  led3 = led2;
  led2 = led1;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led11 = led10;
  led10 = led9;
  led9 = led8;
  led8 = led7;
  led7 = led6;
  led6 = led5;
  led5 = led4;
  led4 = led3;
  led3 = led2;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led12 = led11;
  led11 = led10;
  led10 = led9;
  led9 = led8;
  led8 = led7;
  led7 = led6;
  led6 = led5;
  led5 = led4;
  led4 = led3;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led12 = led11;
  led11 = led10;
  led10 = led9;
  led9 = led8;
  led8 = led7;
  led7 = led6;
  led6 = led5;
  led5 = led4;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led12 = led11;
  led11 = led10;
  led10 = led9;
  led9 = led8;
  led8 = led7;
  led7 = led6;
  led6 = led5;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led12 = led11;
  led11 = led10;
  led10 = led9;
  led9 = led8;
  led8 = led7;
  led7 = led6;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led12 = led11;
  led11 = led10;
  led10 = led9;
  led9 = led8;
  led8 = led7;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led12 = led11;
  led11 = led10;
  led10 = led9;
  led9 = led8;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led12 = led11;
  led11 = led10;
  led10 = led9;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led12 = led11;
  led11 = led10;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  led12 = led11;
  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);

  for(int i = 0; i <= 11; i++) {
    setColumn(i, led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12);     
  }
  delay(delayMs);
 }
}

// Randomly fill full pattern to time limit
void christmasLights::randomLightToLimit(int limit, int delayMs) {  

    for(int i = 0; i < 12; i++) {
      for(int j = 0; j < 12; j++) {
        pixelMap[i][j] = 0;
      }
    }

  for(int i = 0; i < limit; i++) {

    int column = random(0,12);         
    setPixel(column,random(0,12),255);
    setStripe(column);

    delay(delayMs);

  }
} 

// Randomly lights up one led, clear the rest
void christmasLights::randomLight(int limit, int delayMs) {  

  int repetition = 0;
  
  while(repetition < limit) {
    for(int i = 0; i < 12; i++) {
      for(int j = 0; j < 12; j++) {
        pixelMap[i][j] = 0;
      }
    }

    setPixel(random(0,12),random(0,12),255);

    for(int k = 0; k < 12; k++) {
      setStripe(k);
    } 

    delay(delayMs);
    repetition++;
  }
}
//"breath", limit is number of reccurency
void christmasLights::breath(int limit) {
                                          
  int counter = 0;
  while(true) {
    float val = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
    setFullPattern(val);
    counter++;
    if(counter == limit) {
      break;
    }
  }  
}

// Upravit
// Rotate full stripe to one direction with tail
void christmasLights::stripeRotateWithTail(int count, int delayMs) {

  int start = true;
  int end = false;

  for(int i = 0; i < count; i++) {
    if(i == 0) {
      start = true;
      end = false;
    }
    if(i > 0) {
      start = false;
      end = false;
    }
    if(i == count - 1) {
      start = false;
      end = true;
    }
    
    
    
    
    // Start of animation
    if(start == true) {
      setFullColumn(0,255);
      delay(delayMs);
    
      setFullColumn(1,255);
      setFullColumn(0,128);
      delay(delayMs);
    
      setFullColumn(2,255);
      setFullColumn(1,128);
      setFullColumn(0,64);
      delay(delayMs);
    
      setFullColumn(3,255);
      setFullColumn(2,128);
      setFullColumn(1,64);
      setFullColumn(0,32);
      delay(delayMs);
    
      setFullColumn(4,255);
      setFullColumn(3,128);
      setFullColumn(2,64);
      setFullColumn(1,32);
      setFullColumn(0,16);
      delay(delayMs);
    } 
    else {
      // Same part for all
      setFullColumn(5,255);
      setFullColumn(4,128);
      setFullColumn(3,64);
      setFullColumn(2,32);
      setFullColumn(1,16);
      setFullColumn(0,0);
      delay(delayMs);
    
      setFullColumn(6,255);
      setFullColumn(5,128);
      setFullColumn(4,64);
      setFullColumn(3,32);
      setFullColumn(2,16);
      setFullColumn(1,0);
      delay(delayMs);
    
      setFullColumn(7,255);
      setFullColumn(6,128);
      setFullColumn(5,64);
      setFullColumn(4,32);
      setFullColumn(3,16);
      setFullColumn(2,0);
      delay(delayMs);
    
      setFullColumn(8,255);
      setFullColumn(7,128);
      setFullColumn(6,64);
      setFullColumn(5,32);
      setFullColumn(4,16);
      setFullColumn(3,0);
      delay(delayMs);
    
      setFullColumn(9,255);
      setFullColumn(8,128);
      setFullColumn(7,64);
      setFullColumn(6,32);
      setFullColumn(5,16);
      setFullColumn(4,0);
      delay(delayMs);
    
      setFullColumn(10,255);
      setFullColumn(9,128);
      setFullColumn(8,64);
      setFullColumn(7,32);
      setFullColumn(6,16);
      setFullColumn(5,0);
      delay(delayMs);
    
      setFullColumn(11,255);
      setFullColumn(10,128);
      setFullColumn(9,64);
      setFullColumn(8,32);
      setFullColumn(7,16);
      setFullColumn(6,0);
      delay(delayMs);
    }
  // ->Skok sem
  
  // Má zmizet nebo ne?
  //ANO
    if(end == true) {
      setFullColumn(11,128);
      setFullColumn(10,64);
      setFullColumn(9,32);
      setFullColumn(8,16);
      setFullColumn(7,0);
      delay(delayMs);
    
      setFullColumn(11,64);
      setFullColumn(10,32);
      setFullColumn(9,16);
      setFullColumn(8,0);
      delay(delayMs);
    
      setFullColumn(11,32);
      setFullColumn(10,16);
      setFullColumn(9,0);
      delay(delayMs);
    
      setFullColumn(11,16);
      setFullColumn(10,0);
      delay(delayMs);
    
      setFullColumn(11,0);
      delay(delayMs);
    }
    else {
      setFullColumn(0,255);
      setFullColumn(11,128);
      setFullColumn(10,64);
      setFullColumn(9,32);
      setFullColumn(8,16);
      setFullColumn(7,0);
      delay(delayMs);
    
      setFullColumn(1,255);
      setFullColumn(0,128);
      setFullColumn(11,64);
      setFullColumn(10,32);
      setFullColumn(9,16);
      setFullColumn(8,0);
      delay(delayMs);
    
      setFullColumn(2,255);
      setFullColumn(1,128);
      setFullColumn(0,64);
      setFullColumn(11,32);
      setFullColumn(10,16);
      setFullColumn(9,0);
      delay(delayMs);
    
      setFullColumn(3,255);
      setFullColumn(2,128);
      setFullColumn(1,64);
      setFullColumn(0,32);
      setFullColumn(11,16);
      setFullColumn(10,0);
      delay(delayMs);
    
      setFullColumn(4,255);
      setFullColumn(3,128);
      setFullColumn(2,64);
      setFullColumn(1,32);
      setFullColumn(0,16);
      setFullColumn(11,0);
      delay(delayMs);
    }
  }    
}

// Rotote to full stripe to one direction, then clean
void christmasLights::stripeRotateToFull(int grayScale, int delayMs) {

  for(int i = 0; i < 12; i++) {
    setFullColumn(i,grayScale);  
    delay(delayMs);
  }
  for(int i = 0; i < 12; i++) {
    setFullColumn(i,0);  
    delay(delayMs);
  }
}

// One row fall down
void christmasLights::rowFallDown(int grayScale, int delayMs) {

  for(int i = 0; i < 12; i++) {
    setFullRow(i,grayScale);  
    if(i > 0) {
      setFullRow(i-1,0);      
      }
    delay(delayMs);
  }
  setFullRow(11,0);
  delay(delayMs);
}

// Row fall down to full pattern
void christmasLights::rowFallDownToFull(int grayScale, int delayMs) {

  for(int j = 0; j < 12; j++) {
    for(int i = 0; i < 12-j; i++) {
      setFullRow(i,grayScale);  
      if(i > 0) {
        setFullRow(i-1,0);      
      }
      delay(delayMs);
    }
  }
}

// Black row fall down to clear pattern
void christmasLights::rowFallDownToClear(int grayScale, int delayMs) {

  for(int j = 0; j < 12; j++) {
    for(int i = 0; i < 12-j; i++) {
      setFullRow(i,grayScale);  
      if(i > 0) {
        setFullRow(i-1,255);      
      }
      delay(delayMs);
    }
  }
}

// Each LED continully fill row and column
void christmasLights::lightEachLedByRownAndColumn(int grayScale, int delayMs) {

  for(int i = 0; i < 12; i++) {
    for(int j = 0; j < 12; j++) {
      setPixel(j, i, grayScale);
      delay(delayMs);
    }
  }
}