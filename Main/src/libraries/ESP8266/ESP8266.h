#pragma once 

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266
{
public:
    ESP8266(int rx, int tx);
    void init();
    void connectToAP(String ssid, String pwd);
    String openTCP(String ip, String port);
    int closeTCP();
    int status();
    void sendData(String data);
private:
    SoftwareSerial* espSerial = NULL;
    void flushESP();
    String sendCmd(String cmd);
    String readResponse();
    String readResponse(const int timeout);
};

