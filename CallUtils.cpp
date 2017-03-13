#include "CallUtils.h"


void call(const char function[]){
	func f = (void (*)(void))dlsym(RTLD_MAIN_ONLY, function);
	char *error;
	if ((error = dlerror()) != NULL)  {
		printf("error: %s\n", error);
	}
	f();
}

void callGetState(Data *d, int id){
	func2 f = (void (*)(Data*, int)) dlsym(RTLD_MAIN_ONLY, "_Z8getStateP4Datai");
	char *error;
	if ((error = dlerror()) != NULL)  {
		// printf("error: %s\n", error);
	} else{
		f(d, id);
	}
}