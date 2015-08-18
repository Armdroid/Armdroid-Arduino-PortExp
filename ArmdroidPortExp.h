// A specialized Armdroid class using an external PCF8574A Port Expander
// interfacing to regular the 8-bit parallel Armdroid interface
//  Don't forget to #include <Wire.h> in your sketch!

#include "Armdroid.h"

class ArmdroidPortExp : public ArmBase {
 public:
  void begin(uint8_t address = 0x38);

 protected:
  void armdroid_write(uint8_t output);

 private:
  uint8_t i2c_addr;
};
