/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pins_arduino_tiny84.h"
#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>

#ifndef IS_SIM
#  define IS_SIM 1
#endif

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define ANALOG 0x3
#define SERIAL 0x4

#define DEFAULT 0
#define EXTERNAL 1
#define INTERNAL 2

typedef unsigned long ulong;
typedef bool boolean;
typedef uint8_t byte;

void modeString(uint8_t pin);

void logger(const char *format, ...);

void init(void);



void pinMode(uint8_t pin, uint8_t mode);

void digitalWrite(uint8_t pin, uint8_t state);

int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);

void analogReference(uint8_t mode);

void analogWrite(uint8_t pin, int pwm);

unsigned long millis(void);
unsigned long micros(void);
unsigned long realmillis(void);
unsigned long realmicros(void);

void delay(unsigned long ms);

void delayMicroseconds(unsigned int us);

#endif