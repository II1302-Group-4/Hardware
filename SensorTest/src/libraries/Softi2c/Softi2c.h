/*
 *
 *      Created by Andreas Nyström.
 *
 */

#ifndef Softi2c_h
#define Softi2c_h

#include "Arduino.h"

class Softi2c
{
public:
    Softi2c(int sda, int scl);
    uint8_t read(uint8_t address, uint8_t reg);
    void read(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size);
    void write(uint8_t address, uint8_t reg, uint8_t data);
    void write(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size);
    String symbolStream;
private:
    bool readMultipleData(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size);
    bool writeMultipleData(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size);
    void sdaLo();
    void sdaHi();
    void sclLo();
    void sclHi();
    void startCondition();
    void stopCondition();
    void pulse();
    bool readAck();
    bool readNack();
    void writeAck();
    void writeNack();
    bool readBit();
    uint8_t readByte();
    void writeBit(bool bit);
    void writeByte(uint8_t byte);
    void wait();
    int _scl;
    int _sda;
};

#endif
