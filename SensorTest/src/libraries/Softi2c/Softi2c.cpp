/*
 *
 *      Created by Andreas Nyström.
 *
 */

#include "Arduino.h"
#include "Softi2c.h"

Softi2c::Softi2c(int sda, int scl)
{
    _sda = sda;
    _scl = scl;
    sclHi();
    sdaHi();
}

uint8_t Softi2c::read(uint8_t address, uint8_t reg)
{
    uint8_t buf[1];
    
    if(!readMultipleData(address, reg, buf, 1))
    {
        stopCondition();
        Serial.println("ERROR");
    }
    
    return buf[0];
}

void Softi2c::read(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size)
{
    if(!readMultipleData(address, reg, data, size))
    {
        stopCondition();
        Serial.println("ERROR");
    }
}

bool Softi2c::readMultipleData(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size)
{
    symbolStream = "";
    
    startCondition();
    writeByte(address << 1 | 0);
    if(!readAck()){}
    writeByte(reg);
    if(!readAck()){}
    stopCondition();
    
    startCondition();
    writeByte(address << 1 | 1);
    if(!readAck()){}
    
    for(uint8_t i = 0; i < size; i++)
    {
        data[i] = readByte();
        
        if(i != size - 1)
        {
            writeAck();
        }
    }

    writeNack();
    stopCondition();
    return true;
}

void Softi2c::write(uint8_t address, uint8_t reg, uint8_t data)
{
    uint8_t buf[1] = {data};
    
    if(!writeMultipleData(address, reg, buf, 1))
    {
        stopCondition();
        Serial.println("ERROR");
    }
}

void Softi2c::write(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size)
{
    if(!writeMultipleData(address, reg, data, size))
    {
        stopCondition();
        Serial.println("ERROR");
    }
}

bool Softi2c::writeMultipleData(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size)
{
    symbolStream = "";
    
    startCondition();
    writeByte(address << 1 | 0);
    if(!readAck()){}
    writeByte(reg);
    if(!readAck()){}
    
    for(uint8_t i = 0; i < size; i++)
    {
        writeByte(data[i]);
        
        if(!readAck()){}
    }
    
    stopCondition();
    return true;
}



void Softi2c::sdaLo()
{
    digitalWrite(_sda, LOW);
    pinMode(_sda, OUTPUT);
    wait();
}

void Softi2c::sdaHi()
{
    pinMode(_sda, INPUT_PULLUP);
    wait();
}

void Softi2c::sclLo()
{
    digitalWrite(_scl, LOW);
    pinMode(_scl, OUTPUT);
    wait();
}

void Softi2c::sclHi()
{
    pinMode(_scl, INPUT_PULLUP);
    wait();
}

void Softi2c::startCondition()
{
    symbolStream += "S";
    
    sdaHi();
    sclHi();
    wait();
    sdaLo();
    wait();
    sclLo();
    wait();
}

void Softi2c::stopCondition()
{
    symbolStream += "E";
    
    sdaLo();
    wait();
    sclHi();
    wait();
    sdaHi();
    wait();
}

void Softi2c::pulse()
{
    sclHi();
    wait();
    sclLo();
}

bool Softi2c::readAck()
{
    sdaHi();
    return !readBit();
}

bool Softi2c::readNack()
{
    sdaHi();
    return readBit();
}

void Softi2c::writeAck()
{
    symbolStream += "A";
    sdaLo();
    pulse();
}

void Softi2c::writeNack()
{
    symbolStream += "N";
    sdaHi();
    pulse();
}

bool Softi2c::readBit()
{
    symbolStream += "R";
    
    bool data;
    sclHi();
    data = digitalRead(_sda);
    sclLo();
    return data;
}

uint8_t Softi2c::readByte()
{
    uint8_t byte = 0;
    
    sdaHi();
    for (uint8_t i = 0; i < 8; i++) {
        byte = byte | (readBit() << (7 - i));
    }
    
    return byte;
}

void Softi2c::writeBit(bool bit)
{
    symbolStream += bit ? "1" : "0";
    
    if(bit) {sdaHi();} else {sdaLo();}
    pulse();
}

void Softi2c::writeByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++) {
        writeBit((0x80 >> i) & byte);
    }
}

void Softi2c::wait()
{
    delayMicroseconds(50);
}
