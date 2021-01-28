#include <map>
#include <list>
#include "CANBus.h"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> canbus;
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
    while (canbus.read(msg)) {
        _handleMessage(msg);
    }
}

void CANBusController::_handleMessage(CAN_message_t &msg) {

}