#include "freenove_car_core/pca9685.hpp"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cmath>
#include <cstring>
#include <stdexcept>
#include <thread>

namespace freenove_car_core {

namespace {
constexpr int kMode1 = 0x00;
constexpr int kMode2 = 0x01;
constexpr int kLed0OnL = 0x06;
constexpr int kPreScale = 0xFE;

constexpr uint8_t kMode1Restart = 0x80;
constexpr uint8_t kMode1Sleep = 0x10;
constexpr uint8_t kMode1Ai = 0x20;
}  // namespace

PCA9685::PCA9685(const std::string& device, uint8_t address)
    : fd_(-1), addr_(address) {
  fd_ = open(device.c_str(), O_RDWR);
  if (fd_ < 0) {
    throw std::runtime_error("Failed to open I2C device: " + device);
  }
  if (ioctl(fd_, I2C_SLAVE, addr_) < 0) {
    close(fd_);
    throw std::runtime_error("Failed to set I2C slave address");
  }
  writeByte(kMode1, 0x00);  // Normal mode
  setPwmFreq(50.0f);
}

PCA9685::~PCA9685() {
  if (fd_ >= 0) close(fd_);
}

void PCA9685::writeByte(uint8_t reg, uint8_t value) {
  uint8_t buf[2] = {reg, value};
  if (write(fd_, buf, 2) != 2) {
    throw std::runtime_error("I2C write failed");
  }
}

uint8_t PCA9685::readByte(uint8_t reg) {
  if (write(fd_, &reg, 1) != 1) {
    throw std::runtime_error("I2C write (select reg) failed");
  }
  uint8_t value;
  if (read(fd_, &value, 1) != 1) {
    throw std::runtime_error("I2C read failed");
  }
  return value;
}

void PCA9685::setPwmFreq(float freq_hz) {
  float prescale = std::round(25'000'000.0f / (4096.0f * freq_hz)) - 1.0f;
  uint8_t prescale_val = static_cast<uint8_t>(prescale);

  uint8_t old_mode = readByte(kMode1);
  // Go to sleep
  writeByte(kMode1, (old_mode & 0x7F) | kMode1Sleep);
  writeByte(kPreScale, prescale_val);
  // Wake up with auto-increment
  writeByte(kMode1, old_mode | kMode1Ai);
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void PCA9685::setPwm(int channel, int on, int off) {
  int base = kLed0OnL + 4 * channel;
  writeByte(base, on & 0xFF);
  writeByte(base + 1, (on >> 8) & 0xFF);
  writeByte(base + 2, off & 0xFF);
  writeByte(base + 3, (off >> 8) & 0xFF);
}

void PCA9685::setMotorPwm(int channel, int duty) {
  if (duty < 0) duty = 0;
  if (duty > 4095) duty = 4095;
  setPwm(channel, 0, duty);
}

void PCA9685::setServoPulse(int channel, int pulse_us) {
  if (pulse_us < 500) pulse_us = 500;
  if (pulse_us > 2500) pulse_us = 2500;
  // 50 Hz -> 20 ms period -> 4096 counts
  int off_counts = static_cast<int>(pulse_us * 4096LL / 20'000);
  setPwm(channel, 0, off_counts);
}

void PCA9685::reset() {
  writeByte(kMode1, kMode1Restart);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

}  // namespace freenove_car_core
