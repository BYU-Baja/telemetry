#include "GPS_Sensor.h"
#include "RadioModule.h"

RadioModule radiomodule(Serial8);

GPSSensor::GPSSensor(RadioModule &radio) {
    radiomodule = radio;
}

void GPSSensor::setup() {

}

void GPSSensor::update() {

}


