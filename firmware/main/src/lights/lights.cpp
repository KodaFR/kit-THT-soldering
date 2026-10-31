#include "../include/pins.h"
#include "lights.h"
#include <Arduino.h>

void setColorLeft(int R, int G, int B){
    analogWrite(PINLEFT_R, R);
    analogWrite(PINLEFT_G, G);
    analogWrite(PINLEFT_B, B);
}

void setColorRight(int R, int G, int B){
    analogWrite(PINRIGHT_R, R);
    analogWrite(PINRIGHT_G, G);
    analogWrite(PINRIGHT_B, B);
}