#include <cmath>
#include <cstdlib>
#include <Arduino.h>
//#include eeprom

#include "Pet.h"

using namespace std;

Pet::Pet(int talent_learn, int talent_experiment, int talent_play) : learn_coef(talent_learn), experiment_coef(talent_experiment), play_coef(talent_play)
{
    this-> health = 5;
    this ->level = 0;
    this ->age = 0;
    this->hunger = 5;
    
    this->culture = 0;
    this->happyness = 0;
    this->creativity = 0;
}

Pet::~Pet() {
    Serial.println("L'objet Pet a été détruit");
}

void Pet::learn(int points){
    this->culture +=std::ceil(points * learn_coef);
}
void Pet::eat(int points){
    this->hunger += (points);
}
void Pet::play(int points){
    this->happyness +=std::ceil(points * play_coef);
}
void Pet::experiment(int points){
    this->creativity +=std::ceil(points * experiment_coef);
}