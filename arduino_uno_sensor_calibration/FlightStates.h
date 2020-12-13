
enum  FlightStates{
  PRE_FLIGHT_CHECK = 0, // 0
  PRE_FLIGHT_CHECK_RECEIVER, // 1
  PRE_FLIGHT_CHECK_RECEIVER_CENTER, // 2
  PRE_FLIGHT_CHECK_RECEIVER_ENDPOINT, // 3
  PRE_FLIGHT_CHECK_GYRO, // 4
  PRE_FLIGHT_CHECK_GYRO_READING, // 5
  PRE_FLIGHT_CHECK_GYRO_CALIBRATION, // 6
  PRE_FLIGHT_CHECK_GYRO_CONFIGURATION, // 7
  PRE_FLIGHT_LED_CHECK, // 8
  PRE_FLIGHT_FINAL_SETUP_TEST, // 9 
  PRE_FLIGHT_STORE_EEPROM, // 10 
  PRE_FLIGHT_CHECK_FINISHED,
  PRE_FLIGHT_CHECK_ERROR // 
};

