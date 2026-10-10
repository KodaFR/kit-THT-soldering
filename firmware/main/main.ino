#include "../include/setup.h"
#include "../include/config.h"
#include "../include/statemanager.h"
#include "../lib/display/display.h"
void setup() {
  Serial.begin(BAUDRATE);
  Serial.println("COMMUNICATION EN SERIE INITIALISE");
  setupI2C();
  Serial.println("BUS I2C INITIALISE");
  setupPins();
  Serial.println("GPIO INITIALISES");
  initscreen();
  Serial.println("ECRAN INITIALISEE");
}

void loop() {
  renderUI();

}
