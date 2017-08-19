# ArduinoSimulator
A minimal Arduino simulator with ncurses "GUI"

Run `make` to build the dynamic library

See [ArduinoSimulatorExample](https://github.com/mischnic/ArduinoSimulatorExample) for how to use it

Uses code from https://github.com/arduino/Arduino and https://github.com/damellis/attiny

# Usage

- `w` = Start/stop
- `e` = Start paused/(un)pause
- `q` = Quit

Pressable Buttons have a background color.
When changing a analog input voltage, the simulation will be paused, valid formats are:

- a voltage: `2v` `2V` `3.1V` ...,
- an integer ranging from 0-1023 (0 = 0V, 1023 = VCC)


Speed:

- normal click for +1 / -1
- double click for twice / half


# API

You **need** this line at the top of your sketch: `#include "Sketch.h"`.

For code specific to the simulator and not for real devices:

    #ifdef IS_SIM
        /* This code will only be included when using the simulator */
    #end
    


To set pin descriptions and have variables printed in the simulation window (or to have a different VCC apart from 5V), use the following:

    void getState(Data *d, int id){
        if(id & DATA_PIN){
            if((id & DATA_MASK) == 1){ // pin 1
                strcpy(d->desc, "LED");
            }
        } else if(id & DATA_INFO){
            int i = 0;
            if((id & DATA_MASK) == i++){
                strcpy(d->desc, "VCC");
                d->vtype = Data::afloat;
                d->f = 5.0f;
            } else if((id & DATA_MASK) == i++){
                strcpy(d->desc, "counter");
                d->vtype = Data::aint;
                d->i = count;
            } else if((id & DATA_MASK) == i++){
                strcpy(d->desc, "counterEven");
                d->vtype = Data::abool;
                d->b = count%2 == 0;
            } else if((id & DATA_MASK) == i++){
                strcpy(d->desc, "counterDec");
                d->vtype = Data::afloat;
                d->f = count/10.0f;
            } else if((id & DATA_MASK) == i++){
                d->vtype = Data::aMotor;
                // MotorState::NONE ::LEFT ::RIGHT
                d->ms.dir = MotorState::Dirs(count%3);
                // MotorState::RED ::GREEN ::WHITE
                d->ms.state =  MotorState::States((count/3)%3);
            }
        }
    }


SoftwareSerial is supported, although all SoftwareSerial instances print to the same log window. Use `-1` as pin number to recreate for example an USB serial without real pins. If you use valid pin numbers, they will be marked as `SER` in the pin listing

    SoftwareSerial Serial(6, 7);
    // OR
    SoftwareSerial Serial(-1, -1);
    void setup() {
        Serial.begin(9600);
        Serial.println("Setup called");
    }
