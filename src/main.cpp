#include <Arduino.h>
#include "RadioModule.h"
#include "status.h"
#include "CANBus.h"

Status status;
CANBusController _canBus(500000);
RadioModule radio(Serial8);

void setup() {
  // put your setup code here, to run once:
    // while(!Serial)
    // while(!Serial8)

    Serial.begin(9600);
    Serial8.begin(9600);

    delay(1000);

    Serial.println("Setting up can bus and radio module.");
    Serial.flush();

    radio.setup();
    radio.checkRadio();

    status.setup();
    status.setLEDBlink(2, STANDARD);

    _canBus.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.flush();
  status.update();
  // radio.update();
  _canBus.update();
  delay(10);
}
