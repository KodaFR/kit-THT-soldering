#include <cmath>
#include <cstdlib>
#include <Arduino.h>
#include "../../wififeatures\wififeatures.h"
//#include eeprom

#include "Pet.h"

using namespace std;

Pet::Pet(float talent_learn, float talent_experiment, float talent_play) : learn_coef(talent_learn), experiment_coef(talent_experiment), play_coef(talent_play)
{
    this-> health = 5;
    this ->level = 0;
    this ->age = 0;
    this->hunger = 5;
    
    this->culture = 0;
    this->happyness = 0;
    this->creativity = 0;
    this->money = 0;

    this->lastAgeMillis = 5*60*1000; //Le compagnon prend 1 an après 5 minutes et non dès sa naissance

    Serial.println("L'objet Pet a été créé");
}

Pet::~Pet() {
    Serial.println("L'objet Pet a été détruit");
}

void Pet::learn(int points){
    this->culture +=std::ceil(points * learn_coef);
    this->money += 2;
}
bool Pet::eat(int points){
    if (this->money >= 15 ){
        this->hunger += (points);}
}
void Pet::play(int points){
    this->happyness +=std::ceil(points * play_coef);
}
void Pet::experiment(int points){
    this->creativity +=std::ceil(points * experiment_coef);
    this->money += points/4;
}

void Pet::update(){
    if(millis() - this->lastAgeMillis >= 5*60*1000){// on incrémente de 1 an toutes les 5 minutes
        this->lastAgeMillis = millis();
        this->age ++;
        Serial.println("Joyeux annviersaire! Votre compagnon a maintenant: ");
        Serial.print(this->age);
        Serial.println(" ans!");
    }
}