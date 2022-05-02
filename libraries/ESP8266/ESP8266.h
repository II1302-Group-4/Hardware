#pragma once 

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266
{
public:
    ESP8266(int rx, int tx, int baud);
    void test();
    void init();
    void connect(String ssid, String pwd);
    void openTCP(String ip, String port);
    void sendData(String data);
    void sendCmd(String cmd);
    String readResponse();
    String readResponse(int timeout);
    void printIncoming();
private:
    SoftwareSerial * espSerial = NULL;
    void setBaud(int baud);
};

