#include <Arduino.h>

#include "src/include/statemanager.h"
#include "src/display/display.h"
#include "src/display/bitmaps/bitmaps.h"
#include "src/buzzer/buzzer.h"
#include "src/lights/lights.h"
#include "src/include/Pet/pet.h"
#include "src/wififeatures/wififeatures.h"
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
const unsigned char* playBitmap = play_both_bitmap;

const char* techFacts[] = {
    "Huit bits forment un octet, soit un caractere ASCII simple.",
    "Une LED emet de la lumiere quand le courant traverse son semi-conducteur.",
    "Un transistor agit comme un minuscule interrupteur. Les puces en contiennent des milliards.",
    "Un condensateur stocke l'energie electrique et peut lisser une tension.",
    "Le Wi-Fi transmet des donnees par ondes radio, presque a la vitesse de la lumiere.",
    "La RAM est une memoire temporaire : elle perd ses donnees sans alimentation.",
    "Un microcontroleur combine processeur, memoire et broches d'entree-sortie.",
    "Le binaire utilise seulement 0 et 1. Chaque chiffre binaire est un bit."
};
const unsigned int techFactCount = sizeof(techFacts) / sizeof(techFacts[0]);
int currentTechFactIndex = -1;
bool learnFactPending = false;
bool learnFactVisible = false;
bool learnFactDismissed = false;
unsigned long learnFactDueAt = 0;

unsigned int eatBites = 0;
bool eatResultVisible = false;
const char* eatResultTitle = "REPAS";
char eatResultMessage[48] = {};

bool experimentScanActive = false;
bool experimentResultVisible = false;
bool experimentExitAfterScan = false;
unsigned long experimentScanStartedAt = 0;
const char* experimentResultTitle = "RESULTATS WI-FI";
char experimentResultMessage[64] = {};

const unsigned char* petMenuBitmap() {
    static const unsigned char* currentBitmap = need_play_bitmap;
    static unsigned long nextBitmapChange = 0;
    const unsigned long now = millis();
    if (static_cast<long>(now - nextBitmapChange) >= 0) {
        const bool foodIsLow = myPet != nullptr && myPet->hunger <= 2;
        const long choice = random(foodIsLow ? 3 : 2);
        if (foodIsLow && choice == 2) {
            currentBitmap = need_food_bitmap;
        } else if (choice == 0) {
            currentBitmap = need_play_bitmap;
        } else {
            currentBitmap = need_interrogation_bitmap;
        }
        nextBitmapChange = now + 3000;
    }
    return currentBitmap;
}

void renderMenuPage(const MenuPage& page) {
    if (page.state == AppState::STATE_MENU_PET) {
        drawBitmapPage(petMenuBitmap());
    } else {
        drawBitmapPageWithLabel(page.bitmap, page.label, page.acuteAccentIndex);
    }
}

void renderPageForState(AppState state) {
    switch (state) {
    case AppState::STATE_ACTIVITY_PLAY:
        drawBitmapPage(playBitmap);
        return;
    default:
        break;
    }

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

void showNextTechFact() {
    long nextIndex = random(static_cast<long>(techFactCount));
    if (nextIndex == currentTechFactIndex) {
        nextIndex = (nextIndex + 1) % techFactCount;
    }
    currentTechFactIndex = static_cast<int>(nextIndex);
    learnFactVisible = true;
    learnFactPending = false;
}

void beginLearnActivity() {
    currentTechFactIndex = -1;
    learnFactPending = true;
    learnFactVisible = false;
    learnFactDismissed = false;
    learnFactDueAt = millis() + 1000;
}

void resetLearnActivity() {
    learnFactPending = false;
    learnFactVisible = false;
    learnFactDismissed = false;
}

void updateLearnActivity() {
    if (learnFactPending && static_cast<long>(millis() - learnFactDueAt) >= 0) {
        showNextTechFact();
    }
}

void showEatMessage(const char* title, const char* message) {
    eatResultTitle = title;
    snprintf(eatResultMessage, sizeof(eatResultMessage), "%s", message);
    eatResultVisible = true;
}

void beginEatActivity() {
    eatBites = 0;
    eatResultVisible = false;
    if (myPet == nullptr) {
        showEatMessage("SANS COMPAGNON", "Aucun compagnon a nourrir.");
    } else if (myPet->hunger >= 5) {
        showEatMessage("RASASIE", "La faim est deja au maximum.");
    } else if (myPet->money < 15) {
        showEatMessage("PAS DE NOURRITURE", "Il faut 15 pieces.");
    }
}

void handleEatActivity(UserInput input) {
    if (input == UserInput::LONG_PRESS) {
        current_state = AppState::STATE_MENU_PET;
        eatResultVisible = false;
        return;
    }
    if (input != UserInput::PRESS) {
        return;
    }
    if (eatResultVisible) {
        beginEatActivity();
        return;
    }
    if (myPet == nullptr) {
        showEatMessage("SANS COMPAGNON", "Aucun compagnon a nourrir.");
        return;
    }
    if (myPet->money < 15) {
        showEatMessage("PAS DE NOURRITURE", "Il faut 15 pieces.");
        return;
    }
    if (myPet->hunger >= 5) {
        showEatMessage("RASASIE", "La faim est deja au maximum.");
        return;
    }

    ++eatBites;
    if (eatBites == 3) {
        const int hungerGained = myPet->eat(3);
        if (hungerGained > 0) {
            char mealMessage[32];
            snprintf(mealMessage, sizeof(mealMessage), "Faim +%d", hungerGained);
            showEatMessage("REPAS", mealMessage);
        } else {
            showEatMessage("REPAS IMPOSSIBLE", "Le repas n'a pas abouti.");
        }
    }
}

void handlePlayInput(UserInput input) {
    if (input == UserInput::LEFT || input == UserInput::RIGHT || input == UserInput::BOTH) {
        if (myPet == nullptr) {
            Serial.println("Impossible de jouer sans compagnon");
            return;
        }
        myPet->play(1);
    }

    switch (input) {
    case UserInput::LEFT:
        playBitmap = play_left_bitmap;
        setColorLeft(255, 0, 0);
        setColorRight(0, 0, 0);
        playLeftMelody();
        break;
    case UserInput::RIGHT:
        playBitmap = play_right_bitmap;
        setColorLeft(0, 0, 0);
        setColorRight(0, 0, 255);
        playRightMelody();
        break;
    case UserInput::BOTH:
        playBitmap = play_both_bitmap;
        setColorLeft(0, 255, 0);
        setColorRight(0, 255, 0);
        playBothMelody();
        break;
    default:
        break;
    }
}

void showExperimentError(const char* message) {
    experimentResultTitle = "ERREUR WI-FI";
    snprintf(experimentResultMessage, sizeof(experimentResultMessage), "%s", message);
    experimentResultVisible = true;
}

void finishExperimentScan(int scannedNetworks) {
    experimentScanActive = false;
    deleteWifiScanResults();
    if (myPet == nullptr) {
        showExperimentError("Compagnon indisponible.");
    } else if (scannedNetworks < 0) {
        showExperimentError("L'analyse Wi-Fi a echoue.");
    } else {
        const int creativityGained = myPet->experiment(scannedNetworks);
        experimentResultTitle = "RESULTATS WI-FI";
        snprintf(
            experimentResultMessage,
            sizeof(experimentResultMessage),
            "Reseaux trouves : %d\nCreativite +%d",
            scannedNetworks,
            creativityGained
        );
        experimentResultVisible = true;
    }

    if (experimentExitAfterScan) {
        current_state = AppState::STATE_MENU_PET;
        experimentExitAfterScan = false;
        experimentResultVisible = false;
    }
}

void startExperimentScan() {
    if (myPet == nullptr) {
        showExperimentError("Compagnon indisponible.");
        return;
    }

    deleteWifiScanResults();
    experimentResultVisible = false;
    const int status = startWifiScan();
    if (status == -1) {
        experimentScanActive = true;
        experimentScanStartedAt = millis();
    } else if (status < 0) {
        showExperimentError("Impossible de lancer l'analyse Wi-Fi.");
    } else {
        finishExperimentScan(status);
    }
}

void updateExperimentActivity(UserInput input) {
    if (experimentScanActive) {
        if (input == UserInput::LONG_PRESS) {
            experimentExitAfterScan = true;
        }
        const int result = pollWifiScan();
        if (result != -1) {
            finishExperimentScan(result);
        }
        return;
    }

    if (input == UserInput::LONG_PRESS) {
        current_state = AppState::STATE_MENU_PET;
        experimentResultVisible = false;
    } else if (input == UserInput::PRESS) {
        if (experimentResultVisible) {
            experimentResultVisible = false;
        } else {
            startExperimentScan();
        }
    }
}

void restartWithNewEgg() {
    delete myPet;
    myPet = nullptr;
    current_egg_state = EggState::NEW;
    current_state = AppState::STATE_MENU_EGG;
    menuPageIndex = 0;
    playBitmap = play_both_bitmap;
    resetLearnActivity();
    eatBites = 0;
    eatResultVisible = false;
    experimentScanActive = false;
    experimentResultVisible = false;
    experimentExitAfterScan = false;
}
}



void renderUI(){
    if (current_state != AppState::STATE_ACTIVITY_PLAY) {
        if (current_egg_state == EggState::HATCHED) {
            setColorLeft(173, 216, 230);
            setColorRight(173, 216, 230);
        } else {
            setColorLeft(255, 255, 255);
            setColorRight(255, 255, 255);
        }
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
    case AppState::STATE_ACTIVITY_PLAY:
        renderPageForState(current_state);
        break;
    case AppState::STATE_ACTIVITY_EAT:
        if (eatResultVisible) {
            drawPopupPage(eatResultTitle, eatResultMessage);
        } else {
            drawEatActivityFrame(pet_menu_bitmap, eatBites);
        }
        break;
    case AppState::STATE_ACTIVITY_LEARN:
        if (learnFactVisible) {
            drawPopupPage("LE SAVIEZ-VOUS ?", techFacts[currentTechFactIndex]);
        } else {
            drawBitmapPage(activity_learn_bitmap);
        }
        break;
    case AppState::STATE_ACTIVITY_EXPERIMENT:
        if (experimentScanActive) {
            drawRadioScanFrame((millis() - experimentScanStartedAt) / 100);
        } else if (experimentResultVisible) {
            drawPopupPage(experimentResultTitle, experimentResultMessage);
        } else {
            drawBitmapPage(activity_experiment_bitmap);
        }
        break;
    case AppState::STATE_MENU_DEAD:
        drawBitmapPage(dead_pet_bitmap);
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
            if (lastInput == UserInput::ALL &&
                menuPages[menuPageIndex].state == AppState::STATE_MENU_PET &&
                myPet != nullptr) {
                myPet->health = 0;
                current_state = AppState::STATE_MENU_DEAD;
            } else if (lastInput == UserInput::LEFT) {
                menuPageIndex = (menuPageIndex + menuPageCount - 1) % menuPageCount;
            } else if (lastInput == UserInput::RIGHT) {
                menuPageIndex = (menuPageIndex + 1) % menuPageCount;
            } else if (lastInput == UserInput::PRESS) {
                if (menuPages[menuPageIndex].state != AppState::STATE_MENU_PET) {
                    current_state = menuPages[menuPageIndex].state;
                    if (current_state == AppState::STATE_ACTIVITY_PLAY) {
                        playBitmap = play_both_bitmap;
                        setColorLeft(0, 0, 0);
                        setColorRight(0, 0, 0);
                    } else if (current_state == AppState::STATE_ACTIVITY_EAT) {
                        beginEatActivity();
                    } else if (current_state == AppState::STATE_ACTIVITY_LEARN) {
                        beginLearnActivity();
                    } else if (current_state == AppState::STATE_ACTIVITY_EXPERIMENT) {
                        experimentScanActive = false;
                        experimentResultVisible = false;
                        experimentExitAfterScan = false;
                        startExperimentScan();
                    }
                }
            }
            break;
        case AppState::STATE_STATS_HEALTH:
        case AppState::STATE_STATS_SKILLS:
        case AppState::STATE_STATS_PET:
        case AppState::STATE_STATS_MONEY:
        case AppState::STATE_MENU_STATS_PET:
            if (lastInput == UserInput::LONG_PRESS) {
                current_state = AppState::STATE_MENU_PET;
            }
            break;
        case AppState::STATE_ACTIVITY_EAT:
            handleEatActivity(lastInput);
            break;
        case AppState::STATE_ACTIVITY_LEARN:
            updateLearnActivity();
            if (lastInput == UserInput::LONG_PRESS) {
                current_state = AppState::STATE_MENU_PET;
                resetLearnActivity();
            } else if (lastInput == UserInput::PRESS) {
                if (myPet != nullptr) {
                    myPet->learn(1);
                } else {
                    Serial.println("Impossible d'apprendre sans compagnon");
                }
                if (learnFactVisible) {
                    learnFactVisible = false;
                    learnFactDismissed = true;
                } else if (learnFactDismissed) {
                    showNextTechFact();
                }
            }
            break;
        case AppState::STATE_ACTIVITY_EXPERIMENT:
            updateExperimentActivity(lastInput);
            break;
        case AppState::STATE_ACTIVITY_PLAY:
            if (lastInput == UserInput::LONG_PRESS) {
                current_state = AppState::STATE_MENU_PET;
            } else {
                handlePlayInput(lastInput);
            }
            break;
        case AppState::STATE_MENU_DEAD:
            playDeathMelody();
            restartWithNewEgg();
            break;
        default:
            break;
    }
    if (current_egg_state == EggState::HATCHED && myPet != nullptr &&
        current_state != AppState::STATE_MENU_DEAD) {
        myPet->update();
        if (myPet->health <= 0) {
            current_state = AppState::STATE_MENU_DEAD;
        }
    }
}