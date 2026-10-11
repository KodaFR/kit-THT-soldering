#include <Arduino.h>
#include "../include/pins.h"
#include "buzzer.h"

void playbootMelody() {
  for (int i = 0; i < bootNotesCount; i++) {
    tone(BUZZER, bootMelody[i], bootDurations[i]);
    delay(bootDurations[i] * 1.3);
  }

  noTone(BUZZER);
}