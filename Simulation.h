#ifndef Simulation_h
#define Simulation_h

#include "Arduino.h"
#include "Sketch.h"
#include "CallUtils.h"
#include <pthread.h>

#define SETUP_NAME "_Z5setupv"
#define LOOP_NAME "_Z4loopv"

void* run(void *arg);

#endif