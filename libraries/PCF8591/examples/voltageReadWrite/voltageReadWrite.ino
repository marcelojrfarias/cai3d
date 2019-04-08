#include "Arduino.h"
#include "PCF8591.h"
#define PCF8591_I2C_ADDRESS 0x48

PCF8591 pcf8591(PCF8591_I2C_ADDRESS);

void setup()
{
	Serial.begin(115200);
	pcf8591.begin();
}

void loop()
{
	pcf8591.voltageWrite(3.3, false, 3.3); // 2.7Volts output

	float ana0V = pcf8591.voltageRead(AIN0);
	Serial.print(ana0V);
	Serial.print("\t");

	float ana1V = pcf8591.voltageRead(AIN1);
	Serial.print(ana1V);
	Serial.print("\t");

	float ana2V = pcf8591.voltageRead(AIN2);
	Serial.print(ana2V);
	Serial.print("\t");

	float ana3V = pcf8591.voltageRead(AIN3);
	Serial.println(ana3V);

	delay(1000);
}
