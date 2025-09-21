#include "Arduino.h"
#include "Wire.h"
#include "gy33_i2c.h"

#define I2C_SDL D5
#define I2C_SCL D4
GY33_I2C GY33;
uint8_t gy33_config;
void setup() {
  Serial.begin(115200);
  GY33.begin();

  gy33_config = GY33.getConfig();
  Serial.println(gy33_config, HEX);
}

void loop() {
}
