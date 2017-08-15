#include "Simulation.h"

extern bool simulationRunning;
extern bool simulationPaused;
extern bool simulationWaiting;
extern ulong simulationStart;
extern ulong simulationPausedStart;

void* run(void *arg){

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	call(SETUP_NAME);

	static struct timespec tim;
	tim.tv_sec  = 0;
	tim.tv_nsec = 3 * 1000000L;
	
	for(;;){

		if(simulationPaused){
			simulationWaiting = true;
			while (simulationPaused) {
				pthread_testcancel();
				nanosleep(&tim, NULL);
			}
			simulationWaiting = false;
		}

		pthread_testcancel();
		call(LOOP_NAME);
	}
	return 0;
}