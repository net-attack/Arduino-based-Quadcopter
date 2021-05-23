# Arduino based quadcopter project
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
![PLANTUML diagram of sensor calibration setup](out/arduino_uno_sensor_calibration/setup_uml/arduino_uno_sensor_calibration_setup.png)
- Loop routine
![PLANTUML diagram of sensor calibration setup](out/arduino_uno_sensor_calibration/loop_uml/arduino_uno_sensor_calibration_loop.png)



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
