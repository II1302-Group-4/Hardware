#include "ESP8266.h"

ESP8266::ESP8266(int rx, int tx, bool debug) {
    espSerial = new SoftwareSerial(rx, tx);
    DEBUG = debug;
}

void ESP8266::init() {
    // Change baudrate of esp.
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    delay(1000);
    digitalWrite(13, HIGH);
    espSerial->begin(115200); // Default baud of esp is 115200.
    espSerial->print("AT+UART_CUR=9600,8,1,0,0\r\n");

    // Initialize software uart on same baud.
    espSerial->begin(9600);
    flushSerial();
}

void ESP8266::flushSerial() {
    espSerial->flush();
    while (espSerial->available())
        espSerial->read();
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
    sendCmd("AT+CIPSEND=" + data.length());
    delay(200);
    espSerial->print(data);
}

void ESP8266::sendCmd(String cmd) {
    espSerial->print(cmd + "\r\n");
    if (DEBUG) {
        Serial.print("sent: " + cmd + "\r\n");
        Serial.print("resp: " + readResponse(1000));
    }
}

String ESP8266::readResponse(const int timeout) {
    String response = "";
    long int time = millis();
    while ((time+timeout) > millis()) {
        while (espSerial->available()) {
            char c = espSerial->read(); // read the next character.
            response += c;
        }
    }
    return response;
}
