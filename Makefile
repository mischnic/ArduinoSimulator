CXX = clang++

# enable for homebrew version
# LDFLAGS = -L/usr/local/opt/ncurses/lib
# CPPFLAGS = -I/usr/local/opt/ncurses/include

# enable for debugging
# CPPFLAGS = -ggdb

LIB_OBJS = Arduino.o CallUtils.o run.o Print.o Simulation.o SoftwareSerial.o WString.o 
REBUILDABLES = $(LIB_OBJS) $(LINK_TARGET) $(SKETCH)

.PHONY: clean
clean : 
	rm -f $(REBUILDABLES) main2.o Sketch.ino.o Sketch2.ino.o libArduinoSimulator.dylib

.PHONY: lib
lib: libArduinoSimulator.dylib


.DEFAULT_GOAL := libArduinoSimulator.dylib
libArduinoSimulator.dylib: main.o $(LIB_OBJS)
	$(CXX) -dynamiclib -fPIC -o libArduinoSimulator.dylib main.o $(LIB_OBJS) -lncurses -lc++ -ldl

# # Here is a Rule that uses some built-in Make Macros in its command:
# # $@ expands to the rule's target, in this case "test_me.exe".
# # $^ expands to the rule's dependencies
# # main.o, test1.o, and  test2.o.
# .SECONDEXPANSION:
# sim : $$(SKETCH) libArduinoSimulator.dylib
# 	$(CXX) -o $@ $(SKETCH) $(LDFLAGS) -L. -lArduinoSimulator

# Here is a Pattern Rule, often used for compile-line.
# It says how to create a file with a .o suffix, given a file with a .cpp suffix.
# The rule's command uses some built-in Make Macros:
# $@ for the pattern-matched target
# $< for the pattern-matched dependency
# %.o : %.cpp
# 	$(CXX) -o $@ -c $<

# %.o : %.c
# 	$(CC) -o $@ -c $<

# %.o : %.ino
# 	cp $< $<.cpp
# 	$(CXX) $(CPPFLAGS) -o $@ -c $<.cpp
# 	rm $<.cpp

# These are Dependency Rules, which are rules without any command.
# Dependency Rules indicate that if any file to the right of the colon changes,
# the target to the left of the colon should be considered out-of-date.
# The commands for making an out-of-date target up-to-date may be found elsewhere
# (in this case, by the Pattern Rule above).
# Dependency Rules are often used to capture header file dependencies.
# sim.o : Arduino.h kbhit.h pins_arduino.h pins_arduino_tiny84.h Print.h Printable.h Sketch.h Simulation.h SoftwareSerial.h WString.h
#Test1.o : Test1.h Test2.h
#Test2.o : Test2.h
Arduino.o:		 Arduino.h  	SoftwareSerial.h pins_arduino_tiny84.h
CallUtils.o:	 CallUtils.h  	Sketch.h
run.o:			 run.h  		Sketch.h Simulation.h kbhit.h SoftwareSerial.h CallUtils.h
Print.o:		 Print.h  		Arduino.h Print.h
Simulation.o:	 Simulation.h  	Arduino.h Sketch.h CallUtils.h
SoftwareSerial.o:SoftwareSerial.h  Arduino.h Print.h Printable.h WString.h
WString.o:		 WString.h  

# Alternatively to manually capturing dependencies, several automated
# dependency generators exist.  Here is one possibility (commented out)...
# %.dep : %.cpp
# 	$(CXX) -M $(FLAGS) $< > $@
# include $(OBJS:.o=.dep)