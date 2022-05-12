#pragma once 

#include "Arduino.h"
#include <SoftwareSerial.h>

struct Response 
{
    String msg;
    String data;
};

class ESP8266
{
public:
    ESP8266(int rx, int tx);
    void init();
    void connectToAP(String ssid, String pwd);
    Response openTCP(String ip, String port);
    int closeTCP();
    int status();
    void sendData(String data);
private:
    SoftwareSerial* espSerial = NULL;
    void flushESP();
    Response sendCmd(String cmd);
    Response sendCmd(String cmd, bool dataExpected);
    String readResponse();
    String readResponse(const int timeout);
    String readResponseChar(const int timeout);
    String readData();
    String readData(const int timeout);
};

