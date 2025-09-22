/* Example I2C Communication with GY-33 Sensor  */
/*Date: 2025-09-20                              */      
/*Author: Sanguino XIAO                         */
/*
Board: XIAOC3  with SEED STUDIO XIAO Expansion Board base on ESP32C3

I2C SDA: D4 / GPIO6
I2C SCL: D5 / GPIO7

Sensor: GY-33 MCU I2C MODE
Sensor Address: 0x5A

*/


#include "Arduino.h"
#include "Wire.h"
#include "gy33_i2c.h"

#define I2C_SDA D4
#define I2C_SCL D5
#define I2C_ADDRESS (0x5A)
#define I2C_FREQ 100000


GY33_I2C GY33(I2C_ADDRESS,I2C_SDA,I2C_SCL,I2C_FREQ);
uint8_t gy33_config;
void setup() {
  Serial.begin(115200);

  GY33.begin();

  gy33_config = GY33.getConfig();
  Serial.println(gy33_config, HEX);
}

void loop() {
}
