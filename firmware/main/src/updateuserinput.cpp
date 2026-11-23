#include <Arduino.h>

#include "include/pins.h"
#include "include/statemanager.h"

UserInput lastInput = UserInput::NONE;

void updateUserInput() {
  static bool lastRawState[] = {HIGH, HIGH, HIGH};
  static bool stableState[] = {HIGH, HIGH, HIGH};
  static unsigned long lastStateChange[] = {0, 0, 0};
  static unsigned long selectPressedAt = 0;
  static bool selectLongPressSent = false;
  const unsigned long longPressDuration = 800;
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
        if (i == 2) {
          selectPressedAt = now;
          selectLongPressSent = false;
        } else {
          lastInput = buttonInputs[i];
          break;
        }
      } else if (i == 2 && !selectLongPressSent) {
        lastInput = now - selectPressedAt >= longPressDuration
          ? UserInput::LONG_PRESS
          : UserInput::PRESS;
        break;
      }
    }

    if (i == 2 && stableState[i] == LOW && !selectLongPressSent &&
        now - selectPressedAt >= longPressDuration) {
      selectLongPressSent = true;
      lastInput = UserInput::LONG_PRESS;
      break;
    }
  }
}
