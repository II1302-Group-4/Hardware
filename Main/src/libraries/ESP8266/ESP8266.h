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
    String sendData(String &data);
    String readData();
    String readData(const int timeout);
private:
    SoftwareSerial* espSerial = NULL;
    bool DEBUG;
    void flushESP();
    String sendCmd(const char* c);
    String readResponse();
    String readResponse(const int timeout);
    String readResponseChar(const int timeout);
};

