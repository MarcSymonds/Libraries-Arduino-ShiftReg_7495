/* Project: 7495 4-Bit Shift Register Interfacing.
 * File: Shift_7495.cpp
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
 
#include "Arduino.h"
#include "ShiftReg_7495.h"
#include "DirectIO.h"

// Constructor for serial mode.

ShiftReg_7495::ShiftReg_7495(byte clockPin, byte dataPin)
{
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  _clockPin = DIO_GetSetPinFunction(clockPin);
  _dataPin = DIO_GetSetPinFunction(dataPin);

  _clockPin(true);

  _serialMode = true;

  clear();
}

// Constructor for parallel mode.

ShiftReg_7495::ShiftReg_7495(byte clockPin, byte aPin, byte bPin, byte cPin)
{
  pinMode(clockPin, OUTPUT);
  pinMode(aPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(cPin, OUTPUT);

  _clockPin = DIO_GetSetPinFunction(clockPin);
  _aPin = DIO_GetSetPinFunction(aPin);
  _bPin = DIO_GetSetPinFunction(bPin);
  _cPin = DIO_GetSetPinFunction(cPin);

  _clockPin(true);

  _serialMode = false;

  clear();
}

void ShiftReg_7495::setBit(byte bit, boolean state)
{
  _newBits[bit] = state;
}

// Send the data to the 7495 - Serial or parallel.

void ShiftReg_7495::send()
{
  if (_serialMode)
  {
    if (_currBits[0] == _newBits[0] && _currBits[1] == _newBits[1] && _currBits[2] == _newBits[2])
      ; // Don't need to do anything, the register aleady contains the bits we are going to show.
    else
    {
      if (_newBits[2] == _currBits[1] && _newBits[1] == _currBits[0])
        sendBit(_newBits[0]);
      else
      {
        if (_newBits[2] != _currBits[0])
          sendBit(_newBits[2]);

        sendBit(_newBits[1]);
        sendBit(_newBits[0]);
      }

      _currBits[0] = _newBits[0];
      _currBits[1] = _newBits[1];
      _currBits[2] = _newBits[2];
    }
  }
  else
  {
    sendPrime();
    sendTrigger();
  }
}

void ShiftReg_7495::sendPrime()
{
  _clockPin(true);

  _aPin(_newBits[0]);
  _bPin(_newBits[1]);
  _cPin(_newBits[2]);
}

void ShiftReg_7495::sendTrigger()
{
  _clockPin(false);
}

void ShiftReg_7495::sendBit(boolean state)
{
  _clockPin(true);
  _dataPin(state);
  _clockPin(false);
}

void ShiftReg_7495::clear()
{
  for (byte i = 0 ; i < 3 ; i++)
  {
    _currBits[i] = true;
    _newBits[i] = false;
  }

  send();
}