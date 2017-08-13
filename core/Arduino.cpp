#include "Arduino.h"
#include "SoftwareSerial.h"
#include <pthread.h>
#include <time.h>

int pinModes[NUM_DIGITAL_PINS];
int pinStates[NUM_DIGITAL_PINS];
char errors[NUM_DIGITAL_PINS][30];
float vcc = 5.0f;
int speedDivisor = -1;

bool simulationRunning = false;
bool simulationPaused = false;
bool simulationWaiting = false;
ulong simulationStart = 0;
ulong simulationPausedStart = 0;

extern msg serialmsg[SERIALBUFFER_SIZE];

void modeString(char *buffer, uint8_t pin){
    //char buffer[8];
    if(pinModes[pin] == -1){
        strcpy(buffer, "UNDEF");
    } else if(pinModes[pin] == INPUT){
        strcpy(buffer, "INPUT");
    } else if(pinModes[pin] == OUTPUT){
        strcpy(buffer, "OUTPUT");
    } else if(pinModes[pin] == INPUT_PULLUP){
        strcpy(buffer, "INPUT_P");
    } else if(pinModes[pin] == ANALOG){
        strcpy(buffer, "ANALOG");
    } else if(pinModes[pin] == SERIAL){
        strcpy(buffer, "SERIAL");
    } 
}

void logger(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    fprintf(stderr, format, arg);
    //n=sprintf (buffer, format, arg);
    va_end(arg);
}

void init(void){
    for(int i = 0; i < NUM_DIGITAL_PINS; i++){
        pinModes[i] = -1;
        pinStates[i] = LOW;
        errors[i][0] = 0;
    }
    for(int i = 0; i < SERIALBUFFER_SIZE; i++){
        memset(&serialmsg[i].payload, 0, sizeof(char[32]));
        memset(&serialmsg[i].time,    0, sizeof(unsigned long));
    }
}

void pinMode(uint8_t pin, uint8_t mode){
    pinModes[pin] = mode;
}

void digitalWrite(uint8_t pin, uint8_t state){
    if(pinModes[pin] == OUTPUT){
        pinStates[pin] = state;
    }
    else{
        char buffer[30];
        char state[8];
        modeString(state, pin);
        sprintf(buffer, "ERROR: %s, not OUTPUT", state);
        strcpy(( char *) errors[pin],buffer);
    }
}

int digitalRead(uint8_t pin){
    if(pinModes[pin] == -1 || pinModes[pin] == INPUT || pinModes[pin] == OUTPUT || pinModes[pin] == INPUT_PULLUP){
        return pinStates[pin];
    }
    else{
        char buffer[30];
        char state[8];
        modeString(state, pin);
        sprintf(buffer, "ERROR: %s, not INPUT", state);
        strcpy(( char *) errors[pin],buffer);
        
        return -1;
    }
    
}

int analogRead(uint8_t pin){
    if(pinModes[pin] == -1 || pinModes[pin] == ANALOG || pinModes[pin] == INPUT){
        pinModes[pin] = ANALOG;
    }
    else{
        char buffer[30];
        char state[8];
        modeString(state, pin);
        sprintf(buffer, "ERROR: ANALOG pin %i=%s", pin, state);
        strcpy(( char *) errors[pin],buffer);
    }
    return pinStates[pin];
}

void analogReference(uint8_t mode){
    
}

void analogWrite(uint8_t pin, int pwm){
    pinStates[pin] = -pwm;
}

unsigned long millis(void){
    ulong milliseconds = realmillis() - simulationStart;

   if(speedDivisor < 0){
       return (unsigned long)(milliseconds * -speedDivisor);
   } else {
       return (unsigned long)(milliseconds / speedDivisor);
   }

}

unsigned long micros(void){
    ulong microseconds = realmicros() - simulationStart*1000;

   if(speedDivisor < 0){
       return (unsigned long)(microseconds * -speedDivisor);
   } else {
       return (unsigned long)(microseconds / speedDivisor);
   }
}


unsigned long realmillis(void){
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    ulong milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

unsigned long realmicros(void){
	struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    ulong microseconds = te.tv_sec*1000000LL + te.tv_usec; // calculate microseconds
    return microseconds;
}



void delay(unsigned long ms){
    static struct timespec tim;
    tim.tv_sec  = 0;
    tim.tv_nsec = 2 * 1000000L;
    ulong start = millis();
    while(millis() < (start + ms)){
        pthread_testcancel();
        nanosleep(&tim, NULL);

        if(simulationPaused){
            simulationWaiting = true;
            while (simulationPaused) {
                pthread_testcancel();
                nanosleep(&tim, NULL);
            }
            simulationWaiting = false;
        }
        
    }
}

void delayMicroseconds(unsigned int us){
    ulong start = micros();
    while(micros() < (start + us)){
        pthread_testcancel();
    }
}