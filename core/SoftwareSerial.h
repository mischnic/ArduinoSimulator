/*
SoftwareSerial.h (formerly NewSoftSerial.h) - 
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
	 (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
	 multi-instance support, porting to 8MHz processors,
	 various optimizations, PROGMEM delay tables, inverse logic and 
	 direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

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

The latest version of this library can always be found at
http://arduiniana.org.
*/

#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <inttypes.h>
#include <stdio.h> // for size_t
#include "Printable.h"
#include "Print.h"

#include "WString.h"

const int SERIALBUFFER_SIZE = 20 + 1;
const int SERIALBUFFER_PAYLOAD = 128;

typedef struct  {
	 unsigned long time;
	 char payload[SERIALBUFFER_PAYLOAD];
} msg;


class SoftwareSerial : public Print
{
private:
	// per object data
	uint8_t _receivePin;
	uint8_t _transmitPin;
	size_t printNumber(unsigned long, uint8_t);
	size_t printFloat(double, uint8_t);
	void shift(void);
public:
	// public methods
	SoftwareSerial(uint8_t receivePin, uint8_t transmitPin);
	~SoftwareSerial();
	void begin(long speed);
	bool listen();
	void end();

	size_t write(uint8_t);
	size_t write(const char *str);
	size_t write(const uint8_t *buffer, size_t size);
	size_t write(const char *buffer, size_t size) {
		return write((const uint8_t *)buffer, size);
	}
	int available();
	int read();
	
	// size_t print(const String &);
	// size_t print(const char[]);
	// size_t print(char);
	// size_t print(unsigned char, int = DEC);
	// size_t print(int, int = DEC);
	// size_t print(unsigned int, int = DEC);
	// size_t print(long, int = DEC);
	// size_t print(unsigned long, int = DEC);
	// size_t print(double, int = 2);
	// size_t print(const Printable&);

	// size_t println(const String &s);
	// size_t println(const char[]);
	// size_t println(char);
	// size_t println(unsigned char, int = DEC);
	// size_t println(int, int = DEC);
	// size_t println(unsigned int, int = DEC);
	// size_t println(long, int = DEC);
	// size_t println(unsigned long, int = DEC);
	// size_t println(double, int = 2);
	// size_t println(const Printable&);
	// size_t println(void);
};

#endif
