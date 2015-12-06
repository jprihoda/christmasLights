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


class christmasLights {

  public:
    // Basic functions
    void setFullRow(int row, byte grayScale);
    void setFullColumn(int column, byte grayScale);
    void setFullPattern(byte grayScale);
    
    // Advanced functions
    void setPixel(int column, int row, byte grayScale);

    void setRow(int row, byte led1, byte led2, byte led3, byte led4, byte led5, byte led6, byte led7, byte led8, byte led9, byte led10, byte led11, byte led12 );
    void setColumn(int column, byte led1, byte led2, byte led3, byte led4, byte led5, byte led6, byte led7, byte led8, byte led9, byte led10, byte led11, byte led12 );
    void setPattern();

    // Animations

    void allLightsFallDown(int count, int delayMs);
    void randomLightToLimit(int limit, int delayMs);
    void randomLight(int limit, int delayMs);
    void breath(int limit);
    void stripeRotateWithTail(int count, int delayMs);
    void stripeRotateToFull(int grayScale, int delayMs);
    void rowFallDown(int grayScale, int delayMs);
    void rowFallDownToFull(int grayScale, int delayMs);
    void rowFallDownToClear(int grayScale, int delayMs);
    void lightEachLedByRownAndColumn(int grayScale, int delayMs);
    

    // Direct pattern manipulation
    void writePixelMap(int column, int row, byte grayScale);

  private:

    void writeZero(int mcuPin);
    void writeOne(int mcuPin);
    void endOfSequence();
    void gsDataLatchSequence();
    void setLed(int mcuPin, byte out0, byte out1, byte out2);
    void writeData(int mcuPin, byte data);

    void setStripe(int stripeNumber);

};