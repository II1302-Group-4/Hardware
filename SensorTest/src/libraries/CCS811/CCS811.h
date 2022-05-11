/*
 *
 3      Created by Andreas Nyström.
 4
 */

#ifndef CCS811_h
#define CCS811_h

#include "Arduino.h"
#include "../Softi2c/Softi2c.h"

static const uint8_t I2C_ADDRESS = 0x5A;
static const uint8_t HWID = 0x81;

class CCS811
{
public:
    CCS811(int sda, int scl);
    void init();
    void fetchData();
    void setReadInterval(uint8_t mode);
    uint16_t getCO2();
    uint16_t getVOC();
    static const uint8_t INTERVAL_250MS = 0x04;
    static const uint8_t INTERVAL_1SEC = 0x01;
    static const uint8_t INTERVAL_10SEC = 0x02;
    static const uint8_t INTERVAL_60SEC = 0x03;
private:
    Softi2c * i2c = NULL;
    uint16_t _co2;
    uint16_t _voc;
};

#endif
