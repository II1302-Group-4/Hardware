#include "src/libraries/ESP8266/ESP8266.h"
#include "src/libraries/ArduinoUnit/src/ArduinoUnit.h"

const String WIFI_OHLSON = "Android Jakob";
const String PWD_OHLSON = "leonboi11";
const String SERVER = "pollusenseserver.azurewebsites.net";
const String SERVER_PORT = "80";
const String DAYTIME_SERVER = "java.lab.ssvl.kth.se";
const String DAYTIME_SERVER_PORT = "13";

test(openTCP)
{
    ESP8266 esp(2, 3, false);
    assertEqual(esp.)
}

void setup()
{
    delay(2000);
    Serial.begin(9600);
    while(!Serial) {}
    Serial.println("\n--- ESP8266 Testing ---");
}

void loop()
{
    Test::run();
}
