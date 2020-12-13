#include "MPU9250.h"

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
byte roll_axis, pitch_axis, yaw_axis;
byte gyro_check_byte;



class Gyro
{
  public:
    float gyro_pitch;
    float gyro_roll;
    float gyro_yaw;
    
    float acc_x;
    float acc_y;
    float acc_z;
    
    float gyro_roll_cal;
    float gyro_pitch_cal;
    float gyro_yaw_cal;
    Receiver* receiver;

    Gyro(Receiver* receiver){
        receiver = receiver;
        // start communication with IMU 
        int status = IMU.begin();
        if (status < 0) {
            Serial.println("IMU initialization unsuccessful");
            Serial.println("Check IMU wiring or try cycling power");
            Serial.print("Status: ");
            Serial.println(status);
            while(1) {}
        }
    };

    void configure(uint8_t* error){
        Serial.println(F("==================================================="));
        Serial.println(F("Gyro axes configuration"));
        Serial.println(F("==================================================="));
        
        //Detect the left wing up movement
        Serial.println(F("Lift the left side of the quadcopter to a 45 degree angle within 10 seconds"));
        //Check axis movement
        check_gyro_axes(1, error);
        if(*error == 0){
            Serial.println(F("OK!"));
            Serial.print(F("Angle detection = "));
            Serial.println(roll_axis & 0b00000011);
            if(roll_axis & 0b10000000)Serial.println(F("Axis inverted = yes"));
            else Serial.println(F("Axis inverted = no"));
            Serial.println(F("Put the quadcopter back in its original position"));
            Serial.println(F("Move stick 'nose up' and back to center to continue"));
            receiver->check_to_continue();

            //Detect the nose up movement
            Serial.println(F(""));
            Serial.println(F(""));
            Serial.println(F("Lift the nose of the quadcopter to a 45 degree angle within 10 seconds"));
            //Check axis movement
            check_gyro_axes(2, error);
        }
        if(*error == 0){
            Serial.println(F("OK!"));
            Serial.print(F("Angle detection = "));
            Serial.println(pitch_axis & 0b00000011);
            if(pitch_axis & 0b10000000)Serial.println(F("Axis inverted = yes"));
            else Serial.println(F("Axis inverted = no"));
            Serial.println(F("Put the quadcopter back in its original position"));
            Serial.println(F("Move stick 'nose up' and back to center to continue"));
            receiver->check_to_continue();
            
            //Detect the nose right movement
            Serial.println(F(""));
            Serial.println(F(""));
            Serial.println(F("Rotate the nose of the quadcopter 45 degree to the right within 10 seconds"));
            //Check axis movement
            check_gyro_axes(3, error);
        }
        if(*error == 0){
            Serial.println(F("OK!"));
            Serial.print(F("Angle detection = "));
            Serial.println(yaw_axis & 0b00000011);
            if(yaw_axis & 0b10000000)Serial.println(F("Axis inverted = yes"));
            else Serial.println(F("Axis inverted = no"));
            Serial.println(F("Put the quadcopter back in its original position"));
            Serial.println(F("Move stick 'nose up' and back to center to continue"));
            receiver->check_to_continue();
        }
    };

    void calibrate(uint8_t* error){
        delay(3000);
        Serial.println(F(""));
        Serial.println(F("==================================================="));
        Serial.println(F("Gyro calibration"));
        Serial.println(F("==================================================="));
        Serial.println(F("Don't move the quadcopter!! Calibration starts in 3 seconds"));
        delay(3000);
        Serial.println(F("Calibrating the gyro, this will take +/- 8 seconds"));
        Serial.print(F("Please wait"));
        //Let's take multiple gyro data samples so we can determine the average gyro offset (calibration).
        for (int cal_int = 0; cal_int < 2000 ; cal_int ++){              //Take 2000 readings for calibration.
        if(cal_int % 100 == 0)Serial.print(F("."));                //Print dot to indicate calibration.
        gyro_signalen();                                           //Read the gyro output.
        gyro_roll_cal += gyro_roll;                                //Ad roll value to gyro_roll_cal.
        gyro_pitch_cal += gyro_pitch;                              //Ad pitch value to gyro_pitch_cal.
        gyro_yaw_cal += gyro_yaw;                                  //Ad yaw value to gyro_yaw_cal.
        delay(4);                                                  //Wait 3 milliseconds before the next loop.
        }
        //Now that we have 2000 measures, we need to devide by 2000 to get the average gyro offset.
        gyro_roll_cal /= 2000;                                       //Divide the roll total by 2000.
        gyro_pitch_cal /= 2000;                                      //Divide the pitch total by 2000.
        gyro_yaw_cal /= 2000;                                        //Divide the yaw total by 2000.
        
        //Show the calibration results
        Serial.println(F(""));
        Serial.print(F("Axis 1 offset="));
        Serial.println(gyro_roll_cal);
        Serial.print(F("Axis 2 offset="));
        Serial.println(gyro_pitch_cal);
        Serial.print(F("Axis 3 offset="));
        Serial.println(gyro_yaw_cal);
        Serial.println(F(""));
    };

    //Check if the angular position of a gyro axis is changing within 10 seconds
    void check_gyro_axes(byte movement, uint8_t* error){
        byte trigger_axis = 0;
        float gyro_angle_roll, gyro_angle_pitch, gyro_angle_yaw;
        //Reset all axes
        gyro_angle_roll = 0;
        gyro_angle_pitch = 0;
        gyro_angle_yaw = 0;
        gyro_signalen();
        timer = millis() + 10000;    
        int type = 2;
        while(timer > millis() && gyro_angle_roll > -30 && gyro_angle_roll < 30 && gyro_angle_pitch > -30 && gyro_angle_pitch < 30 && gyro_angle_yaw > -30 && gyro_angle_yaw < 30){
            gyro_signalen();
            //print_gyro();
            if(type == 2 || type == 3){
            gyro_angle_roll += gyro_roll;// * 0.00007;              //0.00007 = 17.5 (md/s) / 250(Hz)
            gyro_angle_pitch += gyro_pitch;// * 0.00007;
            gyro_angle_yaw += gyro_yaw;// * 0.00007;
            }
            if(type == 1){
            gyro_angle_roll += gyro_roll * 0.0000611;          // 0.0000611 = 1 / 65.5 (LSB degr/s) / 250(Hz)
            gyro_angle_pitch += gyro_pitch * 0.0000611;
            gyro_angle_yaw += gyro_yaw * 0.0000611;
            }
            
            
            delayMicroseconds(3700); //Loop is running @ 250Hz. +/-300us is used for communication with the gyro
        }
        //Assign the moved axis to the orresponding function (pitch, roll, yaw)
        if((gyro_angle_roll < -30 || gyro_angle_roll > 30) && gyro_angle_pitch > -30 && gyro_angle_pitch < 30 && gyro_angle_yaw > -30 && gyro_angle_yaw < 30){
            gyro_check_byte |= 0b00000001;
            if(gyro_angle_roll < 0)trigger_axis = 0b10000001;
            else trigger_axis = 0b00000001;
        }
        if((gyro_angle_pitch < -30 || gyro_angle_pitch > 30) && gyro_angle_roll > -30 && gyro_angle_roll < 30 && gyro_angle_yaw > -30 && gyro_angle_yaw < 30){
            gyro_check_byte |= 0b00000010;
            if(gyro_angle_pitch < 0)trigger_axis = 0b10000010;
            else trigger_axis = 0b00000010;
        }
        if((gyro_angle_yaw < -30 || gyro_angle_yaw > 30) && gyro_angle_roll > -30 && gyro_angle_roll < 30 && gyro_angle_pitch > -30 && gyro_angle_pitch < 30){
            gyro_check_byte |= 0b00000100;
            if(gyro_angle_yaw < 0)trigger_axis = 0b10000011;
            else trigger_axis = 0b00000011;
        }
        
        if(trigger_axis == 0){
            *error = 1;
            Serial.println(F("No angular motion is detected in the last 10 seconds!!! (ERROR 4)"));
        }
        else
        if(movement == 1)roll_axis = trigger_axis;
        if(movement == 2)pitch_axis = trigger_axis;
        if(movement == 3)yaw_axis = trigger_axis;
    
    };

    void gyro_signalen(){
        IMU.readSensor();
        gyro_pitch = IMU.getGyroY_rads();
        gyro_roll = IMU.getGyroX_rads();
        gyro_yaw = IMU.getGyroZ_rads();
        
    };

    void acc_signalen(){
        IMU.readSensor();
        acc_x = IMU.getAccelX_mss();
        acc_y= IMU.getAccelY_mss();
        acc_z= IMU.getAccelZ_mss();
        
    };

    void print_gyro(){
        Serial.print(gyro_pitch,6);
        Serial.print("\t");
        Serial.print(gyro_roll,6);
        Serial.print("\t");
        Serial.print(gyro_yaw,6);
        Serial.print("\n");
    };

    void read(){
        // read the sensor
        IMU.readSensor();
        // display the data
        Serial.print(IMU.getAccelX_mss(),6);
        Serial.print("\t");
        Serial.print(IMU.getAccelY_mss(),6);
        Serial.print("\t");
        Serial.print(IMU.getAccelZ_mss(),6);
        Serial.print("\t");
        Serial.print(IMU.getGyroX_rads(),6);
        Serial.print("\t");
        Serial.print(IMU.getGyroY_rads(),6);
        Serial.print("\t");
        Serial.print(IMU.getGyroZ_rads(),6);
        Serial.print("\t");
        Serial.print(IMU.getMagX_uT(),6);
        Serial.print("\t");
        Serial.print(IMU.getMagY_uT(),6);
        Serial.print("\t");
        Serial.print(IMU.getMagZ_uT(),6);
        Serial.print("\t");
        Serial.println(IMU.getTemperature_C(),6);
    };

};
