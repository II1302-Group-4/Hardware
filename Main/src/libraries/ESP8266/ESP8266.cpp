#include "ESP8266.h"

ESP8266::ESP8266(int rx, int tx) {
    espSerial = new SoftwareSerial(rx, tx);
}

/*--------------------Public--------------------*/

void ESP8266::init() {
    espSerial->begin(9600);
    sendCmd("AT");
    //sendCmd("AT+CIPMUX=0");
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

void ESP8266::closeTCP(){
    sendCmd("AT+CIPCLOSE");
}

void ESP8266::sendData(String data) {
    String len = "";
    len += data.length();
    sendCmd("AT+CIPSEND=" + len);
    delay(500);
    espSerial->print(data);
    readResponse();
    
}
void ESP8266::httpPost(String path, String host, String data){
    String len = "";
    len += data.length();
    sendCmd("POST " + path + " HTTP/1.1\r\n" +
            "Host: " + host + "\r\n" +
            //"Content-Type: application/json\r\n" +
            "Content-Length: " + len + "\r\n\r\n" +
            //"Connection: Close" + "\r\n\r\n" +
            data);
}

/*--------------------Private--------------------*/

void ESP8266::flushESP() {
    espSerial->flush();
    while (espSerial->available())
        espSerial->read();
}

void ESP8266::sendCmd(String cmd) {
    espSerial->println(cmd);
    readResponse();
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
        }
    }
    return "Timed out\n";
}
