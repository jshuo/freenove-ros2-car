#ifndef FREENOVE_CAR_CORE__PCA9685_HPP_
#define FREENOVE_CAR_CORE__PCA9685_HPP_

#include <cstdint>
#include <string>

namespace freenove_car_core {

/// PCA9685 16-channel 12-bit PWM/Servo driver via Linux I2C (/dev/i2c-*)
class PCA9685 {
public:
  explicit PCA9685(const std::string& device = "/dev/i2c-1",
                   uint8_t address = 0x40);
  ~PCA9685();

  /// Set PWM frequency (Hz) for all channels
  void setPwmFreq(float freq_hz);

  /// Set PWM on/off counts (0-4095) for a channel
  void setPwm(int channel, int on, int off);

  /// Set motor-style PWM: on=0, off=duty (0-4095)
  void setMotorPwm(int channel, int duty);

  /// Set servo pulse width (500-2500 µs)
  void setServoPulse(int channel, int pulse_us);

  /// Reset the device
  void reset();

private:
  int fd_;
  uint8_t addr_;

  void writeByte(uint8_t reg, uint8_t value);
  uint8_t readByte(uint8_t reg);
};

}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__PCA9685_HPP_
