#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/empty.hpp"
#include "freenove_car_msgs/msg/led_command.hpp"
#include "freenove_car_core/buzzer.hpp"
#include "freenove_car_core/led_controller.hpp"

namespace freenove_car_core {

class ActuatorNode : public rclcpp::Node {
public:
  ActuatorNode()
      : Node("freenove_actuators") {
    declare_parameter("buzzer_enable", true);
    declare_parameter("led_enable", true);
    declare_parameter("simulate", false);

    bool simulate = get_parameter("simulate").as_bool();

    // Initialize hardware
    if (!simulate) {
      if (get_parameter("buzzer_enable").as_bool()) {
        try {
          buzzer_ = std::make_unique<Buzzer>();
        } catch (const std::exception& e) {
          RCLCPP_WARN(get_logger(), "Buzzer init failed: %s", e.what());
        }
      }
      if (get_parameter("led_enable").as_bool()) {
        try {
          leds_ = std::make_unique<LEDController>();
          RCLCPP_INFO(get_logger(), "LED controller initialized");
        } catch (const std::exception& e) {
          RCLCPP_WARN(get_logger(), "LED init failed: %s", e.what());
        }
      }
    } else {
      RCLCPP_INFO(get_logger(), "SIMULATION MODE");
    }

    // Subscribers
    buzzer_sub_ = create_subscription<std_msgs::msg::Bool>(
        "/buzzer/command", 10,
        std::bind(&ActuatorNode::onBuzzer, this, std::placeholders::_1));

    led_sub_ = create_subscription<freenove_car_msgs::msg::LEDCommand>(
        "/led/command", 10,
        std::bind(&ActuatorNode::onLED, this, std::placeholders::_1));

    led_clear_sub_ = create_subscription<std_msgs::msg::Empty>(
        "/led/clear", 10,
        std::bind(&ActuatorNode::onLEDClear, this, std::placeholders::_1));

    RCLCPP_INFO(get_logger(), "Actuator node started");
  }

  ~ActuatorNode() override {
    if (buzzer_) buzzer_->off();
    if (leds_) leds_->clear();
  }

private:
  void onBuzzer(const std_msgs::msg::Bool::SharedPtr msg) {
    if (buzzer_) buzzer_->set(msg->data);
    RCLCPP_INFO(get_logger(), "Buzzer: %s", msg->data ? "ON" : "OFF");
  }

  void onLED(const freenove_car_msgs::msg::LEDCommand::SharedPtr msg) {
    if (leds_) {
      for (int i = 0; i < 8; ++i) {
        if (msg->index_mask & (1 << i)) {
          leds_->setPixel(i,
                          static_cast<uint8_t>(msg->r),
                          static_cast<uint8_t>(msg->g),
                          static_cast<uint8_t>(msg->b));
        }
      }
      leds_->show();
    }
  }

  void onLEDClear(const std_msgs::msg::Empty::SharedPtr /*msg*/) {
    if (leds_) leds_->clear();
    RCLCPP_INFO(get_logger(), "LEDs cleared");
  }

  std::unique_ptr<Buzzer> buzzer_;
  std::unique_ptr<LEDController> leds_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr buzzer_sub_;
  rclcpp::Subscription<freenove_car_msgs::msg::LEDCommand>::SharedPtr led_sub_;
  rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr led_clear_sub_;
};

}  // namespace freenove_car_core

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<freenove_car_core::ActuatorNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
