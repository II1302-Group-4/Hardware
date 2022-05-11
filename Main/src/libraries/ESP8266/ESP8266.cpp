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

String ESP8266::openTCP(String ip, String port) {
    String response = sendCmd("AT+CIPSTART=\"TCP\",\"" + ip + "\"," + port);
    flushESP();
    return response;
}

int ESP8266::closeTCP() {
    sendCmd("AT+CIPCLOSE");
    if(status() != 4)
        return 1;
    return 0;
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
                if (espSerial->available())
                    continue;
                else
                    return response;
            else if (response.substring(response.length() - 5).equals("+IPD,")) {
                delay(1000);
                flushESP();
                return response;
            }
            else if(response.endsWith("CLOSED"))
                return response;
        }
    }
    Serial.print("Timed out\n");
    return "";
}