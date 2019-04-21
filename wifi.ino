#include "config.h"

// Connection manager
void wifiConnect() {
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    DEBUG.print("Attempting to connect to SSID: ");
    DEBUG.println(CREDENTIALS_SSID);
    while(WiFi.status() != WL_CONNECTED){
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      WiFi.begin(ssid, pass);  
      DEBUG.print(".");
      delay(5000);     
    } 
    DEBUG.println("Connected.\n");
  }
}