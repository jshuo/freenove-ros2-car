# Freenove 4WD Smart Car — ROS 2 C++ Packages

## Package Structure

```
ros-ws/src/
├── freenove_car_msgs/               # Custom message definitions
│   ├── package.xml
│   ├── CMakeLists.txt
│   └── msg/
│       ├── MotorCommand.msg         # 4-wheel duty targets [lf, lb, rf, rb]
│       ├── ServoCommand.msg         # channel (0-7) + angle (0-180°)
│       ├── InfraredState.msg        # 3-line sensor: left, center, right bools + encoded int
│       ├── LEDCommand.msg           # index_mask (bit 0-7) + R, G, B values
│       └── SensorState.msg          # Combined snapshot: ultrasonic + light + battery + IR
│
├── freenove_car_core/               # C++ hardware library + ROS 2 nodes
│   ├── package.xml
│   ├── CMakeLists.txt
│   ├── include/freenove_car_core/   # Hardware abstraction headers
│   │   ├── pca9685.hpp              # I²C PWM/servo driver (address 0x40)
│   │   ├── motor_controller.hpp     # 4-motor differential drive
│   │   ├── servo_controller.hpp     # 8-channel servo (channels 8-15 on PCA9685)
│   │   ├── ultrasonic.hpp           # HC-SR04 via sysfs GPIO (Trig=27, Echo=22)
│   │   ├── infrared.hpp             # 3-line sensor via sysfs GPIO (14, 15, 23)
│   │   ├── adc.hpp                  # ADS7830 I²C ADC (address 0x48)
│   │   ├── buzzer.hpp               # Active buzzer via sysfs GPIO (17)
│   │   └── led_controller.hpp       # 8× WS2812 LEDs via SPI (/dev/spidev0.0)
│   └── src/
│       ├── pca9685.cpp
│       ├── motor_controller.cpp
│       ├── servo_controller.cpp
│       ├── ultrasonic.cpp
│       ├── infrared.cpp
│       ├── adc.cpp
│       ├── buzzer.cpp
│       ├── led_controller.cpp
│       ├── base_node.cpp            # ROS 2 node: motors + servos
│       ├── sensor_node.cpp          # ROS 2 node: all sensors
│       ├── actuator_node.cpp        # ROS 2 node: buzzer + LEDs
│       └── camera_node.cpp          # ROS 2 node: Pi Camera (sim only)
│
└── freenove_car_bringup/            # Launch files & configuration
    ├── package.xml
    ├── CMakeLists.txt
    ├── launch/
    │   └── car.launch.py            # Launch all nodes (supports simulate:=true)
    └── config/
        ├── car_params.yaml           # Hardware mode parameters
        └── car_params_sim.yaml       # Simulation mode parameters
```

---

## Build

```bash
cd ~/workspaces/ros-ws
colcon build
source install/setup.sh
```

> **Note:** If you get a sudo password prompt, make sure no background `pip install` is being triggered. Add `--event-handlers console_direct+` to debug:  
> `colcon build --event-handlers console_direct+`

---

## Run

### Launch all nodes (simulation mode — no hardware required)

```bash
ros2 launch freenove_car_bringup car.launch.py simulate:=true
```

### Launch all nodes (on real Raspberry Pi with hardware)

```bash
ros2 launch freenove_car_bringup car.launch.py
```

### Run individual nodes

```bash
# Base node (motors + servos)
ros2 run freenove_car_core base_node --ros-args -p simulate:=true

# Sensor node (ultrasonic, infrared, photoresistor, battery)
ros2 run freenove_car_core sensor_node --ros-args -p simulate:=true

# Actuator node (buzzer, LEDs)
ros2 run freenove_car_core actuator_node --ros-args -p simulate:=true

# Camera node
ros2 run freenove_car_core camera_node --ros-args -p simulate:=true
```

---

## ROS 2 Topics

### Subscribed Topics

| Topic | Type | Description |
|---|---|---|
| `/cmd_vel` | `geometry_msgs/Twist` | Velocity commands → differential drive motors |
| `/servo_command` | `freenove_car_msgs/ServoCommand` | `[channel, angle]` servo position |
| `/buzzer/command` | `std_msgs/Bool` | `true` = on, `false` = off |
| `/led/command` | `freenove_car_msgs/LEDCommand` | `[index_mask, R, G, B]` — set LEDs by bitmask |
| `/led/clear` | `std_msgs/Empty` | Turn off all LEDs |

### Published Topics

| Topic | Type | Frequency | Description |
|---|---|---|---|
| `/ultrasonic/distance` | `std_msgs/Float32` | 2 Hz | Distance in cm |
| `/infrared/state` | `freenove_car_msgs/InfraredState` | ~3 Hz | Line sensor booleans + encoded value |
| `/light/voltage` | `std_msgs/Float32MultiArray` | ~3 Hz | `[left_V, right_V]` |
| `/battery/voltage` | `std_msgs/Float32` | 0.5 Hz | Battery voltage |
| `/sensor/state` | `freenove_car_msgs/SensorState` | 5 Hz | Combined sensor snapshot (stamped) |
| `/camera/image_raw` | `sensor_msgs/Image` | 15 Hz | Camera frames (sim only) |

---

## Teleoperation

Use the keyboard teleop package to drive the car:

```bash
# Install if not already available
sudo apt install ros-jazzy-teleop-twist-keyboard

# Run (in another terminal, after source install/setup.sh)
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

Keys: `i`/`j`/`k`/`l`/`,` for forward/left/stop/right/backward.

---

## Hardware Pin Mapping

| Component | Interface | Pins / Address |
|---|---|---|
| PCA9685 (PWM) | I²C-1 | `0x40` |
| ADS7830 (ADC) | I²C-1 | `0x48` |
| Motor LF | PCA9685 CH0 (Fwd), CH1 (Rev) | — |
| Motor LB | PCA9685 CH2 (Fwd), CH3 (Rev) | — |
| Motor RB | PCA9685 CH4 (Fwd), CH5 (Rev) | — |
| Motor RF | PCA9685 CH6 (Fwd), CH7 (Rev) | — |
| Servos | PCA9685 CH8-CH15 | — |
| Ultrasonic Trig | GPIO | 27 |
| Ultrasonic Echo | GPIO | 22 |
| IR Sensor Left | GPIO | 14 |
| IR Sensor Center | GPIO | 15 |
| IR Sensor Right | GPIO | 23 |
| Buzzer | GPIO | 17 |
| LED Strip | SPI0-MOSI | GPIO 10 (6.4 MHz) |
| Camera | CSI (Pi Camera) | picamera2 |

---

## Parameter Reference

All parameters are set via YAML in `config/car_params.yaml`:

| Parameter | Default | Description |
|---|---|---|
| `simulate` | `false` | Run without hardware |
| `i2c_address` | `64` (0x40) | PCA9685 I²C address |
| `max_duty` | `4095` | Maximum motor PWM duty |
| `servo_channels` | `2` | Number of active servo channels |
| `ultrasonic_enable` | `true` | Enable ultrasonic sensor |
| `infrared_enable` | `true` | Enable IR line sensors |
| `light_enable` | `true` | Enable photoresistor ADC |
| `battery_enable` | `true` | Enable battery voltage ADC |
| `pcb_version` | `1` | PCB version (affects ADC voltage calc) |
| `buzzer_enable` | `true` | Enable buzzer |
| `led_enable` | `true` | Enable WS2812 LEDs |
