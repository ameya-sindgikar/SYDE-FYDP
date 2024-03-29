import processing.serial.*;

Serial myPort;  // Create object from Serial class
static final int WIDTH = 800;
static final int HEIGHT = 600;

int[] ValX = new int [WIDTH];
int[] ValY = new int [WIDTH];
int[] ValZ = new int [WIDTH];
int[] ValT = new int [WIDTH];
int[] ValPitch = new int [WIDTH];

float scaledX;
float scaledY;
float scaledZ;
float scaledT;

float x;
float y;
float z;
float t;

float pitch;
float roll;

final float alpha = 0.5;

void setup() {
  x = 0;
  y = 0;
  z = 0;
  t = 0;
  
  pitch = 0;
  roll = 0;
  
  scaledX = 0;
  scaledY = 0;
  scaledZ = 0;
  scaledT = 0;
  
  size(800, 600, P3D);
  
  noSmooth();
  
  String portName = Serial.list()[0]; //COM3 for my PC, different for Macs
  println(portName);
  myPort = new Serial(this, portName, 9600);
}

void draw(){
  //shift data
  for (int i=0; i<WIDTH-1; i++){
    ValX[i] = ValX[i+1];
    ValY[i] = ValY[i+1];
    ValZ[i] = ValZ[i+1];
    ValT[i] = ValT[i+1];
    ValPitch[i] = ValPitch[i+1];
    ValX[WIDTH-1] = int(scaledX);
    ValY[WIDTH-1] = int(scaledY);
    ValZ[WIDTH-1] = int(scaledZ);
    ValT[WIDTH-1] = int(scaledT);
    ValPitch[WIDTH-1] = int(pitch);
  }
  
  background(0); //clears screen
  
  //plot
  for (int j=0; j<WIDTH-1; j++){
    stroke(255, 0, 0);
    line (j, ValX[j], j+1, ValX[j+1]);
    stroke(0, 255, 0);
    line (j, ValY[j], j+1, ValY[j+1]);
    stroke(0, 0, 255);
    line (j, ValZ[j], j+1, ValZ[j+1]);
    stroke(255, 255, 255);
    line (j, ValT[j], j+1, ValT[j+1]);
    stroke(255, 255, 0);
    line (j, ValPitch[j], j+1, ValPitch[j+1]);
  }
}

void drawCube(){
  //Set background
  background(0.5);

  pushMatrix(); 
  
  translate(width/2, height/2, -30); 

  //Rotate
  rotateX(pitch*PI/180.0); 
  rotateZ(roll*PI/180.0); 

  //Print data
  print("Pitch: ");
  print(pitch);
  print(", Roll: ");
  println(roll);

  
  scale(90);
  beginShape(QUADS);

  fill(0, 255, 0); vertex(-1,  1,  1);
  fill(0, 255, 0); vertex( 1,  1,  1);
  fill(0, 255, 0); vertex( 1, -1,  1);
  fill(0, 255, 0); vertex(-1, -1,  1);

  fill(0, 255, 255); vertex( 1,  1,  1);
  fill(0, 255, 255); vertex( 1,  1, -1);
  fill(0, 255, 255); vertex( 1, -1, -1);
  fill(0, 255, 255); vertex( 1, -1,  1);


  fill(255, 0, 255); vertex( 1,  1, -1);
  fill(255, 0, 255); vertex(-1,  1, -1);
  fill(255, 0, 255); vertex(-1, -1, -1);
  fill(255, 0, 255); vertex( 1, -1, -1);

  fill(255, 255, 0); vertex(-1,  1, -1);
  fill(255, 255, 0); vertex(-1,  1,  1);
  fill(255, 255, 0); vertex(-1, -1,  1);
  fill(255, 255, 0); vertex(-1, -1, -1);


  fill(255, 0, 0); vertex(-1,  1, -1);
  fill(255, 0, 0); vertex( 1,  1, -1);
  fill(255, 0, 0); vertex( 1,  1,  1);
  fill(255, 0, 0); vertex(-1,  1,  1);


  fill(0, 0, 255); vertex(-1, -1, -1);
  fill(0, 0, 255); vertex( 1, -1, -1);
  fill(0, 0, 255); vertex( 1, -1,  1);
  fill(0, 0, 255); vertex(-1, -1,  1);


  endShape();

  popMatrix(); 
}

void drawDrone(){
  //Set background
  background(0.5);

  pushMatrix(); 
  
  translate(width/2, height/2, -30); 

  //Rotate
  rotateX(pitch*PI/180.0); 
  rotateZ(roll*PI/180.0); 

  //Print data
  print("Pitch: ");
  print(pitch);
  print(", Roll: ");
  println(roll);

   // draw main body in red
  fill(255, 0, 0, 200);
  box(10, 10, 200);
    
  // draw front-facing tip in blue
  fill(0, 0, 255, 200);
  pushMatrix();
  translate(0, 0, -120);
  rotateX(PI/2);
  drawCylinder(0, 20, 20, 8);
  popMatrix();
  // draw wings and tail fin in green
  fill(0, 255, 0, 200);
  
  beginShape(TRIANGLES);
  vertex(-100,  2, 30); vertex(0,  2, -80); vertex(100,  2, 30);  // wing top layer
  vertex(-100, -2, 30); vertex(0, -2, -80); vertex(100, -2, 30);  // wing bottom layer
  vertex(-2, 0, 98); vertex(-2, -30, 98); vertex(-2, 0, 70);  // tail left layer
  vertex( 2, 0, 98); vertex( 2, -30, 98); vertex( 2, 0, 70);  // tail right layer
  endShape();
  
  beginShape(QUADS);
  vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
  vertex( 100, 2, 30); vertex( 100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
  vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(100, -2,  30); vertex(100, 2,  30);
  vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2, -30, 98); vertex(-2, -30, 98);
  vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
  vertex(-2, -30, 98); vertex(2, -30, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
  endShape();

  popMatrix(); 
}

void serialEvent (Serial serialData) {
  String data = serialData.readStringUntil('\n'); //get line of data
  
  if (data!=null){
    println("GETTING DATA FROM COM3");
    String[] tokenizedData = splitTokens(data);
    println(tokenizedData.length);
    //println("---begin loop---");
    //for (int j=0; j<tokenizedData.length; j++){
    //  float var = float(tokenizedData[j]);
    //  println(var);
    //}
    //println("---end loop---");
    
    if (tokenizedData.length>=3){
      println("--Original data--");
      x = float(tokenizedData[0]);
      println(x);
      y = float(tokenizedData[1]);
      println(y);
      z = float(tokenizedData[2]);
      println(z);
      t = sqrt(x*x + y*y + z*z);
      println(t);
      
      println("--Scaled data--");
      scaledX = HEIGHT/2 - HEIGHT/4 * x;
      println(scaledX);
      scaledY = HEIGHT/2 - HEIGHT/4 * y;
      println(scaledY);;
      scaledZ = HEIGHT/2 - HEIGHT/4 * z;
      println(scaledZ);
      scaledT = HEIGHT/2 - HEIGHT/4 * t;
      println(scaledT);
      pitch = calculatePitch();
      roll = calculateRoll();
      redraw();
    }
  }
}

float calculatePitch() {
  float fXg = 0;
  float fYg = 0;
  float fZg = 0;

  //low-pass filter
  fXg = (x)*alpha + (fXg*(1.0-alpha));
  fYg = (y)*alpha + (fYg*(1.0-alpha));
  fZg = (z)*alpha + (fZg*(1.0-alpha));
  
  pitch = (atan2(fXg,sqrt(fYg*fYg + fZg*fZg))*180.0)/PI;
  print(" Pitch: ");
  println(pitch);
  
  return pitch;
}

float calculateRoll() {
  float fYg = 0;
  float fZg = 0;

  //low-pass filter
  fYg = (y)*alpha + (fYg*(1.0-alpha));
  fZg = (z)*alpha + (fZg*(1.0-alpha));
  
  roll = (atan2(-fYg, fZg)*180.0)/PI;
  print(" Roll: ");
  println(roll);
  
  return roll;
}

void drawCylinder(float topRadius, float bottomRadius, float tall, int sides) {
    float angle = 0;
    float angleIncrement = TWO_PI / sides;
    beginShape(QUAD_STRIP);
    for (int i = 0; i < sides + 1; ++i) {
        vertex(topRadius*cos(angle), 0, topRadius*sin(angle));
        vertex(bottomRadius*cos(angle), tall, bottomRadius*sin(angle));
        angle += angleIncrement;
    }
    endShape();
    
    // If it is not a cone, draw the circular top cap
    if (topRadius != 0) {
        angle = 0;
        beginShape(TRIANGLE_FAN);
        
        // Center point
        vertex(0, 0, 0);
        for (int i = 0; i < sides + 1; i++) {
            vertex(topRadius * cos(angle), 0, topRadius * sin(angle));
            angle += angleIncrement;
        }
        endShape();
    }
  
    // If it is not a cone, draw the circular bottom cap
    if (bottomRadius != 0) {
        angle = 0;
        beginShape(TRIANGLE_FAN);
    
        // Center point
        vertex(0, tall, 0);
        for (int i = 0; i < sides + 1; i++) {
            vertex(bottomRadius * cos(angle), tall, bottomRadius * sin(angle));
            angle += angleIncrement;
        }
        endShape();
    }
}
  