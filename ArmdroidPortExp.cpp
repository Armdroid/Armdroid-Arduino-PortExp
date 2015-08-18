#include "ArmdroidPortExp.h"
#include <Wire.h>

void ArmdroidPortExp::begin(uint8_t address)
{
  i2c_addr = address;

  Wire.begin();

  Wire.beginTransmission(i2c_addr);
  Wire.write(STROBE);
  Wire.endTransmission();
}

void ArmdroidPortExp::armdroid_write(uint8_t output)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(output);
  Wire.endTransmission();
}
