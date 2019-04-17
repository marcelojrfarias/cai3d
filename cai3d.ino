// Config file
#include "config.h"

// PCF8591 8-bit A/D and D/A converter
#include "PCF8591.h"
PCF8591 pcf8591(PCF8591_I2C_ADDRESS);

void setup() {
  // Start the logging serial port
  LOG.begin(LOG_SPEED);

  // Print the project name and version  
  LOG.print(PROJECT_NAME); LOG.print(" "); LOG.println(PROJECT_VERSION);

  // Start the AD/DA converter
  pcf8591.begin();
  setFanSpeed(100);
}

void loop() {
	LOG.print(getInsideTemperature()); LOG.print("\t");
	LOG.print(getInsideLuminosity()); LOG.print("\n");
  delay(500);
}

// Reads the temperature read by the LM35 through the PCF8591 in degrees Celsius
int getInsideTemperature() {
  return pcf8591.voltageRead(PCF8591_SENSOR_LM35) * PCF8591_FACTOR_LM35;
}

// Reads the LDR luminosity through the PCF8591
int getInsideLuminosity() {
  return pcf8591.voltageRead(PCF8591_SENSOR_LIGHT) * PCF8591_FACTOR_LIGHT;
}

// Set the fan speed through the AD/DA converter and convert output
void setFanSpeed(int speed) {
  // convert speed to voltage - 0% (0V) - 100% (3.3V)
  speed = constrain(speed, 0, 100);
  float voltage = (float) speed * 3.3 / 100.0;

  // write the voltage on the PCF8591 output
  pcf8591.voltageWrite(voltage, false, 3.3);
}