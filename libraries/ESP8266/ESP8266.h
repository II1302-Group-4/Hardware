#pragma once 

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266
{
public:
    ESP8266(int rx, int tx, bool debug);
    void init();
    String test();
    void clientMode();
    void connect(String ssid, String pwd);
    void openTCP(String ip, String port);
    void sendData(String data);
private:
    SoftwareSerial* espSerial = NULL;
    void flushSerial();
    String sendCmd(String cmd);
    String readResponse(const int timeout);
};

#pragma once 

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266
{
public:
    ESP8266(int rx, int tx, bool debug);
    void init();
    String test();
    void clientMode();
    void connect(String ssid, String pwd);
    void openTCP(String ip, String port);
    void sendData(String data);
private:
    SoftwareSerial* espSerial = NULL;
    void flushSerial();
    String sendCmd(String cmd);
    String readResponse(const int timeout);
};

