#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "freenove_car_msgs/msg/servo_command.hpp"
#include "freenove_car_core/pca9685.hpp"
#include "freenove_car_core/motor_controller.hpp"
#include "freenove_car_core/servo_controller.hpp"

namespace freenove_car_core {

class BaseNode : public rclcpp::Node {
public:
  BaseNode()
      : Node("freenove_base") {
    declare_parameter("i2c_address", 0x40);
    declare_parameter("max_duty", 4095);
    declare_parameter("servo_channels", 2);
    declare_parameter("simulate", false);

    bool simulate = get_parameter("simulate").as_bool();
    max_duty_ = get_parameter("max_duty").as_int();

    // Initialize hardware
    if (!simulate) {
      try {
        uint8_t addr = static_cast<uint8_t>(get_parameter("i2c_address").as_int());
        pca_ = std::make_unique<PCA9685>("/dev/i2c-1", addr);
        motors_ = std::make_unique<MotorController>(*pca_);
        servos_ = std::make_unique<ServoController>(*pca_);
        RCLCPP_INFO(get_logger(), "PCA9685 initialized at 0x%02x", addr);
      } catch (const std::exception& e) {
        RCLCPP_ERROR(get_logger(), "Hardware init failed: %s", e.what());
        rclcpp::shutdown();
        return;
      }
    } else {
      RCLCPP_INFO(get_logger(), "SIMULATION MODE");
    }

    // Subscribers
    cmd_vel_sub_ = create_subscription<geometry_msgs::msg::Twist>(
        "/cmd_vel", 10,
        std::bind(&BaseNode::onCmdVel, this, std::placeholders::_1));

    servo_sub_ = create_subscription<freenove_car_msgs::msg::ServoCommand>(
        "/servo_command", 10,
        std::bind(&BaseNode::onServoCommand, this, std::placeholders::_1));

    RCLCPP_INFO(get_logger(), "Base node started");
  }

  ~BaseNode() override {
    if (motors_) motors_->stop();
  }

private:
  void onCmdVel(const geometry_msgs::msg::Twist::SharedPtr msg) {
    if (motors_) {
      motors_->drive(msg->linear.x, msg->angular.z,
                     static_cast<float>(max_duty_));
    }
  }

  void onServoCommand(const freenove_car_msgs::msg::ServoCommand::SharedPtr msg) {
    if (servos_) {
      servos_->setAngle(msg->channel, msg->angle);
      RCLCPP_INFO(get_logger(), "Servo %d -> %d°", msg->channel, msg->angle);
    }
  }

  std::unique_ptr<PCA9685> pca_;
  std::unique_ptr<MotorController> motors_;
  std::unique_ptr<ServoController> servos_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub_;
  rclcpp::Subscription<freenove_car_msgs::msg::ServoCommand>::SharedPtr servo_sub_;
  int max_duty_;
};

}  // namespace freenove_car_core

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<freenove_car_core::BaseNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
