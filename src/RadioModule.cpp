#include "RadioModule.h"
#include <Arduino.h>

SXPro module = SXPro((int8_t) 9, false);

RadioModule::RadioModule(Stream &port) {
    this->radioModulePort = &port;
}

void RadioModule::readParam(char *param, SXPro mod) {
    Serial.print(param);
    Serial.print(": ");
    Serial.println(mod.readParam(param));
}

bool RadioModule::checkRadio() {
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
        return true;
    }
    
    Serial.println("Failed to connect");
    return false;
}

void RadioModule::setup() {
    module.begin(*radioModulePort);
}

void RadioModule::update() {
    // Check the incoming data from the radio module.
}