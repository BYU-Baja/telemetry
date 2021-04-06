#include <map>
#include <list>
#include "CANBus.h"
#include "frame.h"
#include "Wire.h"

//write to SD card libraries
#include <SPI.h>
#include <SD.h>

#define TIRE_DIAMETER_INCHES 20
#define RPM_MPH(a) (a * TIRE_DIAMETER_INCHES * 3.1415926 / 62260)

#define TRANSPARENT
#define DISPLAY_ADDR 0x16

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> canbus;
std::map<int, CANMessage> _converter = {};
std::map<CANMessage, std::list<void (*)(CANMessage)>> _handlers;
CAN_message_t msg;
RadioModule rm(Serial8);

//for battery life test
int startMillis = 0;
int currentMillis = 0;
int prevMillis = 0;
int chipSelect = BUILTIN_SDCARD;
File myFile;

void doBatteryTest(){

    currentMillis = millis();
    if(currentMillis - prevMillis > 1000 * 60 * 10){ //this writes every 10 minutes
        prevMillis = currentMillis;
        Serial.println("One testing period has elapsed");
        //write a file
        String cMillis = String(currentMillis);
        //String myFileName = cMillis + ".txt";
        
        /*
        myFile = SD.open("battery_test.txt", FILE_WRITE);
        if(myFile){
            Serial.println("File is open");
            myFile.println("Battery is still alive at: " + currentMillis);
            myFile.close();
        }else {
            Serial.println("File failed to open");
            myFile.close();
        }
        */

        
        myFile = SD.open("test.txt", FILE_WRITE);

        // if the file opened okay, write to it:
        if (myFile) {
            Serial.print("Writing to test.txt...");
            myFile.println("Battery is still alive at: " + String(currentMillis));
            // close the file:
            myFile.close();
            Serial.println("done.");
        } else {
            // if the file didn't open, print an error:
            Serial.println("error opening test.txt");
        }

        // re-open the file for reading:
        
        myFile = SD.open("test.txt");
        if (myFile) {
            Serial.println("test.txt: opened");

            // read from the file until there's nothing else in it:
            while (myFile.available()) {
            Serial.write(myFile.read());
            }
            // close the file:
            myFile.close();
        } else {
            // if the file didn't open, print an error:
            Serial.println("error opening test.txt");
        }
        
        
    }
}

CANBusController::CANBusController(int baudrate, RadioModule &radio) {
    this->baudrate = baudrate;
    rm = radio;
}

void CANBusController::setup() {
    canbus.begin();
    canbus.setBaudRate(this->baudrate);
    Wire1.begin();
    startMillis = millis();
    prevMillis = startMillis;
    if(!SD.begin(BUILTIN_SDCARD)) Serial.println("SD offline");
    SD.remove("test.txt");
//    File newFile = SD.open("setetTest.txt", FILE_WRITE);
//    if(newFile) Serial.println("test file has been opened");
//    newFile.close();

}

void CANBusController::update() {
    if (canbus.read(msg)) {
        _handleMessage(msg);
    }
    doBatteryTest(); //comment this out if not doing a bettery life test
    Serial.flush();
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
    if (msg.id == 0xa4) {
        char data[50];
        float_uint8_converter conv;
        conv.data[0] = msg.buf[0];
        conv.data[1] = msg.buf[1];
        conv.data[2] = msg.buf[2];
        conv.data[3] = msg.buf[3];

        float speed = RPM_MPH(conv.value);
        //float speed = conv.value; 
        //speed  = 5.0;
        sprintf(data, "speed: %f\n", speed);
        Serial.printf(data, strlen(data));

        // Transmit to I2C
        conv.value = speed;
        Wire1.beginTransmission(DISPLAY_ADDR);
        Wire1.write(conv.data[0]);
        Wire1.write(conv.data[1]);
        Wire1.write(conv.data[2]);
        Wire1.write(conv.data[3]);
        Wire1.endTransmission();
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
    // if (msg.id == 0xa4) {
    //     float_uint8_converter conv;
    //     conv.data[0] = msg.buf[0];
    //     conv.data[1] = msg.buf[1];
    //     conv.data[2] = msg.buf[2];
    //     conv.data[3] = msg.buf[3];
    //     sprintf(data, "left rpm: %f\n", conv.value);
    //     Serial.printf(data, strlen(data));
    //     rm.sendMessage((uint8_t *)data, strlen(data));
    // }
}
