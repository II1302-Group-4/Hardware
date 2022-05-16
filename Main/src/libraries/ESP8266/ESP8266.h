#pragma once 

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266
{
public:
    String ssid;
    String pwd;
    ESP8266(int rx, int tx, bool debug);
    void init();
    bool connectToAP(String ssid, String pwd);
    bool openTCP(String ip, String port);
    int closeTCP();
    int status();
    void sendData(String len);
    void pushData(String data);
    String readData();
    String readData(const int timeout);
private:
    SoftwareSerial* espSerial = NULL;
    bool DEBUG;
    void flushESP();
    String sendCmd(String cmd);
    String readResponse();
    String readResponse(const int timeout);
    String readResponseChar(const int timeout);
};

