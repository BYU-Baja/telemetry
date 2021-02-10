#include "Arduino.h"
#include "sx_pro.h"

SXPro::SXPro(int8_t rst, bool lineEnding) {
    pinMode(rst, OUTPUT);

    _rst = rst;
    mySerial = 0;
    newline = lineEnding;
}

void SXPro::begin(Stream &port) { mySerial = &port; }

void SXPro::resetDevice(uint8_t pulse, uint16_t duration) {
    if (pulse == LOW) {
        digitalWrite(_rst, HIGH);
        delay(10);
        digitalWrite(_rst, LOW);
        delay(duration);
        digitalWrite(_rst, HIGH);
    } else if (pulse == HIGH) {
        digitalWrite(_rst, LOW);
        delay(10);
        digitalWrite(_rst, HIGH);
        delay(duration);
        digitalWrite(_rst, LOW);
    }
}

bool SXPro::sendBlindCommand(char *command) {
    Serial.print(F("\t---> "));
    Serial.println(command);
    if (newline)
        mySerial->println(command);
    else
        mySerial->print(command);
}

bool SXPro::sendCommand(char *command, uint16_t timeout) {
    while (mySerial->available())
        mySerial->read(); // Clear input buffer

    // Serial.print(F("\t---> "));
    // Serial.println(command);
    if (newline)
        mySerial->println(command);
    else
        mySerial->print(command);

    bool gotReply = false;
    timer = millis();

    while (!gotReply && millis() - timer < timeout) {
        if (mySerial->available())
            gotReply = true;
    }

    if (gotReply)
        return true;
    else
        return false;
}

bool SXPro::sendCommand(char *command, char *reply, uint16_t timeout) {
    while (mySerial->available())
        mySerial->read(); // Clear input buffer

    // Serial.print(F("\t---> "));
    // Serial.println(command);
    if (newline)
        mySerial->println(command);
    else
        mySerial->print(command);

    uint8_t idx = 0;
    bool replyMatch = false;
    timer = millis();

    while (!replyMatch && millis() - timer < timeout) {
        if (mySerial->available()) {
            replybuffer[idx] = mySerial->read();
            idx++;
            // if (strstr(replybuffer, reply) != NULL) replyMatch = true; // Only checks if desired
            // reply is inside replybuffer
            if (strcmp(replybuffer, reply) == 0)
                replyMatch =
                    true; // This means the reply must start with the desired reply to be successful
        }
    }

    // Serial.print(F("\t<--- "));
    // Serial.println(replybuffer);

    if (replyMatch)
        return true;
    else
        return false;
}

bool SXPro::reset() { return sendCommand("ATRE\r", "OK"); }

bool SXPro::write() { return true; }

bool SXPro::applyChanges() { return sendCommand("ATAC\r", "OK"); }

bool SXPro::setParam(char *param, char *value) {
    uint8_t size_param = strlen(param);
    uint8_t size_value = strlen(value);
    uint8_t size = size_param + size_value;

    char command[size + 6];
    command[0] = 'A';
    command[1] = 'T';
    for (uint8_t i = 0; i < size_param; i++)
        command[2 + i] = param[i];
    for (uint8_t i = 0; i < size_value; i++)
        command[2 + i + size_param] = value[i];

    command[size + 2] = '\r';
    command[size + 3] = '\0';
    return sendCommand(command, "OK");
}

// Sends an AT command
String SXPro::readParam(char *param) {
    uint8_t size = strlen(param);
    char command[size + 3];
    command[0] = 'A';
    command[1] = 'T';
    for (uint8_t i = 0; i < size; i++)
        command[2 + i] = param[i];
    command[size + 2] = '\r';
    command[size + 3] = '\0';

    if (sendCommand(command)) {
        return mySerial->readStringUntil('\r');
    } else {
        return "ERROR";
    }
}

bool SXPro::enterCommandMode() {
    delay(1000);
    return sendCommand("+++", "OK", 2000);
}

bool SXPro::exitCommandMode() { return sendCommand("ATCN\r"); }
