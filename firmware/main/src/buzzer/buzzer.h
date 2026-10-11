// Tamagotchi — booting Melody
// Passive buzzer avec tone()

const int bootMelody[] = {
  523, 659, 784, 1047,
  784, 880, 1047, 1319,
  1047, 880, 1047, 1568
};

const int bootDurations[] = {
  120, 120, 120, 280,
  120, 120, 200, 320,
  120, 120, 180, 450
};

const int bootNotesCount = sizeof(bootMelody) / sizeof(bootMelody[0]);

void playbootMelody();