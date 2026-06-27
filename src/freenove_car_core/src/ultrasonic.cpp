#include "freenove_car_core/ultrasonic.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <thread>

namespace freenove_car_core {

Ultrasonic::Ultrasonic(int trigger_pin, int echo_pin)
    : trigger_pin_(trigger_pin), echo_pin_(echo_pin) {
  exportGpio(trigger_pin_);
  exportGpio(echo_pin_);
  setGpioDirection(trigger_pin_, "out");
  setGpioDirection(echo_pin_, "in");
  writeGpio(trigger_pin_, 0);
}

Ultrasonic::~Ultrasonic() {
  unexportGpio(trigger_pin_);
  unexportGpio(echo_pin_);
}

void Ultrasonic::exportGpio(int pin) {
  int fd = open("/sys/class/gpio/export", O_WRONLY);
  if (fd >= 0) {
    char buf[4];
    int len = snprintf(buf, sizeof(buf), "%d", pin);
    write(fd, buf, len);
    close(fd);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void Ultrasonic::unexportGpio(int pin) {
  int fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if (fd >= 0) {
    char buf[4];
    int len = snprintf(buf, sizeof(buf), "%d", pin);
    write(fd, buf, len);
    close(fd);
  }
}

void Ultrasonic::setGpioDirection(int pin, const char* dir) {
  char path[64];
  snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);
  int fd = open(path, O_WRONLY);
  if (fd >= 0) {
    write(fd, dir, strlen(dir));
    close(fd);
  }
}

void Ultrasonic::writeGpio(int pin, int value) {
  char path[64];
  snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
  int fd = open(path, O_WRONLY);
  if (fd >= 0) {
    write(fd, value ? "1" : "0", 1);
    close(fd);
  }
}

int Ultrasonic::readGpio(int pin) {
  char path[64];
  snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
  int fd = open(path, O_RDONLY);
  if (fd < 0) return 0;
  char buf;
  read(fd, &buf, 1);
  close(fd);
  return (buf == '1') ? 1 : 0;
}

void Ultrasonic::delayUs(int us) {
  // Busy-wait for microsecond precision
  auto start = std::chrono::steady_clock::now();
  auto dur = std::chrono::microseconds(us);
  while (std::chrono::steady_clock::now() - start < dur) {
    // spin
  }
}

float Ultrasonic::getDistanceCm() {
  // Send 10µs trigger pulse
  writeGpio(trigger_pin_, 1);
  delayUs(10);
  writeGpio(trigger_pin_, 0);

  // Wait for echo to go high (timeout ~30 ms = ~5m)
  int wait_us = 0;
  while (readGpio(echo_pin_) == 0) {
    delayUs(2);
    wait_us += 2;
    if (wait_us > 30000) return -1.0f;  // timeout
  }

  // Measure echo pulse width
  auto start = std::chrono::steady_clock::now();
  wait_us = 0;
  while (readGpio(echo_pin_) == 1) {
    delayUs(2);
    wait_us += 2;
    if (wait_us > 60000) return -1.0f;  // timeout (max ~10m)
  }
  auto end = std::chrono::steady_clock::now();

  auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(
                        end - start).count();
  // Speed of sound = 343 m/s = 0.0343 cm/µs
  // Distance = (time * 0.0343) / 2 (round trip)
  float distance = static_cast<float>(elapsed_us) * 0.0343f / 2.0f;
  return distance;
}

}  // namespace freenove_car_core
