#include "PolluSense.h"

PolluSense::PolluSense(int rx, int tx, bool debug) {
    HTTP_POST_HEADER = "POST /data HTTP/1.1\r\nContent-Type: application/json\r\nAccept: */*\r\nHost: pollusenseserver.azurewebsites.net\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: ";
    wifiModule = new ESP8266(rx, tx, debug);
    sensorModule = new CCS811(A2, A3);
}

/*--------------------Public--------------------*/

void PolluSense::postData(String time, String voc, String co2) {
    String data = String("\r\n\r\n{\"time\": \"" + time + "\",\"VOC\": \"" + voc + "\",\"CO2\": \"" + co2 + "\"}");
    String dataLen = String(data.length() - 4);
    String len = String((HTTP_POST_HEADER.length() + dataLen.length() + data.length()));

    wifiModule->sendData(len);
    wifiModule->pushData(HTTP_POST_HEADER);
    wifiModule->pushData(dataLen);
    wifiModule->pushData(data);
    wifiModule->readData();
}

/*
 * This function returns the approximate UNIX time, note that 
 * disregards leap seconds.
 */
long PolluSense::getEpoch(String host, String port) {
    return getEpoch(host, port, 5000);
}

/*--------------------Private--------------------*/

String PolluSense::getSubstring(String str, String divider) {
    int pos = str.indexOf(divider);
    return str.substring(0,pos);
}

String PolluSense::trimString(String str, String remove) {
    int len = remove.length();
    return str.substring(len + 1);
}

long PolluSense::getEpoch(String host, String port, int timeout) {
    wifiModule->openTCP(host, port);
    String response = wifiModule->readData();

    // If the ESP8266 can't connected to the daytime-server within
    // the timeout, the setup fails.
    if(response == "")
        return 0;
    String day = getSubstring(response, " ");
    String month = getSubstring(response = trimString(response, day), " ");
    String year = getSubstring(response = trimString(response, month), " ");
    String hour = getSubstring(response = trimString(response, year), ":");
    String minute = getSubstring(response = trimString(response, hour), ":");
    String second = getSubstring(trimString(response, minute), " ");

    char monthArray[4];
    month.toUpperCase();
    month.toCharArray(monthArray, 4);

    return calcUnixTime(year.toInt(), monthArray, day.toInt(), hour.toInt(), minute.toInt(), second.toInt());
}

long PolluSense::calcUnixTime(int year, char month[], int day, int hour, int minute, int second) {
    long unixTime = 0;

    if(year < 1970)
        return 0;
    if(day <= 0 || day > 31)
        return 0;
    if(hour < 0 || hour > 23)
        return 0;
    if(minute < 0 || minute > 59)
        return 0;
    if(second < 0 || second > 59)
        return 0;


    int days_in_year = getDays(month);
    if(days_in_year < 0 || days_in_year > 365)
        return 0;

    days_in_year += day;
    unixTime = (year - 1970) * 31556926;
    unixTime += days_in_year * 86400;
    unixTime += hour * 3600; 
    unixTime += minute * 60;
    unixTime += second;
    
    // Adds a constant amount of time to adjust for the year constant,
    // which is not exact.116 minutes to be exact. 
    // This was found through testing.
    unixTime += (116 * 60);
    return unixTime;
}


int PolluSense::getDays(char month[]) {

    //January 
    if(month[0] == 74 && month[1] == 65)
        return 0;
    //February 
    if(month[0] == 70)
        return 31;
    //Mars
    if(month[0] == 77 && month[1] == 65 && month[2] == 82)
        return 59;
    //May
    if(month[0] == 77 && month[1] == 65 && month[2] == 89)
        return 121;
    //June
    if(month[0] == 74 && month[1] == 85 && month[2] == 78)
        return 152;
    //July
    if(month[0] == 74 && month[1] == 85 && month[2] == 76)
        return 152;
    //asdfaf
    if(month[0] == 65 && month[1] == 85)
        return 213;
    //October
    if(month[0] == 79)
        return 274;
    //November
    if(month[0] == 78)
        return 305;
    //December
    if(month[0] == 68)
        return 335;

    return -1;
}
