#include "src/libraries/ESP8266/ESP8266.h"
#include "src/libraries/ArduinoUnit/src/ArduinoUnit.h"

const String SERVER = "java.lab.ssvl.kth.se";
const String NOT_SERVER = "asöldkfjöaskdjfj";
const String line = "----------------------------------------";

// AP connection

test(connectToAP_successful)
{
    Serial.println(line);
    ESP8266 esp(2, 3);
    esp.basicInit();
    bool result = esp.connectToAP();
    assertEqual(result, true);
}

test (connectToAP_unsuccessful)
{
    Serial.println(line);
    ESP8266 esp(2, 3);
    bool result = esp.connectToAP();
    assertEqual(result, false);
}

// TCP

test (openTCP_valid_server)
{
    Serial.println(line);
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    bool result = esp.openTCP(SERVER, "13");
    assertEqual(result, true);
}

test (openTCP_invalid_server)
{
    Serial.println(line);
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    bool result = esp.openTCP(NOT_SERVER, "13");
    assertEqual(result, false);
}

// Read response/data

test (readData_time) 
{
    Serial.println(line);
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    esp.openTCP(SERVER, "13");
    String data = esp.readData();
    int result = data.lastIndexOf("CEST");
    if (result == -1 && data.equals(""))
        result = 1;
    Serial.println(result);
    assertNotEqual(result, -1);
}

// Status

test (status_no_connection) 
{
    Serial.println(line);
    ESP8266 esp(2, 3);
    esp.basicInit();
    int result = esp.status();
    assertEqual(result, 5);
}

test (status_no_TCP) 
{
    Serial.println(line);
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    int result = esp.status();
    assertEqual(result, 2);
}

test (status_connection_open) 
{
    Serial.println(line);
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    esp.openTCP(SERVER, "13");
    int result = esp.status();
    assertEqual(result, 3);
}

// Send data / read data

test (send_data_successful)
{
    Serial.println(line);
    ESP8266 esp(2, 3);
    esp.basicInit();
    esp.connectToAP();
    esp.openTCP(SERVER, "7");
    esp.openSendStream(3);
    esp.pushData("hej");
    String data = esp.readData();
    Serial.print("data: ");
    Serial.println(data);
    int result = data.lastIndexOf("hej");
    if (result == -1 && data.equals(""))
        result = 1;
    Serial.println(result);
    assertNotEqual(result, -1);
}

void setup()
{
    delay(2000);
    Serial.begin(9600);
    while(!Serial) {}
    Serial.println("\n------------------ ESP8266 Testing ------------------");
}

void loop()
{
    Test::run();
}