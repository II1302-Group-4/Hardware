#include "src/libraries/CCS811/CCS811.h"
#include "src/libraries/Softi2c/Softi2c.h"
#include "src/libraries/ArduinoUnit/src/ArduinoUnit.h"

//Start of I2C tests

//unit test
test(writeOneByte)
{
  Softi2c i2c(A2, A3);
  //uint8_t data[]= {0x11, 0x43, 0x22};
  i2c.write(0b00001111, 0b11110000, 0b10110010);
  assertEqual(i2c.symbolStream, "S00011110R11110000R10110010RE", i2c.symbolStream);
}

//unit test
test(readOneByte)
{
  Softi2c i2c(A2, A3);
  //uint8_t data[]= {0x11, 0x43, 0x22};
  i2c.read(0b00001111, 0b11110000);
  assertEqual(i2c.symbolStream, "S00011110R11110000RES00011111RRRRRRRRRNE", i2c.symbolStream);
}

//unit test
test(writeFourByte)
{
  Softi2c i2c(A2, A3);
  uint8_t data[]= {0b10101010, 0x11001100, 0x00001111, 0b00111100};
  i2c.write(0b00001111, 0b11101000, data, 4);
  assertEqual(i2c.symbolStream, "S00011110R11101000R10101010R00000000R00010001R00111100RE", i2c.symbolStream);
}

//unit test
test(writeNull)
{
  Softi2c i2c(A2, A3);
  i2c.write(0b00001111, 0b11101000, NULL, 0);
  assertEqual(i2c.symbolStream, "S00011110R11101000RE", i2c.symbolStream);
}

//unit test
test(readTwoByte)
{
  Softi2c i2c(A2, A3);
  uint8_t data[2];
  i2c.read(0b00001111, 0b11101000, data, 2);
  assertEqual(i2c.symbolStream, "S00011110R11101000RES00011111RRRRRRRRRARRRRRRRRNE", i2c.symbolStream);
}

//performance test
test(performance)
{
  long clockCycles = 9 * (3 + 20);
  long maxTime = clockCycles * 1000; //Min is 1KHz = 1000 microseconds per clock
  long minTime = clockCycles * 2.5;  //Max is 400KHz = 2.5 microseconds per clock
  
  Softi2c i2c(A2, A3);
  uint8_t data[20];
  long startTime = micros();
  i2c.read(0b00001111, 0b11101000, data, 2);
  long timet = micros() - startTime;

  assertEqual(timet > minTime && timet < maxTime, true, "Data transfer speed not in bound");
}

//Start of CCS811 tests

//Component test
test(sensorConectionEstablished)
{
  CCS811 ccs(A2, A3);
  assertEqual(ccs.init(), true, "Sensor faild to start");
}

//Component test
test(rejectIncorrectI2CSddress)
{
  CCS811 ccs(A2, A3);
  ccs.I2C_ADDRESS = 0x5B;
  assertEqual(ccs.init(), false, "Responded to incorrect i2c address");
}

//Component test
test(rejectIncorrectHardwareID)
{
  CCS811 ccs(A2, A3);
  ccs.HWID = 0x80;
  assertEqual(ccs.init(), false, "Accepted incorrect hardwareID");
}

//Component test
test(fetchData)
{
  CCS811 ccs(A2, A3);
  assertEqual(ccs.init(), true, "Sensor failed to start");
  ccs.fetchData();
  uint16_t voc = ccs.getVOC();
  uint16_t co2 = ccs.getCO2();
  assertEqual(voc, 0, "VOC not correct");
  assertEqual(co2, 400, "CO2 not correct");
}

void setup()
{
  delay(2000);
  Serial.begin(9600);
  while(!Serial) {}
}

void loop()
{
  Test::run();
}
