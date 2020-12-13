#include <EEPROM.h>             //Include the EEPROM.h library so we can store information onto the EEPROM
#include "FlightStates.h"
#include "Receiver.h"
#include "Gyro.h"
//#include "Engine.h"

//Engine *engine;
Receiver *receiver;
Gyro *gyro;

uint8_t state = 0; 
uint8_t error = 0;

void setup(){
    Serial.begin(57600);
    delay(250);
    state = PRE_FLIGHT_CHECK_GYRO;
}




void loop() {
    switch(state){
       case PRE_FLIGHT_CHECK:
            intro();
            state = PRE_FLIGHT_CHECK_RECEIVER;
            break;
       case PRE_FLIGHT_CHECK_RECEIVER:
            receiver = new Receiver();
            state = PRE_FLIGHT_CHECK_RECEIVER_CENTER;
            break;
       case PRE_FLIGHT_CHECK_RECEIVER_CENTER:
            Serial.println(F(""));
            Serial.println(F("==================================================="));
            Serial.println(F("Transmitter setup"));
            Serial.println(F("==================================================="));
            delay(1000);

            Serial.println(F("Checking for valid receiver signals."));
            receiver->wait_for_receiver(&error);
            if(error != 0){
               state = PRE_FLIGHT_CHECK_ERROR; 
               break;
            }
            
            receiver->check_center_position(&error);
            if(error != 0){
               state = PRE_FLIGHT_CHECK_ERROR; 
               break;
            }

            state = PRE_FLIGHT_CHECK_RECEIVER_ENDPOINT;
            break;

            
       case PRE_FLIGHT_CHECK_RECEIVER_ENDPOINT:
            Serial.println(F(""));
            Serial.println(F(""));
            Serial.println(F("Gently move all the sticks simultaneously to their extends"));
            Serial.println(F("When ready put the sticks back in their center positions"));
            //Register the min and max values of the receiver channels
            receiver->register_min_max();
            Serial.println(F(""));
            Serial.println(F(""));
            Serial.println(F("High, low and center values found during setup"));
            Serial.print(F("Digital input 08 values:"));
            Serial.print(low_channel_1);
            Serial.print(F(" - "));
            Serial.print(center_channel_1);
            Serial.print(F(" - "));
            Serial.println(high_channel_1);
            Serial.print(F("Digital input 09 values:"));
            Serial.print(low_channel_2);
            Serial.print(F(" - "));
            Serial.print(center_channel_2);
            Serial.print(F(" - "));
            Serial.println(high_channel_2);
            Serial.print(F("Digital input 10 values:"));
            Serial.print(low_channel_3);
            Serial.print(F(" - "));
            Serial.print(center_channel_3);
            Serial.print(F(" - "));
            Serial.println(high_channel_3);
            Serial.print(F("Digital input 11 values:"));
            Serial.print(low_channel_4);
            Serial.print(F(" - "));
            Serial.print(center_channel_4);
            Serial.print(F(" - "));
            Serial.println(high_channel_4);
            Serial.println(F("Move stick 'nose up' and back to center to continue"));
            receiver->check_to_continue();
            state = PRE_FLIGHT_CHECK_GYRO;
            Serial.println(F("Receiver Check Finished! \n ###################################"));
            break;

       case PRE_FLIGHT_CHECK_GYRO:
            Serial.println(F("Gyro Check "));
            gyro = new Gyro(receiver);
            state = PRE_FLIGHT_CHECK_GYRO_CALIBRATION;

            break;
       case PRE_FLIGHT_CHECK_GYRO_CALIBRATION:
            Serial.println(F("Gyro found!"));
            gyro->calibrate(&error);
            if(error != 0){
               state = PRE_FLIGHT_CHECK_ERROR; 
               break;
            }
            state = PRE_FLIGHT_CHECK_GYRO_CONFIGURATION;
            break;


       case PRE_FLIGHT_CHECK_GYRO_CONFIGURATION:
            gyro->configure(&error);
            if(error != 0){
               state = PRE_FLIGHT_CHECK_ERROR; 
               break;
            }
            state = PRE_FLIGHT_LED_CHECK;
            break;



       case PRE_FLIGHT_LED_CHECK:
            Serial.println(F(""));
            Serial.println(F("==================================================="));
            Serial.println(F("LED test"));
            Serial.println(F("==================================================="));
            digitalWrite(13, HIGH);
            Serial.println(F("The LED should now be lit"));
            Serial.println(F("Move stick 'nose up' and back to center to continue"));
            receiver->check_to_continue();
            digitalWrite(13, LOW);
            break;


       case PRE_FLIGHT_FINAL_SETUP_TEST:
            Serial.println(F("==================================================="));
            Serial.println(F("Final setup check"));
            Serial.println(F("==================================================="));
            delay(1000);
            if(receiver_check_byte == 0b00001111){
              Serial.println(F("Receiver channels ok"));
            }
            else{
              Serial.println(F("Receiver channel verification failed!!! (ERROR 6)"));
              error = 1;
            }
            delay(1000);
            if(gyro_check_byte == 0b00000111){
              Serial.println(F("Gyro axes ok"));
            }
            else{
              Serial.println(F("Gyro exes verification failed!!! (ERROR 7)"));
              error = 1;
            }
            break;
       case PRE_FLIGHT_STORE_EEPROM:
            write_eeprom();
            break;
       case PRE_FLIGHT_CHECK_ERROR:
            while(1){
              Serial.println(F("Error while setup."));
              delay(1000);
            }
            break;
            
       default:
           break; 
    }

  //  receiver->checkInput(&value_a, &value_b, &value_c, &value_d );
    //engine->updateEngine(value);
}
 

//Intro subroutine
void intro(){
  Serial.println(F("==================================================="));
//  delay(1500);
  Serial.println(F(""));
  Serial.println(F("Your"));
//  delay(500);
  Serial.println(F("  Multicopter"));
//  delay(500);
  Serial.println(F("    Flight"));
//  delay(500);
  Serial.println(F("      Controller"));
//  delay(1000);
  Serial.println(F(""));
  Serial.println(F( "Setup Program"));
  Serial.println(F(""));
  Serial.println(F("==================================================="));
//  delay(1500);
  Serial.println(F("Have fun!"));
}

void write_eeprom(){//If all is good, store the information in the EEPROM
    Serial.println(F(""));
    Serial.println(F("==================================================="));
    Serial.println(F("Storing EEPROM information"));
    Serial.println(F("==================================================="));
    Serial.println(F("Writing EEPROM"));
    delay(1000);
    Serial.println(F("Done!"));
    EEPROM.write(0, center_channel_1 & 0b11111111);
    EEPROM.write(1, center_channel_1 >> 8);
    EEPROM.write(2, center_channel_2 & 0b11111111);
    EEPROM.write(3, center_channel_2 >> 8);
    EEPROM.write(4, center_channel_3 & 0b11111111);
    EEPROM.write(5, center_channel_3 >> 8);
    EEPROM.write(6, center_channel_4 & 0b11111111);
    EEPROM.write(7, center_channel_4 >> 8);
    EEPROM.write(8, high_channel_1 & 0b11111111);
    EEPROM.write(9, high_channel_1 >> 8);
    EEPROM.write(10, high_channel_2 & 0b11111111);
    EEPROM.write(11, high_channel_2 >> 8);
    EEPROM.write(12, high_channel_3 & 0b11111111);
    EEPROM.write(13, high_channel_3 >> 8);
    EEPROM.write(14, high_channel_4 & 0b11111111);
    EEPROM.write(15, high_channel_4 >> 8);
    EEPROM.write(16, low_channel_1 & 0b11111111);
    EEPROM.write(17, low_channel_1 >> 8);
    EEPROM.write(18, low_channel_2 & 0b11111111);
    EEPROM.write(19, low_channel_2 >> 8);
    EEPROM.write(20, low_channel_3 & 0b11111111);
    EEPROM.write(21, low_channel_3 >> 8);
    EEPROM.write(22, low_channel_4 & 0b11111111);
    EEPROM.write(23, low_channel_4 >> 8);
    EEPROM.write(24, channel_1_assign);
    EEPROM.write(25, channel_2_assign);
    EEPROM.write(26, channel_3_assign);
    EEPROM.write(27, channel_4_assign);
    EEPROM.write(28, roll_axis);
    EEPROM.write(29, pitch_axis);
    EEPROM.write(30, yaw_axis);
    EEPROM.write(31, 2);
    EEPROM.write(32, 0x68);
    //Write the EEPROM signature
    EEPROM.write(33, 'J'); 
    EEPROM.write(34, 'M');
    EEPROM.write(35, 'B');
        
    
    //To make sure evrything is ok, verify the EEPROM data.
    Serial.println(F("Verify EEPROM data"));
    delay(1000);
    if(center_channel_1 != ((EEPROM.read(1) << 8) | EEPROM.read(0)))error = 1;
    if(center_channel_2 != ((EEPROM.read(3) << 8) | EEPROM.read(2)))error = 1;
    if(center_channel_3 != ((EEPROM.read(5) << 8) | EEPROM.read(4)))error = 1;
    if(center_channel_4 != ((EEPROM.read(7) << 8) | EEPROM.read(6)))error = 1;
    
    if(high_channel_1 != ((EEPROM.read(9) << 8) | EEPROM.read(8)))error = 1;
    if(high_channel_2 != ((EEPROM.read(11) << 8) | EEPROM.read(10)))error = 1;
    if(high_channel_3 != ((EEPROM.read(13) << 8) | EEPROM.read(12)))error = 1;
    if(high_channel_4 != ((EEPROM.read(15) << 8) | EEPROM.read(14)))error = 1;
    
    if(low_channel_1 != ((EEPROM.read(17) << 8) | EEPROM.read(16)))error = 1;
    if(low_channel_2 != ((EEPROM.read(19) << 8) | EEPROM.read(18)))error = 1;
    if(low_channel_3 != ((EEPROM.read(21) << 8) | EEPROM.read(20)))error = 1;
    if(low_channel_4 != ((EEPROM.read(23) << 8) | EEPROM.read(22)))error = 1;
    
    if(channel_1_assign != EEPROM.read(24))error = 1;
    if(channel_2_assign != EEPROM.read(25))error = 1;
    if(channel_3_assign != EEPROM.read(26))error = 1;
    if(channel_4_assign != EEPROM.read(27))error = 1;
    
    if(roll_axis != EEPROM.read(28))error = 1;
    if(pitch_axis != EEPROM.read(29))error = 1;
    if(yaw_axis != EEPROM.read(30))error = 1;
    if(2 != EEPROM.read(31))error = 1;
    if(0x68 != EEPROM.read(32))error = 1;
    
    if('J' != EEPROM.read(33))error = 1;
    if('M' != EEPROM.read(34))error = 1;
    if('B' != EEPROM.read(35))error = 1;
  
    if(error == 1)Serial.println(F("EEPROM verification failed!!! (ERROR 5)"));
    else Serial.println(F("Verification done"));
  }