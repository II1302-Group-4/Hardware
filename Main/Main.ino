#include "src/libraries/ESP8266/ESP8266.h"
#include "src/libraries/CCS811/CCS811.h"

const String WIFI_OHLSON = "Android Jakob";
const String PWD_OHLSON = "leonboi11";
const String WIFI_GOTBERG = "wifi_adefcade";
const String PWD_GOTBERG = "therobotsaretakingourjobs";
const String SERVER = "pollusenseserver.azurewebsites.net";
const String SERVER_PORT = "80";
const String DAYTIME_SERVER =  "java.lab.ssvl.kth.se";
const String DAYTIME_SERVER_PORT = "13";
long unixTime = 0;
const int GREEN_LED = 8;
const int RED_LED = 9;

ESP8266 esp(2, 3);
CCS811 ccs(A2, A3);

void greenHighRedLow(){
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
}
void greenLowRedHigh(){
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
}

void setup() {
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
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
    if(unixTime == 0)
    {
        Serial.println("\n---SETUP FAILED---");
        delay(1000);
        exit(1);
        greenHighRedLow();
    }
    Serial.println("\n---Setup completed---");
    greenLowRedHigh();
    
}

void loop() {
    // Get sensor values and increment timer
    unixTime += millis() / 1000; 
    ccs.fetchData();
    String voc = String(ccs.getVOC());
    String co2 = String(ccs.getCO2());

    Serial.println("\n--TIME--");
    Serial.print(unixTime);
    Serial.println("\n--TIME--");

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
    if(esp.status() == 2)
          greenHighRedLow();
      else if(esp.status() == 1 || esp.status() == 0)
          greenLowRedHigh();
 

    //Wait <seconds> seconds
    int seconds = 10;
    Serial.println();
    for (int i = 0; i < seconds; i++) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
}
