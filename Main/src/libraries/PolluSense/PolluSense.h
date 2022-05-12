#include "../ESP8266/ESP8266.h"

class PolluSense 
{
public:
    ESP8266* wifiModule = NULL;
    PolluSense(int rx, int tx);
    long getEpoch(String host, String port);
    void postData(String unixTime, String voc, String co2);
private:
    const String HTTP_POST_HEADER;
    String readResponseDaytime(const int timeout);
    String getSubstring(String str, String divider);
    String trimString(String str, String remove);
    long getEpoch(String host, String port, int timeout);
    long calcUnixTime(int year, String month, int day, int hour, int minute, int second);
    int getDays(String month);
};
