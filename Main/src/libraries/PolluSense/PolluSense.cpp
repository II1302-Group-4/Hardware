#include "PolluSense.h"

PolluSense::PolluSense(int rx, int tx) {
    HTTP_POST_HEADER = "POST /data HTTP/1.1\r\nContent-Type: application/json\r\nAccept: */*\r\nHost: pollusenseserver.azurewebsites.net\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: ";
    wifiModule = new ESP8266(rx, tx);
}

/*--------------------Public--------------------*/

void PolluSense::postData(String time, String voc, String co2) {
    String data = "{\"time\": \"" + time + "\",\"VOC\": \"" + voc + "\",\"CO2\": \"" + co2 + "\"}";
    String len = "";
    len += data.length();

    wifiModule->sendData(HTTP_POST_HEADER + len + "\r\n\r\n" + data);
}

/*
 * This function returns the approximate UNIX time, note that 
 * disregards leap seconds.
 */
long PolluSense::getEpoch(String host, String port) {
    return getEpoch(host, port, 5000);
}

/*--------------------Private--------------------*/

String PolluSense::getSubstring(String str, String divider){
    int pos = str.indexOf(divider);
    return str.substring(0,pos);
}

String PolluSense::trimString(String str, String remove){
    int len = remove.length();
    return str.substring(len + 1);
}

long PolluSense::getEpoch(String host, String port, int timeout){
    int time = millis();
    bool connected = false;
    int position;
    String response;

    // If the ESP8266 can't connected to the daytime-server within
    // the timeout, the setup fails.
    response = wifiModule->openTCP(host, port);
    if(response == "")
        return 0;

    position = response.lastIndexOf("+IPD");
    response = response.substring(position + 8);

    String day = getSubstring(response, " ");
    String month = getSubstring(response = trimString(response, day), " ");
    String year = getSubstring(response = trimString(response, month), " ");
    String hour = getSubstring(response = trimString(response, year), ":");
    String minute = getSubstring(response = trimString(response, hour), ":");
    String second = getSubstring(trimString(response, minute), " ");

    return calcUnixTime(year.toInt(), month, day.toInt(), hour.toInt(), minute.toInt(), second.toInt());
}

long PolluSense::calcUnixTime(int year, String month, int day, int hour, int minute, int second){
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

    month.toUpperCase();
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


int PolluSense::getDays(String month) {
    int days = 0;
    if(month.startsWith("JAN"))
        return days;
    days += 31;
    if(month.startsWith("FEB"))
        return days;
    days += 28;
    if(month.startsWith("MAR"))
        return days;
    days += 31;
    if(month.startsWith("APR"))
        return days;
    days += 30;
    if(month.startsWith("MAY"))
        return days;
    days += 31;
    if(month.startsWith("JUN"))
        return days;
    days += 30;
    if(month.startsWith("JUL"))
        return days;
    days += 31;
    if(month.startsWith("AUG"))
        return days;
    days += 31;
    if(month.startsWith("SEP"))
        return days;
    days += 30;
    if(month.startsWith("OCT"))
        return days;
    days += 31;
    if(month.startsWith("NOV"))
        return days;
    days += 30;
    if(month.startsWith("DEC"))
        return days;
    return -1;
}