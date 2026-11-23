#include <Arduino.h>

#include "src/include/statemanager.h"
#include "src/display/display.h"
#include "src/display/bitmaps/bitmaps.h"
#include "src/buzzer/buzzer.h"
#include "src/lights/lights.h"
#include "src/include/Pet/pet.h"
#include <stdio.h>

AppState current_state = AppState::STATE_BOOT;
EggState current_egg_state = EggState::NEW;
Pet *myPet = nullptr;

namespace {
struct MenuPage {
    const unsigned char* bitmap;
    const char* label;
    int acuteAccentIndex;
    AppState state;
};

const MenuPage menuPages[] = {
    {health_menu_bitmap, "SANTE", 4, AppState::STATE_STATS_HEALTH},
    {skills_menu_bitmap, "COMPETENCES", 4, AppState::STATE_STATS_SKILLS},
    {stats_menu_bitmap, "STATISTIQUES", -1, AppState::STATE_MENU_STATS_PET},
    {money_menu_bitmap, "ARGENT", -1, AppState::STATE_STATS_MONEY},
    {pet_menu_bitmap, "", -1, AppState::STATE_MENU_PET},
    {food_menu_bitmap, "NOURRIR", -1, AppState::STATE_ACTIVITY_EAT},
    {learn_menu_bitmap, "APPRENDRE", -1, AppState::STATE_ACTIVITY_LEARN},
    {play_menu_bitmap, "JOUER", -1, AppState::STATE_ACTIVITY_PLAY},
    {experiment_menu_bitmap, "EXPERIMENTER", 3, AppState::STATE_ACTIVITY_EXPERIMENT},
};
const unsigned int menuPageCount = sizeof(menuPages) / sizeof(menuPages[0]);
unsigned int menuPageIndex = 0;

void renderMenuPage(const MenuPage& page) {
    drawBitmapPageWithLabel(page.bitmap, page.label, page.acuteAccentIndex);
}

void renderPageForState(AppState state) {
    for (unsigned int i = 0; i < menuPageCount; ++i) {
        const MenuPage& page = menuPages[i];
        if (page.state != state) {
            continue;
        }

        const char* labels[5] = {};
        int values[5] = {};
        unsigned int lineCount = 0;
        if (myPet != nullptr) {
            switch (state) {
            case AppState::STATE_STATS_HEALTH:
                labels[0] = "SANTE";
                values[0] = myPet->health;
                labels[1] = "FAIM";
                values[1] = myPet->hunger;
                lineCount = 2;
                break;
            case AppState::STATE_STATS_SKILLS:
                labels[0] = "CULTURE";
                values[0] = myPet->culture;
                labels[1] = "CREATIVITE";
                values[1] = myPet->creativity;
                labels[2] = "BONHEUR";
                values[2] = myPet->happyness;
                lineCount = 3;
                break;
            case AppState::STATE_MENU_STATS_PET:
                labels[0] = "AGE (ANS)";
                values[0] = myPet->age;
                labels[1] = "NIVEAU";
                values[1] = myPet->level;
                lineCount = 2;
                break;
            case AppState::STATE_STATS_MONEY:
                labels[0] = "SOLDE";
                values[0] = myPet->money;
                lineCount = 1;
                break;
            case AppState::STATE_STATS_PET:
                labels[0] = "AGE (ANS)";
                values[0] = myPet->age;
                labels[1] = "NIVEAU";
                values[1] = myPet->level;
                labels[2] = "BONHEUR";
                values[2] = myPet->happyness;
                lineCount = 3;
                break;
            default:
                break;
            }
        }

        char statLines[5][24];
        const char* lines[5];
        for (unsigned int line = 0; line < lineCount; ++line) {
            snprintf(statLines[line], sizeof(statLines[line]), "%s: %d", labels[line], values[line]);
            lines[line] = statLines[line];
        }

        const char* title = page.label[0] == '\0' ? "ANIMAL" : page.label;
        drawTextPage(title, lines, lineCount);
        return;
    }
}
}



void renderUI(){
    if (current_egg_state == EggState::HATCHED) {
        setColorLeft(173, 216, 230);
        setColorRight(173, 216, 230);
    } else {
        setColorLeft(255, 255, 255);
        setColorRight(255, 255, 255);
    }

    switch (current_state)
    {
    case AppState::STATE_BOOT:
        drawBitmapPage(logo_bitmap);
        break;
    case AppState::STATE_MENU_EGG:
        switch (current_egg_state)
        {
        case EggState::NEW:
            drawBitmapPage(egg1_bitmap);
            break;
        case EggState::CRACK1:
            drawBitmapPage(egg2_bitmap);
            break;
        case EggState::CRACK2:
            drawBitmapPage(egg3_bitmap);
            break;
        case EggState::CRACK3:
            drawBitmapPage(egg4_bitmap);
            break;
        case EggState::HATCHED:
            current_state = AppState::STATE_MENU_PET;
            break;
        }
        break;
    case AppState::STATE_MENU_PET:
        renderMenuPage(menuPages[menuPageIndex]);
        break;
    case AppState::STATE_STATS_HEALTH:
    case AppState::STATE_STATS_SKILLS:
    case AppState::STATE_STATS_PET:
    case AppState::STATE_STATS_MONEY:
    case AppState::STATE_MENU_STATS_PET:
    case AppState::STATE_ACTIVITY_EAT:
    case AppState::STATE_ACTIVITY_PLAY:
    case AppState::STATE_ACTIVITY_LEARN:
    case AppState::STATE_ACTIVITY_EXPERIMENT:
        renderPageForState(current_state);
        break;
    case AppState::STATE_MENU_DEAD:
        drawBitmapPage(egg4_bitmap);
        break;
    
    default:
        break;
    }
}

    
void stateManager(){
    switch (current_state)
    {
        case AppState::STATE_BOOT:
            if (lastInput == UserInput::PRESS){
                current_state = AppState::STATE_MENU_EGG;
            }
            break;
        case AppState::STATE_MENU_EGG:
            if (lastInput == UserInput::PRESS) {
                switch (current_egg_state)
                {
                case EggState::NEW:
                    current_egg_state = EggState::CRACK1;
                    break;
                case EggState::CRACK1:
                    current_egg_state = EggState::CRACK2;
                    break;
                case EggState::CRACK2:
                    current_egg_state = EggState::CRACK3;
                    break;
                case EggState::CRACK3:
                    {
                    current_egg_state = EggState::HATCHED;
                    current_state = AppState::STATE_MENU_PET;
                    myPet = new Pet(0.5, 0.5, 0.5);
                    playHatchMelody();
                    }
                    break;
                case EggState::HATCHED:
                    current_state = AppState::STATE_MENU_PET;
                    break;
                }
            }
            break;
        case AppState::STATE_MENU_PET:
            if (lastInput == UserInput::LEFT) {
                menuPageIndex = (menuPageIndex + menuPageCount - 1) % menuPageCount;
            } else if (lastInput == UserInput::RIGHT) {
                menuPageIndex = (menuPageIndex + 1) % menuPageCount;
            } else if (lastInput == UserInput::PRESS) {
                current_state = menuPages[menuPageIndex].state;
            }
            break;
        case AppState::STATE_STATS_HEALTH:
        case AppState::STATE_STATS_SKILLS:
        case AppState::STATE_STATS_PET:
        case AppState::STATE_STATS_MONEY:
        case AppState::STATE_MENU_STATS_PET:
        case AppState::STATE_ACTIVITY_EAT:
        case AppState::STATE_ACTIVITY_PLAY:
        case AppState::STATE_ACTIVITY_LEARN:
        case AppState::STATE_ACTIVITY_EXPERIMENT:
            if (lastInput == UserInput::LONG_PRESS) {
                current_state = AppState::STATE_MENU_PET;
            }
            break;
        case AppState::STATE_MENU_DEAD:
            playDeathMelody();
            break;
        default:
            break;
    }
    if (current_egg_state == EggState::HATCHED && myPet != nullptr){
        myPet->update();
    }
}