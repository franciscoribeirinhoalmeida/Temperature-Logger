#ifndef _23K256SRAM_H_
#define _23K256SRAM_H_

#include <Arduino.h>
#define SRAM_MAX_SIZE 0x7FFF

class SRAM
{
public:
    uint8_t csPin;

    void begin_spi(uint8_t csPin);

    void write_byte(uint16_t addr, uint8_t *data);

    uint8_t read_byte(uint16_t addr);

    void write_array(uint16_t addr, const uint8_t *data, uint16_t size);

    void read_array(uint16_t addr, uint8_t *data, uint16_t size);

    void wipe();
};

#endif