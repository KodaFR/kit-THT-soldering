#include "../include/setup.h"
#include "../include/config.h"

void setup() {
  Serial.begin(BAUDRATE);
  Serial.println("COMMUNICATION EN SERIE INITIALISE");
  setupI2C();
  Serial.println("BUS I2C INITIALISE");
  setupPins();
  Serial.println("GPIO INITIALISES");
}

void loop() {
  // put your main code here, to run repeatedly:

}
