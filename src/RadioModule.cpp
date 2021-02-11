#include "RadioModule.h"
#include <Arduino.h>

void RadioModule::readParam(char *param, SXPro mod) {
    Serial.print(param);
    Serial.print(": ");
    Serial.println(mod.readParam(param));
}

void RadioModule::setup() {
    Serial8.begin(9600);
    SXPro module = SXPro((int8_t) 9, false);
    module.begin(Serial8);

    Serial.println("Attempting to connect to radio module...");

    if (module.enterCommandMode()) {
        Serial.println("Entered command mode.");
        module.reset();
        delay(2000);
        readParam("AF", module);
        readParam("ID", module);
        readParam("BR", module);
        readParam("PL", module);
        readParam("NI", module);
        module.setParam("NI", "Baja Vehicle");
        module.setParam("ID", "2015");
        readParam("NI", module);
        readParam("ID", module);
        if (module.exitCommandMode())
            Serial.println("Exited command mode.");
        Serial.println("Done.");
    } else {
        Serial.println("Failed to connect");
    }
}

void RadioModule::update(){

}