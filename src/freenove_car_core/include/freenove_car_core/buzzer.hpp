#ifndef FREENOVE_CAR_CORE__BUZZER_HPP_
#define FREENOVE_CAR_CORE__BUZZER_HPP_

namespace freenove_car_core {

/// Active buzzer via sysfs GPIO
class Buzzer {
public:
  explicit Buzzer(int pin = 17);
  ~Buzzer();

  void on();
  void off();
  void set(bool state);

private:
  int pin_;
  void exportGpio();
  void unexportGpio();
  void writeGpio(int value);
};

}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__BUZZER_HPP_
