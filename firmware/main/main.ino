#include "src/include/setup.h"
#include "src/include/config.h"
#include "src/include/statemanager.h"
#include "src/include/pins.h"
#include "src/display/display.h"
#include "src/buzzer/buzzer.h"
#include "WiFi.h"

UserInput lastInput = UserInput::NONE;

void updateUserInput() {
  static bool lastRawState = HIGH;
  static bool stableState = HIGH;
  static unsigned long lastStateChange = 0;

  const bool rawState = digitalRead(BTN_SELECT);
  const unsigned long now = millis();
  lastInput = UserInput::NONE;

  if (rawState != lastRawState) {
    lastRawState = rawState;
    lastStateChange = now;
  }

  if (now - lastStateChange >= 30 && rawState != stableState) {
    stableState = rawState;
    if (stableState == LOW) {
      lastInput = UserInput::PRESS;
    }
  }
}

void setup() {
  Serial.begin(BAUDRATE);
  Serial.println("COMMUNICATION EN SERIE INITIALISE");
  setupI2C();
  Serial.println("BUS I2C INITIALISE");
  setupPins();
  Serial.println("GPIO INITIALISES");
  initscreen();
  renderUI();
  Serial.println("ECRAN INITIALISE");
  playbootMelody();
  WiFi.mode(WIFI_STA);
}

void loop() {
  updateUserInput();
  stateManager();
  renderUI();
}
