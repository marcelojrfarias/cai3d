#include "config.h"

// Update the fields with the sensor values
void updateThingSpeakFields() {
  ThingSpeak.setStatus(status);
  ThingSpeak.setField(1, insideTemperature);
  ThingSpeak.setField(2, insideLuminosity);
  ThingSpeak.setField(3, fanSpeed);
  
  // Send to internet
  sendThingSpeakFields();
}

// Send the sensors data to internet
void sendThingSpeakFields() {
	DEBUG.print(F("[THINGSPEAK] Sending data to the ThingSpeak... "));
  // Manage WiFi connection
  if (wifiConnect()) {
    int statusCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    // Verifies the HTTP status code
    if (statusCode == 200) { // successful
      DEBUG.println(F("OK!"));
    }
    else { // Error
      DEBUG.print(F("Problem updating channel. HTTP error code ")); DEBUG.println(String(statusCode));
    }
  }
}