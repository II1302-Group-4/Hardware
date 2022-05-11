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
    int closeTCP();
    int status();
    void sendData(String data);
    void postData(String unixTime, String voc, String co2);
    long getEpoch(String host, String port);
private:
    SoftwareSerial* espSerial = NULL;
    void flushESP();
    String sendCmd(String cmd);
    String readResponse();
    String readResponse(const int timeout);
    String readResponseDaytime(const int timeout);
    String getSubstring(String str, String divider);
    String trimString(String str, String remove);
    long getEpoch(String host, String port, int timeout);
    long calcUnixTime(int year, String month, int day, int hour, int minute, int second);
    int getDays(String month);
};

