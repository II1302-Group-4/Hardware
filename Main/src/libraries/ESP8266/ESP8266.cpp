#include "ESP8266.h"

ESP8266::ESP8266(int rx, int tx, bool debug) {
    espSerial = new SoftwareSerial(rx, tx);
    DEBUG = debug;
}

/*--------------------Public--------------------*/

/*
 * Initiates the ESP8266 without needing ssid and password for
 * outside source. 
 * Needs hard-coded ssid and password.
 */
void ESP8266::basicInit() {
    espSerial->begin(9600);
    sendCmd("AT");
    sendCmd("AT+CWMODE=1");
    sendCmd("AT+CIPSERVER=0");
    sendCmd("AT+CIPMUX=0");
}

void ESP8266::init() {
    espSerial->begin(9600);

    sendCmd("AT+RST");
    delay(1000);

    sendCmd("AT+CIPSERVER=0");
    sendCmd("AT+CIPMUX=0");
    sendCmd("AT+CIPMUX?");
    sendCmd("AT+CIPMODE=0");

    sendCmd("AT+CWMODE=3");
    sendCmd("AT+CIPMUX=1");
    sendCmd("AT+CIPSERVER=1,5000");
    sendCmd("AT+CIFSR");
    sendCmd("AT+CWMODE?");

    String msg;
    readData(msg, 900000);

    if (DEBUG)
        Serial.println("\nMessage from client:" + msg);
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
    sendCmd("AT+CIPSERVER=0");

    sendCmd("AT");
    sendCmd("AT+CWMODE=1");
    sendCmd("AT+CIPSERVER=0");
    sendCmd("AT+CIPMUX=0");
}

bool ESP8266::connectToAP() {
    espSerial->print("AT+CWJAP=\"");
    espSerial->print(ssid);
    espSerial->print("\",\"");
    espSerial->print(pwd);
    espSerial->println("\"");
    bool response = readResponse();
    return response;
}

bool ESP8266::openTCP(String ip, String port) {
    espSerial->print("AT+CIPSTART=\"TCP\",\"");
    espSerial->print(ip);
    espSerial->print("\",");
    espSerial->println(port);
    bool result = readResponse();
    return result;
}

int ESP8266::closeTCP() {
    sendCmd("AT+CIPCLOSE");
    if(status() != 4)
        return 1;
    return 0;
}

int ESP8266::status() {
    /*espSerial->println("AT+CIPSTATUS");
    char buffer[300];
    readResponse()
    char stat[] = "\r\nSTATUS:";
    int spos = 0, pos = 0;
    while (true) {
        if (c == stat[spos++]) 
            if (stat[spos] == '\0')
                break;
        else
            spos = 0;
        pos++;
        if (buffer[pos] == '\0')
            break;
    }
    return response.substring(pos, pos + 1).toInt();*/
    return 1;
}

bool ESP8266::sendData(String &data, String &res) {
    espSerial->print("AT+CIPSEND=");
    espSerial->println(data.length());
    delay(500);
    espSerial->print(data);
    return readData(res);
}

bool ESP8266::readData(String &res) {
    return readData(res, 15000);
}

bool ESP8266::readData(String &res, const int timeout) {
    res = "";
    char c;
    int time = millis();
    while ((time+timeout) > millis()) {
        while (espSerial->available()) {
            c = espSerial->read();
            res += c;
            if (DEBUG)
                Serial.print(c);
            if (res.endsWith("+IPD,")) {
                String slen = "";
                delay(100);
                while (espSerial->available()) {
                    c = espSerial->read();
                    if (DEBUG)
                        Serial.print(c);
                    if (c == ':')
                        break;
                    slen += c;
                    if (c == ',')
                        slen = "";
                }
                res = "";
                int len = slen.toInt();
                for (int i = 0; i < len; i++) {
                    delay(1);
                    if (!espSerial->available())
                        return true;
                    c = espSerial->read();
                    res += c;
                    if (DEBUG)
                        Serial.print(c);
                    Serial.println(".");
                }
                return true;
            }
        }
    }
    if (DEBUG)
        Serial.println(res);
        Serial.print("Timed out\n");
    return false;
}

/*--------------------Private--------------------*/

void ESP8266::flushESP() {
    espSerial->flush();
    while (espSerial->available()) {
        delay(1);
        espSerial->read();
    }
}

bool ESP8266::sendCmd(const char* c) {
    espSerial->println(c);
    const short buffSize = 300;
    char buffer[buffSize];
    return readResponse(buffer, buffSize);
}

bool ESP8266::readResponse(const int timeout) {
    const short buffSize = 300;
    char buffer[buffSize];
    return readResponse(buffer, buffSize);
}

bool ESP8266::readResponse(char* buffer, const short buffSize, const int timeout) {
    short pos = 0;
    char c;
    long int time = millis();
    while ((time+timeout) > millis()) {
        for (; espSerial->available(); pos++) {
            c = espSerial->read();
            if (pos < buffSize - 1)
                buffer[pos] = c;
            else {
                flushESP();
                Serial.print("\nResponse Overflow! pos = ");
                Serial.println(pos);
                buffer[pos] = '\0';
                return false;
            }
            if (DEBUG)
                Serial.print(c);
            if (pos >= 5 && buffer[pos-5]=='\r' && buffer[pos-4]=='\n' && buffer[pos-3]=='O' &&
                buffer[pos-2]=='K' && buffer[pos-1]=='\r' && buffer[pos]=='\n')
            {
                buffer[++pos] = '\0';
                return true;
            }
            if (pos >= 8 && buffer[pos-8]=='\r' && buffer[pos-7]=='\n' && buffer[pos-6]=='E' &&
                buffer[pos-5]=='R' && buffer[pos-4]=='R' && buffer[pos-3]=='O' &&
                buffer[pos-2]=='R' && buffer[pos-1]=='\r' && buffer[pos]=='\n')
            {
                buffer[++pos] = '\0';
                return false;
            }
        }
    }
    if (DEBUG) 
        Serial.print("Timed out\n");
    buffer[pos] = '\0';
    return false;
}