#ifndef Sketch_h
#define Sketch_h

#include "Arduino.h"

#define DATA_PIN    (1<<7)
#define DATA_INFO   (1<<8)
#define DATA_MASK ~(11<<7)

typedef struct {
	enum Dirs {NONE=0, RIGHT, LEFT} dir:2;
	enum States {WHITE=0, RED, GREEN} state:2;
} MotorState;

typedef struct {
	char desc[16];
	enum { aint, afloat , abool, aMotor} vtype;
	union {
		int i;
		float f;
		bool b;
		MotorState ms;
	};
} Data;

void setup(void);
void loop(void);
void getState(Data *d, int id);

#endif
