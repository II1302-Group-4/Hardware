#include "src/libraries/ESP8266/ESP8266.h"
#include "src/libraries/CCS811/CCS811.h"

ESP8266 esp(2, 3);
CCS811 ccs(A2, A3);

void setup() {
    Serial.begin(9600);
    Serial.println("\n------------------------");

    // Initialize sensor
    ccs.init();
    ccs.setReadInterval(ccs.INTERVAL_1SEC);

    // Initialize wifi communication
    esp.init();
    esp.connectToAP("Android Jakob", "leonboi11");

    Serial.println("\n---Setup completed---");
    
}

void loop() {
    ccs.fetchData();
    String voc = "";
    String co2 = "";
    voc += ccs.getVOC();
    co2 += ccs.getCO2();

    esp.openTCP("pollusenseserver.azurewebsites.net", "80");
    esp.postData(voc, co2);
    esp.closeTCP();

    Serial.println();
    for (int i = 0; i < 10; i++) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
}
