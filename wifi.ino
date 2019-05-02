#include "config.h"

// Connection manager
bool wifiConnect() {
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED) {
    DEBUG.print(F("Attempting to connect to SSID: "));
    DEBUG.println(ssid);
    int attempts = 0;
    bool connected = WiFi.status() == WL_CONNECTED;

    while(!connected && attempts++ < WIFI_ATTEMPTS) {
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      connected = WiFi.status() == WL_CONNECTED;
      WiFi.begin(ssid, pass);  
      DEBUG.print(". ");
      delay(WIFI_ATTEMPTS_INTERVAL);
    }

    if (connected) {
      DEBUG.println(F("WiFi Connected."));
      return true; 
    } 
    DEBUG.println(F("WiFi connection failed!"));
    return false;
  }
  // DEBUG.println(F("WiFi already connected."));
  return true; 
}