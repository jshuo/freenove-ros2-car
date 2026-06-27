#ifndef FREENOVE_CAR_CORE__ULTRASONIC_HPP_
#define FREENOVE_CAR_CORE__ULTRASONIC_HPP_

namespace freenove_car_core {

/// HC-SR04 ultrasonic distance sensor via sysfs GPIO
class Ultrasonic {
public:
  /// trigger_pin = GPIO27, echo_pin = GPIO22
  Ultrasonic(int trigger_pin = 27, int echo_pin = 22);
  ~Ultrasonic();

  /// Get distance in centimeters. Returns -1.0 on error.
  float getDistanceCm();

private:
  int trigger_pin_;
  int echo_pin_;

  void exportGpio(int pin);
  void unexportGpio(int pin);
  void setGpioDirection(int pin, const char* dir);
  void writeGpio(int pin, int value);
  int readGpio(int pin);
  void delayUs(int us);
};

}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__ULTRASONIC_HPP_
