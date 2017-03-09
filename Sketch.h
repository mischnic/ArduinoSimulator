#ifndef Sketch_h
#define Sketch_h

typedef struct  {
	char desc[16];
	int value;
} Data;

void setup(void);
void loop(void);
void getState(Data *d, int id);

#endif
