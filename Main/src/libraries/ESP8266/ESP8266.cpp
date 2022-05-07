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

void ESP8266::closeTCP() {
    sendCmd("AT+CIPCLOSE");
}

int ESP8266::status() {
    String response = sendCmd("AT+CIPSTATUS");
    int start = response.lastIndexOf("\r\nSTATUS:") + 9;
    return response.substring(start, start + 1).toInt();
}

void ESP8266::sendData(String data) {
    String len = "";
    len += data.length();
    sendCmd("AT+CIPSEND=" + len);
    delay(500);
    espSerial->print(data);
    readResponse();
}

void ESP8266::postData(String time, String voc, String co2) {
    String data = "{\"TIME\": {\"value\"" + time + "\",\"unit\": \"sec\"},\"VOC\": {\"value\": \"" + voc + "\",\"unit\": \"ppb\"},\"CO2\": {\"value\": \"" + co2 + "\",\"unit\": \"ppm\"}}";
    String len = "";
    len += data.length();
    sendData("POST /data HTTP/1.1\r\nContent-Type: application/json\r\nAccept: */*\r\nHost: pollusenseserver.azurewebsites.net\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: " + len + "\r\n\r\n" + data);
}

/*
 * This function returns the approximate UNIX time, note that 
 * disregards leap seconds.
 */
long ESP8266::getEpoch(String host, String port){
    espSerial->println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + port);
    String response = readResponseDaytime(10000);
    int position = response.lastIndexOf("+IPD");
    response = response.substring(position + 8);

    String day = getSubstring(response, " ");
    String month = getSubstring(response = trimString(response, day), " ");
    String year = getSubstring(response = trimString(response, month), " ");
    String hour = getSubstring(response = trimString(response, year), ":");
    String minute = getSubstring(response = trimString(response, hour), ":");
    String second = getSubstring(response = trimString(response, minute), " ");

    return calcUnixTime(year, month, day, hour, minute, second);
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
            if(response.endsWith("CLOSED"))
                return response;
        }
    }
    return response;
}

String ESP8266::getSubstring(String str, String divider){
    int pos = str.indexOf(divider);
    return str.substring(0,pos);
}

String ESP8266::trimString(String str, String remove){
    int len = remove.length();
    return str.substring(len + 1);
}

long ESP8266::calcUnixTime(String year, String month, String day, String hour, String minute, String second){
    long unixTime = 0;
    month.toUpperCase();
    int days = getDays(month);
    days += day.toInt();

    unixTime = (year.toInt() - 1970) * 31556926;
    unixTime += days * 86400;
    unixTime += hour.toInt() * 3600;
    unixTime += minute.toInt() * 60;
    return unixTime + second.toInt();
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
}
