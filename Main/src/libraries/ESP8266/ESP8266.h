#pragma once 

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266
{
public:
    String ssid;
    String pwd;
    ESP8266(int rx, int tx, bool debug);
    void basicInit();
    void init();
    bool connectToAP();
    bool openTCP(String ip, String port);
    int closeTCP();
    int status();
    bool sendData(String &data, String &res);
    bool readData(String &res);
    bool readData(String &res, const int timeout);
private:
    SoftwareSerial* espSerial = NULL;
    bool DEBUG;
    void flushESP();
    bool sendCmd(const char* c);
    bool readResponse(const int timeout = 15000);
    bool readResponse(char* buffer, const short buffSize, const int timeout = 15000);
};

