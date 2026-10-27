#include <Arduino.h>

#include "src/include/statemanager.h"
#include "src/display/display.h"
#include "src/display/bitmaps/bitmaps.h"
#include "src/buzzer/buzzer.h"
#include "src/lights/lights.h"
#include "src/include/Pet/pet.h"

AppState current_state = AppState::STATE_BOOT;
EggState current_egg_state = EggState::NEW;

namespace {
struct MenuPage {
    const unsigned char* bitmap;
    const char* label;
    int acuteAccentIndex;
};

const MenuPage menuPages[] = {
    {health_menu_bitmap, "SANTE", 4},
    {skills_menu_bitmap, "COMPETENCES", 4},
    {stats_menu_bitmap, "STATISTIQUES", -1},
    {money_menu_bitmap, "ARGENT", -1},
    {pet_menu_bitmap, "ANIMAL", -1},
    {food_menu_bitmap, "NOURRIR", -1},
    {learn_menu_bitmap, "APPRENDRE", -1},
    {play_menu_bitmap, "JOUER", -1},
    {experiment_menu_bitmap, "EXPERIMENTER", 3},
};
const unsigned int menuPageCount = sizeof(menuPages) / sizeof(menuPages[0]);
unsigned int menuPageIndex = 0;
}

void renderUI(){
    switch (current_state)
    {
    case AppState::STATE_BOOT:
        drawBitmapPage(logo_bitmap);
        if (!isLedOn){
        setColorLeft(255, 255 ,255);
        setColorRight(255, 255 ,255);
        isLedOn = true;
        }
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
        drawBitmapPageWithLabel(
            menuPages[menuPageIndex].bitmap,
            menuPages[menuPageIndex].label,
            menuPages[menuPageIndex].acuteAccentIndex
        );
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
                    Pet myPet(0.5, 0.5, 0.5);
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
            }
            break;
        default:
            break;
    }
}