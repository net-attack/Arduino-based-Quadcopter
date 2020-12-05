#define pin_engine_fl 6
#define pin_engine_fr 5
#define pin_engine_rl 10
#define pin_engine_rr 11


class Engine
{
  public:
    Engine(){
      initEngines();
    };

    void updateEngine(int value){
       engine_fl.write(value);
       engine_fr.write(value);
       engine_rl.write(value);
       engine_rr.write(value);
    };

    void initEngines(){
       #ifdef debug
       Serial.print("\tInit of the engine controller .... ");
       #endif
       engine_fl.attach(pin_engine_fl); 
       engine_fr.attach(pin_engine_fr); 
       engine_rl.attach(pin_engine_rl); 
       engine_rr.attach(pin_engine_rr); 
       engine_fl.write(initValue);
       engine_fr.write(initValue);
       engine_rl.write(initValue);
       engine_rr.write(initValue);
       #ifdef debug
       Serial.println("finished!");
       #endif
    };
    
  private:
    Servo engine_fl;
    Servo engine_fr; 
    Servo engine_rl;
    Servo engine_rr;
    int initValue = 25;

};
