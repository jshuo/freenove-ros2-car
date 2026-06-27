#include "freenove_car_core/infrared.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <thread>

namespace freenove_car_core {

Infrared::Infrared(int pin_left, int pin_center, int pin_right) {
  pins_[0] = pin_left;
  pins_[1] = pin_center;
  pins_[2] = pin_right;
  for (int i = 0; i < 3; ++i) {
    exportGpio(pins_[i], i);
    exported_[i] = true;
  }
}

Infrared::~Infrared() {
  for (int i = 0; i < 3; ++i) {
    if (exported_[i]) {
      unexportGpio(pins_[i], i);
    }
  }
}

void Infrared::exportGpio(int pin, int /*idx*/) {
  int fd = open("/sys/class/gpio/export", O_WRONLY);
  if (fd >= 0) {
    char buf[4];
    int len = snprintf(buf, sizeof(buf), "%d", pin);
    write(fd, buf, len);
    close(fd);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

void Infrared::unexportGpio(int pin, int /*idx*/) {
  int fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if (fd >= 0) {
    char buf[4];
    int len = snprintf(buf, sizeof(buf), "%d", pin);
    write(fd, buf, len);
    close(fd);
  }
}

int Infrared::readGpio(int pin) {
  char path[64];
  snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
  int fd = open(path, O_RDONLY);
  if (fd < 0) return 0;
  char buf;
  read(fd, &buf, 1);
  close(fd);
  return (buf == '1') ? 1 : 0;
}

std::tuple<bool, bool, bool> Infrared::readAll() {
  return std::make_tuple(
      readGpio(pins_[0]) != 0,
      readGpio(pins_[1]) != 0,
      readGpio(pins_[2]) != 0);
}

int Infrared::readEncoded() {
  auto [l, c, r] = readAll();
  return (static_cast<int>(l) << 2) |
         (static_cast<int>(c) << 1) |
         static_cast<int>(r);
}

}  // namespace freenove_car_core
