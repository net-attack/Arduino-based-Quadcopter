
import processing.serial.*;
import processing.opengl.*;
import peasy.*;

PeasyCam cam;

Serial drone;
float inBytes[] = new float[10];

void setup() {
  fullScreen();
  background(255);
  //println(Serial.list());
  while (Serial.list().length<2) {
  }
  drone = new Serial(this, Serial.list()[0], 57600);

  // don't generate a serialEvent() unless you get a newline character:
  drone.bufferUntil('\n');
}

float minMag = 0;
float maxMag = 0;
void draw() {
  background(0);
  translate(width/2, height/2);
  stroke(255);
  circle(0,0,10);
  if(inBytes[6] < minMag) minMag = inBytes[6];
  if(inBytes[7] < minMag) minMag = inBytes[7];
  if(inBytes[8] < minMag) minMag = inBytes[8];
  if(inBytes[6] > maxMag) maxMag = inBytes[6];
  if(inBytes[7] > maxMag) maxMag = inBytes[7];
  if(inBytes[8] > maxMag) maxMag = inBytes[8];
  
  int x = floor(map(inBytes[6],minMag, maxMag, -height/8, height/8));
  int y = floor(map(inBytes[7],minMag, maxMag, -height/8, height/8));
  int z = floor(map(inBytes[8],minMag, maxMag, -height/8, height/8));
  arrow(x,y, z);
}
boolean newdata = false;

float x2 = 0;
float y2 = 0;

void arrow(float x, float y, float z) {
  x2 = lerp(x2,cos(0.5235987756) * (x + y/2),0.5);
  y2 = lerp(y2,sin(0.5235987756) * (x - y/2) - z, 0.5);  
  float x1 = 0;
  float y1 = 0;
  
  stroke(200);
  line(0,0,cos(0.5235987756) * width/4,sin(0.5235987756) * width/4); 
  text("X",cos(0.5235987756) * width/4,sin(0.5235987756) * width/4);
  line(0,0,cos(0.5235987756) * width/8,-sin(0.5235987756) * width/8); 
  text("Y",cos(0.5235987756) * width/8,-sin(0.5235987756) * width/8);
  line(0,0,0, -width/4); 
  text("Z",0,-width/4);
  
  stroke(255, 0, 0);
  line(x1, y1, x2, y2);
  pushMatrix();
  translate(x2, y2);
  float a = atan2(x1-x2, y2-y1);
  rotate(a);
  line(0, 0, -10, -10);
  line(0, 0, 10, -10);
  popMatrix();
} 
void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');
  newdata = true;
  //println(inString);
  if (inString != null) {
    if (inString.contains("Start"))
      return;
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

    String[] inStrings = split(inString, '\t');
    for ( int i = 0; i < inStrings.length; i++) {
      String s = inStrings[i];
      Double v;
      switch(i) {
      case 0:

        // Gyro X
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 1:
        // Gyro Y
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 2:
        // Gyro Z
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 3:
        // Acc X
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 4:
        // Acc Y
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 5:
        // Acc Z
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 6:
        // Magnet X
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 7:
        // Magnet Y
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 8:
        // Magnet Z
        v = (double)float(trim(s));
        if (v.isNaN()) {
          inBytes[i] = 0;
        } else {
          inBytes[i] = v.floatValue();
        }
        break;
      case 9:
        // Temperature
        inBytes[i] = float(trim(s));
        break;
      default:
        inBytes[i] = float(trim(s));
        break;
      }
    }
    // convert to an int and map to the screen height:
  }
}
