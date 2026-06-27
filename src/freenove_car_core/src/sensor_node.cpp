#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "freenove_car_msgs/msg/sensor_state.hpp"
#include "freenove_car_msgs/msg/infrared_state.hpp"
#include "freenove_car_core/ultrasonic.hpp"
#include "freenove_car_core/infrared.hpp"
#include "freenove_car_core/adc.hpp"

namespace freenove_car_core {

class SensorNode : public rclcpp::Node {
public:
  SensorNode()
      : Node("freenove_sensors") {
    declare_parameter("ultrasonic_enable", true);
    declare_parameter("infrared_enable", true);
    declare_parameter("light_enable", true);
    declare_parameter("battery_enable", true);
    declare_parameter("pcb_version", 1);
    declare_parameter("simulate", false);

    bool simulate = get_parameter("simulate").as_bool();
    int pcb_ver = get_parameter("pcb_version").as_int();

    // Initialize hardware
    if (!simulate) {
      if (get_parameter("ultrasonic_enable").as_bool()) {
        ultrasonic_ = std::make_unique<Ultrasonic>();
      }
      if (get_parameter("infrared_enable").as_bool()) {
        infrared_ = std::make_unique<Infrared>();
      }
      if (get_parameter("light_enable").as_bool() ||
          get_parameter("battery_enable").as_bool()) {
        adc_ = std::make_unique<ADC>();
      }
    } else {
      RCLCPP_INFO(get_logger(), "SIMULATION MODE");
    }

    // Publishers
    sonic_pub_ = create_publisher<std_msgs::msg::Float32>("/ultrasonic/distance", 10);
    ir_state_pub_ = create_publisher<freenove_car_msgs::msg::InfraredState>("/infrared/state", 10);
    light_pub_ = create_publisher<std_msgs::msg::Float32MultiArray>("/light/voltage", 10);
    bat_pub_ = create_publisher<std_msgs::msg::Float32>("/battery/voltage", 10);
    sensor_state_pub_ = create_publisher<freenove_car_msgs::msg::SensorState>("/sensor/state", 10);

    // Timers
    if (get_parameter("ultrasonic_enable").as_bool()) {
      create_wall_timer(std::chrono::milliseconds(500),
                        std::bind(&SensorNode::publishUltrasonic, this));
    }
    if (get_parameter("infrared_enable").as_bool()) {
      create_wall_timer(std::chrono::milliseconds(300),
                        std::bind(&SensorNode::publishInfrared, this));
    }
    if (get_parameter("light_enable").as_bool()) {
      create_wall_timer(std::chrono::milliseconds(300),
                        std::bind(&SensorNode::publishLight, this));
    }
    if (get_parameter("battery_enable").as_bool()) {
      create_wall_timer(std::chrono::seconds(2),
                        std::bind(&SensorNode::publishBattery, this));
    }
    // Combined state at 5 Hz
    create_wall_timer(std::chrono::milliseconds(200),
                      std::bind(&SensorNode::publishSensorState, this));

    RCLCPP_INFO(get_logger(), "Sensor node started");
  }

private:
  void publishUltrasonic() {
    auto msg = std_msgs::msg::Float32();
    msg.data = ultrasonic_ ? ultrasonic_->getDistanceCm() : 50.0f;
    sonic_pub_->publish(msg);
  }

  void publishInfrared() {
    auto msg = freenove_car_msgs::msg::InfraredState();
    if (infrared_) {
      auto [l, c, r] = infrared_->readAll();
      msg.left = l;
      msg.center = c;
      msg.right = r;
      msg.encoded = infrared_->readEncoded();
    } else {
      msg.left = msg.center = msg.right = false;
      msg.encoded = 0;
    }
    ir_state_pub_->publish(msg);
  }

  void publishLight() {
    auto msg = std_msgs::msg::Float32MultiArray();
    if (adc_) {
      msg.data = {adc_->readLightLeft(), adc_->readLightRight()};
    } else {
      msg.data = {2.5f, 2.5f};
    }
    light_pub_->publish(msg);
  }

  void publishBattery() {
    auto msg = std_msgs::msg::Float32();
    msg.data = adc_ ? adc_->readBattery(get_parameter("pcb_version").as_int())
                    : 7.4f;
    bat_pub_->publish(msg);
  }

  void publishSensorState() {
    auto msg = freenove_car_msgs::msg::SensorState();
    msg.stamp = now();
    msg.ultrasonic_distance = ultrasonic_ ? ultrasonic_->getDistanceCm() : -1.0f;

    if (adc_) {
      msg.light_left = adc_->readLightLeft();
      msg.light_right = adc_->readLightRight();
      msg.battery_voltage = adc_->readBattery(get_parameter("pcb_version").as_int());
    } else {
      msg.light_left = msg.light_right = 2.5f;
      msg.battery_voltage = 7.4f;
    }

    if (infrared_) {
      auto [l, c, r] = infrared_->readAll();
      msg.infrared.left = l;
      msg.infrared.center = c;
      msg.infrared.right = r;
      msg.infrared.encoded = infrared_->readEncoded();
    }

    sensor_state_pub_->publish(msg);
  }

  std::unique_ptr<Ultrasonic> ultrasonic_;
  std::unique_ptr<Infrared> infrared_;
  std::unique_ptr<ADC> adc_;

  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr sonic_pub_;
  rclcpp::Publisher<freenove_car_msgs::msg::InfraredState>::SharedPtr ir_state_pub_;
  rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr light_pub_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr bat_pub_;
  rclcpp::Publisher<freenove_car_msgs::msg::SensorState>::SharedPtr sensor_state_pub_;
};

}  // namespace freenove_car_core

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<freenove_car_core::SensorNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
