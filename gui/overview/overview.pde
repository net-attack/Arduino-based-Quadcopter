import processing.serial.*;

Serial drone;
int xPos = 1;
float inBytes[] = new float[19];
float inBytesCalibration[] = new float[19];

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

int magnet_min = -100;
int magnet_max = 100;

int receiver_min = 1000;
int receiver_max = 2000;
int acc_min = -30;
int acc_max = 30;
int gyro_min = -10;
int gyro_max = 10;
int temp_min = -100;
int temp_max = 100;
int numberPlots = 13;
int sizeCircle = 1;

boolean newdata = false;

boolean calibrated = false;


float rollcalibration = 0;
float pitchcalibration = 0;
float headingCalibration = 0;
void draw() {
  boolean on = false;
  for (int i=0; i < numberPlots; i++) {
    String name = "";
    float value = 0;
    switch(i) {
    case 0:
      // Gyro X
      name = "Gyro X";
      value = map(inBytes[i+1], gyro_min, gyro_max, 0, height);
      break;
    case 1:
      // Gyro Y
      name = "Gyro Y";
      value = map(inBytes[i+1], gyro_min, gyro_max, 0, height);
      break;
    case 2:
      // Gyro Z
      name = "Gyro Z";
      value = map(inBytes[i+1], gyro_min, gyro_max, 0, height);
      break;
    case 3:
      // Acc X
      name = "Acc X";
      value = map(inBytes[i+1], acc_min, acc_max, 0, height);
      break;
    case 4:
      // Acc Y
      name = "Acc Y";
      value = map(inBytes[i+1], acc_min, acc_max, 0, height);
      break;
    case 5:
      // Acc Z
      name = "Acc Z";
      value = map(inBytes[i+1], acc_min, acc_max, 0, height);
      break;
    case 6:
      // Magnet X
      name = "Magneto X";
      value = map(inBytes[i+1], magnet_min, magnet_max, 0, height);
      break;
    case 7:
      // Magnet Y
      name = "Magneto Y";
      value = map(inBytes[i+1], magnet_min, magnet_max, 0, height);
      break;
    case 8:
      // Magnet Z
      name = "Magneto Z";
      value = map(inBytes[i+1], magnet_min, magnet_max, 0, height);
      break;
    case 9:
      // Receiver 1
      name = "Receiver 1";
      value = map(inBytes[i+1], receiver_min, receiver_max, 0, height);
      break;
    case 10:
      // Receiver 2
      name = "Receiver 2";
      value = map(inBytes[i+1], receiver_min, receiver_max, 0, height);
      break;
    case 11:
      // Receiver 3
      name = "Receiver 3";
      value = map(inBytes[i+1], receiver_min, receiver_max, 0, height);
      break;
    case 12:
      // Receiver 4
      name = "Receiver 4";
      value = map(inBytes[i+1], receiver_min, receiver_max, 0, height);
      break;
    case 13:
      // ESC 1
      name = "ESC 1";
      value = map(inBytes[i+1], receiver_min, receiver_max, 0, height);
      break;
    case 14:
      // ESC 2
      name = "ESC 2";
      value = map(inBytes[i+1], receiver_min, receiver_max, 0, height);
      break;
    case 15:
      // ESC 3
      name = "ESC 3";
      value = map(inBytes[i+1], receiver_min, receiver_max, 0, height);
      break;
    case 16:
      // ESC 4
      name = "ESC 4";
      value = map(inBytes[i+1], receiver_min, receiver_max, 0, height);
      break;
    case 17:
      // Temperature
      name = "Temperature";
      value = map(inBytes[i+1], receiver_min, temp_max, 0, height);
      break;
    default:
      name = "";
      break;
    }
    if (newdata & inBytes[1] != 0) on = true;
    stroke(0);
    textSize(10);
    fill(0);
    text(name, 0, 10 + i*height/numberPlots);
    stroke(127, 0, 5);
    circle(xPos, (height - value)/numberPlots+ i*height/numberPlots, sizeCircle);
  }  


  if (on & !calibrated) {
    println("Start Calibration");
    int count = 2000;
    int i= 0;
    while (i < count) {
      if (newdata) {
        float acc_x = inBytes[3];
        float acc_y = inBytes[4];
        float acc_z = inBytes[5];

        float mag_x = inBytes[6];
        float mag_y = inBytes[7];
        float mag_z = inBytes[8];
        float alpha = atan2(mag_y, mag_x);


        for (int j = 0; j < 10; j++) {
          inBytesCalibration[j] += inBytes[j];
        }
        float roll = atan(acc_y / sqrt(pow(acc_x, 2) + pow(acc_z, 2)));
        float pitch = atan(-1 * acc_x / sqrt(pow(acc_y, 2) + pow(acc_z, 2)));
        pitchcalibration += pitch;
        rollcalibration += roll;
        headingCalibration += alpha;

        newdata = false;
        i += 1;
      }
    }
    for (int j = 0; j < 10; j++) {
      inBytesCalibration[j] = inBytesCalibration[j]/count;
    }
    pitchcalibration /= count;
    rollcalibration /= count;
    headingCalibration /= count;
    println("End Calibration");
    calibrated = true;
  }
  if (calibrated) {
    drawCompass();
  }


  // at the edge of the screen, go back to the beginning:
  if (xPos >= width/2) {
    xPos = 0;
    background(255);
  } else {
    // increment the horizontal position:
    xPos++;
  }
}

float minMag = 0;
float maxMag = 0;
float heading = 0;
void drawCompass() {
  float acc_x = inBytes[3];
  float acc_y = inBytes[4];
  float acc_z = inBytes[5];
  //println(acc_x + " " + acc_y + " " + acc_z);


  float roll = atan(acc_y / sqrt(pow(acc_x, 2) + pow(acc_z, 2))) - rollcalibration;
  float pitch = atan(-1 * acc_x / sqrt(pow(acc_y, 2) + pow(acc_z, 2))) - pitchcalibration;

  float mag_x = inBytes[6];
  float mag_y = inBytes[7];
  float mag_z = inBytes[8];
  float alpha = -atan2(mag_y, mag_x) + headingCalibration;
  heading = lerp(heading, alpha, 0.5);
  fill(255);
  stroke(255);
  rect(width/2, 0, width/2, height);
  fill(0);
  stroke(0);
  text("HEADING: " + heading/2/3.1415*360, width*3/4, height/6);
  circle(width/2 + width/4, height/2, width/3);
  float x = width/2 + width/4;
  float y = height/2 + width/6 + 100;

  strokeWeight(10);
  //line(x, y - sin(pitch)*width/12 - 10, x,  y + sin(pitch)*width/12 + 10);
  //line(x - cos(roll)*width/6, y - sin(roll)*width/6, x + cos(roll)*width/6,  y + sin(roll)*width/6);

  line(x - cos(roll)*width/6, y - sin(pitch)*width/12 - sin(roll)*width/12, x + cos(roll)*width/6, y + sin(pitch)*width/12 + sin(roll)*width/12);
  line(x - cos(roll)*width/6, y + sin(pitch)*width/12 - sin(roll)*width/12, x + cos(roll)*width/6, y - sin(pitch)*width/12 + sin(roll)*width/12);


  strokeWeight(2);
  circle(x - cos(roll)*width/6, y - sin(pitch)*width/12  - sin(roll)*width/12, width/24);
  circle(x + cos(roll)*width/6, y - sin(pitch)*width/12 + sin(roll)*width/12, width/24);

  stroke(255, 0, 0);
  fill(255, 0, 0);
  circle(x - cos(roll)*width/6, y + sin(pitch)*width/12  - sin(roll)*width/12, width/24);
  stroke(0, 255, 0);
  fill(0, 255, 0);
  circle(x + cos(roll)*width/6, y + sin(pitch)*width/12  + sin(roll)*width/12, width/24);



  fill(0);
  strokeWeight(1);
  stroke(255, 0, 0);

  //alpha = alpha /360 * 2 * 3.1415;
  float x1 = width/2 + width/4;
  float x2 = width/2 + width/4 + width/6 * cos(heading+ 3.1415/2);
  float y1 = height/2;
  float y2 =  height/2 - width/6 * sin(heading+ 3.1415/2);

  line(x1, y1, x2, y2); 
  line(x2, y2, x2 - width/36 * sin(heading + 3.1415/4 + 3.1415/2), y2 - width/36*cos(heading + 3.1415/4+ 3.1415/2)); 
  line(x2, y2, x2 - width/36 * cos(heading + 3.1415/4+ 3.1415/2), y2 + width/36*sin(heading + 3.1415/4+ 3.1415/2)); 


  if (inBytes[6] < minMag) minMag = inBytes[6];
  if (inBytes[7] < minMag) minMag = inBytes[7];
  if (inBytes[8] < minMag) minMag = inBytes[8];
  if (inBytes[6] > maxMag) maxMag = inBytes[6];
  if (inBytes[7] > maxMag) maxMag = inBytes[7];
  if (inBytes[8] > maxMag) maxMag = inBytes[8];


  x = floor(map(inBytes[6], minMag, maxMag, -height/8, height/8));
  y = floor(map(inBytes[7], minMag, maxMag, -height/8, height/8));
  int z = floor(map(inBytes[8], minMag, maxMag, -height/8, height/8));
  arrow(x, y, z);
}


float x2 = 0;
float y2 = 0;

void arrow(float x, float y, float z) {
  x2 = lerp(x2, cos(0.5235987756) * (x + y/2), 0.5);
  y2 = lerp(y2, sin(0.5235987756) * (x - y/2) - z, 0.5);  
  float x1 = 0;
  float y1 = 0;
  translate(width*3/4, height/8);
  int l = width/16;
  stroke(200);
  line(0, 0, cos(0.5235987756) * l, sin(0.5235987756) * l); 
  text("X", cos(0.5235987756) * l, sin(0.5235987756) * l);
  line(0, 0, cos(0.5235987756) * l/2, -sin(0.5235987756) * l/2); 
  text("Y", cos(0.5235987756) * l/2, -sin(0.5235987756) * l/2);
  line(0, 0, 0, -l); 
  text("Z", 0, -l);

  x2 = x1 + x2;
  y2 = y1 + y2;
  //println(x,y,z);
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
    String[] inStrings = split(inString, '\n');
    inStrings = split(inStrings[inStrings.length -2], '\t');
    
    if(inStrings.length  != 19) return;
    for ( int i = 0; i < inStrings.length; i++) {
      String s = inStrings[i];
      Double v;
      
      switch(i) {
      case 0:      
        // Time
        int r = int(trim(s));
        inBytes[i] = r;
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
        // Receiver 1
        inBytes[i] = int(trim(s));
        break;
      case 11:
        // Receiver 2
        inBytes[i] = int(trim(s));
        break;
      case 12:
        // Receiver 3
        inBytes[i] = int(trim(s));
        break;
      case 13:
        // Receiver 4
        inBytes[i] = int(trim(s));
        break;
      case 14:
        // ESC 1
        inBytes[i] = int(trim(s));
        break;
      case 15:
        // ESC 2
        inBytes[i] = int(trim(s));
        break;
      case 16:
        // ESC 3
        inBytes[i] = int(trim(s));
        break;
      case 17:
        // ESC 4
        inBytes[i] = int(trim(s));
        break;
      case 18:
        // Temperature
        inBytes[i] = float(trim(s));
        break;
      default:
        inBytes[i] = float(trim(s));
        break;
      }
    }
    // convert to an int and map to the screen height:
    //println(inBytes);
  }
}
