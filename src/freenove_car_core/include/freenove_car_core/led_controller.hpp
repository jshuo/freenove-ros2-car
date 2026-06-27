#ifndef FREENOVE_CAR_CORE__LED_CONTROLLER_HPP_
#define FREENOVE_CAR_CORE__LED_CONTROLLER_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace freenove_car_core {

/// WS2812 RGB LED strip controller via SPI (/dev/spidev0.0)
class LEDController {
public:
  explicit LEDController(int num_leds = 8,
                         const std::string& spi_dev = "/dev/spidev0.0");
  ~LEDController();

  void setPixel(int index, uint8_t r, uint8_t g, uint8_t b);
  void setAll(uint8_t r, uint8_t g, uint8_t b);
  void show();
  void clear();

private:
  int fd_;
  int num_leds_;
  std::vector<uint8_t> pixels_;  // GRB format
  std::vector<uint8_t> spi_buffer_;

  uint8_t gammaCorrect(uint8_t value);
  void buildSpiBuffer();
};

}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LED_CONTROLLER_HPP_
