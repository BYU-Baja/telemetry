
#ifndef __STATUS_CONTROLLER_H__
#define __STATUS_CONTROLLER_H__

#include "Controller.h"
#include <stdint.h>

enum BLINK_RATE {
    NONE_,
    STANDARD,
    FAST, 
    SLOW,
    DIT_DOT,
    DOUBLE_BLINK
};

class Status: public Controller {
    public:
        void setup();
        void update();
        void setStatusLED(uint8_t led, uint8_t signal);
        void setLEDBlink(uint8_t led, BLINK_RATE blinkRate);
};

#endif
