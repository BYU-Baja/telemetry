#include "status.h"
#include <Arduino.h>

uint8_t __STATUS_LED_ARRAY[5] = {2, 3, 4, 5, 6};
uint8_t __STATUS_LED_NUM = 5;
BLINK_RATE __STATUS_LED_BR[5] = {NONE_, NONE_, NONE_, NONE_, NONE_};
uint32_t lastTime;
uint8_t state = 0;

void Status::setup() {
    for (uint8_t i = 0; i < __STATUS_LED_NUM; i++)
        pinMode(__STATUS_LED_ARRAY[i], OUTPUT);
    lastTime = millis();
}

void Status::update() {
    switch(state) {
        case 0:
            for (uint8_t i = 0; i < __STATUS_LED_NUM; i++)
                if (__STATUS_LED_BR[i] != NONE_)
                    setStatusLED(i, HIGH);
            state = 1;
            lastTime = millis();
            break;
        case 1:
            if (millis() - lastTime > 500) {
                state = 2;
                lastTime = millis();
                for (uint8_t i = 0; i < __STATUS_LED_NUM; i++)
                    if (__STATUS_LED_BR[i] == FAST)
                        setStatusLED(i, LOW);
            }
            break;
        case 2:
            if (millis() - lastTime > 500) {
                state = 3;
                lastTime = millis();
                for (uint8_t i = 0; i < __STATUS_LED_NUM; i++) {
                    if (__STATUS_LED_BR[i] == FAST)
                        setStatusLED(i, HIGH);
                    if (__STATUS_LED_BR[i] == STANDARD)
                        setStatusLED(i, LOW);
                }
            }
            break;
        case 3:
            if (millis() - lastTime > 500) {
                state = 1;
                lastTime = millis();
                for (uint8_t i = 0; i < __STATUS_LED_NUM; i++)
                    if (__STATUS_LED_BR[i] == FAST)
                        setStatusLED(i, LOW);
            }
            break;
        case 4:
            if (millis() - lastTime > 500) {
                state = 1;
                lastTime = millis();
                for (uint8_t i = 0; i < __STATUS_LED_NUM; i++)
                        setStatusLED(i, LOW);
            }
            break;
    }
}

void Status::setLEDBlink(uint8_t led, BLINK_RATE blinkRate) {
    if (led <= __STATUS_LED_NUM)
        __STATUS_LED_BR[led] = blinkRate;
}

void Status::setStatusLED(uint8_t led, uint8_t signal) {
    if (led <= __STATUS_LED_NUM) {
        __STATUS_LED_BR[led] = NONE_;
        digitalWrite(__STATUS_LED_ARRAY[led], signal);
    }
}