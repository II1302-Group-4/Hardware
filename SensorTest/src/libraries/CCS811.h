#ifndef CCS811_h
#define CCS811_h

#include "Arduino.h"
#include "../Softi2c/Softi2c.h"


class CCS811
{
public:
    CCS811(int sda, int scl);
    bool init();
    void fetchData();
    void setReadInterval(uint8_t mode);
    
    uint16_t getCO2();
    uint16_t getVOC();
    
    static const uint8_t INTERVAL_250MS = 0x04;
    static const uint8_t INTERVAL_1SEC = 0x01;
    static const uint8_t INTERVAL_10SEC = 0x02;
    static const uint8_t INTERVAL_60SEC = 0x03;
    
    static const uint8_t HWID_REGISTER = 0x20;
    static const uint8_t RESET_REGISTER = 0xff;
    static const uint8_t STATUS_REGISTER = 0x00;
    static const uint8_t START_REGISTER = 0xf4;
    static const uint8_t DATA_REGISTER = 0x02;
    static const uint8_t INTERVAL_REGISTER = 0x01;
    
    uint8_t I2C_ADDRESS = 0x5A;
    uint8_t HWID = 0x81;
    
    Softi2c * i2c = NULL;
private:
    uint16_t _co2;
    uint16_t _voc;
};

#endif
