#define TIMEOUT 5000
#define channel_a 2
#define channel_b 3
#define channel_c 7
#define channel_d 1



class Receiver
{
  public:
    Receiver(){
        Serial.print("\tInit of the Receiver .... ");
        pinMode(channel_a, INPUT);
        pinMode(channel_b, INPUT);
        pinMode(channel_c, INPUT);
        pinMode(channel_d, INPUT);
        Serial.println("finished!");
    };

    
    int checkInput(int *pwm_value_a, int *pwm_value_b, int *pwm_value_c, int *pwm_value_d ){
      
      *pwm_value_a = pulseIn(channel_a, HIGH);
      *pwm_value_b = pulseIn(channel_b, HIGH);
      *pwm_value_c = pulseIn(channel_c, HIGH);
      //*pwm_value_d = pulseIn(channel_d, HIGH);
   };

  private:
    
    int valueA = 0;
    int valueB = 0;
    int valueC = 0;
    int valueD = 0;

};
