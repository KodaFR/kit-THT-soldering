#include "src/include/setup.h"
#include "src/include/config.h"
#include "src/include/statemanager.h"
#include "src/display/display.h"
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
