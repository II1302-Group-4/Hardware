#include "src/libraries/ESP8266/ESP8266.h"
#include "src/libraries/CCS811/CCS811.h"

ESP8266 esp(2, 3);
//CCS811 ccs(A2, A3);

String url = "ptsv2.com/t/e1oit-1651498913";
String test_ip = "216.239.34.21";
String daytime = "java.lab.ssvl.kth.se";
String ip = "192.168.43.44";

void setup() {
    Serial.begin(9600);
    Serial.println("\n------------------------");
//    ccs.init();
//    ccs.setReadInterval(ccs.INTERVAL_1SEC);
//    ccs.fetchData();
//    String co2 = "";
//    String voc = "";
//    co2 += ccs.getCO2();
//    voc += ccs.getVOC();

    esp.init();
    esp.connectToAP("Android Jakob", "leonboi11");
    esp.openTCP(ip, "8080");
    //esp.sendData("CO2: " + co2 + " VOC: " + voc);
}

void loop() {

}
