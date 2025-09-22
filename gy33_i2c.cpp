
/**************************************************************************/
/*!
    @file     GY33_MCU.h
    @author   BPoHVoodoo (FabLab Luenen)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2018,
    All rights reserved.

    Driver for the GY-33 MCU digital color sensors.

    @section  HISTORY

    v1.0 - First release
*/
/**************************************************************************/

#include <stdlib.h>
#include <math.h>

#include "gy33_i2c.h"

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
    Constructor
*/
/**************************************************************************/
GY33_I2C::GY33_I2C(uint8_t addr, uint8_t sda, uint8_t scl, uint32_t freq)
{

    uint8_t error;
    Wire.begin(sda, scl, freq);
    delay(10);
    Wire.beginTransmission(addr);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("GY-33 I2C device found");
    } else {
        Serial.println("No GY-33 I2C device found ... check your wiring?");
    }
    delay(10);

    _MCUInitialised = false;
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

/**************************************************************************/
/*!
    Initializes I2C and configures the sensor (call this function before
    doing anything else)
*/
/**************************************************************************/
boolean GY33_I2C::begin()
{
    uint8_t x;

    /* Set default integration time and gain */
    x = read8(MCU_CONFIG);
    Serial.println(x, HEX);
    if (x != 0x10)
    {
        return false;
    }
    /* Make sure we're actually connected */
    _MCUInitialised = true;
    return true;
}

/**************************************************************************/
/*!
    @brief  Reads the raw red, green, blue and clear channel values
*/
/**************************************************************************/
void GY33_I2C::getRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c, uint16_t *lux, uint16_t *ct)
{
    if (!_MCUInitialised)
        begin();

    *r = read16(MCU_RDATAH);
    *g = read16(MCU_GDATAH);
    *b = read16(MCU_BDATAH);
    *c = read16(MCU_CDATAH);
    *lux = read16(MCU_LDATAH);
    *ct = read16(MCU_CTDATAH);
}

void GY33_I2C::getData(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *c, uint8_t *conf)
{
    if (!_MCUInitialised)
        begin();

    *r = read8(MCU_RDATA);
    *g = read8(MCU_GDATA);
    *b = read8(MCU_BDATA);
    *c = read8(MCU_COLDATA);
    *conf = read8(MCU_CONFIG);
}

/**************************************************************************/
/*!
    @brief  Converts the raw R/G/B values to color temperature in degrees
            Kelvin
*/
/**************************************************************************/
uint16_t GY33_I2C::calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b)
{
    float X, Y, Z; /* RGB to XYZ correlation      */
    float xc, yc;  /* Chromaticity co-ordinates   */
    float n;       /* McCamy's formula            */
    float cct;

    /* 1. Map RGB values to their XYZ counterparts.    */
    /* Based on 6500K fluorescent, 3000K fluorescent   */
    /* and 60W incandescent values for a wide range.   */
    /* Note: Y = Illuminance or lux                    */
    X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
    Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
    Z = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

    /* 2. Calculate the chromaticity co-ordinates      */
    xc = (X) / (X + Y + Z);
    yc = (Y) / (X + Y + Z);

    /* 3. Use McCamy's formula to determine the CCT    */
    n = (xc - 0.3320F) / (0.1858F - yc);

    /* Calculate the final CCT */
    cct = (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

    /* Return the results in degrees Kelvin */
    return (uint16_t)cct;
}

/**************************************************************************/
/*!
    @brief  Converts the raw R/G/B values to lux
*/
/**************************************************************************/
uint16_t GY33_I2C::calculateLux(uint16_t r, uint16_t g, uint16_t b)
{
    float illuminance;

    /* This only uses RGB ... how can we integrate clear or calculate lux */
    /* based exclusively on clear since this might be more reliable?      */
    illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

    return (uint16_t)illuminance;
}

void GY33_I2C::setConfig(uint8_t high, uint8_t low)
{
    Serial.println("GY-33: ");
    Serial.println(high | low, HEX);
    write8(MCU_CONFIG, high | low);
}
uint8_t GY33_I2C::getConfig(void)
{
    if (!_MCUInitialised)
        begin();

    return read8(MCU_CONFIG);
}

/*========================================================================*/
/*                          PRIVATE FUNCTIONS                             */
/*========================================================================*/

/**************************************************************************/
/*!
    @brief  Implements missing powf function
*/
/**************************************************************************/
float powf(const float x, const float y)
{
    return (float)(pow((double)x, (double)y));
}

/**************************************************************************/
/*!
    @brief  Writes a register and an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t GY33_I2C::write8(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(MCU_ADDRESS);
    Wire.write(reg);
    Wire.write(val & 0xFF);
    return Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t GY33_I2C::read8(uint8_t reg)
{
    Wire.beginTransmission(MCU_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(MCU_ADDRESS, 1);
    if (Wire.available())
    {
        return Wire.read();
    }
    return 0;
    ;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit values over I2C
*/
/**************************************************************************/
uint16_t GY33_I2C::read16(uint8_t reg)
{
    uint16_t x;
    uint16_t t;

    Wire.beginTransmission(MCU_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(MCU_ADDRESS, 2);
    t = Wire.read();
    x = Wire.read();
    x <<= 8;
    x |= t;
    return x;
}
