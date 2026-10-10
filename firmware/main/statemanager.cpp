#include <Arduino.h>

#include "src/include/statemanager.h"
#include "src/display/display.h"
#include "src/display/bitmaps/kodalogo.h"
#include "src/lights/lights.h"

AppState current_state = AppState::STATE_BOOT;

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
    
    default:
        break;
    }
}

    
void stateManager(){}