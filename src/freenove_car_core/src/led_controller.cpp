#include "freenove_car_core/led_controller.hpp"

#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <stdexcept>

namespace freenove_car_core {

LEDController::LEDController(int num_leds, const std::string& spi_dev)
    : fd_(-1), num_leds_(num_leds) {
  fd_ = open(spi_dev.c_str(), O_RDWR);
  if (fd_ < 0) {
    throw std::runtime_error("Failed to open SPI device: " + spi_dev);
  }

  // Configure SPI: mode 0, 8 bits, 6.4 MHz
  uint8_t mode = 0;
  ioctl(fd_, SPI_IOC_WR_MODE, &mode);

  uint8_t bits = 8;
  ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &bits);

  uint32_t speed = 6'400'000;
  ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

  pixels_.resize(num_leds_ * 3, 0);  // GRB order
  spi_buffer_.resize(num_leds_ * 8 * 3 + 4, 0);  // 8 bits per byte * 3 bytes per LED + reset
}

LEDController::~LEDController() {
  clear();
  if (fd_ >= 0) close(fd_);
}

uint8_t LEDController::gammaCorrect(uint8_t value) {
  return static_cast<uint8_t>(
      std::round(std::pow(value / 255.0f, 2.8f) * 255.0f));
}

void LEDController::buildSpiBuffer() {
  int buf_idx = 0;
  for (int i = 0; i < num_leds_; ++i) {
    // WS2812 GRB order: pixels_ stored as [G, R, B]
    for (int j = 0; j < 3; ++j) {
      uint8_t val = pixels_[i * 3 + j];
      for (int bit = 7; bit >= 0; --bit) {
        spi_buffer_[buf_idx++] = (val >> bit) & 1 ? 0xFC : 0xC0;
      }
    }
  }
  // Reset latch: >50 µs low
  for (int i = 0; i < 4; ++i) {
    spi_buffer_[buf_idx++] = 0x00;
  }
}

void LEDController::setPixel(int index, uint8_t r, uint8_t g, uint8_t b) {
  if (index < 0 || index >= num_leds_) return;
  // Store in GRB order
  pixels_[index * 3 + 0] = gammaCorrect(g);
  pixels_[index * 3 + 1] = gammaCorrect(r);
  pixels_[index * 3 + 2] = gammaCorrect(b);
}

void LEDController::setAll(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < num_leds_; ++i) {
    setPixel(i, r, g, b);
  }
}

void LEDController::show() {
  buildSpiBuffer();
  struct spi_ioc_transfer tr = {};
  tr.tx_buf = reinterpret_cast<uint64_t>(spi_buffer_.data());
  tr.len = spi_buffer_.size();
  tr.speed_hz = 6'400'000;
  tr.bits_per_word = 8;
  ioctl(fd_, SPI_IOC_MESSAGE(1), &tr);
}

void LEDController::clear() {
  std::fill(pixels_.begin(), pixels_.end(), 0);
  show();
}

}  // namespace freenove_car_core
