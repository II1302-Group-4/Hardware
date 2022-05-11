#include "ESP8266.h"

ESP8266::ESP8266(int rx, int tx) {
    espSerial = new SoftwareSerial(rx, tx);
}

/*--------------------Public--------------------*/

void ESP8266::init() {
    espSerial->begin(9600);
    sendCmd("AT");
    sendCmd("AT+CWMODE=1");
    flushESP();
    Serial.println("Initialization successful");
}

void ESP8266::connectToAP(String ssid, String pwd) {
    sendCmd("AT+CWJAP=\"" + ssid + "\",\"" + pwd + "\"");
    flushESP();
}

void ESP8266::openTCP(String ip, String port) {
    sendCmd("AT+CIPSTART=\"TCP\",\"" + ip + "\"," + port);
    flushESP();
}

int ESP8266::closeTCP()
{
    sendCmd("AT+CIPCLOSE");
    if(status() == 2)
        return 1;
    return 0;
}

int ESP8266::status() {
    String response = sendCmd("AT+CIPSTATUS");
    int start = response.lastIndexOf("\r\nSTATUS:") + 9;
    return response.substring(start, start + 1).toInt();
}

void ESP8266::sendData(String data) {
    Serial.println("DATA: ");
    Serial.print(data);
    Serial.println();
    String len = "";
    len += data.length();
    sendCmd("AT+CIPSEND=" + len);
    delay(500);
    espSerial->print(data);
    readResponse();
}
//
void ESP8266::postData(String time, String voc, String co2) {
    Serial.println("time: ");
    Serial.println(time);
    String data1 = "{\"time\": \"" + time + "\",\"VOC\": {\"value\": \"" + voc + "\",\"unit\": \"ppb\"},\"CO2\": {\"value\": \"" + co2 + "\",\"unit\": \"ppm\"}}";
    String data = "{\"time\": \"" + time + "\",\"VOC\": \"" + voc + "\",\"CO2\": \"" + co2 + "\"}";

    String header = "POST /data HTTP/1.1\r\nContent-Type: application/json\r\nAccept: */*\r\nHost: pollusenseserver.azurewebsites.net\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: ";
    String len = "";
    String msgLen = "";
    msgLen += data.length() + header.length() + 7;
    len += data.length();
    sendCmd("AT+CIPSEND=" + msgLen);
    delay(500);
    espSerial->print("POST /data HTTP/1.1\r\nContent-Type: application/json\r\nAccept: */*\r\nHost: pollusenseserver.azurewebsites.net\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: ");
    espSerial->print(len);
    espSerial->print("\r\n\r\n");
    espSerial->print(data);
    readResponse();



    Serial.print("\n-- MsgLenght: ");
    Serial.print(msgLen);
    Serial.println(" --");
    Serial.println("\n-- This is the MSG --");
    Serial.print("POST /data HTTP/1.1\r\nContent-Type: application/json\r\nAccept: */*\r\nHost: pollusenseserver.azurewebsites.net\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: ");
    Serial.print(len);
    Serial.print("\r\n\r\n");
    Serial.print(data);
}

/*
 * This function returns the approximate UNIX time, note that 
 * disregards leap seconds.
 */
long ESP8266::getEpoch(String host, String port){
    return getEpoch(host, port, 5000);
}

/*--------------------Private--------------------*/

void ESP8266::flushESP() {
    espSerial->flush();
    while (espSerial->available())
        espSerial->read();
}

String ESP8266::sendCmd(String cmd) {
    espSerial->println(cmd);
    return readResponse();
}

String ESP8266::readResponse() {
    return readResponse(30000);
}

String ESP8266::readResponse(const int timeout) {
    String response = "";
    long int time = millis();
    while ((time+timeout) > millis()) {
        while (espSerial->available()) {
            char c = espSerial->read();
            response += c;
            Serial.print(c);
            if (response.substring(response.length() - 6).equals("\r\nOK\r\n") ||
                response.substring(response.length() - 9).equals("\r\nERROR\r\n"))
                return response;
            else if (response.substring(response.length() - 5).equals("+IPD,")) {
                delay(1000);
                flushESP();
                return response;
            }
        }
    }
    return "Timed out\n";
}

String ESP8266::readResponseDaytime(const int timeout){
    String response = "";
    long int time = millis();
    while ((time+timeout) > millis())
    {
        while(espSerial->available())
        {
            char c = espSerial->read();
            response += c;
            Serial.print(c);
            if(response.endsWith("CLOSED"))
                return response;
        }
    }
    return "";
}

String ESP8266::getSubstring(String str, String divider){
    int pos = str.indexOf(divider);
    return str.substring(0,pos);
}

String ESP8266::trimString(String str, String remove){
    int len = remove.length();
    return str.substring(len + 1);
}

long ESP8266::getEpoch(String host, String port, int timeout){
    int time = millis();
    bool connected = false;
    int position;
    String response;

    // If the ESP8266 can't connected to the daytime-server within
    // the timeout, the setup fails.
    espSerial->println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + port);
    response = readResponseDaytime(timeout);
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

long ESP8266::calcUnixTime(int year, String month, int day, int hour, int minute, int second){
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
    if(days_in_year < 0)
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

}

int ESP8266::getDays(String month){
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
