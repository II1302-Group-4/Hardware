#include "ESP8266.h"

ESP8266 esp(2, 3, true);

void setup() {
    Serial.begin(9600);
    Serial.println("\n------------------------");
    esp.init();
    esp.clientMode();
    esp.connect("Android Jakob", "leonboi11");
    esp.openTCP("google.se", "80");
    esp.sendData("GET / HTTP/1.1\r\n\r\n");
    Serial.print(esp.pickupData());
}

void loop() {

}
