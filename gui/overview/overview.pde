import processing.serial.*;

Serial drone;
int xPos = 1;
float inBytes[] = new float[10];
float inBytesCalibration[] = new float[10];

void setup(){
  fullScreen();
  background(255);
  //println(Serial.list());
  while(Serial.list().length<2){
  }
  drone = new Serial(this, Serial.list()[1], 57600);
  
  // don't generate a serialEvent() unless you get a newline character:
  drone.bufferUntil('\n');
}

int magnet_min = -100;
int magnet_max = 100;
int acc_min = -30;
int acc_max = 30;
int gyro_min = -10;
int gyro_max = 10;
int temp_min = -100;
int temp_max = 100;
int numberPlots = 10;
int sizeCircle = 1;

boolean newdata = false;

boolean calibrated = false;


float rollcalibration = 0;
float pitchcalibration = 0;
void draw(){
  boolean on = false;
  for(int i=0; i < numberPlots; i++){
    String name = "";
    float value = 0;
    switch(i){
      case 0:
        // Gyro X
        name = "Gyro X";
        value = map(inBytes[i], gyro_min, gyro_max, 0, height);
        break;
      case 1:
        // Gyro Y
        name = "Gyro Y";
        value = map(inBytes[i], gyro_min, gyro_max, 0, height);
        break;
      case 2:
        // Gyro Z
        name = "Gyro Z";
        value = map(inBytes[i], gyro_min, gyro_max, 0, height);
        break;
      case 3:
        // Acc X
        name = "Acc X";
        value = map(inBytes[i], acc_min, acc_max, 0, height);
        break;
      case 4:
        // Acc Y
        name = "Acc Y";
        value = map(inBytes[i], acc_min, acc_max, 0, height);
        break;
      case 5:
        // Acc Z
        name = "Acc Z";
        value = map(inBytes[i], acc_min, acc_max, 0, height);
        break;
      case 6:
        // Magnet X
        name = "Magneto X";
        value = map(inBytes[i], magnet_min, magnet_max, 0, height);
        break;
      case 7:
        // Magnet Y
        name = "Magneto Y";
        value = map(inBytes[i], magnet_min, magnet_max, 0, height);
        break;
      case 8:
        // Magnet Z
        name = "Magneto Z";
        value = map(inBytes[i], magnet_min, magnet_max, 0, height);
        break;
      case 9:
        // Temperature
        name = "Temperature";
        
        value = map(inBytes[i], temp_min, temp_max, 0, height);
        break;
      default:
        name = "";
        break;
    }
    if (newdata & inBytes[0] != 0) on = true;
    stroke(0);
    textSize(10);
    fill(0);
    text(name, 0, 10 + i*height/numberPlots);
    stroke(127, 0, 5);
    circle(xPos, (height - value)/numberPlots+ i*height/numberPlots, sizeCircle);
  }  
  
  
  if(on & !calibrated){
    println("Start Calibration");
    int count = 20;
    int i= 0;
    while(i < count){
      if(newdata){
        float acc_x = inBytes[3];
        float acc_y = inBytes[4];
        float acc_z = inBytes[5];
        for(int j = 0; j < 10; j++){
          inBytesCalibration[j] += inBytes[j]; 
        }
        float roll = atan(acc_y / sqrt(pow(acc_x, 2) + pow(acc_z, 2)));
        float pitch = atan(-1 * acc_x / sqrt(pow(acc_y, 2) + pow(acc_z, 2)));
        pitchcalibration += pitch;
        rollcalibration += pitch;
        newdata = false;
        i += 1;
      }
      
    }
    for(int j = 0; j < 10; j++){
      inBytesCalibration[j] = inBytesCalibration[j]/count;
    }
    pitchcalibration /= count;
    rollcalibration /= count;
    println("End Calibration");
    calibrated = true;
  }
  if(calibrated){
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

void drawCompass(){
  float acc_x = inBytes[3];
  float acc_y = inBytes[4];
  float acc_z = inBytes[5];
  //println(acc_x + " " + acc_y + " " + acc_z);
  float mag_x = inBytes[6];
  float mag_y = inBytes[7];
  float mag_z = inBytes[8];
  
  float roll = atan(acc_y / sqrt(pow(acc_x, 2) + pow(acc_z, 2))) + rollcalibration;
  float pitch = atan(-1 * acc_x / sqrt(pow(acc_y, 2) + pow(acc_z, 2))) + pitchcalibration;
  fill(255);
  stroke(255);
  rect(width/2,0,width/2,height);
  fill(0);
  stroke(0);
  circle(width/2 + width/4, height/2, width/3);
  float x = width/2 + width/4;
  float y = height/2 + width/6 + 100;
  
  strokeWeight(10);
  //line(x, y - sin(pitch)*width/12 - 10, x,  y + sin(pitch)*width/12 + 10);
  //line(x - cos(roll)*width/6, y - sin(roll)*width/6, x + cos(roll)*width/6,  y + sin(roll)*width/6);
  
  line(x - cos(roll)*width/6, y - sin(pitch)*width/12 - sin(roll)*width/12, x + cos(roll)*width/6,  y + sin(pitch)*width/12 + sin(roll)*width/12);
  line(x - cos(roll)*width/6, y + sin(pitch)*width/12 + sin(roll)*width/12, x + cos(roll)*width/6,  y - sin(pitch)*width/12 - sin(roll)*width/12);
  strokeWeight(2);
  circle(x - cos(roll)*width/6, y - sin(pitch)*width/12  - sin(roll)*width/12, width/24);
  circle(x + cos(roll)*width/6, y - sin(pitch)*width/12 - sin(roll)*width/12, width/24);
  stroke(255,0,0);
  fill(255,0,0);
  circle(x - cos(roll)*width/6, y + sin(pitch)*width/12  + sin(roll)*width/12, width/24);
  stroke(0,255,0);
  fill(0,255,0);
  circle(x + cos(roll)*width/6, y + sin(pitch)*width/12  + sin(roll)*width/12, width/24);
  fill(0);
  strokeWeight(1);
  stroke(255,0,0);
  float alpha = atan2(mag_y, mag_x);
  //alpha = alpha /360 * 2 * 3.1415;
  float x1 = width/2 + width/4;
  float x2 = width/2 + width/4 + width/6 * cos(alpha);
  float y1 = height/2;
  float y2 =  height/2 - width/6 * sin(alpha);
  
  line(x1, y1, x2, y2); 
  line(x2, y2, x2 - width/36 * sin(alpha + 3.1415/4), y2 - width/36*cos(alpha + 3.1415/4)); 
  line(x2, y2, x2 - width/36 * cos(alpha + 3.1415/4), y2 + width/36*sin(alpha + 3.1415/4)); 

}



void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');
  newdata = true;
  //println(inString);
  if (inString != null) {
    if(inString.contains("Start"))
      return;
    if(inString.contains("Finished")){
      myPort.write("g");
      println("Send");
      return;
    }
    if(inString.contains("Read EEPROM"))
      return;
    if(inString.contains("Receiver")){
      println("Check for Receiver");
      return;
    }
    // trim off any whitespace:
    //inString = trim(inString);
    
    String[] inStrings = split(inString, '\t');
    for( int i = 0; i < inStrings.length; i++){
      String s = inStrings[i];
      switch(i){
        case 0:
          
          // Gyro X
          inBytes[i] = float(trim(s));
          break;
        case 1:
          // Gyro Y
          inBytes[i] = float(trim(s));
          break;
        case 2:
          // Gyro Z
          inBytes[i] = float(trim(s));
          break;
        case 3:
          // Acc X
          inBytes[i] = float(trim(s));
          break;
        case 4:
          // Acc Y
          inBytes[i] = float(trim(s));
          break;
        case 5:
          // Acc Z
          inBytes[i] = float(trim(s));
          break;
        case 6:
          // Magnet X
          inBytes[i] = float(trim(s));
          break;
        case 7:
          // Magnet Y
          inBytes[i] = float(trim(s));
          break;
        case 8:
          // Magnet Z
          inBytes[i] = float(trim(s));
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
