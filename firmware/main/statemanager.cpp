#include <Arduino.h>

#include "../include/statemanager.h"
#include "../lib/display/display.h"
#include "../lib/display/bitmaps/kodalogo.h"

AppState current_state = AppState::STATE_BOOT;

void renderUI(){
    switch (current_state)
    {
    case AppState::STATE_BOOT:
        drawBitmapPage(logo_bitmap);
        break;
    
    default:
        break;
    }
}

    
void stateManager(){}