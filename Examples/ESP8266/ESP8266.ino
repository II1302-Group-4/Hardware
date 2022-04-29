#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(2, 3);      // RX, TX for ESP8266

bool DEBUG = true;   //show more logs
int responseTime = 5000; //communication timeout

void setup()
{
  // Open serial communications and wait for port to open esp8266:
  Serial.begin(115200);
  wifiSerial.begin(115200);
  Serial.println("-----------------------------");
  delay(200);
  sendToWifi("AT+CWMODE=1",responseTime,DEBUG);
  delay(1000);
  sendToWifi("AT+CWJAP=\"Android Jakob\",\"leonboi11\"",responseTime,DEBUG);
  delay(1000);
  sendToWifi("AT+CIPSTART=\"TCP\",\"java.lab.ssvl.kth.se\",13",responseTime,DEBUG);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
