#ifndef CallUtils_h
#define CallUtils_h

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>


#include "Sketch.h"

typedef void (*func)(void);
typedef void (*func2)(Data*, int);

void call(const char function[]);

void callGetState(Data *d, int id);

#endif