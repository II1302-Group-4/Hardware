#include "ESP8266.h"

ESP8266::ESP8266(int rx, int tx, bool debug) {
    espSerial = new SoftwareSerial(rx, tx);
    DEBUG = debug;
}

void ESP8266::init() {
    espSerial->begin(9600);
    Serial.println("Initialization successful");
}

void ESP8266::test() {
    sendCmd("AT");
}

void ESP8266::clientMode() {
    sendCmd("AT+CWMODE=1");
}

void ESP8266::connect(String ssid, String pwd) {
    sendCmd("AT+CWJAP=\"" + ssid + "\",\"" + pwd + "\"");
}

void ESP8266::openTCP(String ip, String port) {
    sendCmd("AT+CIPSTART=\"TCP\",\"" + ip + "\"," + port);
}

void ESP8266::sendData(String data) {
    espSerial->print("AT+CIPSEND=" + data.length());
    delay(200);
    espSerial->print(data);
}

String ESP8266::pickupData() {
    return pickupData(10000);
}

String ESP8266::pickupData(const int timeout) {
    String data = "";
    long int time = millis();
    while ((time+timeout) > millis()) {
        while (espSerial->available()) {
            char c = espSerial->read(); // read the next character.
            data += c;
        }
    }
    return data;
}

void ESP8266::sendCmd(String cmd) {
    espSerial->print(cmd + "\r\n");
    if (DEBUG) {
        Serial.print(readResponse());
    }
}

String ESP8266::readResponse() {
    return readResponse(30000);
}

String ESP8266::readResponse(const int timeout) {
    String response = "";
    long int time = millis();
    while ((time+timeout) > millis()) {
        while (espSerial->available()) {
            char c = espSerial->read(); // read the next character.
            response += c;
            if (response.substring(response.length() - 6).equals("\r\nOK\r\n") ||
                response.substring(response.length() - 9).equals("\r\nERROR\r\n"))
                return response;
        }
    }
    return "Timed out";
}