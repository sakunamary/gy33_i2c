#include "Arduino.h"
#include "Wire.h"

#define I2C_SDL D5
#define I2C_SCL D4
GY33_I2C GY33;

void setup()
{
    Serial.begin(115200);
    GY33.beign();

}

void loop()
{}
