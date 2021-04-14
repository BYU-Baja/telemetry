#include "GPS_Sensor.h"
#include "RadioModule.h"
#include "SparkFun_I2C_GPS_Arduino_Library.h"
#include "TinyGPS++.h"
#include "frame.h"

RadioModule radiomodule(Serial8);
TinyGPSPlus gps;
bool started = true;
int gps_lasttime = 0;
int n = 0;

GPSSensor::GPSSensor(RadioModule &radio) {
    radiomodule = radio;
}

void GPSSensor::setup() {
    Serial7.begin(9600);
    // if (!myI2CGPS.begin())
    // {
    //     started = false;
    //     Serial.println("Module failed to respond. Please check wiring and reset.");
    // } else {
    //     Serial.println("Connected to gps sensor.");
    //     String configString;
    //     configString = myI2CGPS.createMTKpacket(220, ",100"); //Configures a packet to set the GPS update rate to 10Hz. (Set 100 for 10Hz, 500 for 2Hz, 1000 for 1Hz, etc.)
    //     myI2CGPS.sendMTKpacket(configString); //Sends the packet
    //     Serial.println("Starting GPS sensor");

    //     configString = myI2CGPS.createMTKpacket(251, ",115200"); //Configures a packet to set the GPS baude rate to 115200
    //     myI2CGPS.sendMTKpacket(configString); //Sends the packet
    //     Serial.println("Starting GPS sensor");
    // }
    // Serial.flush();
}

void GPSSensor::update() {
    // if (millis() - gps_lasttime > 500) {
        // while (Serial7.available()){ //available() returns the number of new bytes available from the GPS module
        //     gps.encode(Serial7.read()); //Feed the GPS parser
        // }

        if (gps.time.isUpdated()){ //Check to see if new GPS info is available
            parseGPSData(); //Runs the function to output GPS data
        }
    //     gps_lasttime = millis();
    // }

    while(Serial7.available()) {
        Serial.write(Serial7.read());
    }
}

void GPSSensor::parseGPSData() {
    // if (started) {
        if (!gps.time.isValid()) //Checks if the GPS has valid data for time
            Serial.println(F("Time not yet valid"));


        if (gps.location.isValid()) //Checks if the GPS has valid location data
        {
            if (gps.location.age() > 2000 && gps.location.isValid()) //Runs if the location data has not been updated in 2 seconds, indicating a loss of signal
            {
                Serial.println("Location Lost... Reacquiring Signal");
            }    
            n = n+1;
            // spd = spd+gps.speed.mph(); //Assigns the variable to the speed of the GPS sensor in MPH
        }
        else
        {
            Serial.print("LAT="); Serial.print(gps.location.lat(), 6);
            Serial.print("LNG="); Serial.println(gps.location.lng(), 6);
            Serial.println(F("Acquiring Location")); //Runs if the GPS has not acquired an initial signal
        }
        
        
        if (n==10) //Change value of n to update Hz
        {
            float la = gps.location.lat();
            float ln = gps.location.lng();
            
            float_uint8_converter conv;
            conv.value = la;

            rf_frame frame;
            frame.id = 0xc0;
            frame.data_length = 8;

            frame.data[0] = conv.data[0];
            frame.data[1] = conv.data[1];
            frame.data[2] = conv.data[2];
            frame.data[3] = conv.data[3];

            conv.value = ln;

            frame.data[4] = conv.data[0];
            frame.data[5] = conv.data[1];
            frame.data[6] = conv.data[2];
            frame.data[7] = conv.data[3];

            radiomodule.sendFrame(frame);

            n=0;
        }
    // }
}
