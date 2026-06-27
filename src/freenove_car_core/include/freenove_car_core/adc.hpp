#ifndef FREENOVE_CAR_CORE__ADC_HPP_
#define FREENOVE_CAR_CORE__ADC_HPP_

#include <cstdint>
#include <string>

namespace freenove_car_core {

/// ADS7830 8-bit ADC via Linux I2C
class ADC {
public:
  explicit ADC(const std::string& device = "/dev/i2c-1",
               uint8_t address = 0x48);
  ~ADC();

  /// Read ADC channel (0-7) and return voltage
  float readChannel(int channel, float v_ref = 3.3f);

  /// Read left photoresistor (channel 0)
  float readLightLeft(float v_ref = 3.3f);

  /// Read right photoresistor (channel 1)
  float readLightRight(float v_ref = 3.3f);

  /// Read battery voltage. PCB v1: ×3, v2: ×2
  float readBattery(int pcb_version = 1);

private:
  int fd_;
  uint8_t addr_;

  int readRaw(int channel);
};

}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__ADC_HPP_
