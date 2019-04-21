#include "config.h"

// Update the fields with the sensor values
void updateThingSpeakFields() {
  ThingSpeak.setStatus(status);
  ThingSpeak.setField(1, insideTemperature);
  ThingSpeak.setField(2, insideLuminosity);
  ThingSpeak.setField(3, fanSpeed);
}

// Send the sensors data to internet
void sendThingSpeakFields() {
  int statusCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  // Verifies the HTTP status code
  if (statusCode == 200) { // successful
    DEBUG.println("Channel update successful.");
  }
  else { // Error
    DEBUG.println("Problem updating channel. HTTP error code " + String(statusCode));
  }
}