import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val; // Data received from the serial port

static final int WIDTH = 800;
static final int HEIGHT = 600;

int[] ValX = new int [WIDTH];
int[] ValY = new int [WIDTH];
int[] ValZ = new int [WIDTH];
int[] ValT = new int [WIDTH];

float x;
float y;
float z;
float t;

void setup() 
{
  x = 0;
  y = 0;
  z = 0;
  t = 0;
  
  size(800, 600);
  
  String portName = Serial.list()[0]; //COM3 for my PC, different for Macs
  myPort = new Serial(this, portName, 9600);
}

void draw()
{
  //shift data
  for (int i=0; i<WIDTH-1; i++){
    ValX[i] = ValX[i+1];
    ValY[i] = ValY[i+1];
    ValZ[i] = ValZ[i+1];
    ValT[i] = ValT[i+1];
    ValX[WIDTH-1] = int(x);
    ValY[WIDTH-1] = int(y);
    ValZ[WIDTH-1] = int(z);
    ValT[WIDTH-1] = int(t);
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
  }
}

void serialEvent (Serial serialData){
  String data = serialData.readStringUntil('\n'); //get line of data
  
  if (data!=null){
    String[] tokenizedData = splitTokens(data);
    if (tokenizedData.length==3){
      x = HEIGHT/2 - HEIGHT/4 * float(tokenizedData[0]);
      y = HEIGHT/2 - HEIGHT/4 * float(tokenizedData[1]);
      z = HEIGHT/2 - HEIGHT/4 * float(tokenizedData[2]);
      t = HEIGHT/2 - HEIGHT/4 * sqrt(float(tokenizedData[0])*float(tokenizedData[0]) + float(tokenizedData[1])*float(tokenizedData[1]) + float(tokenizedData[2])*float(tokenizedData[2]));
      redraw();
    }
  }
}



/*

// Wiring / Arduino Code
// Code for sensing a switch status and writing the value to the serial port.

int switchPin = 4;                       // Switch connected to pin 4

void setup() {
  pinMode(switchPin, INPUT);             // Set pin 0 as an input
  Serial.begin(9600);                    // Start serial communication at 9600 bps
}

void loop() {
  if (digitalRead(switchPin) == HIGH) {  // If switch is ON,
    Serial.write(1);               // send 1 to Processing
  } else {                               // If the switch is not ON,
    Serial.write(0);               // send 0 to Processing
  }
  delay(100);                            // Wait 100 milliseconds
}

*/