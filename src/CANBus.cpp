#include <map>
#include <list>
#include "CANBus.h"
#include "frame.h"
#include "Wire.h"

#define TIRE_DIAMETER_INCHES 23
#define RPM_MPH(a) (a * TIRE_DIAMETER_INCHES * 3.1415926 * 60 / 63360)

#define TRANSPARENT
#define DISPLAY_ADDR 0x16   //I2C address
#define FUEL_ID 0x03
#define SPEED_ID 0x04       //CAN bus id for speed
#define RPM_ID 0x02         //CAN bus id for speed
#define THROTTLE_ID 0x03    //CAN bus id for throttle position
#define TEST_DISPLAY_DATA

//setup CANbus 
//CAN2 = which can port to use on teensy
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> canbus;

std::map<int, CANMessage> _converter = {};
std::map<CANMessage, std::list<void (*)(CANMessage)>> _handlers;
CAN_message_t msg;
RadioModule rm(Serial8);
float speed = 0;
float engine_rpm = 0;
float fuel = 1;
unsigned long lasttime = 0;

CANBusController::CANBusController(int baudrate, RadioModule &radio) {
    this->baudrate = baudrate;
    rm = radio;
}

void CANBusController::setup() {
    canbus.begin();
    canbus.setBaudRate(this->baudrate);
    Wire1.begin();
}

void CANBusController::update() {
    
    if (canbus.read(msg)) {
        _handleMessage(msg);
    }

    if (millis() - lasttime > 500) {
        #ifdef TEST_DISPLAY_DATA
        speed += 0.5;
        if (speed >= 99)
            speed = 0;
        fuel -= 0.01;
        if (fuel <= 0)
            fuel = 1;
        engine_rpm += 0.05;
        if (engine_rpm > 9)
            engine_rpm = 0;
        #endif
        float_uint8_converter conv;
        conv.value = speed;
        Wire1.beginTransmission(DISPLAY_ADDR);
        Wire1.write(SPEED_ID);
        Wire1.write(conv.data[0]);
        Wire1.write(conv.data[1]);
        Wire1.write(conv.data[2]);
        Wire1.write(conv.data[3]);
        Wire1.endTransmission();

        conv.value = engine_rpm;
        Wire1.beginTransmission(DISPLAY_ADDR);
        Wire1.write(RPM_ID);
        Wire1.write(conv.data[0]);
        Wire1.write(conv.data[1]);
        Wire1.write(conv.data[2]);
        Wire1.write(conv.data[3]);
        Wire1.endTransmission();

        Wire1.beginTransmission(DISPLAY_ADDR);
        Wire1.write(FUEL_ID);
        // conv.value = fuel;
        uint8_t scaled_fuel = (uint8_t)(fuel * 100);
        Wire1.write(scaled_fuel);
        // Wire1.write(conv.data[0]);
        // Wire1.write(conv.data[1]);
        // Wire1.write(conv.data[2]);
        // Wire1.write(conv.data[3]);
        Wire1.endTransmission();
        lasttime = millis();
    }
}

void CANBusController::_handleMessage(CAN_message_t &msg) {
    // Serial.println("Received Message.");
    Serial.flush();
    Serial.flush();
    Serial.print(msg.id, HEX);
    Serial.print(" ");
    Serial.print(msg.len);
    Serial.print(" ");

    for (int i = 0; i < msg.len; i++)  {  // print the data
        Serial.print(msg.buf[i], HEX);
        Serial.print(" ");
    }
    Serial.println();


    rf_frame frame;
    frame.flag = 0;
    frame.id = msg.id;
    frame.data_length = msg.len;
    for (uint16_t i = 0; i < msg.len; i++) {
        frame.data[i] = msg.buf[i];
    }
    rm.sendFrame(frame);

    // Calculate the speed of the vehicle
    if (msg.id == 0xa3) {
        char data[50];
        float_uint8_converter conv;
        conv.data[0] = msg.buf[0];
        conv.data[1] = msg.buf[1];
        conv.data[2] = msg.buf[2];
        conv.data[3] = msg.buf[3];

        speed = RPM_MPH(conv.value);
        sprintf(data, "speed: %f\n", speed);
        Serial.printf(data, strlen(data));
        // Serial.print(speed);

        // Transmit to I2C
        // uint32_uint8_converter converter;
    }


    // if (msg.id == 0xa1) {
    //     float_uint8_converter conv;
    //     conv.data[0] = msg.buf[0];
    //     conv.data[1] = msg.buf[1];
    //     conv.data[2] = msg.buf[2];
    //     conv.data[3] = msg.buf[3];
    //     sprintf(data, "throttle: %f\n", conv.value);
    //     Serial.printf(data, strlen(data));
    //     rm.sendMessage((uint8_t *)data, strlen(data));
    // }
    // if (msg.id == 0xa2) {
    //     float_uint8_converter conv;
    //     conv.data[0] = msg.buf[0];
    //     conv.data[1] = msg.buf[1];
    //     conv.data[2] = msg.buf[2];
    //     conv.data[3] = msg.buf[3];
    //     sprintf(data, "engine rpm: %f\n", conv.value);
    //     Serial.printf(data, strlen(data));
    //     rm.sendMessage((uint8_t *)data, strlen(data));
    // }
    // if (msg.id == 0xa3) {
    //     float_uint8_converter conv;
    //     conv.data[0] = msg.buf[0];
    //     conv.data[1] = msg.buf[1];
    //     conv.data[2] = msg.buf[2];
    //     conv.data[3] = msg.buf[3];
    //     sprintf(data, "right rpm: %f\n", conv.value);
    //     Serial.printf(data, strlen(data));
    //     rm.sendMessage((uint8_t *)data, strlen(data));
    // }
    if (msg.id == 0xa4) {
        float_uint8_converter conv;
        conv.data[0] = msg.buf[0];
        conv.data[1] = msg.buf[1];
        conv.data[2] = msg.buf[2];
        conv.data[3] = msg.buf[3];
        // sprintf(data, "left rpm: %f\n", conv.value);
        // Serial.printf(data, strlen(data));
        // rm.sendMessage((uint8_t *)data, strlen(data));
        engine_rpm = conv.value;
    }
}
