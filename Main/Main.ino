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
    // Get sensor values
    ccs.fetchData();
    String voc = String(ccs.getVOC());
    String co2 = String(ccs.getCO2());

    // Make a post to the database
    switch (esp.status()) {
        default:
        case 1:
        case 5:
            esp.connectToAP("Android Jakob", "leonboi11");
        case 2:
        case 4:
            esp.openTCP("pollusenseserver.azurewebsites.net", "80");
        case 3:
            esp.postData(voc, co2);
    }

    // Wait <seconds> seconds
    int seconds = 10;
    Serial.println();
    for (int i = 0; i < seconds; i++) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
}
