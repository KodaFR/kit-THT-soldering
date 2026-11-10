#include "display.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "../include/config.h"
#include <stdio.h>
#include <string.h>
#include "../include/pins.h"

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void initscreen(){
    display.begin(screen_i2c_address, true);
}


void drawBitmapPage(const unsigned char bitmap[1024]){
  drawBitmapPageWithLabel(bitmap, nullptr);
}

void drawBitmapPageWithLabel(const unsigned char bitmap[1024], const char* label, int acuteAccentIndex, const char* subtitle){
  display.clearDisplay();
  display.drawBitmap(
    0, 0,
    bitmap,
    128,
    64,
    SH110X_WHITE
  );
  if (label != nullptr && label[0] != '\0') {
    const size_t labelLength = strlen(label);
    const int textX = (SCREEN_WIDTH - static_cast<int>(labelLength) * 6) / 2;
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(textX, 56);
    display.print(label);
    if (acuteAccentIndex >= 0 && static_cast<size_t>(acuteAccentIndex) < labelLength) {
      const int accentX = textX + acuteAccentIndex * 6;
      display.drawLine(accentX + 2, 55, accentX + 4, 53, SH110X_WHITE);
    }
  }
  if (subtitle != nullptr && subtitle[0] != '\0') {
    const size_t subtitleLength = strlen(subtitle);
    const int textX = (SCREEN_WIDTH - static_cast<int>(subtitleLength) * 6) / 2;
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(textX, 44);
    display.print(subtitle);
  }
  display.display();
}