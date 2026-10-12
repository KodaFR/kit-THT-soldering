#include "WiFi.h"
#include "wififeatures.h"

int scanWifi(){
    return WiFi.scanNetworks();
}