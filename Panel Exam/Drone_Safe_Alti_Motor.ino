// to calibrate the motor start from 180 degrees plug in
// the battery and throttle down to 0 then it will make beep
// and motor can be used  
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high


Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

float prevaltm;
float scale = 16384.0;
float gyroscale = 131.0;
float startaltm;
int startaltmcount = 0;
float altm;
int altmcount = 0;
int falling = 0;

const float alpha = 0.5;

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define OUTPUT_READABLE_ACCELGYRO

Servo esc;

int escPin = 9;
int minPulseRate = 1000;
int maxPulseRate = 2000;
int throttleChangeDelay = 100;

#define LED_PIN 13
bool blinkState = false;

void setup() {
  
  Serial.begin(9600);
  Serial.setTimeout(500);
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  //Serial.begin(38400);

  
  // set to +/- 8G sensitivity
  accelgyro.setFullScaleAccelRange(0);

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  
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

  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

      // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    Serial.print("Acc X ");
    Serial.print(ax/scale);
    Serial.print("Acc y ");
    Serial.print(ay/scale);
    Serial.print("Acc z ");
    Serial.println(az/scale);
    Serial.print("Gyro X ");
    Serial.print(gx/gyroscale);
    Serial.print("Gyro y ");
    Serial.print(gy/gyroscale);
    Serial.print("Gyro z ");
    Serial.println(gz/gyroscale);
    float pitch = calculatePitch();
    float roll = calculateRoll();
    
    altm = baro.getAltitude();
    // Calculate rate of descent
    if ((prevaltm - altm) > 0.75) {
      falling = 1;
    }
    
    // Get starting altitude value
    if (startaltmcount == 5) {
      startaltm = altm;
      }
    Serial.println("altitude ----------");
    Serial.println(altm);
    
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
    int throttle = 10;
    if (falling == 1) {
      Serial.println("--FALLING--");
      //turn on propeller only if pitch angle is between -30 degrees and +30 degrees
      if (pitch>-30.0 && pitch<30.0){
       esc.write(throttle);
        //changeThrottle(throttle); // Change throttle to the new value
        //falling = 0;
      }
      Serial.println("Pitch angle out of range to turn on prop.");
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

//Function to calculate pitch
float calculatePitch(){
  float pitch = 0;
  //fXg, fYg, fZg are filtered acceleration values
  double fXg = 0;
  double fYg = 0;
  double fZg = 0;

  //low-pass filter
  fXg = (ax)*alpha + (fXg*(1.0-alpha));
  fYg = (ay)*alpha + (fYg*(1.0-alpha));
  fZg = (az)*alpha + (fZg*(1.0-alpha));

  //calculate pitch
  pitch = (atan2(fXg,sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;
  Serial.print(" Pitch: ");
  Serial.println(pitch);
  return pitch;
}

//Function to calculate roll
float calculateRoll(){
  float roll = 0;
  //fXg, fYg, fZg are filtered acceleration values
  double fXg = 0;
  double fYg = 0;
  double fZg = 0;

  //low-pass filter
  fXg = (ax)*alpha + (fXg*(1.0-alpha));
  fYg = (ay)*alpha + (fYg*(1.0-alpha));
  fZg = (az)*alpha + (fZg*(1.0-alpha));

  //calculate roll
  roll = (atan2(-fYg, fZg)*180.0)/M_PI;
  Serial.print(" Roll: ");
  Serial.println(roll);
  return roll;
}
