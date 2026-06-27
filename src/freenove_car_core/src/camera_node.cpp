#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

namespace freenove_car_core {

/// Pi Camera node using picamera2 (via Python bridge or direct).
/// Captures frames and publishes as sensor_msgs/Image.
class CameraNode : public rclcpp::Node {
public:
  CameraNode()
      : Node("freenove_camera") {
    declare_parameter("width", 640);
    declare_parameter("height", 480);
    declare_parameter("fps", 15);
    declare_parameter("simulate", false);

    simulate_ = get_parameter("simulate").as_bool();

    if (!simulate_) {
      // On Raspberry Pi, use a subprocess-based Python camera bridge
      // or compile with picamera2 C API. For now, log a message.
      RCLCPP_WARN(get_logger(),
                  "Camera requires picamera2 (Raspberry Pi only). "
                  "Use simulate:=true for testing without camera.");
    }

    // Publisher
    image_pub_ = create_publisher<sensor_msgs::msg::Image>("/camera/image_raw", 10);

    // Timer at configured FPS
    int fps = get_parameter("fps").as_int();
    create_wall_timer(std::chrono::milliseconds(1000 / fps),
                      std::bind(&CameraNode::publishFrame, this));

    RCLCPP_INFO(get_logger(), "Camera node started (simulate=%s)",
                simulate_ ? "true" : "false");
  }

private:
  void publishFrame() {
    if (simulate_) {
      // Publish a blank frame in simulation mode
      auto msg = sensor_msgs::msg::Image();
      msg.header.stamp = now();
      msg.header.frame_id = "camera_frame";
      msg.height = get_parameter("height").as_int();
      msg.width = get_parameter("width").as_int();
      msg.encoding = "rgb8";
      msg.is_bigendian = false;
      msg.step = msg.width * 3;
      msg.data.resize(msg.height * msg.step, 0);
      image_pub_->publish(msg);
    }
    // On real hardware, this would capture from picamera2.
    // For now, we only publish frames in sim mode.
  }

  bool simulate_;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_pub_;
};

}  // namespace freenove_car_core

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<freenove_car_core::CameraNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
