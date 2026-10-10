#include <Arduino.h>

#include "src/include/statemanager.h"
#include "src/display/display.h"
#include "src/display/bitmaps/kodalogo.h"

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