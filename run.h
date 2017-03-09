#ifndef run_h
#define run_h


#include <ncurses.h>

#include <pthread.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Sketch.h"
#include "Simulation.h"
#include "kbhit.h"
#include "SoftwareSerial.h"
#include "CallUtils.h"


int guiMain(void);

#endif