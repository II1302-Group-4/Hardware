#include "ESP8266.h"

ESP8266::ESP8266(int rx, int tx) {
    espSerial = new SoftwareSerial(rx, tx);
}

/*--------------------Public--------------------*/

void ESP8266::init() {
    espSerial->begin(9600);
    sendCmd("AT");
    sendCmd("AT+CWMODE=1");
    sendCmd("AT+CIPMUX=0");
    flushESP();
    Serial.println("Initialization successful");
}

void ESP8266::connectToAP(String ssid, String pwd) {
    sendCmd("AT+CWJAP=\"" + ssid + "\",\"" + pwd + "\"");
    flushESP();
}

Response ESP8266::openTCP(String ip, String port) {
    Response response = sendCmd("AT+CIPSTART=\"TCP\",\"" + ip + "\"," + port, true);
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
    String response = sendCmd("AT+CIPSTATUS").msg;
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

Response ESP8266::sendCmd(String cmd) {
    return sendCmd(cmd, false);
}

Response ESP8266::sendCmd(String cmd, bool dataExpected) {
    espSerial->println(cmd);
    Response response = { "", "" };
    response.msg = readResponse();
    if (dataExpected) {
        response.data = readData();
    }
    return response;
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

String ESP8266::readResponseChar(const int timeout)
{
    char buffer[100]; 
    char c;
    int i = 0;
    long int time = millis();
    while ((time+timeout) > millis()) {
        while (espSerial->available()) {
            buffer[i++] = espSerial->read();
            Serial.print(c);
            if(buffer[i-3] == 79 && buffer[i-2] == 75 && buffer[i-1] == 13 && buffer[10] == 10)
                return String(buffer); 
        }
    }
    Serial.print("Timed out\n");
    return "";
}


String ESP8266::readData() {
    return readData(15000);
}

String ESP8266::readData(const int timeout) {
    String data = "";
    char c;
    long int time = millis();
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
                    if (c == ':')
                        break;
                    slen += c;
                }
                data = "";
                int len = slen.toInt();
                char c;
                for (int i = 0; i < len; i++) {
                    while (!espSerial->available());
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
