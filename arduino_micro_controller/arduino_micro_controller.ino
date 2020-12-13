#include "FlightStates.h"
#include "Receiver.h"

//#include "Engine.h"

//Engine *engine;
Receiver *receiver;

uint8_t state = 0; 
uint8_t error = 0;

void setup(){
    Serial.begin(57600);
    delay(250);
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
            Serial.print(receiver->low_channel_1);
            Serial.print(F(" - "));
            Serial.print(receiver->center_channel_1);
            Serial.print(F(" - "));
            Serial.println(receiver->high_channel_1);
            Serial.print(F("Digital input 09 values:"));
            Serial.print(receiver->low_channel_2);
            Serial.print(F(" - "));
            Serial.print(receiver->center_channel_2);
            Serial.print(F(" - "));
            Serial.println(receiver->high_channel_2);
            Serial.print(F("Digital input 10 values:"));
            Serial.print(receiver->low_channel_3);
            Serial.print(F(" - "));
            Serial.print(receiver->center_channel_3);
            Serial.print(F(" - "));
            Serial.println(receiver->high_channel_3);
            Serial.print(F("Digital input 11 values:"));
            Serial.print(receiver->low_channel_4);
            Serial.print(F(" - "));
            Serial.print(receiver->center_channel_4);
            Serial.print(F(" - "));
            Serial.println(receiver->high_channel_4);
            Serial.println(F("Move stick 'nose up' and back to center to continue"));
            receiver->check_to_continue();
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

