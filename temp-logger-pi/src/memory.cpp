#include "23K256SRAM.h"
#include "memory.h"
#include "globalvars.h"
#include <Arduino.h>

SRAM Mem;

void wipe_memory()
{
    Mem.wipe();

    globalRegCount = 0;
}

void setup_memory()
{
    Mem.begin_spi(CS_PIN);

    globalRegCount = read_reg_count();

    if (globalRegCount == DEFAULT_REG_COUNT)
    {
        globalRegCount = 0;

        write_reg_count(globalRegCount);
    }
}

void write_reg(uint16_t regAddr, float *temp, uint32_t *unixTime)
{

    Mem.write_array((regAddr * 8) + 2, (uint8_t *)temp, 4);
    Mem.write_array((regAddr * 8) + 6, (uint8_t *)unixTime, 4);
}

void read_reg(uint16_t regAddr, float *temp, uint32_t *unixTime)
{
    uint8_t regRead[8];
    float tempRead;
    uint32_t aux, timeRead;

    Mem.read_array((regAddr * 8) + 2, (uint8_t *)regRead, sizeof(regRead));

    // https://stackoverflow.com/questions/5057428/converting-uint-to-float-in-c
    aux = (uint32_t)(regRead[3] << 24) | (uint32_t)(regRead[2] << 16) | (uint32_t)(regRead[1] << 8) | (uint32_t)regRead[0];
    tempRead = *(float *)&aux;
    timeRead = (uint32_t)(regRead[7] << 24) | (uint32_t)(regRead[6] << 16) | (uint32_t)(regRead[5] << 8) | (uint32_t)regRead[4];

    *temp = tempRead;
    *unixTime = timeRead;

    // debug
    /* Serial.printf("reg: %u\n", regAddr);
    Serial.printf("temperatura read_reg: %f\n", tempRead);
    Serial.printf("tempo read_reg: %u\n", timeRead); */
}

void write_reg_count(uint16_t regCount)
{
    //////////////ERRO????//////////////////
    Mem.write_array(ADDR_REG_COUNT, (uint8_t *)&regCount, BUFFER_SIZE);
}

uint16_t read_reg_count()
{
    uint16_t regCount;
    uint8_t regRead[2];

    Mem.read_array(ADDR_REG_COUNT, (uint8_t *)regRead, BUFFER_SIZE);

    regCount = ((uint16_t)regRead[1] << 8) | (uint16_t)regRead[0];

    return regCount;
}