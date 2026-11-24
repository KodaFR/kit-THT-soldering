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

void drawTextPage(const char* title, const char* const lines[], unsigned int lineCount){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.print(title);
  for (unsigned int i = 0; i < lineCount; ++i) {
    display.setCursor(0, 12 + i * 10);
    display.print(lines[i]);
  }
  display.display();
}

void drawPopupPage(const char* title, const char* message){
  display.clearDisplay();
  display.drawRect(1, 1, SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2, SH110X_WHITE);
  display.fillRect(3, 3, SCREEN_WIDTH - 6, 13, SH110X_WHITE);
  display.setTextSize(1);
  display.setTextColor(SH110X_BLACK);
  const int titleX = (SCREEN_WIDTH - static_cast<int>(strlen(title)) * 6) / 2;
  display.setCursor(titleX, 6);
  display.print(title);
  display.setTextColor(SH110X_WHITE);
  display.setTextWrap(false);
  const size_t maxCharacters = (SCREEN_WIDTH - 8) / 6;
  char lines[5][21] = {};
  unsigned int lineCount = 0;
  const char* cursor = message;
  while (*cursor != '\0' && lineCount < 5) {
    char line[21] = {};
    size_t lineLength = 0;
    while (*cursor == ' ') {
      ++cursor;
    }
    if (*cursor == '\n') {
      ++cursor;
      ++lineCount;
      continue;
    }

    while (*cursor != '\0' && *cursor != '\n') {
      while (*cursor == ' ') {
        ++cursor;
      }
      const char* wordStart = cursor;
      while (*cursor != '\0' && *cursor != ' ' && *cursor != '\n') {
        ++cursor;
      }
      const size_t wordLength = static_cast<size_t>(cursor - wordStart);
      if (wordLength == 0) {
        break;
      }
      if (lineLength > 0 && lineLength + 1 + wordLength > maxCharacters) {
        cursor = wordStart;
        break;
      }

      if (lineLength > 0) {
        line[lineLength++] = ' ';
      }
      const size_t availableCharacters = maxCharacters - lineLength;
      const size_t copiedCharacters = wordLength < availableCharacters
        ? wordLength
        : availableCharacters;
      memcpy(line + lineLength, wordStart, copiedCharacters);
      lineLength += copiedCharacters;
      if (copiedCharacters < wordLength) {
        cursor = wordStart + copiedCharacters;
        break;
      }
    }

    line[lineLength] = '\0';
    memcpy(lines[lineCount], line, lineLength + 1);
    ++lineCount;
    if (*cursor == '\n') {
      ++cursor;
    }
  }
  const int bodyTop = 18;
  const int bodyHeight = SCREEN_HEIGHT - 2 - bodyTop;
  const int textY = bodyTop + (bodyHeight - static_cast<int>(lineCount) * 8) / 2;
  for (unsigned int line = 0; line < lineCount; ++line) {
    const size_t lineLength = strlen(lines[line]);
    const int textX = (SCREEN_WIDTH - static_cast<int>(lineLength) * 6) / 2;
    display.setCursor(textX, textY + static_cast<int>(line) * 8);
    display.print(lines[line]);
  }
  display.display();
}

void drawRadioScanFrame(unsigned long frame){
  display.clearDisplay();
  display.drawRect(1, 1, SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2, SH110X_WHITE);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  const char* scanTitle = "RECHERCHE WI-FI";
  const int scanTitleX = (SCREEN_WIDTH - static_cast<int>(strlen(scanTitle)) * 6) / 2;
  display.setCursor(scanTitleX, 7);
  display.print(scanTitle);

  display.drawLine(8, 30, 8, 44, SH110X_WHITE);
  display.drawLine(3, 44, 13, 44, SH110X_WHITE);
  for (unsigned int wave = 0; wave < 3; ++wave) {
    const int x = 14 + static_cast<int>((frame * 2 + wave * 36) % 108);
    const int radius = 2 + static_cast<int>((frame / 2 + wave) % 5);
    display.drawCircle(x, 37, radius, SH110X_WHITE);
  }

  const char* scanMessage = "Recherche Wi-Fi";
  const int scanMessageX = (SCREEN_WIDTH - static_cast<int>(strlen(scanMessage)) * 6) / 2;
  display.setCursor(scanMessageX, 53);
  display.print(scanMessage);
  display.display();
}

void drawEatActivityFrame(const unsigned char petBitmap[1024], unsigned int bites){
  display.clearDisplay();
  display.drawBitmap(0, 0, petBitmap, SCREEN_WIDTH, SCREEN_HEIGHT, SH110X_WHITE);

  const int appleX = 105;
  const int appleY = 33;
  display.fillCircle(appleX, appleY, 8, SH110X_WHITE);
  display.fillCircle(appleX + 7, appleY, 8, SH110X_WHITE);
  display.fillCircle(appleX + 5, appleY - 5, 4, SH110X_BLACK);
  display.drawLine(appleX + 4, appleY - 8, appleX + 6, appleY - 14, SH110X_WHITE);
  display.fillTriangle(
    appleX + 6, appleY - 12,
    appleX + 13, appleY - 15,
    appleX + 11, appleY - 9,
    SH110X_WHITE
  );
  if (bites >= 1) {
    display.fillCircle(appleX + 13, appleY - 4, 4, SH110X_BLACK);
  }
  if (bites >= 2) {
    display.fillCircle(appleX + 13, appleY + 5, 4, SH110X_BLACK);
  }

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  char biteMessage[20];
  snprintf(biteMessage, sizeof(biteMessage), "MORSURES %u/3", bites);
  const int biteMessageX = (SCREEN_WIDTH - static_cast<int>(strlen(biteMessage)) * 6) / 2;
  display.setCursor(biteMessageX, 54);
  display.print(biteMessage);
  display.display();
}