#include "PolluSense.h"

PolluSense::PolluSense(int rx, int tx, bool debug) {
    wifiModule = new ESP8266(rx, tx, debug);
    sensorModule = new CCS811(A2, A3);
}

/*--------------------Public--------------------*/

bool PolluSense::postData(String time, String voc, String co2) {
    String data = String("\r\n\r\n{\"time\": \"" + time + "\",\"VOC\": \"" + voc + "\",\"CO2\": \"" + co2 + "\"}");
    String dataLen = String(data.length() - 4);

    String response = wifiModule->sendData(HTTP_POST_HEADER + dataLen + data);

    char buffer[13];
    response.toCharArray(buffer, 13);

    if(validateResponse(buffer))
    {
        return true;
    }
    return false;
}

long PolluSense::getEpoch(String host, String port) {
    return getEpoch(host, port, 3);
}
/*--------------------Private--------------------*/

/*
 * This function returns the unix time, setup to work in
 * GMT+2
 */
long PolluSense::getEpoch(String host, String port, int timeout) {
    if(port != "13")
        return 0;
    int time = millis();
    wifiModule->openTCP(host, port);
    String response = wifiModule->readData();
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


String PolluSense::getSubstring(String str, String divider) {
    int pos = str.indexOf(divider);
    return str.substring(0,pos);
}

String PolluSense::trimString(String str, String remove) {
    int len = remove.length();
    return str.substring(len + 1);
}

long PolluSense::calcUnixTime(int year, char month[], int day, int hour, int minute, int second) {
    long unixTime = 0;
    int days_since_epoch = 0;

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

    days_since_epoch = (year - 1970) * 365;
    days_since_epoch += getLeapDays(year, days_in_year);
    days_since_epoch += getDays(month);
    days_since_epoch += day - 3;

    long hh = 0;
    for(int i = 0; i < hour; i++)
        hh += 3600;

    unixTime += days_since_epoch * 86400;
    unixTime += hh;
    unixTime += minute * 60;
    unixTime += second;
    
    // Remove two hours to account for timezone difference.
    unixTime -= 7200;
    return unixTime;
}

int PolluSense::getLeapDays(int year, int days){
    if(days > 59)
        year++;

    int leapdays = 0;
    for(int i = 1970; i < year; i += 4)
        leapdays++;
    return leapdays;
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

bool PolluSense::validateResponse(char buffer[]){

    if(buffer[9] == 50 && buffer[10] == 48 && buffer[11] == 49)
    {
        return true;
    }
    else
    {
        return false;
    }

}
