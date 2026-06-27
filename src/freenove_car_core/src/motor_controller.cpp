#include "freenove_car_core/motor_controller.hpp"

#include <algorithm>

namespace freenove_car_core {

// PWM channel indices
enum MotorCh : int {
  LF_F = 0, LF_R = 1,
  LB_F = 2, LB_R = 3,
  RB_F = 4, RB_R = 5,
  RF_F = 6, RF_R = 7,
};

MotorController::MotorController(PCA9685& pca) : pca_(pca) {}

int MotorController::clamp(int duty) const {
  return std::max(-kMaxDuty, std::min(kMaxDuty, duty));
}

void MotorController::setMotor(int lf, int lb, int rf, int rb) {
  int duties[4] = {clamp(lf), clamp(lb), clamp(rf), clamp(rb)};
  int fwd_ch[4] = {LF_F, LB_F, RF_F, RB_F};
  int rev_ch[4] = {LF_R, LB_R, RF_R, RB_R};

  for (int i = 0; i < 4; ++i) {
    int d = duties[i];
    if (d >= 0) {
      pca_.setMotorPwm(fwd_ch[i], d);
      pca_.setMotorPwm(rev_ch[i], 0);
    } else {
      pca_.setMotorPwm(fwd_ch[i], 0);
      pca_.setMotorPwm(rev_ch[i], -d);
    }
  }
}

void MotorController::drive(float linear, float angular, float max_duty) {
  float left = (linear - angular) * max_duty;
  float right = (linear + angular) * max_duty;
  setMotor(static_cast<int>(left), static_cast<int>(left),
           static_cast<int>(right), static_cast<int>(right));
}

void MotorController::stop() {
  setMotor(0, 0, 0, 0);
}

}  // namespace freenove_car_core
