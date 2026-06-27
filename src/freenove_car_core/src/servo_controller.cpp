#include "freenove_car_core/servo_controller.hpp"

#include <algorithm>

namespace freenove_car_core {

ServoController::ServoController(PCA9685& pca)
    : pca_(pca) {
  current_angles_.fill(90);
}

void ServoController::setAngle(int channel, int angle, int error) {
  channel = std::max(0, std::min(7, channel));
  angle = std::max(0, std::min(180, angle));

  int pca_ch = channel + kServoChannelOffset;
  double pulse;

  if (channel == 0) {
    // Channel 0 uses inverted mapping
    pulse = 2500.0 - (angle + error) / 0.09;
  } else {
    pulse = 500.0 + (angle + error) / 0.09;
  }

  pulse = std::max(500.0, std::min(2500.0, pulse));
  pca_.setServoPulse(pca_ch, static_cast<int>(pulse));
  current_angles_[channel] = angle;
}

int ServoController::getAngle(int channel) const {
  if (channel < 0 || channel >= 8) return -1;
  return current_angles_[channel];
}

void ServoController::centerAll() {
  for (int ch = 0; ch < 8; ++ch) {
    setAngle(ch, 90);
  }
}

}  // namespace freenove_car_core
