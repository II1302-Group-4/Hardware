#include "ESP8266.h"

ESP8266 esp(2, 3, true);

String url = "ptsv2.com/t/e1oit-1651498913";
String test_ip = "216.239.34.21";
String daytime = "java.lab.ssvl.kth.se";

void setup() {
    Serial.begin(9600);
    Serial.println("\n------------------------");
    esp.init();
    esp.clientMode();
    esp.connect("Android Jakob", "leonboi11");
    esp.openTCP(test_ip,"80");
    //esp.httpPostTest("this is a text about birds.");
    esp.sendCmd("this is a text");
    esp.closeAll();
}

void loop() {

}
