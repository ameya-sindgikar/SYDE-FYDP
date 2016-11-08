// to calibrate the motor start from 180 degrees plug in
// the battery and throttle down to 0 then it will make beep
// and motor can be used  
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

float prevaltm;
float startaltm;
int startaltmcount = 0;
float altm;
int altmcount = 0;
int falling = 0;

Servo esc;

int escPin = 9;
int minPulseRate = 1000;
int maxPulseRate = 2000;
int throttleChangeDelay = 100;

void setup() {
  
  Serial.begin(9600);
  Serial.setTimeout(500);
  
  // Attach the the servo to the correct pin and set the pulse range
  esc.attach(escPin, minPulseRate, maxPulseRate); 
  // Write a minimum value (most ESCs require this correct startup)
  esc.write(0);
  
}

void loop() {
  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
  }

  altm = baro.getAltitude();
  // Calculate rate of descent
  if ((prevaltm - altm) > 0.5) {
      falling = 1;
    }
  // Get starting altitude value
  if (startaltmcount == 5) {
    startaltm = altm;
    
    }
  
  // Get previous altitude value
  if (altmcount != 0) {
     prevaltm = altm;
     altmcount = 0;
    }

  // Wait for some input
  /*if (Serial.available() > 0 ) {
    
    // Read the new throttle value
    int throttle = normalizeThrottle( Serial.parseInt() );
    
    // Print it out
    Serial.println(falling);
    Serial.println(prevaltm - altm);
    Serial.print("Setting throttle to: ");
    Serial.println(throttle);
*/
    int throttle = 20;
    if (falling == 1) {
    // Change throttle to the new value
    // dummy throttle value
      esc.write(throttle);
    //changeThrottle(throttle);
    //falling = 0;
    }
    
  //}
  altmcount = 1;
  startaltmcount++;
  delay(10);
}

void changeThrottle(int throttle) {
  
  // Read the current throttle value
  int currentThrottle = readThrottle();
  
  // Are we going up or down?
  int step = 1;
  if( throttle < currentThrottle )
    step = -1;
  
  // Slowly move to the new throttle value 
  while( currentThrottle != throttle ) {
    esc.write(currentThrottle + step);
    currentThrottle = readThrottle();
    delay(throttleChangeDelay);
  }
  
}

int readThrottle() {
  int throttle = esc.read();
  
  Serial.print("Current throttle is: ");
  Serial.println(throttle);
  
  return throttle;
}

// Ensure the throttle value is between 0 - 180
int normalizeThrottle(int value) {
  if( value < 0 )
    return 0;
  if( value > 180 )
    return 180;
  return value;
}
