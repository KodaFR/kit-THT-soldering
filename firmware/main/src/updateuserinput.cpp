#include <Arduino.h>

#include "include/pins.h"
#include "include/statemanager.h"

UserInput lastInput = UserInput::NONE;

void updateUserInput() {
  static bool lastRawState[] = {HIGH, HIGH, HIGH};
  static bool stableState[] = {HIGH, HIGH, HIGH};
  static unsigned long lastStateChange[] = {0, 0, 0};
  const uint8_t buttonPins[] = {BTN_LEFT, BTN_RIGHT, BTN_SELECT};
  const UserInput buttonInputs[] = {
    UserInput::LEFT,
    UserInput::RIGHT,
    UserInput::PRESS,
  };
  const unsigned long now = millis();
  lastInput = UserInput::NONE;

  for (unsigned int i = 0; i < 3; ++i) {
    const bool rawState = digitalRead(buttonPins[i]);
    if (rawState != lastRawState[i]) {
      lastRawState[i] = rawState;
      lastStateChange[i] = now;
    }

    if (now - lastStateChange[i] >= 30 && rawState != stableState[i]) {
      stableState[i] = rawState;
      if (stableState[i] == LOW) {
        lastInput = buttonInputs[i];
        break;
      }
    }
  }
}
