#include "src/libraries/ESP8266/ESP8266.h"
#include "src/libraries/CCS811/CCS811.h"

String WIFI_OHLSON = "Android Jakob";
String PWD_OHLSON = "leonboi11";
String WIFI_GOTBERG = "wifi_adefcade";
String PWD_GOTBERG = "therobotsaretakingourjobs";
String SERVER = "pollusenseserver.azurewebsites.net";
String SERVER_PORT = "80";
String DAYTIME_SERVER =  "java.lab.ssvl.kth.se";
String DAYTIME_SERVER_PORT = "13";
long unixTime = 0;

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
    esp.connectToAP(WIFI_GOTBERG, PWD_GOTBERG);

    // Get and calculate date
    unixTime = esp.getEpoch(DAYTIME_SERVER, DAYTIME_SERVER_PORT);

    Serial.println("\n---Setup completed---");
}

void loop() {
    // Get sensor values and increment timer
    unixTime += millis() / 1000; 
    ccs.fetchData();
    String voc = String(ccs.getVOC());
    String co2 = String(ccs.getCO2());

    // Make a post to the database
    switch (esp.status()) {
        default:
        case 1:
        case 5:
            esp.connectToAP(WIFI_GOTBERG, PWD_GOTBERG);
        case 2:
        case 4:
            esp.openTCP(SERVER, SERVER_PORT);
        case 3:
            esp.postData(String(unixTime), voc, co2);
    }
    

    //Wait <seconds> seconds
    int seconds = 10;
    Serial.println();
    for (int i = 0; i < seconds; i++) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
}
