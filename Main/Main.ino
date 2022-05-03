#include "ESP8266.h"

ESP8266 esp(2, 3, true);

void setup() {
    Serial.begin(9600);
    Serial.println("\n------------------------");
    esp.init();
    esp.test();
    esp.clientMode();
    esp.connect("Android Jakob", "leonboi11");
}

void loop() {

}
