
#include "config.h"

// Updates sensors data
void updateSensorsData() {
  insideTemperature = getInsideTemperature();
  insideLuminosity = getInsideLuminosity();
	// LOG.println(insideTemperature);
	// LOG.println(insideLuminosity);
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
  fanSpeed = constrain(speed, 0, 100);
  float voltage = (float) speed * 3.3 / 100.0;

  // write the voltage on the PCF8591 output
  pcf8591.voltageWrite(voltage, false, 3.3);
}