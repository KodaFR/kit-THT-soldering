#include <Arduino.h>
#include <Wire.h>

#include "src/include/pins.h"
#include "src/include/setup.h"

void setupI2C(){
    Wire.begin(SDA_PIN, SCL_PIN);
}

void setupPins(){
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_SELECT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    }