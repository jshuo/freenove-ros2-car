#ifndef FREENOVE_CAR_CORE__SERVO_CONTROLLER_HPP_
#define FREENOVE_CAR_CORE__SERVO_CONTROLLER_HPP_

#include <array>
#include "freenove_car_core/pca9685.hpp"

namespace freenove_car_core {

/// Controls up to 8 servos on PCA9685 channels 8-15
class ServoController {
public:
  explicit ServoController(PCA9685& pca);

  /// Set servo angle (0-180°)
  void setAngle(int channel, int angle, int error = 10);

  /// Get current angle of a channel
  int getAngle(int channel) const;

  /// Center all servos at 90°
  void centerAll();

private:
  PCA9685& pca_;
  std::array<int, 8> current_angles_;
  static constexpr int kServoChannelOffset = 8;
};

}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__SERVO_CONTROLLER_HPP_
