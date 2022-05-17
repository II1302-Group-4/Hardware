#pragma once

#include "../ESP8266/ESP8266.h"
#include "../CCS811/CCS811.h"

class PolluSense 
{
public:
    ESP8266* wifiModule = NULL;
    CCS811* sensorModule = NULL;
    PolluSense(int rx, int tx, bool debug);
    long getEpoch(String host, String port);
    bool postData(String unixTime, String voc, String co2);
private:
    const String HTTP_POST_HEADER = "POST /data HTTP/1.1\r\nContent-Type: application/json\r\nAccept: */*\r\nHost: pollusenseserver.azurewebsites.net\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: ";
    long getEpoch(String host, String port, int timeout);
 //   String readResponseDaytime(const int timeout);
//    String getSubstring(String str, String divider);
  //  String trimString(String str, String remove);
    long calcUnixTime(int year, char month[], int day, int hour, int minute, int second);
    int getDays(char month[]);
    int getLeapDays(int year, int days);
    bool validateResponse(char response[]);
    int getIndex(char buffer[], int pos, char divider);
    int bufferToInt(char buffer[], int start, int end);
    void fillCharArray(char buffer[], char * res, int start, int end);
};
    

