#include "GPS_Sensor.h"
#include "RadioModule.h"
#include "SparkFun_I2C_GPS_Arduino_Library.h"
#include "TinyGPS++.h"

RadioModule radiomodule(Serial8);
I2CGPS myI2CGPS;
TinyGPSPlus gps;
bool started = true;
int gps_lasttime = 0;

GPSSensor::GPSSensor(RadioModule &radio) {
    radiomodule = radio;
}

void GPSSensor::setup() {
    if (!myI2CGPS.begin())
    {
        started = false;
        Serial.println("Module failed to respond. Please check wiring and reset.");
    } else {
        Serial.println("Connected to gps sensor.");
        String configString;
        configString = myI2CGPS.createMTKpacket(220, ",100"); //Configures a packet to set the GPS update rate to 10Hz. (Set 100 for 10Hz, 500 for 2Hz, 1000 for 1Hz, etc.)
        myI2CGPS.sendMTKpacket(configString); //Sends the packet
        Serial.println("Starting GPS sensor");

        configString = myI2CGPS.createMTKpacket(251, ",115200"); //Configures a packet to set the GPS baude rate to 115200
        myI2CGPS.sendMTKpacket(configString); //Sends the packet
        Serial.println("Starting GPS sensor");
    }
    Serial.flush();
}

void GPSSensor::update() {
    if (millis() - gps_lasttime > 500) {
        if (started && myI2CGPS.available()){ //available() returns the number of new bytes available from the GPS module
            gps.encode(myI2CGPS.read()); //Feed the GPS parser
        }

        if (gps.time.isUpdated()){ //Check to see if new GPS info is available
            parseGPSData(); //Runs the function to output GPS data
        }
        gps_lasttime = millis();
    }
}

void GPSSensor::parseGPSData() {
    if (started) {
        if (!gps.time.isValid()) //Checks if the GPS has valid data for time
            Serial.println(F("Time not yet valid"));


        if (gps.location.isValid()) //Checks if the GPS has valid location data
        {
            if (gps.location.age() > 2000 && gps.location.isValid()) //Runs if the location data has not been updated in 2 seconds, indicating a loss of signal
            {
            Serial.println("Location Lost... Reacquiring Signal");
            }    
            // n = n+1;
            // spd = spd+gps.speed.mph(); //Assigns the variable to the speed of the GPS sensor in MPH
        }
        else
        {
        Serial.println(F("Acquiring Location")); //Runs if the GPS has not acquired an initial signal
        }
        
        
        // if (n==10) //Change value of n to update Hz
        // {
        //     la = gps.location.lat();
        //     ln = gps.location.lng();
            
        //     byte gpslocation[4];
        //     float2Bytes(la,&gpslocation[0]);

        //     canMsg.data[0] = gpslocation[0];
        //     canMsg.data[1] = gpslocation[1];
        //     canMsg.data[2] = gpslocation[2];
        //     canMsg.data[3] = gpslocation[3];

        //     float2Bytes(ln,&gpslocation[0]);

        //     canMsg.data[4] = gpslocation[0];
        //     canMsg.data[5] = gpslocation[1];
        //     canMsg.data[6] = gpslocation[2];
        //     canMsg.data[7] = gpslocation[3];

        //     mcp2515.sendMessage(&canMsg);

        //     n=0;
        // }
    }
}
