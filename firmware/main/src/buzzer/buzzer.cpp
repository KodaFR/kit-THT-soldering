#include <Arduino.h>
#include "../include/pins.h"
#include "buzzer.h"

void playDeathMelody() {
  const int melody[] = {
    392, 349, 330, 294, 262, 220, 196, 174,
    220, 196, 174, 146
  };
  const int durations[] = {
    220, 220, 220, 260, 260, 260, 420, 420,
    220, 220, 220, 540
  };
  const int notesCount = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < notesCount; i++) {
    tone(BUZZER, melody[i], durations[i]);
    delay(durations[i] * 1.3);
  }

  noTone(BUZZER);
}

void playbootMelody() {
  for (int i = 0; i < bootNotesCount; i++) {
    tone(BUZZER, bootMelody[i], bootDurations[i]);
    delay(bootDurations[i] * 1.3);
  }

  noTone(BUZZER);
}

void playHatchMelody() {
  const int melody[] = {
    784, 988, 1175, 988, 784, 659,
    523, 392, 523, 659, 784, 1568,
  };
  const int durations[] = {
    100, 100, 180, 100, 100, 180,
    100, 220, 100, 100, 180, 400,
  };
  const int notesCount = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < notesCount; i++) {
    tone(BUZZER, melody[i], durations[i]);
    delay(durations[i] * 1.3);
  }

  noTone(BUZZER);
}