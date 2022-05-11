#include "src/libraries/CCS811/CCS811.h"
#include "src/libraries/Softi2c/Softi2c.h"
#include "src/libraries/ArduinoUnit/src/ArduinoUnit.h"


test(writeOneByte)
{
  Softi2c i2c(A2, A3);
  //uint8_t data[]= {0x11, 0x43, 0x22};
  i2c.write(0b00001111, 0b11110000, 0b10110010);
  assertEqual(i2c.symbolStream, "S00011110R11110000R10110010RE", i2c.symbolStream);
}

test(readOneByte)
{
  Softi2c i2c(A2, A3);
  //uint8_t data[]= {0x11, 0x43, 0x22};
  i2c.read(0b00001111, 0b11110000);
  assertEqual(i2c.symbolStream, "S00011110R11110000RES00011111RRRRRRRRRNE", i2c.symbolStream);
}

test(writeFourByte)
{
  Softi2c i2c(A2, A3);
  uint8_t data[]= {0b10101010, 0x11001100, 0x00001111, 0b00111100};
  i2c.write(0b00001111, 0b11101000, data, 4);
  assertEqual(i2c.symbolStream, "S00011110R11101000R10101010R00000000R00010001R00111100RE", i2c.symbolStream);
}

test(readTwoByte)
{
  Softi2c i2c(A2, A3);
  uint8_t data[2];
  i2c.read(0b00001111, 0b11101000, data, 2);
  assertEqual(i2c.symbolStream, "S00011110R11101000RES00011111RRRRRRRRRARRRRRRRRNE", i2c.symbolStream);
}

test(performance)
{
  long clockCycles = 9 * (3 + 20);
  long maxTime = clockCycles * 1000; //Min is 1KHz
  long minTime = clockCycles * 2.5;  //Max is 400KHz
  
  Softi2c i2c(A2, A3);
  uint8_t data[20];
  long startTime = micros();
  i2c.read(0b00001111, 0b11101000, data, 2);
  long timet = micros() - startTime;

  assertEqual(timet > minTime && timet < maxTime, true, timet);
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
