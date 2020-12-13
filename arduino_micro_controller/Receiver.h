volatile int receiver_input_channel_1, receiver_input_channel_2, receiver_input_channel_3, receiver_input_channel_4;
byte last_channel_1, last_channel_2, last_channel_3, last_channel_4;
unsigned long timer, timer_1, timer_2, timer_3, timer_4, current_time;
byte receiver_check_byte;

class Receiver
{
  public:
    int center_channel_1, center_channel_2, center_channel_3, center_channel_4;
    int high_channel_1, high_channel_2, high_channel_3, high_channel_4;
    int low_channel_1, low_channel_2, low_channel_3, low_channel_4;
    byte channel_1_assign, channel_2_assign, channel_3_assign, channel_4_assign;


    Receiver(){
        //Arduino (Atmega) pins default to inputs, so they don't need to be explicitly declared as inputs
        PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
        PCMSK0 |= (1 << PCINT1);  // set PCINT1 (digital input 9) to trigger an interrupt on state change
        PCMSK0 |= (1 << PCINT2);  // set PCINT2 (digital input 10)to trigger an interrupt on state change
        PCMSK0 |= (1 << PCINT3);  // set PCINT3 (digital input 11)to trigger an interrupt on state change
        PCMSK0 |= (1 << PCINT4);  // set PCINT4 (digital input 12)to trigger an interrupt on state change
    };

    
   //Checck if the receiver values are valid within 10 seconds
    void wait_for_receiver(uint8_t* error){
      byte zero = 0;
      timer = millis() + 10000;
      while(timer > millis() && zero < 15){
      
        if(receiver_input_channel_1 < 2100 && receiver_input_channel_1 > 900)zero |= 0b00000001;
        if(receiver_input_channel_2 < 2100 && receiver_input_channel_2 > 900)zero |= 0b00000010;
        if(receiver_input_channel_3 < 2100 && receiver_input_channel_3 > 900)zero |= 0b00000100;
        if(receiver_input_channel_4 < 2100 && receiver_input_channel_4 > 900)zero |= 0b00001000;
        
        delay(500);
        Serial.print(F("."));
      }
      if(zero == 0){
        *error = 1;
        Serial.println(F("."));
        Serial.println(F("No valid receiver signals found!!! (ERROR 1)"));
      }
      else Serial.println(F(" OK"));
    };

    void check_center_position(uint8_t* error){
      delay(2100);
      Serial.println(F("Place all sticks and subtrims in the center position within 10 seconds."));
      for(int i = 9;i > 0;i--){
        delay(500);
        Serial.print(i);
        Serial.print(" ");
      }
      Serial.println(" ");
      //Store the central stick positions
      center_channel_1 = receiver_input_channel_1;
      center_channel_2 = receiver_input_channel_2;
      center_channel_3 = receiver_input_channel_3;
      center_channel_4 = receiver_input_channel_4;
      Serial.println(F(""));
      Serial.println(F("Center positions stored."));
      Serial.print(F("Digital input 08 = "));
      Serial.println(receiver_input_channel_1);
      Serial.print(F("Digital input 09 = "));
      Serial.println(receiver_input_channel_2);
      Serial.print(F("Digital input 10 = "));
      Serial.println(receiver_input_channel_3);
      Serial.print(F("Digital input 11 = "));
      Serial.println(receiver_input_channel_4);
      Serial.println(F(""));
      Serial.println(F(""));

      Serial.println(F("Move the throttle stick to full throttle and back to center"));
      //Check for throttle movement
      check_receiver_inputs(1, error);
      Serial.print(F("Throttle is connected to digital input "));
      Serial.println((channel_3_assign & 0b00000111) + 7);
      if(channel_3_assign & 0b10000000)Serial.println(F("Channel inverted = yes"));
      else Serial.println(F("Channel inverted = no"));
      wait_sticks_zero();
      
      Serial.println(F(""));
      Serial.println(F(""));
      Serial.println(F("Move the roll stick to simulate left wing up and back to center"));
      //Check for throttle movement
      check_receiver_inputs(2, error);
      Serial.print(F("Roll is connected to digital input "));
      Serial.println((channel_1_assign & 0b00000111) + 7);
      if(channel_1_assign & 0b10000000)Serial.println(F("Channel inverted = yes"));
      else Serial.println(F("Channel inverted = no"));
      wait_sticks_zero();
    
      Serial.println(F(""));
      Serial.println(F(""));
      Serial.println(F("Move the pitch stick to simulate nose up and back to center"));
      //Check for throttle movement
      check_receiver_inputs(3, error);
      Serial.print(F("Pitch is connected to digital input "));
      Serial.println((channel_2_assign & 0b00000111) + 7);
      if(channel_2_assign & 0b10000000)Serial.println(F("Channel inverted = yes"));
      else Serial.println(F("Channel inverted = no"));
      wait_sticks_zero();
    
      Serial.println(F(""));
      Serial.println(F(""));
      Serial.println(F("Move the yaw stick to simulate nose right and back to center"));
      //Check for throttle movement
      check_receiver_inputs(4, error);
      Serial.print(F("Yaw is connected to digital input "));
      Serial.println((channel_4_assign & 0b00000111) + 7);
      if(channel_4_assign & 0b10000000)Serial.println(F("Channel inverted = yes"));
      else Serial.println(F("Channel inverted = no"));
      wait_sticks_zero();
    
    };

    void register_min_max(){
      byte zero = 0;
      low_channel_1 = receiver_input_channel_1;
      low_channel_2 = receiver_input_channel_2;
      low_channel_3 = receiver_input_channel_3;
      low_channel_4 = receiver_input_channel_4;
      while(receiver_input_channel_1 < center_channel_1 + 20 && receiver_input_channel_1 > center_channel_1 - 20) delay(250);
      Serial.println(F("Measuring endpoints...."));
      while(zero < 15){
        if(receiver_input_channel_1 < center_channel_1 + 20 && receiver_input_channel_1 > center_channel_1 - 20)zero |= 0b00000001;
        if(receiver_input_channel_2 < center_channel_2 + 20 && receiver_input_channel_2 > center_channel_2 - 20)zero |= 0b00000010;
        if(receiver_input_channel_3 < center_channel_3 + 20 && receiver_input_channel_3 > center_channel_3 - 20)zero |= 0b00000100;
        if(receiver_input_channel_4 < center_channel_4 + 20 && receiver_input_channel_4 > center_channel_4 - 20)zero |= 0b00001000;
        if(receiver_input_channel_1 < low_channel_1)low_channel_1 = receiver_input_channel_1;
        if(receiver_input_channel_2 < low_channel_2)low_channel_2 = receiver_input_channel_2;
        if(receiver_input_channel_3 < low_channel_3)low_channel_3 = receiver_input_channel_3;
        if(receiver_input_channel_4 < low_channel_4)low_channel_4 = receiver_input_channel_4;
        if(receiver_input_channel_1 > high_channel_1)high_channel_1 = receiver_input_channel_1;
        if(receiver_input_channel_2 > high_channel_2)high_channel_2 = receiver_input_channel_2;
        if(receiver_input_channel_3 > high_channel_3)high_channel_3 = receiver_input_channel_3;
        if(receiver_input_channel_4 > high_channel_4)high_channel_4 = receiver_input_channel_4;
        delay(100);
      }
    };
    
    void check_to_continue(){
      byte continue_byte = 0;
      while(continue_byte == 0){
        if(channel_2_assign == 0b00000001 && receiver_input_channel_1 > center_channel_1 + 150)continue_byte = 1;
        if(channel_2_assign == 0b10000001 && receiver_input_channel_1 < center_channel_1 - 150)continue_byte = 1;
        if(channel_2_assign == 0b00000010 && receiver_input_channel_2 > center_channel_2 + 150)continue_byte = 1;
        if(channel_2_assign == 0b10000010 && receiver_input_channel_2 < center_channel_2 - 150)continue_byte = 1;
        if(channel_2_assign == 0b00000011 && receiver_input_channel_3 > center_channel_3 + 150)continue_byte = 1;
        if(channel_2_assign == 0b10000011 && receiver_input_channel_3 < center_channel_3 - 150)continue_byte = 1;
        if(channel_2_assign == 0b00000100 && receiver_input_channel_4 > center_channel_4 + 150)continue_byte = 1;
        if(channel_2_assign == 0b10000100 && receiver_input_channel_4 < center_channel_4 - 150)continue_byte = 1;
        delay(100);
      }
      wait_sticks_zero();
    };
   
    //Check if a receiver input value is changing within 30 seconds
    void check_receiver_inputs(byte movement, uint8_t* error){
      byte trigger = 0;
      int pulse_length;
      timer = millis() + 30000;
      while(timer > millis() && trigger == 0){
        delay(250);
        if(receiver_input_channel_1 > 1750 || receiver_input_channel_1 < 1250){
          trigger = 1;
          receiver_check_byte |= 0b00000001;
          pulse_length = receiver_input_channel_1;
        }
        if(receiver_input_channel_2 > 1750 || receiver_input_channel_2 < 1250){
          trigger = 2;
          receiver_check_byte |= 0b00000010;
          pulse_length = receiver_input_channel_2;
        }
        if(receiver_input_channel_3 > 1750 || receiver_input_channel_3 < 1250){
          trigger = 3;
          receiver_check_byte |= 0b00000100;
          pulse_length = receiver_input_channel_3;
        }
        if(receiver_input_channel_4 > 1750 || receiver_input_channel_4 < 1250){
          trigger = 4;
          receiver_check_byte |= 0b00001000;
          pulse_length = receiver_input_channel_4;
        } 
      }
      if(trigger == 0){
        *error = 1;
        Serial.println(F("No stick movement detected in the last 30 seconds!!! (ERROR 2)"));
      }
      //Assign the stick to the function.
      else{
        if(movement == 1){
          channel_3_assign = trigger;
          if(pulse_length < 1250)channel_3_assign += 0b10000000;
        }
        if(movement == 2){
          channel_1_assign = trigger;
          if(pulse_length < 1250)channel_1_assign += 0b10000000;
        }
        if(movement == 3){
          channel_2_assign = trigger;
          if(pulse_length < 1250)channel_2_assign += 0b10000000;
        }
        if(movement == 4){
          channel_4_assign = trigger;
          if(pulse_length < 1250)channel_4_assign += 0b10000000;
        }
      }
    }

    //Check if the transmitter sticks are in the neutral position
    void wait_sticks_zero(){
      byte zero = 0;
      while(zero < 15){
        if(receiver_input_channel_1 < center_channel_1 + 20 && receiver_input_channel_1 > center_channel_1 - 20)zero |= 0b00000001;
        if(receiver_input_channel_2 < center_channel_2 + 20 && receiver_input_channel_2 > center_channel_2 - 20)zero |= 0b00000010;
        if(receiver_input_channel_3 < center_channel_3 + 20 && receiver_input_channel_3 > center_channel_3 - 20)zero |= 0b00000100;
        if(receiver_input_channel_4 < center_channel_4 + 20 && receiver_input_channel_4 > center_channel_4 - 20)zero |= 0b00001000;
        delay(100);
      }
    }
};



//This routine is called every time input 8, 9, 10 or 11 changed state
ISR(PCINT0_vect){
  current_time = micros();
  //Channel 1=========================================
  if(PINB & B00000010){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
      last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    }
  }
  else if(last_channel_1 == 1){                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    receiver_input_channel_1 = current_time - timer_1;         //Channel 1 is current_time - timer_1
  }
  //Channel 2=========================================
  if(PINB & B00000100 ){                                       //Is input 9 high?
    if(last_channel_2 == 0){                                   //Input 9 changed from 0 to 1
      last_channel_2 = 1;                                      //Remember current input state
      timer_2 = current_time;                                  //Set timer_2 to current_time
    }
  }
  else if(last_channel_2 == 1){                                //Input 9 is not high and changed from 1 to 0
    last_channel_2 = 0;                                        //Remember current input state
    receiver_input_channel_2 = current_time - timer_2;         //Channel 2 is current_time - timer_2
  }
  //Channel 3=========================================
  if(PINB & B00001000 ){                                       //Is input 10 high?
    if(last_channel_3 == 0){                                   //Input 10 changed from 0 to 1
      last_channel_3 = 1;                                      //Remember current input state
      timer_3 = current_time;                                  //Set timer_3 to current_time
    }
  }
  else if(last_channel_3 == 1){                                //Input 10 is not high and changed from 1 to 0
    last_channel_3 = 0;                                        //Remember current input state
    receiver_input_channel_3 = current_time - timer_3;         //Channel 3 is current_time - timer_3

  }
  //Channel 4=========================================
  if(PINB & B00010000 ){                                       //Is input 11 high?
    if(last_channel_4 == 0){                                   //Input 11 changed from 0 to 1
      last_channel_4 = 1;                                      //Remember current input state
      timer_4 = current_time;                                  //Set timer_4 to current_time
    }
  }
  else if(last_channel_4 == 1){                                //Input 11 is not high and changed from 1 to 0
    last_channel_4 = 0;                                        //Remember current input state
    receiver_input_channel_4 = current_time - timer_4;         //Channel 4 is current_time - timer_4
  }
}
