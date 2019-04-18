// Config file
#include "config.h"

// Credentials file
#include "mycredentials.h"

// WiFi library and parameters
#include <WiFi.h>
char ssid[] = CREDENTIALS_SSID;   // your network SSID (name) 
char pass[] = CREDENTIALS_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// ThingSpeak library and parameters
#include "ThingSpeak.h"
unsigned long myChannelNumber = CREDENTIALS_CH_ID;
const char * myWriteAPIKey = CREDENTIALS_WRITE_APIKEY;

// ThingSpeak Tallback parameters
unsigned long myTalkBackID = CREDENTIALS_TALLBACK_ID;
const char * myTalkbackKey = CREDENTIALS_TALLBACK_KEY;

// PCF8591 8-bit A/D and D/A converter
#include "PCF8591.h"
PCF8591 pcf8591(PCF8591_I2C_ADDRESS);

// Envoriment data
int insideTemperature = 0;
float insideLuminosity = 0;
int fanSpeed = 0;
String status = String("CAI3D");

void setup() {
  // Start the logging serial port
  LOG.begin(LOG_SPEED);

  // Print the project name and version  
  LOG.print(PROJECT_NAME); LOG.print(" "); LOG.println(PROJECT_VERSION);

  // Config the Builtin Led pin as output
  pinMode(LED_BUILTIN, OUTPUT);

  // Start the AD/DA converter
  pcf8591.begin();

  // Set initial fan speed
  setFanSpeed(fanSpeed);

  // Initialize the connection
  WiFi.mode(WIFI_STA);   

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Manage WiFi connection
  wifiConnect();

  // Delete all Talback commands
  deleteAllTalbackCommands();
}

void loop() {
  // Manage WiFi connection
  wifiConnect();
  
  // Check commands fifo
  checkForCommand();

  // Update sensors data
  updateSensorsData();

  // Write to the ThingSpeak channel fields
  updateThingSpeakFields();

  // Send to internet
  sendThingSpeakFields();

  // Wait 15 seconds for the next update
  delay(15100); // ThingSpeak Policy
}