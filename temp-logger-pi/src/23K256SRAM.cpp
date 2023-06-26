#include <SPI.h>
#include <Arduino.h>
#include "23K256SRAM.h"

void SRAM::write_byte(uint16_t addr, uint8_t *data)
{

    digitalWrite(csPin, LOW);
    SPI.transfer16(0x0101); // byte mode
    digitalWrite(csPin, HIGH);

    /*
    //CHECK MODE CONFIG
    digitalWrite(csPin, LOW);
    SPI.transfer(0x05);
    uint8_t print = SPI.transfer(0x00);
    digitalWrite(csPin, HIGH);
    Serial.println(print, HEX);

    */

    digitalWrite(csPin, LOW);
    SPI.transfer(0x02);   // write data
    SPI.transfer16(addr); // in addr
    SPI.transfer(*data);
    digitalWrite(csPin, HIGH);
}

uint8_t SRAM::read_byte(uint16_t addr)
{

    digitalWrite(csPin, LOW);
    SPI.transfer16(0x0101); // byte mode
    digitalWrite(csPin, HIGH);

    digitalWrite(csPin, LOW);
    SPI.transfer(0x03);   // read data
    SPI.transfer16(addr); // from addr
    uint8_t readData = SPI.transfer(0x00);
    digitalWrite(csPin, HIGH);

    return readData;
}

void SRAM::write_array(uint16_t addr, const uint8_t *data, uint16_t size)
{
    digitalWrite(csPin, LOW);
    SPI.transfer16(0x0141); // sequential mode
    digitalWrite(csPin, HIGH);

    digitalWrite(csPin, LOW);
    SPI.transfer(0x02);   // write data
    SPI.transfer16(addr); // in addr
    SPI.transferBytes(data, NULL, size);
    digitalWrite(csPin, HIGH);
}

void SRAM::read_array(uint16_t addr, uint8_t *data, uint16_t size)
{

    digitalWrite(csPin, LOW);
    SPI.transfer16(0x0141); // sequential mode
    digitalWrite(csPin, HIGH);

    digitalWrite(csPin, LOW);
    SPI.transfer(0x03);   // read data
    SPI.transfer16(addr); // from addr
    SPI.transferBytes(NULL, data, size);
    digitalWrite(csPin, HIGH);
}

void SRAM::begin_spi(uint8_t cspin)
{
    csPin = cspin;

    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);

    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPI.setDataMode(SPI_MODE0);

    delay(10);
}

void SRAM::wipe()
{
    uint8_t read;
    uint8_t ptr;
    ptr = 0xFF;

    // Serial.printf("wiping mem...\n");

    for (uint16_t i = 0; i <= SRAM_MAX_SIZE; i++)
    {
        write_byte(i, &ptr);
        //read = read_byte(i);
        // Serial.printf("add(%x): %x\n", i, read);
    }

    Serial.printf("mem wiped\n");
}