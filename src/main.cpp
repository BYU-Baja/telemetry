#include <Arduino.h>
#include "RadioModule.h"
#include "status.h"

// 2, 3, 4, 5, 6
#define LED 3
Status status;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    // Serial.println("Hello.");
    // RadioModule radio;
    // radio.setup();

    status.setup();
    status.setLEDBlink(0, STANDARD);
}

void loop() {
  // put your main code here, to run repeatedly:
  status.update();
}
