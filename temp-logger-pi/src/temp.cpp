#include <Wire.h>
#include <Arduino.h>

#define MCP9808_ADDRESS 0x18
#define MCP9808_REG_AMBIENT 0x05
#define MCP9808_REG_MANUF_ID 0x06
#define MCP9808_REG_RESOLUTION 0x08

float get_temp()
{
    uint8_t upperByte, lowerByte;
    float temp;

    Wire.beginTransmission(MCP9808_ADDRESS);
    Wire.write(MCP9808_REG_AMBIENT);
    Wire.endTransmission();

    Wire.requestFrom(MCP9808_ADDRESS, 2);
    //uint8_t count = Wire.available();

    upperByte = Wire.read();
    lowerByte = Wire.read();
    
    upperByte &= 0x0F;
    temp = (float)(upperByte * 16 + (float)lowerByte / 16);
    Wire.endTransmission();
    
    return temp;
}
