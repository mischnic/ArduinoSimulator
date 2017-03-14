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

void startSim();
void stopSim();
void pauseSim();
void unpauseSim();
void toggleSimPause();
void quit();

void printDesc(bool);
void renderMotor(int, int, int);

void update();

int main();

#endif