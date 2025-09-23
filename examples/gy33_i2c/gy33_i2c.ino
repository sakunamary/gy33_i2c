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

// #include "Arduino.h"
#include "Wire.h"
#include "gy33_i2c.h"

#define I2C_SDA (D4)
#define I2C_SCL (D5)
#define I2C_ADDRESS (0x5A)
#define I2C_FREQ 100000

GY33_I2C GY33_I2C;
uint8_t gy33_config;
uint8_t error;
uint8_t red,green,blue,clean,config;
uint16_t red_raw,green_raw,blue_raw,clean_raw,lux_raw,ct_raw;


void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA, I2C_SCL);
  delay(3000);

  Serial.println("start GY33 testing...");

  GY33_I2C.begin();

//   gy33_config = GY33_I2C.getConfig();
//   Serial.print("before ");
//   Serial.println(gy33_config, HEX);
//   if (gy33_config == 0) {
//     Serial.print("Writing new result:");
//       error = GY33_I2C.setConfig(0b00000000, 0b00000001);
//       Serial.println(error, HEX);
//   }

//   delay(3000);
//   gy33_config = GY33_I2C.getConfig();
//   Serial.print("after ");
//   Serial.println(gy33_config, HEX);
}

void loop() {

GY33_I2C.getData( red, green, blue, clean, config);
Serial.println("get color data ");
Serial.print("red:");Serial.print(red,BIN);Serial.println();
Serial.print("green:");Serial.print(green,BIN);Serial.println();
Serial.print("blue:");Serial.print(blue,BIN);Serial.println();

GY33_I2C.getRawData( &red_raw, &green_raw, &blue_raw, &clean_raw, &lux_raw,&ct_raw);
Serial.println("get color RAW data ");
Serial.print("red raw :");Serial.print(red,BIN);Serial.println();
Serial.print("green raw:");Serial.print(green,BIN);Serial.println();
Serial.print("blue raw :");Serial.print(blue,BIN);Serial.println();


delay(5000);

}
