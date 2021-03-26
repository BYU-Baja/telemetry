#ifndef __GPS_SENSOR_H__
#define __GPS_SENSOR_H__

#include <Arduino.h>
#include "RadioModule.h"

class GPSSensor: public Controller {
    public:
        GPSSensor(RadioModule &radio);
        void setup();
        void update();
};

#endif
