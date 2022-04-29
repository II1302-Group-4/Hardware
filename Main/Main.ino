#include "CCS811.h"
#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(2, 3);      // RX, TX for ESP8266
CCS811 ccs(A2, A3);

bool DEBUG = true;   //show more logs
int responseTime = 5000; //communication timeout

void setup() {
  Serial.begin(115200);
  wifiSerial.begin(115200);
  
  ccs.init();
  ccs.setReadInterval(ccs.INTERVAL_1SEC);

  Serial.println("-----------------------------");
  delay(200);
  sendToWifi("AT+CWMODE=1",responseTime,DEBUG);
  delay(1000);
  sendToWifi("AT+CWJAP=\"Android Jakob\",\"leonboi11\"",responseTime,DEBUG);
  delay(1000);
  sendToWifi("AT+CIPSTART=\"TCP\",\"java.lab.ssvl.kth.se\",7",responseTime,DEBUG);
  delay(1000);
  ccs.fetchData();
  String msg = "CO2: ";
  msg = msg + String(ccs.getCO2()) + "\0";
  sendData(msg);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //ccs.fetchData();

  //Serial.print(ccs.getCO2());


}
