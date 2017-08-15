/*
SoftwareSerial.cpp (formerly NewSoftSerial.cpp) - 
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

#include "Arduino.h"
#include "SoftwareSerial.h"
#include <stdlib.h>

msg serialmsg[SERIALBUFFER_SIZE];

//
// Constructor
//
SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin):
	_receivePin(receivePin),
	_transmitPin(transmitPin)
{

}

SoftwareSerial::~SoftwareSerial()
{
}

void SoftwareSerial::begin(long speed)
{
	hasBegun = true;
	pinMode(_receivePin, SERIAL);
	pinMode(_transmitPin, SERIAL);
}
void SoftwareSerial::end()
{
	hasBegun = false;
	pinMode(_receivePin, -1);
	pinMode(_transmitPin, -1);
}


// Read data from buffer
int SoftwareSerial::read()
{
	return 0;  
}

int SoftwareSerial::available()
{
	return 0;
}

void SoftwareSerial::shift(void){
	for(int i = SERIALBUFFER_SIZE-2; i >= 0; i--){
		memcpy(&serialmsg[i+1], &serialmsg[i], sizeof(msg));
	}
	memset(&serialmsg[0].payload, 0, sizeof(char[SERIALBUFFER_PAYLOAD]));
	memset(&serialmsg[0].time,    0, sizeof(unsigned long));
}

size_t SoftwareSerial::write(uint8_t c)
{
	if(hasBegun){
		if(c == '\n'){
			 shift();
		} else{
			unsigned long len = strlen(serialmsg[0].payload);
			serialmsg[0].payload[len] = c;
			serialmsg[0].payload[len+1] = '\0';
			serialmsg[0].time = millis();
		}
		return sizeof(c);
	} else {
		hasBegun = true;
		write("Written to Serial without calling begin()\n");
		hasBegun = false;
		return 0;
	}
}

size_t SoftwareSerial::write(const char *str){
	return write(str, strlen(str));
}

size_t SoftwareSerial::write(const uint8_t *buffer, size_t size)
{
	size_t n = 0;
	while (size--) {
		if (write(*buffer++)) {
			n++;
		}
		else{
			break;
		}
	}
	return n;
}