
class Interface
{
  public:
    Interface(int initValue){
        #ifdef debug
        Serial.print("\tInit of the interface .... ");
        #endif
        value = initValue;
        #ifdef debug
        Serial.println("finished!");
        #endif
        
    };

    
    int checkInput(){
      #ifdef debug
        if (Serial.available() > 0) {
        int inByte = Serial.read();
        switch (inByte) {
          case '+':
            value += 5;
            Serial.println(value);
            break;
          case '-':
            value -= 5;
            Serial.println(value);
            break;
          case ' ':
            value = 25;
            Serial.println(value);
          default:
            Serial.println(value);
            break;
        }
      
        }
   #endif
   return value;
  };

  private:
    int value = 0;
  

};
