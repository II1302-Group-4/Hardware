#include "ESP8266.h"

ESP8266::ESP8266(int rx, int tx, bool debug) {
    espSerial = new SoftwareSerial(rx, tx);
    DEBUG = debug;
}

/*--------------------Public--------------------*/

void ESP8266::init() {
    espSerial->begin(9600);

    sendCharCmd("AT+CIPSERVER=0");
    sendCharCmd("AT+CIPMUX=0");
    sendCharCmd("AT+CIPMODE=0");

    sendCharCmd("AT+CWMODE=3");
    sendCharCmd("AT+CIPMUX=1");
    sendCharCmd("AT+CIPSERVER=1,5000");
    sendCharCmd("AT+CIFSR");
    sendCharCmd("AT+CWMODE?");

    String msg = "";
    char c;
    // Ignore connect message
    while (!espSerial->available());
    while (espSerial->available()) {
        delay(10);
        espSerial->read();
    }
    if (DEBUG)
        Serial.println("Client connected!");
    // Read the actual message
    while (!espSerial->available());
    while (espSerial->available()) {
        delay(10);
        c = espSerial->read();
        msg += c;
    }

    if (DEBUG)
        Serial.println("Message from client:" + msg);
    // Break out parts of msg
    int ipdPos = msg.indexOf(':');
    msg = msg.substring(ipdPos + 1);
    int delimPos = msg.indexOf(',');
    ssid = msg.substring(0, delimPos);
    pwd = msg.substring(delimPos + 1);
    ssid.trim();
    pwd.trim();
    if (DEBUG)
        Serial.println("msg: " + ssid + pwd);
    sendCharCmd("AT+CIPSERVER=0");

    sendCharCmd("AT");
    sendCharCmd("AT+CWMODE=1");
    sendCharCmd("AT+CIPSERVER=0");
    sendCharCmd("AT+CIPMUX=0");
    flushESP();
}

bool ESP8266::connectToAP(String ssid, String pwd) {
    String response = sendCmd("AT+CWJAP=\"" + ssid + "\",\"" + pwd + "\"");
    if (response.endsWith("\r\nOK\r\n"))
        return true;
    else
        return false;
    flushESP();
}

bool ESP8266::openTCP(String ip, String port) {
    String response = sendCmd("AT+CIPSTART=\"TCP\",\"" + ip + "\"," + port);

    if (response.endsWith("\r\nOK\r\n"))
        return true;
    else
        return false;
    flushESP();
}

int ESP8266::closeTCP() {
    sendCharCmd("AT+CIPCLOSE");
    if(status() != 4)
        return 1;
    return 0;
}

int ESP8266::status() {
    String response = sendCmd("AT+CIPSTATUS");
    int start = response.lastIndexOf("\r\nSTATUS:") + 9;
    return response.substring(start, start + 1).toInt();
}

void ESP8266::sendData(String len) {
    sendCmd("AT+CIPSEND=" + len);
    delay(500);
}

void ESP8266::pushData(String data) {
    if (DEBUG)
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
            if (DEBUG)
                Serial.print(c);
            if (data.endsWith("+IPD,")) {
                String slen = "";
                delay(100);
                while (espSerial->available()) {
                    c = espSerial->read();
                    if (DEBUG)
                        Serial.print(c);
                    if (c == ':')
                        break;
                    slen += c;
                }
                data = "";
                int len = slen.toInt();
                for (int i = 0; i < len; i++) {
                    delay(1);
                    if (!espSerial->available())
                        return data;
                    c = espSerial->read();
                    data += c;
                    if (DEBUG)
                        Serial.print(c);
                }
                return data;
            }
        }
    }
    if (DEBUG)
        Serial.print("Timed out\n");
    return "";
}

/*--------------------Private--------------------*/

void ESP8266::flushESP() {
    espSerial->flush();
    while (espSerial->available())
        espSerial->read();
}

String ESP8266::sendCmd(String cmd) {
    espSerial->println(cmd);
    String response = readResponse();
    if (DEBUG)
        Serial.println("----------");
    return response;
}

void ESP8266::sendCharCmd(const char *c) {
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
            if (DEBUG)
                Serial.print(c);
            if (msg.endsWith("\r\nOK\r\n") || msg.endsWith("\r\nERROR\r\n")) {
                return msg;
            }
        }
    }
    if (DEBUG)
        Serial.print("Timed out\n");
    return "";
}
