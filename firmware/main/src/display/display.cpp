#include "display.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "../include/config.h"
#include <stdio.h>
#include "../include/pins.h"

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void initscreen(){
    display.begin(screen_i2c_address, true);
}


void drawBitmapPage(const unsigned char bitmap[1024]){
  display.clearDisplay();
  display.drawBitmap(
    0, 0,
    bitmap,
    128,
    64,
    SH110X_WHITE
  );
  display.display();
}