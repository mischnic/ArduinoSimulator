#ifndef Sketch_h
#define Sketch_h


#define DATA_PIN    (1<<7)
#define DATA_INFO   (1<<8)
#define DATA_MASK ~(11<<7)

typedef struct {
	char desc[16];
	enum { aint, afloat , abool} vtype;
	union {
		int i;
		float f;
		bool b;
	};
} Data;

void setup(void);
void loop(void);
void getState(Data *d, int id);

#endif
