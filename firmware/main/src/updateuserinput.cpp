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
  bool leftOrRightPressed[2] = {false, false};
  bool selectReleased = false;
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
          leftOrRightPressed[i] = true;
        }
      } else if (i == 2 && !selectLongPressSent) {
        selectReleased = true;
      }
    }

    if (i == 2 && stableState[i] == LOW && !selectLongPressSent &&
        now - selectPressedAt >= longPressDuration) {
      selectLongPressSent = true;
      lastInput = UserInput::LONG_PRESS;
    }
  }

  const bool bothButtonsDown =
    (stableState[0] == LOW && stableState[1] == LOW) ||
    (digitalRead(buttonPins[0]) == LOW && digitalRead(buttonPins[1]) == LOW);
  if ((leftOrRightPressed[0] || leftOrRightPressed[1]) && bothButtonsDown) {
    lastInput = UserInput::BOTH;
  } else if (leftOrRightPressed[0]) {
    lastInput = UserInput::LEFT;
  } else if (leftOrRightPressed[1]) {
    lastInput = UserInput::RIGHT;
  } else if (selectReleased) {
    lastInput = now - selectPressedAt >= longPressDuration
      ? UserInput::LONG_PRESS
      : UserInput::PRESS;
  }
}
