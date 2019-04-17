// Config file
#include "config.h"

// Credentials file
#include "mycredentials.h"

// ThingSpeak library and parameters
#include "ThingSpeak.h"
unsigned long myChannelNumber = CREDENTIALS_CH_ID;
const char * myWriteAPIKey = CREDENTIALS_WRITE_APIKEY;

// WiFi library and parameters
#include <WiFi.h>
char ssid[] = CREDENTIALS_SSID;   // your network SSID (name) 
char pass[] = CREDENTIALS_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// PCF8591 8-bit A/D and D/A converter
#include "PCF8591.h"
PCF8591 pcf8591(PCF8591_I2C_ADDRESS);

// Envoriment data
int insideTemperature = 0;
float insideLuminosity = 0;

void setup() {
  // Start the logging serial port
  LOG.begin(LOG_SPEED);

  // Print the project name and version  
  LOG.print(PROJECT_NAME); LOG.print(" "); LOG.println(PROJECT_VERSION);

  // Start the AD/DA converter
  pcf8591.begin();
  setFanSpeed(100);

  // Initialize the connection
  WiFi.mode(WIFI_STA);   

  // Initialize ThingSpeak
  ThingSpeak.begin(client);  
}

void loop() {
  // Connect to WiFi network
  wifiConnect();

  // Update the sensors values
  insideTemperature = getInsideTemperature();
	LOG.print(insideTemperature); LOG.print("\t");
  insideLuminosity = getInsideLuminosity();
	LOG.print(insideLuminosity); LOG.print("\t");
  delay(500);

  // Update the ThingSpeak channels
  updateThingSpeakChannel();

  // write to the ThingSpeak channel - send to internet
  int statusCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // Verifies the HTTP status code
  if(statusCode == 200){ // successful
    LOG.println("Channel update successful.");
  }
  else{ // Error
    LOG.println("Problem updating channel. HTTP error code " + String(statusCode));
  }

  // Wait 20 seconds for the next update
  delay(20000);
}

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

// Update the fields with the sensor values
void updateThingSpeakChannel() {
  ThingSpeak.setStatus("CAI3D");
  ThingSpeak.setField(1, insideTemperature);
  ThingSpeak.setField(2, insideLuminosity);
}

// Reads the temperature read by the LM35 through the PCF8591 in degrees Celsius
int getInsideTemperature() {
  return pcf8591.voltageRead(PCF8591_SENSOR_LM35) * PCF8591_FACTOR_LM35;
}

// Reads the LDR luminosity through the PCF8591
float getInsideLuminosity() {
  return 100.0 - pcf8591.voltageRead(PCF8591_SENSOR_LIGHT) * PCF8591_FACTOR_LIGHT;
}

// Set the fan speed through the AD/DA converter and convert output
void setFanSpeed(int speed) {
  // convert speed to voltage - 0% (0V) - 100% (3.3V)
  speed = constrain(speed, 0, 100);
  float voltage = (float) speed * 3.3 / 100.0;

  // write the voltage on the PCF8591 output
  pcf8591.voltageWrite(voltage, false, 3.3);
}