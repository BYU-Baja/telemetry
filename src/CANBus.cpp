#include <map>
#include <list>
#include "CANBus.h"

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> canbus;
std::map<int, CANMessage> _converter = {};
std::map<CANMessage, std::list<void (*)(CANMessage)>> _handlers;
CAN_message_t msg;

CANBusController::CANBusController(int baudrate) {
    this->baudrate = baudrate;
}

void CANBusController::setup() {
    canbus.begin();
    canbus.setBaudRate(this->baudrate);
}

void CANBusController::update() {
    if (canbus.read(msg)) {
        _handleMessage(msg);
    }
}

void CANBusController::_handleMessage(CAN_message_t &msg) {
    Serial.println("Received Message.");
    Serial.flush();
    Serial.print(msg.len, HEX);
    Serial.print(" ");
    Serial.print(msg.len);
    Serial.print(" ");

    for (int i = 0; i < msg.len; i++)  {  // print the data
      Serial.print(msg.buf[i], HEX);
      Serial.print(" ");
    }

    Serial.println();
}
