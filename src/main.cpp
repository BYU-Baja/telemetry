#include <Arduino.h>
#include "RadioModule.h"
#include "status.h"
#include "CANBus.h"

Status status;
RadioModule radio(Serial8);
CANBusController _canBus(500000, radio);

long _lasttime = 0;
bool _isRadioConnected = false;

void setup() {
  // put your setup code here, to run once:
    // while(!Serial)
    // while(!Serial8)

    Serial.begin(9600);
    Serial8.begin(9600);

    delay(1000);

    Serial.println("Setting up can bus and radio module.");
    Serial.flush();
    
    status.setup();
    // status.setStatusLED(0, HIGH);
    // status.setStatusLED(1, HIGH);
    // status.setStatusLED(2, HIGH);
    // status.setStatusLED(3, HIGH);
    // status.setStatusLED(4, HIGH);
    // status.setLEDBlink(2, DOUBLE_BLINK);

    //radio.setup();
    //_isRadioConnected = radio.checkRadio();


    if (_isRadioConnected) {
      status.setStatusLED(0, HIGH);
    } else {
      status.setLEDBlink(0, DOUBLE_BLINK);
    }

    _canBus.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.flush();
  status.update();
  // if (millis() - _lasttime > 1000) {
  //   radio.sendMessage((uint8_t *)"A", 1);
  //   _lasttime = 0;
  // }
  //radio.update();
  _canBus.update();
  delay(10);
}
