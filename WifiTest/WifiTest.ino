#include "src/libraries/ESP8266/ESP8266.h"
#include "src/libraries/ArduinoUnit/src/ArduinoUnit.h"

// When testing there must be an AP available with the 
// parameters in below variables.
const String WIFI = "Android Jakob";
const String PWD = "leonboi11";

const String NOT_WIFI = "öaffoiauhpaömph";
const String NOT_PWD = "alofppjk498";
const String SERVER = "java.lab.ssvl.kth.se";
const String NOT_SERVER = "asöldkfjöaskdjfj";

test(connectToAP_successful)
{
    ESP8266 esp(2, 3, false);
    esp.basicInit();
    bool result = esp.connectToAP(WIFI, PWD);
    assertEqual(result, true);
}

test (connectToAP_failed)
{
    ESP8266 esp(2, 3, false);
    esp.basicInit();
    bool result = esp.connectToAP(NOT_WIFI, NOT_PWD);
    assertEqual(result, false);
}

test (connectToAP_wrong_password)
{
    ESP8266 esp(2, 3, false);
    esp.basicInit();
    bool result = esp.connectToAP(WIFI, NOT_PWD);
    assertEqual(result, false);
}

test (connectToAP_empty_params)
{
    ESP8266 esp(2, 3, false);
    esp.basicInit();
    bool result = esp.connectToAP("", "");
    assertEqual(result, false);
}

test (openTCP_valid_server)
{
    ESP8266 esp(2, 3, false);
    esp.basicInit();
    esp.connectToAP(WIFI, PWD);
    bool result = esp.openTCP(SERVER, "13");
    assertEqual(result, true);
}

test (openTCP_invalid_server)
{
    ESP8266 esp(2, 3, false);
    esp.basicInit();
    esp.connectToAP(WIFI, PWD);
    bool result = esp.openTCP(NOT_SERVER, "13");
    assertEqual(result, true);
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
