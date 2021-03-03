#include "RadioModule.h"
#include "frame.h"
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
        module.setParam("BR", "0");
        readParam("NI", module);
        readParam("ID", module);
        readParam("BR", module);
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

void RadioModule::sendMessage(uint8_t* msg, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        this->radioModulePort->write(msg[i]);
    }
}

void RadioModule::sendFrame(rf_frame frame) {
    uint8_t buf[FRAME_MAX_LEN];
    uint16_t buf_len;
    frame_to_buff(frame, buf, &buf_len);
    // for (uint8_t i = 0; i < buf_len; i++)
    // {
    // Serial.print((char)buf[i]);
    // Serial.print(buf[i], HEX);
    // Serial.print(' ');
    // Serial.write(&buf[i], 1);
    // }
    // Serial.print('\n');
    radioModulePort->write(buf, buf_len);
    radioModulePort->flush();
}

void RadioModule::update() {
    // Check the incoming data from the radio module.
}