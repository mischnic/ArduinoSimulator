CXX = clang++

LIB_OBJS = Arduino.o Print.o SoftwareSerial.o WString.o 

CPPFLAGS = -I..

core.a: $(LIB_OBJS)
	ar rcs core.a  $(LIB_OBJS)
	ranlib core.a

.PHONY: clean
clean : 
	rm -f $(LIB_OBJS) core.a

Arduino.o:		  Arduino.h  		SoftwareSerial.h ../pins_arduino_tiny84.h
Print.o:		  Print.h  			Arduino.h Print.h
SoftwareSerial.o: SoftwareSerial.h  Arduino.h Print.h Printable.h WString.h
WString.o:		  WString.h  
