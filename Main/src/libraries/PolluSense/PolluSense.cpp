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
    return validateResponse(buffer);
}

long PolluSense::getEpoch(String host, String port) {
    return getEpoch(host, port, 3);
}
/*--------------------Private--------------------*/

/*
 * This function returns the unix time, setup to work in
 * GMT+2
 */

int PolluSense::getIndex(char buffer[], int pos, char divider){
    int i = pos;
    while(buffer[i] != divider)
        i++;
    i++;
    return i;
}

int PolluSense::bufferToInt(char buffer[], int start, int end){
    int len = end - start;
    int multiplier = 10;
    if(len > 3)
        multiplier = 1000;
    int res = 0;
    for(int i = start; i < end; i++)
    {
        res += (int) (buffer[i] - 48) * multiplier;
        multiplier = multiplier / 10;
    }
    return res;
}
void PolluSense::fillCharArray(char buffer[], char * res, int start, int end){
    res[0] = buffer[start];
    res[1] = buffer[start+1];
    res[2] = buffer[start+2];
}

long PolluSense::getEpoch(String host, String port, int timeout) {
    if(port == "13");
    int indexA = 0;
    int indexB = 0;
    wifiModule->openTCP(host, port);
    String response = wifiModule->readData();
    Serial.println("== getEpoch ==");
    Serial.println(response);
    int length = response.length();
    if(length == 0)
        return 0;
    char buffer[length];
    response.toCharArray(buffer, length);

    indexA = getIndex(buffer, 0, ' ');
    int day = bufferToInt(buffer, 0, indexA);

    indexB = getIndex(buffer, indexA, ' ');
    char month[3];
    fillCharArray(buffer, month, indexA, indexB);
    
    indexA = getIndex(buffer, indexB, ' ');
    int year = bufferToInt(buffer, indexB, indexA);

    indexB = getIndex(buffer, indexA, ':');
    int hour = bufferToInt(buffer, indexA, indexB -1);

    indexA = getIndex(buffer, indexB, ':');
    int minute = bufferToInt(buffer, indexB, indexA -1);

    indexB = getIndex(buffer, indexB, ' ');
    int second = bufferToInt(buffer, indexA, indexB);

    return calcUnixTime(year, month, day, hour, minute, second);
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
        return true;
    else
        return false;
}
