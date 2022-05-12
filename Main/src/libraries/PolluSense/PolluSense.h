#include "../ESP8266/ESP8266.h"
#include "../CCS811/CCS811.h"

class PolluSense 
{
public:
    ESP8266* wifiModule = NULL;
    CCS811* sensorModule = NULL;
    PolluSense(int rx, int tx, bool debug);
    long getEpoch(String host, String port);
    void postData(String unixTime, String voc, String co2);
private:
    const String HTTP_POST_HEADER;
    String readResponseDaytime(const int timeout);
    String getSubstring(String str, String divider);
    String trimString(String str, String remove);
    long getEpoch(String host, String port, int timeout);
    long calcUnixTime(int year, char month[], int day, int hour, int minute, int second);
    int getDays(char month[]);
};
