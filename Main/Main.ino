#include "src/libraries/ESP8266/ESP8266.h"
#include "src/libraries/CCS811/CCS811.h"

ESP8266 esp(2, 3);

String url = "ptsv2.com/t/e1oit-1651498913";
String test_ip = "216.239.34.21";
String daytime = "java.lab.ssvl.kth.se";
String ip = "192.168.43.44";
String data = "{\"VOC\": {\"value\": \"88888\",\"unit\": \"ppb\"},\"CO2\": {\"value\": \"450\",\"unit\": \"ppm\"}}";

void setup() {
    Serial.begin(9600);
    Serial.println("\n------------------------");

    esp.init();
    esp.connectToAP("Android Jakob", "leonboi11");
    esp.openTCP("pollusenseserver.azurewebsites.net", "80");
    esp.postData(data);
}

void loop() {

}
