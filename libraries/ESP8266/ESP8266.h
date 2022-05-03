#pragma once 

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266
{
public:
    ESP8266(int rx, int tx, bool DEBUG);
    void init();
    void test();
    void clientMode();
    void connect(String ssid, String pwd);
    void openTCP(String ip, String port);
    void sendData(String data);
private:
    SoftwareSerial* espSerial = NULL;
    bool DEBUG;
    void flushSerial();
    void sendCmd(String cmd);
    String readResponse();
    String readResponse(const int timeout);
};

