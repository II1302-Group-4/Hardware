#include "ESP8266.h"

ESP8266 esp(2, 3);

String url = "ptsv2.com/t/e1oit-1651498913";
String test_ip = "216.239.34.21";
String daytime = "java.lab.ssvl.kth.se";

void setup() {
    Serial.begin(9600);
    Serial.println("\n------------------------");
    esp.init();
    esp.connectToAP("Android Jakob", "leonboi11");
    esp.openTCP("java.lab.ssvl.kth.se", "7");
    esp.sendData("hej hej");
}

void loop() {

}
