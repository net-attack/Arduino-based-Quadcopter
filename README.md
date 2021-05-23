# Arduino based quadcopter project
> This projects is related to the YMFC from Youtube.

![Quadcopter](doc/IMG_6664.JPEG)
# Hardware

## Hardware multicopter orientation
> TODO Add image of multicopter

|  | BACK                               |                |
|----|-------------------------------------------|--------------------|
| **RIGHT**   | ![](https://electronoobs.com/images/Robotica/tut_5/full_flight_controller.png)|**LEFT**  |
| | <center>**FRONT**</center>||

### Details
The arduino UNO ist located in the middle of the multicopter. The orientation is shown in the image. The usb port is the backside of the drone.
It has a connection to the remote sensor and reads in the pwm control signals from the remote control. Via I2C a MPU6050 is connected and can provide gyro and accelerometer data. The power supply is done via a power separation board to spread the power supply to every engine.

1. **Power Supply**
The power is provided via a 11.7V 3 Cell LIPO accumulator
The voltage is send to the esc and is converted to 5V to provide power to the arduino and the remote receiver.

1. **ESC Signal Connection**
The connection of the esc cables to the sensor board is shown in the next image:
![](doc/esc_connection.JPEG)
|Front Right (*GPIO 4*)  | Rear Right (*GPIO 5*)| Rear Left (*GPIO 6*)| Front Left (*GPIO 7*) |
1. **Remote Signal Connection**
The signal of the remote receiver are processed via interrupt handling on the arduino, this is why i need to attach the ports to specific pins on the UNO. Because of a broken pin at GPIO8 it was not possible to use the same sketch as YMFC, so i need to shift the GPIO Interrupts by one. By this i was not able to use the remote LED on *GPIO 12*.

| CHANNEL 1  | CHANNEL 2 |CHANNEL 3|CHANNEL 4 |
|---|---|---|----|
| *GPIO 9*|*GPIO 10*|*GPIO 11*|*GPIO 12*|
| Roll 🡰 🡲 | Pitch 🡱 🡳| Yaw  ⭯ ⭮ | Throttle ⮥ ⮦ |



![Arduino Sensor Calibration Test](https://github.com/net-attack/drone_controller/actions/workflows/test-setup.yml/badge.svg)

![Arduino ESC Calibration Test](https://github.com/net-attack/drone_controller/actions/workflows/test-calibration.yml/badge.svg)

![Arduino Flight Controller Test](https://github.com/net-attack/drone_controller/actions/workflows/test-controller.yml/badge.svg)






# Software parts:

1. Arduino Sensor Calibration
1. Arduino ESC Calibration
1. Arduino Flight Controller Simple
1. Arduino Flight Controller

## Arduino Sensor Calibration

> Arduino software for checking if all sensors are working. Also if the remote sensors are attached correctly. Then it is used to detect the orientation of the IMU sensor to have valid ranges.
- Setup routine

 ![PLANTUML diagram of sensor calibration setup](out/arduino_uno_sensor_calibration/setup_uml/arduino_uno_sensor_calibration_setup.png?style=centerme)
- Loop routine
 
  ![PLANTUML diagram of sensor calibration setup](out/arduino_uno_sensor_calibration/loop_uml/arduino_uno_sensor_calibration_loop.png?style=centerme)



## Arduino ESC Calibration
> Routine to test the vibration of the certain speed controllers and rotors
- Setup routine
![PLANTUML diagram of esc calibration setup](out/arduino_uno_esc_calibration/setup_uml/arduino_uno_esc_calibration_setup.png)
- Loop routine
![PLANTUML diagram of esc calibration setup](out/arduino_uno_esc_calibration/loop_uml/arduino_uno_esc_calibration_loop.png)


## Arduino Flight Controller Simple
> Own draft of a flight controller based on YMFC code.
- Setup routine
![PLANTUML diagram of esc calibration setup](out/arduino_uno_flight_controller_simple/setup_uml/arduino_uno_flight_controller_simple_setup.png)
- Loop routine
![PLANTUML diagram of esc calibration setup](out/arduino_uno_flight_controller_simple/loop_uml/arduino_uno_flight_controller_simple_loop.png)
