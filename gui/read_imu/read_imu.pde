import processing.serial.*;
import processing.video.*;

Capture video;
Serial drone;
float inBytes[] = new float[11];

PrintWriter output;
void setup() {
 size(640,480);
 background(255);
  //println(Serial.list());
  while (Serial.list().length<2) {
  }
  drone = new Serial(this, "/dev/ttyACM0", 57600);
  output = createWriter("sensors.txt"); 
  
  video = new Capture(this, 640, 480);
  video.start();
}

boolean running = false;
boolean resteted = false;

void draw() {
 
  
  
    // convert to an int and map to the screen height:
    if(resteted){
    if(running){
    if (counter > 10){
      if (video.available()) {
        video.read();
        image(video, 0, 0);
        f(inString != null) output.print(inString);
        saveFrame("video/record_#####.jpg");
      }
      if(inString != null){
        
            print(inString); //<>//
      }
    }
    
    }
    counter++;
    }
    
      
  
}

boolean newdata = false;
String inString; //<>//
int counter = 0;

void serialEvent (Serial myPort) {
  // get the ASCII string:
  inString = myPort.readStringUntil('\n');
  newdata = true;
  
  if (inString != null) {
    if (inString.contains("Start")){
      resteted = true;
      return;
    }
    if (inString.contains("Check Setup")){
      return;
    }
    if (inString.contains("Finished")) {
      myPort.write("g");
      println("Send");
      return;
    }
    if (inString.contains("Read EEPROM"))
      return;
    if (inString.contains("Receiver")) {
      println("Check for Receiver");
      return;
    }
    // trim off any whitespace:
    //inString = trim(inString);
  if(resteted){
    output.print(inString);
    String[] inStrings = split(inString, '\t');
    
    for ( int i = 0; i < inStrings.length; i++) {
      String s = inStrings[i];
      Double v;
      switch(i) {
      case 0:

        // Time
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.intValue();
        }
        break;
      case 1:
        // Gyro X
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 2:
        // Gyro Y
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 3:
        // Gyro Z
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 4:
        // Acc X
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 5:
        // Acc Y
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 6:
        // Acc Z
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 7:
        // Magnet X
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 8:
        // Magnet Y
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 9:
        // Magnet Z
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 10:
        // Temperature
        inBytes[i] = float(trim(s));
        running = true;
        break;
      default:


        break;
      }
      
    }
  }
  
  }
}

void keyPressed() {
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  exit(); // Stops the program
}
