#pragma once 

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266
{
public:
    String ssid;
    String pwd;
    ESP8266(int rx, int tx);
    void basicInit();
    void init();
    bool connectToAP();
    bool openTCP(const String& ip, const String& port);
    void closeTCP();
    int status();
    void openSendStream(const int& len);
    void pushData(const String& data);
    String readData();
    String readData(const int timeout);
private:
    SoftwareSerial* espSerial = NULL;
    void flushESP();
    void sendCmd(const char* c);
    String readResponse();
    String readResponse(const int timeout);
};

