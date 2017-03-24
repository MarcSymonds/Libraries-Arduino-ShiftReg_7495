/* Project: 7495 4-Bit Shift Register Interfacing.
 * File: Shift_7495.h
 * Description: Interface with a 7495 4-bit shift register in
 *     parallel or serial mode.
 *     Uses the DirectIO library.
 *
 * Copyright (C) 2014 Marc Symonds
 * All rights reserved.
 *
 * This software may be used and redistributed, with or without
 * modification, as long as it is understood that this software
 * is provided as-is without any explicit or implied warranties
 * of merchant-ability or fitness of purpose.
 */
 
#ifndef ShiftReg_7495_h
#define ShiftReg_7495_h

#include "Arduino.h"
#include "DirectIO.h"

class ShiftReg_7495
{
  public:
    ShiftReg_7495(byte clockPin, byte dataPin); // Serial mode.
    ShiftReg_7495(byte clockPin, byte aPin, byte bPin, byte cPin); // Parallel mode.

    void setBit(byte bit, boolean state);
    void send();
    void sendPrime();
    void sendTrigger();
    void clear();

  private:
    DIO_SetPinFPtr _clockPin;
    DIO_SetPinFPtr _dataPin;
    
    DIO_SetPinFPtr _aPin;
    DIO_SetPinFPtr _bPin;
    DIO_SetPinFPtr _cPin;

    boolean _currBits[3];
    boolean _newBits[3];

    boolean _serialMode;

    void sendBit(boolean state);
};

#endif
