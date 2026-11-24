#include "WiFi.h"
#include "wififeatures.h"

int scanWifi(){
    return WiFi.scanNetworks();
}

int startWifiScan(){
    return WiFi.scanNetworks(true);
}

int pollWifiScan(){
    return WiFi.scanComplete();
}

void deleteWifiScanResults(){
    WiFi.scanDelete();
}