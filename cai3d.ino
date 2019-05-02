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

// Envoriment data
int insideTemperature = 25;
int insideLuminosity = 40;
int fanSpeed = 0;
bool ledState;
String status = String("CAI3D");

// Thread and Thread Controller Libraries
#include "Thread.h"
#include "ThreadController.h"
// Systems Threads
void updateSensorsData();
void updateThingSpeakFields();
void checkForCommand();
void logSensorsToSerial();
void logDataToSD();
void getCommandsFromSerial();
void heartBeat();
Thread threadUpdateSensorsData(updateSensorsData, 2500);
Thread threadUpdateThingSpeakFields(updateThingSpeakFields, 15001); // ThingSpeak Policy
Thread threadCheckForCommand(checkForCommand, 5000);
Thread threadLogSensorsToSerial(logSensorsToSerial, 3000);
Thread threadLogDataToSD(logDataToSD, 5000);
Thread threadGetCommandsFromSerial(getCommandsFromSerial, 100);
Thread threadHeartBeat(heartBeat, LED_INTERVAL);
ThreadController System;

void setup() {
  // Start the serial port
  DEBUG.begin(DEBUG_SPEED);

  // Print the project name and version  
  DEBUG.print(PROJECT_NAME); DEBUG.print(" "); DEBUG.println(PROJECT_VERSION);

  // Heart Beat Led setup
  pinMode(LED_PIN, OUTPUT);

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

  // Connect to WiFi
  wifiConnect();

  // Delete all Talback commands
  deleteAllTalbackCommands();

  //init and get the time
  getTimeFromNtpServer();

  // Setup the system Threads
  System.add(&threadUpdateSensorsData);
  System.add(&threadUpdateThingSpeakFields);
  System.add(&threadCheckForCommand);
  System.add(&threadLogSensorsToSerial);
  System.add(&threadLogDataToSD);
  System.add(&threadGetCommandsFromSerial);
  System.add(&threadHeartBeat);
}

void loop() {
  System.run();
}

