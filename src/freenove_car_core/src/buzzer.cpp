#include "freenove_car_core/buzzer.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <thread>

namespace freenove_car_core {

Buzzer::Buzzer(int pin) : pin_(pin) {
  exportGpio();
}

Buzzer::~Buzzer() {
  unexportGpio();
}

void Buzzer::exportGpio() {
  int fd = open("/sys/class/gpio/export", O_WRONLY);
  if (fd >= 0) {
    char buf[4];
    int len = snprintf(buf, sizeof(buf), "%d", pin_);
    write(fd, buf, len);
    close(fd);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Set direction to output
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin_);
    fd = open(path, O_WRONLY);
    if (fd >= 0) {
      write(fd, "out", 3);
      close(fd);
    }
  }
}

void Buzzer::unexportGpio() {
  int fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if (fd >= 0) {
    char buf[4];
    int len = snprintf(buf, sizeof(buf), "%d", pin_);
    write(fd, buf, len);
    close(fd);
  }
}

void Buzzer::writeGpio(int value) {
  char path[64];
  snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin_);
  int fd = open(path, O_WRONLY);
  if (fd >= 0) {
    write(fd, value ? "1" : "0", 1);
    close(fd);
  }
}

void Buzzer::on() { writeGpio(1); }
void Buzzer::off() { writeGpio(0); }
void Buzzer::set(bool state) { writeGpio(state ? 1 : 0); }

}  // namespace freenove_car_core
