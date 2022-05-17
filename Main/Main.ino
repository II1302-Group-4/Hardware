#include "src/libraries/PolluSense/PolluSense.h"

const String SERVER = "pollusenseserver.azurewebsites.net";
const String SERVER_PORT = "80";
const String DAYTIME_SERVER = "java.lab.ssvl.kth.se";
const String DAYTIME_SERVER_PORT = "13";
long unixTime = 0;
const int GREEN_LED = 8;
const int RED_LED = 9;

PolluSense pollu(2, 3, true);

void setup() {
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    Serial.begin(9600);
    Serial.println("\n------------------------");

    // Initialize sensor
    pollu.sensorModule->init();
    pollu.sensorModule->setReadInterval(pollu.sensorModule->INTERVAL_1SEC);
    
    // Initialize wifi communication
    pollu.wifiModule->init();
    unixTime = 0;
    while (unixTime == 0) {
        if (!pollu.wifiModule->connectToAP(pollu.wifiModule->ssid, pollu.wifiModule->pwd))
            continue;
        // Get and calculate date
        unixTime = pollu.getEpoch(DAYTIME_SERVER, DAYTIME_SERVER_PORT);
    }
    greenHighRedLow();
    
}

void loop() {
    // Get sensor values and increment timer
    unixTime += millis() / 1000; 
    pollu.sensorModule->fetchData();
    String voc = String(pollu.sensorModule->getVOC());
    String co2 = String(pollu.sensorModule->getCO2());

// Put switch-case into loop?
    // Make a post to the database
    switch (pollu.wifiModule->status()) {
        case 1:
        case 5:
            greenLowRedHigh();
            if (!pollu.wifiModule->connectToAP(pollu.wifiModule->ssid, pollu.wifiModule->pwd))
                break;
            greenHighRedLow();
        case 2:
        case 4:
            if(!pollu.wifiModule->openTCP(SERVER, SERVER_PORT))
                break;
        case 3:
            pollu.postData(String(unixTime), voc, co2);
    }

    wait(60);
}

void greenHighRedLow() {
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
}

void greenLowRedHigh() {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
}

void wait(int seconds) {
    //Wait <seconds> seconds
    int statusCheck = 5;
    Serial.println();
    for (int i = 0; i < seconds / statusCheck; i++) {
        for (int j = 0; j < statusCheck; j++) {
            delay(1000);
            Serial.print(".");
        }
        Serial.println();
        switch (pollu.wifiModule->status()) {
            case 1:
            case 5:
                greenLowRedHigh();
                continue;
            default:
                greenHighRedLow();
                continue;
        }
    }
    Serial.println();
}
