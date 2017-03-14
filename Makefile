CXX = clang++

# enable for homebrew version
# LDFLAGS = -L/usr/local/opt/ncurses/lib
# CPPFLAGS = -I/usr/local/opt/ncurses/include

# enable for debugging
# CPPFLAGS = -ggdb

CPPFLAGS = -I. -Icore

OBJS = CallUtils.o run.o Simulation.o

.PHONY: clean
clean : 
	rm -f $(OBJS) libArduinoSimulator.dylib

.PHONY: lib
lib: libArduinoSimulator.dylib

.PHONY: core/core.a
core/core.a:
	$(MAKE) -C core

.DEFAULT_GOAL := libArduinoSimulator.dylib
libArduinoSimulator.dylib: core/core.a $(OBJS)
	$(CXX) -dynamiclib -fPIC -o libArduinoSimulator.dylib $^ -lncurses -lc++ -ldl


CallUtils.o:	  CallUtils.h  		Sketch.h
run.o:			  run.h  			Sketch.h Simulation.h kbhit.h core/SoftwareSerial.h CallUtils.h
Simulation.o:	  Simulation.h  	core/Arduino.h Sketch.h CallUtils.h
