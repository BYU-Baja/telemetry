#include <map>
#include <list>
#include "CANBus.h"
#include "frame.h"

#define TRANSPARENT

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> canbus;
std::map<int, CANMessage> _converter = {};
std::map<CANMessage, std::list<void (*)(CANMessage)>> _handlers;
CAN_message_t msg;
RadioModule rm(Serial8);

CANBusController::CANBusController(int baudrate, RadioModule &radio) {
    this->baudrate = baudrate;
    rm = radio;
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
    // #ifdef TRANSPARENT
    // Serial.println("Received Message.");
    // Serial.flush();
    // if (msg.id == 0x0F5) {
    //   byte lng[4];
    //   byte lat[4];
    //   char formatted[100];
    //   lng[0] = msg.buf[0];
    //   lng[1] = msg.buf[1];
    //   lng[2] = msg.buf[2];
    //   lng[3] = msg.buf[3];
      
    //   Serial.print("Latitude: ");
    //   Serial.print(*(float *)lng);
    //   Serial.print(", ");
      
    //   lat[0] = msg.buf[4];
    //   lat[1] = msg.buf[5];
    //   lat[2] = msg.buf[6];
    //   lat[3] = msg.buf[7];  
          
    //   Serial.print("Longitude: ");
    //   Serial.print(*(float *)lat);

    //   sprintf(formatted, "%f, %f\n", *(float *)lng, *(float *)lat);
    //   rm.sendMessage((uint8_t*)formatted, strlen(formatted));
      
    // }  else {
    //     Serial.flush();
    //     Serial.print(msg.len, HEX);
    //     Serial.print(" ");
    //     Serial.print(msg.len);
    //     Serial.print(" ");

    //     for (int i = 0; i < msg.len; i++)  {  // print the data
    //     Serial.print(msg.buf[i], HEX);
    //     Serial.print(" ");
    //     }
    //     rm.sendMessage(msg.buf, msg.len);
    // }
    // Serial.println();
    // #endif
    rf_frame frame;
    frame.flag = 0;
    frame.id = msg.id;
    frame.data_length = msg.len;
    for (uint16_t i = 0; i < msg.len; i++) {
        frame.data[i] = msg.buf[i];
    }
    rm.sendFrame(frame);
}
