#include "freenove_car_core/adc.hpp"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cstring>
#include <stdexcept>

namespace freenove_car_core {

namespace {
constexpr uint8_t kCmdTemplate = 0x84;  // start | single-ended
}  // namespace

ADC::ADC(const std::string& device, uint8_t address)
    : fd_(-1), addr_(address) {
  fd_ = open(device.c_str(), O_RDWR);
  if (fd_ < 0) {
    throw std::runtime_error("Failed to open I2C device: " + device);
  }
  if (ioctl(fd_, I2C_SLAVE, addr_) < 0) {
    close(fd_);
    throw std::runtime_error("Failed to set I2C slave address");
  }
}

ADC::~ADC() {
  if (fd_ >= 0) close(fd_);
}

int ADC::readRaw(int channel) {
  if (channel < 0) channel = 0;
  if (channel > 7) channel = 7;

  uint8_t cmd = kCmdTemplate | (channel << 4);
  if (write(fd_, &cmd, 1) != 1) {
    return 0;
  }
  uint8_t value;
  if (read(fd_, &value, 1) != 1) {
    return 0;
  }
  return static_cast<int>(value);
}

float ADC::readChannel(int channel, float v_ref) {
  // Stabilize: read until two consecutive samples match
  int prev = -1;
  int value = 0;
  for (int i = 0; i < 10; ++i) {
    value = readRaw(channel);
    if (prev >= 0 && std::abs(value - prev) <= 1) break;
    prev = value;
  }
  return static_cast<float>(value) * v_ref / 255.0f;
}

float ADC::readLightLeft(float v_ref) {
  return readChannel(0, v_ref);
}

float ADC::readLightRight(float v_ref) {
  return readChannel(1, v_ref);
}

float ADC::readBattery(int pcb_version) {
  float v_ref = (pcb_version == 1) ? 3.3f : 5.2f;
  float multiplier = (pcb_version == 1) ? 3.0f : 2.0f;
  return readChannel(2, v_ref) * multiplier;
}

}  // namespace freenove_car_core
