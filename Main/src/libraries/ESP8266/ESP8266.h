#pragma once 

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266
{
public:
    ESP8266(int rx, int tx);
    void init();
    void connectToAP(String ssid, String pwd);
    void openTCP(String ip, String port);
    void closeTCP();
    void sendData(String data);
    void postData(String voc, String co2);
private:
    SoftwareSerial* espSerial = NULL;
    void flushESP();
    void sendCmd(String cmd);
    String readResponse();
    String readResponse(const int timeout);
};

