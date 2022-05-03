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
    
    while(!readMultipleData(address, reg, buf, 1))
    {
        stopCondition();
        Serial.println("ERROR");
    }
    
    return buf[0];
}

void Softi2c::read(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size)
{
    while(!readMultipleData(address, reg, data, size))
    {
        stopCondition();
        Serial.println("ERROR");
    }
}

bool Softi2c::readMultipleData(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size)
{
    startCondition();
    writeByte(address << 1 | 0);
    if(!readAck()){return false;}
    writeByte(reg);
    if(!readAck()){return false;}
    stopCondition();
    
    startCondition();
    writeByte(address << 1 | 1);
    if(!readAck()){return false;}
    
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
    
    while(!writeMultipleData(address, reg, buf, 1))
    {
        stopCondition();
        Serial.println("ERROR");
    }
}

void Softi2c::write(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size)
{
    while(!writeMultipleData(address, reg, data, size))
    {
        stopCondition();
        Serial.println("ERROR");
    }
}

bool Softi2c::writeMultipleData(uint8_t address, uint8_t reg, uint8_t* data, uint8_t size)
{
    startCondition();
    writeByte(address << 1 | 0);
    if(!readAck()){return false;}
    writeByte(reg);
    if(!readAck()){return false;}
    
    for(uint8_t i = 0; i < size; i++)
    {
        writeByte(data[i]);
        
        if(!readAck()){return false;}
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
    writeBit(false);
}

void Softi2c::writeNack()
{
    writeBit(true);
}

bool Softi2c::readBit()
{
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
    delayMicroseconds(100);
}
