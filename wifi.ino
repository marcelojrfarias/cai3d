#include "config.h"

// Connection manager
void wifiConnect() {
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    LOG.print("Attempting to connect to SSID: ");
    LOG.println(CREDENTIALS_SSID);
    while(WiFi.status() != WL_CONNECTED){
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      WiFi.begin(ssid, pass);  
      LOG.print(".");
      delay(5000);     
    } 
    LOG.println("Connected.\n");
  }
}