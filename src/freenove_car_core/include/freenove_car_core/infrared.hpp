#ifndef FREENOVE_CAR_CORE__INFRARED_HPP_
#define FREENOVE_CAR_CORE__INFRARED_HPP_

#include <tuple>

namespace freenove_car_core {

/// 3-channel infrared line sensor via sysfs GPIO
/// CH1=GPIO14, CH2=GPIO15, CH3=GPIO23
class Infrared {
public:
  Infrared(int pin_left = 14, int pin_center = 15, int pin_right = 23);
  ~Infrared();

  /// Read all 3 sensors. Returns (left, center, right) as bools.
  std::tuple<bool, bool, bool> readAll();

  /// Read 3-bit encoded value: 7=all, 0=none
  int readEncoded();

private:
  int pins_[3];
  bool exported_[3];

  void exportGpio(int pin, int idx);
  void unexportGpio(int pin, int idx);
  int readGpio(int pin);
};

}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__INFRARED_HPP_
