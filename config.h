
#ifndef CONFIG_h
#define CONFIG_h

// Project info
#define PROJECT_NAME "CAI3D"
#define PROJECT_VERSION "v1.0"

// Debug settings
#define DEBUG_ENABLED true
#define DEBUG if(DEBUG_ENABLED) Serial
#define DEBUG_SPEED 115200

// Logger settings
#define LOG_ENABLED true
#define LOG if(LOG_ENABLED) Serial
#define LOG_SPEED 115200

// PCF8591 8-bit A/D and D/A converter
#define PCF8591_PIN_SDA D21
#define PCF8591_PIN_SCL D22
#define PCF8591_I2C_ADDRESS 0x48
#define PCF8591_SENSOR_LIGHT AIN0
#define PCF8591_SENSOR_NTC AIN1
#define PCF8591_SENSOR_LM35 AIN2
#define PCF8591_SENSOR_POT AIN3
#define PCF8591_FACTOR_LM35 100.0
#define PCF8591_FACTOR_LIGHT 0.30 * 100.0

// Time parameters
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC (-3)*3600 // GMT-3
#define DAY_LIGHT_OFFSET_SEC 0 // s 

#endif
