#include "RadioModule.h"
#include <Arduino.h>
#include <sx_pro.h>

void readParam(char *param, SXPro mod) {
    Serial.print(param);
    Serial.print(": ");
    Serial.println(mod.readParam(param));
}

void setup() {
    Serial.begin(9600);
    Serial5.begin(9600);
    SXPro module = SXPro((int8_t) 9, false);
    module.begin(Serial5);

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
    }

    Serial.println("Done.");
}

void update(){

}