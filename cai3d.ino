// Config file
#include "config.h"

// Credentials file
#include "mycredentials.h"

// SD Card libraries
#include "FS.h"
#include "SD.h"
#include "SPI.h"
String sdBuffer;

// WiFi library and parameters
#include <WiFi.h>
char ssid[] = CREDENTIALS_SSID;   // your network SSID (name) 
char pass[] = CREDENTIALS_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// Time library and parameters
#include "time.h"
struct tm currentTime;
char currentTimeFormatted[64];

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

// Envoriment updateSensorsData
int insideTemperature = 25;
int insideLuminosity = 40;
int fanSpeed = 0;
String status = String("CAI3D");

void setup() {
  // Start the serial port
  DEBUG.begin(DEBUG_SPEED);

  // Print the project name and version  
  DEBUG.print(PROJECT_NAME); DEBUG.print(" "); DEBUG.println(PROJECT_VERSION);
  
  // Config the Builtin Led pin as output
  pinMode(LED_BUILTIN, OUTPUT);

  // Start the AD/DA converter
  pcf8591.begin();

  // Set initial fan speed 
  setFanSpeed(0);

  // init SD Card
  if (initSDCard()) {
    // Read wifi credentials from SD
    // readWiFiCredentialsFromSD(ssid, pass);
  }
  
  // Setup the wifi parameters
  WiFi.mode(WIFI_STA);

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Delete all Talback commands
  deleteAllTalbackCommands();

  //init and get the time
  getTimeFromNtpServer();
}

void loop() {
  // Update sensors data
  updateSensorsData(true);

  // Send data for the ThingSpeak
  updateThingSpeakFields();

  // Check commands fifo
  checkForCommand();

  // Send by the serial
  logSensorsToSerial();

  // Save data to SD card
  logDataToSD();
  
  // Wait 15 seconds for the next update
  delay(15100); // ThingSpeak Policy
}

