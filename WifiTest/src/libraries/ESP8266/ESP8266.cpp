#include "ESP8266.h"

ESP8266::ESP8266(int rx, int tx) {
    espSerial = new SoftwareSerial(rx, tx);
}

/*--------------------Public--------------------*/

/*
 * Initiates the ESP8266 without needing ssid and password for
 * outside source. 
 * Needs hard-coded ssid and password.
 */
void ESP8266::basicInit() {
    espSerial->begin(9600);
    ssid = "Android Jakob";
    pwd = "leonboi11";
    sendCmd("AT");
    sendCmd("AT+CWMODE=1");
    sendCmd("AT+CIPSERVER=0");
    sendCmd("AT+CIPMUX=0");
    flushESP();
}

void ESP8266::init() {
    espSerial->begin(9600);
    sendCmd("AT+RST");
    delay(1000);
    flushESP();
    sendCmd("AT+CIPSERVER=0");
    sendCmd("AT+CIPMUX=0");
    sendCmd("AT+CIPMODE=0");
    sendCmd("AT+CWMODE=3");
    sendCmd("AT+CIPMUX=1");
    sendCmd("AT+CIPSERVER=1,5000");
    sendCmd("AT+CIFSR");
    sendCmd("AT+CWMODE?");
    String msg = readData(900000);
    Serial.println("\nMessage from client:" + msg);
    // Break out parts of msg
    int ipdPos = msg.indexOf(':');
    msg = msg.substring(ipdPos + 1);
    int delimPos = msg.indexOf(',');
    ssid = msg.substring(0, delimPos);
    pwd = msg.substring(delimPos + 1);
    ssid.trim();
    pwd.trim();
    Serial.println("msg: " + ssid + pwd);
    sendCmd("AT+CIPSERVER=0");
    sendCmd("AT");
    sendCmd("AT+CWMODE=1");
    sendCmd("AT+CIPSERVER=0");
    sendCmd("AT+CIPMUX=0");
    flushESP();
}

bool ESP8266::connectToAP() {
    espSerial->print("AT+CWJAP=\"");
    espSerial->print(ssid);
    espSerial->print("\",\"");
    espSerial->print(pwd);
    espSerial->println("\"");
    String response = readResponse();
    if (response.endsWith("\r\nOK\r\n"))
        return true;
    else
        return false;
    flushESP();
}


bool ESP8266::openTCP(const String& ip, const String& port) {
    espSerial->print("AT+CIPSTART=\"TCP\",\"");
    espSerial->print(ip);
    espSerial->print("\",");
    espSerial->println(port);
    String response = readResponse();
    if (response.endsWith("\r\nOK\r\n"))
        return true;
    else
        return false;
    flushESP();
}

void ESP8266::closeTCP() {
    sendCmd("AT+CIPCLOSE");
    flushESP();
}

int ESP8266::status() {
    espSerial->println("AT+CIPSTATUS");
    String response = readResponse();
    int start = response.lastIndexOf("\r\nSTATUS:") + 9;
    return response.substring(start, start + 1).toInt();
}

void ESP8266::openSendStream(const int& len) {
    espSerial->print("AT+CIPSEND=");
    espSerial->println(len);
    readResponse();
    delay(500);
}

void ESP8266::pushData(const String& data) {
    Serial.print(data);
    espSerial->print(data);
}

String ESP8266::readData() {
    return readData(15000);
}

String ESP8266::readData(const int timeout) {
    String data = "";
    char c;
    int time = millis();
    while ((time+timeout) > millis()) {
        while (espSerial->available()) {
            c = espSerial->read();
            data += c;
            Serial.print(c);
            if (data.endsWith("+IPD,")) {
                String slen = "";
                delay(100);
                while (espSerial->available()) {
                    c = espSerial->read();
                    Serial.print(c);
                    if (c == ':')
                        break;
                    slen += c;
                    if (c == ',')
                        slen = "";
                }
                data = "";
                int len = slen.toInt();
                for (int i = 0; i < len; i++) {
                    delay(1);
                    if (!espSerial->available())
                        return data;
                    c = espSerial->read();
                    data += c;
                    Serial.print(c);
                }
                return data;
            }
        }
    }
    Serial.print("Timed out\n");
    return "";
}

/*--------------------Private--------------------*/

void ESP8266::flushESP() {
    espSerial->flush();
    while (espSerial->available())
        espSerial->read();
}

void ESP8266::sendCmd(const char* c) {
    espSerial->println(c);
    readResponse();
}

String ESP8266::readResponse() {
    return readResponse(15000);
}

String ESP8266::readResponse(const int timeout) {
    String msg = "";
    char c;
    long int time = millis();
    while ((time+timeout) > millis()) {
        while (espSerial->available()) {
            c = espSerial->read();
            msg += c;
            Serial.print(c);
            if (msg.endsWith("\r\nOK\r\n") || msg.endsWith("\r\nERROR\r\n")) {
                return msg;
            }
        }
    }
    Serial.print("Timed out\n");
    return "";
}
