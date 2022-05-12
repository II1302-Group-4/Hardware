#include "Arduino.h"
#include "CCS811.h"
#include "../Softi2c/Softi2c.h"


CCS811::CCS811(int sda, int scl)
{
    i2c = new Softi2c(sda, scl);
}

bool CCS811::init()
{
    //check hardware ID
    if(i2c->read(I2C_ADDRESS, HWID_REGISTER) != HWID)
        return false;
    
    //reset device
    uint8_t reset[4] = {0x11, 0xe5, 0x72, 0x8a};
    i2c->write(I2C_ADDRESS, RESET_REGISTER, reset, 4);
    delay(100);
    
    //start device
    i2c->write(I2C_ADDRESS, START_REGISTER, NULL, 0);
    delay(100);
    
    //check for error
    if(i2c->read(I2C_ADDRESS, STATUS_REGISTER) != 0x90)// should be 0x90
        return false;
    
    //set drive mode 1 sec default
    setReadInterval(INTERVAL_1SEC);
    
    return true;
}

void CCS811::fetchData()
{
    //wait for data
    while((i2c->read(I2C_ADDRESS, STATUS_REGISTER) & 0x08) == 0);
    
    uint8_t results[8];
    i2c->read(I2C_ADDRESS, DATA_REGISTER, results, 8);
    
    _co2 = (results[0] << 8) | (results[1]);
    _voc = (results[2] << 8) | (results[3]);
}

void CCS811::setReadInterval(uint8_t mode)
{
    i2c->write(I2C_ADDRESS, INTERVAL_REGISTER, mode << 4);
}

uint16_t CCS811::getCO2()
{
    return _co2;
}

uint16_t CCS811::getVOC()
{
    return _voc;
}
