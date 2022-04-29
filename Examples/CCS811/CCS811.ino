#include "CCS811.h"

CCS811 ccs(A2, A3);

void setup() {
  Serial.begin(115200);
  
  ccs.init();
  ccs.setReadInterval(ccs.INTERVAL_1SEC);
}

void loop() {
  // put your main code here, to run repeatedly:

  ccs.fetchData();

  Serial.print("CO2 : ");
  Serial.print(ccs.getCO2());

  Serial.print(" VOC : ");
  Serial.println(ccs.getVOC());

}
