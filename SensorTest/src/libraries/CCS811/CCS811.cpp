#include "Arduino.h"
#include "CCS811.h"
#include "../Softi2c/Softi2c.h"

CCS811::CCS811(int sda, int scl)
{
    i2c = new Softi2c(sda, scl);
}

void CCS811::init()
{
    //check hardware ID
    if(i2c->read(I2C_ADDRESS, 0x20) != HWID)
        return false;
    
    //reset device
    uint8_t reset[4] = {0x11, 0xe5, 0x72, 0x8a};
    i2c->write(I2C_ADDRESS, 0xff, reset, 4);
    delay(100);
    
    //start device
    i2c->write(I2C_ADDRESS, 0xf4, NULL, 0);
    delay(100);
    
    //check for error
    i2c->read(I2C_ADDRESS, 0x00);// should be 0x90 if no error

    //set drive mode 1 sec default
    i2c->write(I2C_ADDRESS, 0x01, 0x10);
}

void CCS811::fetchData()
{
    //wait for data
    while((i2c->read(I2C_ADDRESS, 0x00) & 0x08) == 0);
    
    uint8_t results[8];
    i2c->read(I2C_ADDRESS, 0x02, results, 8);
    
    _co2 = (results[0] << 8) | (results[1]);
    _voc = (results[2] << 8) | (results[3]);
}

void CCS811::setReadInterval(uint8_t mode)
{
    i2c->write(I2C_ADDRESS, 0x01, mode << 4);
}

uint16_t CCS811::getCO2()
{
    return _co2;
}

uint16_t CCS811::getVOC()
{
    return _voc;
}
