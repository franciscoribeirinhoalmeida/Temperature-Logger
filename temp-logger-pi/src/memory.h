#ifndef _MEMORY_H_
#define _MEMORY_H_

#define ADDR_REG_COUNT 0
#define BUFFER_SIZE 2
#define CS_PIN 5
#define DEFAULT_REG_COUNT 0xFFFF

void wipe_memory();

void setup_memory();

void write_reg(uint16_t regAddr, float* temp, uint32_t* unixTime);

void read_reg(uint16_t regAddr, float* temp, uint32_t* unixTime);

void write_reg_count(uint16_t);

uint16_t read_reg_count();

#endif