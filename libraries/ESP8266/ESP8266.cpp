#include "ESP8266.h"

ESP8266::ESP8266(int rx, int tx, int baud) {
    espSerial = new SoftwareSerial(rx, tx);
    setBaud(baud);
}

void ESP8266::test() {
    sendCmd("AT");
}

void ESP8266::init() {
    sendCmd("AT+RST");
    sendCmd("AT+CWMODE=1");
}

void ESP8266::connect(String ssid, String pwd) {
    sendCmd("AT+CWJAP=\"" + ssid + "\",\"" + pwd + "\"");
}

void ESP8266::openTCP(String ip, String port) {
    sendCmd("AT+CIPSTART=\"TCP\",\"" + ip + "\",\"" + port + "\"");
}

void ESP8266::sendData(String data) {
    sendCmd("AT+CIPSEND=" + data.length());
    delay(200);
    espSerial->print(data);
}

void ESP8266::sendCmd(String cmd) {
    espSerial->print(cmd + "\r\n");
}

String ESP8266::readResponse() {
    readResponse(1000);
    printIncoming();
}

String ESP8266::readResponse(int timeout) {
    String response;
    long int time = millis();
    while ((time+timeout) > millis()) {
        while (espSerial->peek() != 13)
            response = "" + espSerial->read();
    }
    return response;
}

void ESP8266::printIncoming() {
    delay(1000);
    String msg = "";
    Serial.println("--- Incoming data ---");
    while (espSerial->available()) {
        char c = espSerial->read();
        Serial.println(c);
        msg += c;
    }
    Serial.println(msg);
    Serial.println("--- end of data ---");
}

void ESP8266::setBaud(int baud) {
    int bauds [13] = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600, 115200};
    for (int b : bauds) {
        espSerial->begin(b);
        espSerial->print("AT+IPR=" + baud + "\r\n")
    }
    espSerial->begin(baud);
}