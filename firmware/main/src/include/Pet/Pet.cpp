#include <cmath>
#include <cstdlib>
#include <Arduino.h>
#include "../../wififeatures\wififeatures.h"
//#include eeprom

#include "Pet.h"

using namespace std;

namespace {
const float baseTalentCoefficient = 0.5f;

float randomizeTalent(float talent) {
    return talent * random(950, 1051) / 1000.0f;
}
}

Pet::Pet(float talent_learn, float talent_experiment, float talent_play)
    : learn_coef(randomizeTalent(talent_learn)),
      experiment_coef(randomizeTalent(talent_experiment)),
      play_coef(randomizeTalent(talent_play)),
      learnProgress(0.0f),
      experimentProgress(0.0f),
      playProgress(0.0f),
      lastUpdateMillis(millis())
{
    this-> health = 5;
    this ->level = 0;
    this ->age = 0;
    this->hunger = 5;
    
    this->culture = 0;
    this->happyness = 0;
    this->creativity = 0;
    this->money = 0;

    Serial.println("L'objet Pet a été créé");
}

Pet::~Pet() {
    Serial.println("L'objet Pet a été détruit");
}

void Pet::learn(int points){
    learnProgress += points * (learn_coef / baseTalentCoefficient);
    while (learnProgress >= 5.0f) {
        ++culture;
        learnProgress -= 5.0f;
    }
    this->money += 2;
}
int Pet::eat(int points){
    if (points <= 0 || this->money < 15 || this->hunger >= 5) {
        return 0;
    }
    const int previousHunger = this->hunger;
    this->hunger += points;
    if (this->hunger > 5) {
        this->hunger = 5;
    }
    this->money -= 15;
    return this->hunger - previousHunger;
}
void Pet::play(int points){
    playProgress += points * (play_coef / baseTalentCoefficient);
    while (playProgress >= 50.0f) {
        ++happyness;
        playProgress -= 50.0f;
    }
}
int Pet::experiment(int points){
    const int previousCreativity = creativity;
    experimentProgress += points * (experiment_coef / baseTalentCoefficient);
    while (experimentProgress >= 1.0f) {
        ++creativity;
        experimentProgress -= 1.0f;
    }
    this->money += points/4;
    return creativity - previousCreativity;
}

void Pet::update(){
    const unsigned long fiveMinutes = 5UL * 60UL * 1000UL;
    const unsigned long now = millis();
    while (now - lastUpdateMillis >= fiveMinutes) {
        lastUpdateMillis += fiveMinutes;
        ++age;
        Serial.println("Joyeux anniversaire ! Votre compagnon a maintenant :");
        Serial.print(age);
        Serial.println(" ans !");

        if (hunger == 0) {
            if (health > 0) {
                --health;
            }
        } else if (hunger == 5 && health < 5) {
            ++health;
        }
        if (hunger > 0) {
            --hunger;
        }
    }
}