#include <Arduino.h>
#include "RadioModule.h"
#include "status.h"
#include "CANBus.h"
#include "GPS_Sensor.h"

#define BAUD_RATE 500000


//Radio module communicates on teensy's serial8 port
RadioModule radio(Serial8);

//instantiate CANbus controller with baudrate and instance of radio module
CANBusController _canBus(BAUD_RATE, radio);

Status status;
long _lasttime = 0;
bool _isRadioConnected = false;

void setup() {
  // put your setup code here, to run once:
    while(!Serial)
    while(!Serial8)

    Serial.begin(9600);
    Serial8.begin(9600);

    delay(1000);

    Serial.println("Setting up can bus and radio module.");
    Serial.flush();
    
    status.setup();

    radio.setup();
    _isRadioConnected = radio.checkRadio();

    //if the radio check was succesful, blink with HIGH speed
    //see status.h for blink speed definitions
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

   //update state machine for status LEDs
  status.update();

  //transmit the radio message every second
  if (millis() - _lasttime > 1000) {
    radio.sendMessage((uint8_t *)"A", 1);
     _lasttime = 0;
  }

  //listen for a message from the radio 
  radio.update();

  //check for updates on the CAN bus
  _canBus.update();

  //wait 10 ms
  delay(10);
}
