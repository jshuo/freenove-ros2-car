#ifndef FREENOVE_CAR_CORE__MOTOR_CONTROLLER_HPP_
#define FREENOVE_CAR_CORE__MOTOR_CONTROLLER_HPP_

#include "freenove_car_core/pca9685.hpp"

namespace freenove_car_core {

/// 4-wheel motor controller via PCA9685
/// PWM channel mapping (via PCA9685):
///   LF_F=0, LF_R=1, LB_F=2, LB_R=3
///   RB_F=4, RB_R=5, RF_F=6, RF_R=7
class MotorController {
public:
  explicit MotorController(PCA9685& pca);

  /// Set all 4 motor duties (-4095 to +4095). Positive = forward.
  void setMotor(int lf, int lb, int rf, int rb);

  /// Convenience: differential drive from linear/angular velocities
  void drive(float linear, float angular, float max_duty = 2048.0f);

  /// Stop all motors
  void stop();

private:
  PCA9685& pca_;
  static constexpr int kMaxDuty = 4095;

  int clamp(int duty) const;
};

}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__MOTOR_CONTROLLER_HPP_
