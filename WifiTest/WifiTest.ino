#include "src/libraries/ESP8266/ESP8266.h"
#include "src/libraries/ArduinoUnit/src/ArduinoUnit.h"

const String SERVER = "java.lab.ssvl.kth.se";
const String NOT_SERVER = "asöldkfjöaskdjfj";

// AP connection

test(connectToAP_successful)
{
    ESP8266 esp(2, 3);
    esp.basicInit();
    bool result = esp.connectToAP();
    assertEqual(result, true);
}

test (connectToAP_unsuccessful)
{
    ESP8266 esp(2, 3);
    bool result = esp.connectToAP();
    assertEqual(result, false);
}

// TCP

test (openTCP_valid_server)
{
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    bool result = esp.openTCP(SERVER, "13");
    assertEqual(result, true);
}

test (openTCP_invalid_server)
{
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    bool result = esp.openTCP(NOT_SERVER, "13");
    assertEqual(result, false);
}

// Status

test (status_no_connection) 
{
    ESP8266 esp(2, 3);
    esp.basicInit();
    int result = esp.status();
    assertEqual(result, 2);
}

test (status_no_TCP) 
{
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    int result = esp.status();
    assertEqual(result, 2);
}

test (status_connection_open) 
{
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    esp.openTCP(SERVER, "13");
    int result = esp.status();
    assertEqual(result, 4);
}

// Send data / read data

test (send_data_successful)
{
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    esp.openTCP(SERVER, "7");
    esp.openSendStream(3);
    esp.pushData("hej");
    String data = esp.readData();
    Serial.println(data);
    assertEqual(data, "hej");
}

// AP user init

test (init_AP)
{
    Serial.println("########### User init ###########");
    ESP8266 esp(2, 3);
    esp.init();
    esp.connectToAP();
    int status = esp.status();
    assertEqual(status, 2);
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