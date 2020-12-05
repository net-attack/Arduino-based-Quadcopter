
#include <Servo.h> // Die nette Library, die uns das Rechnen ersparrt
#include "Interface.h"
#include "Engine.h"
#include "Receiver.h"


#define debug 

int value = 25;




Interface *interface;
Engine *engine;
Receiver *receiver;


int state = 0;


void setup(){
  #ifdef debug
  Serial.begin(9600); 
  while(!Serial.available()) {
    Serial.println("Waiting for input ....");
  }
  #endif
}

void loop() {
  if( state == 0){
    #ifdef debug
    Serial.print("Init of the arduino controller ....");
    #endif
    engine = new Engine();
    interface = new Interface(25);
    receiver = new Receiver();
    #ifdef debug
    Serial.println(" finished!");
    #endif
    state = 1;
  }else{
    int value = interface->checkInput();
    int value_a; 
    int value_b;
    int value_c;   //throttle
    int value_d;
    receiver->checkInput(&value_a, &value_b, &value_c, &value_d );
    #ifdef debug
    Serial.print(value_a);
    Serial.print("\t");
    Serial.print(value_b);
    Serial.print("\t");
    Serial.print(value_c);
    Serial.print("\t");
    Serial.print(value_d);
    Serial.print("\n");
    #endif
    value = map(value_c,960,2000,0,180);
    engine->updateEngine(value);
  }
 
}
