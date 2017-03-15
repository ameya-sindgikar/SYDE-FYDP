/**************************************************************************/
/*!
Drone Safe
*/
/**************************************************************************/

#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

// Power by connecting Vin to 3-5V, GND to GND
// Uses I2C - connect SCL to the SCL pin, SDA to SDA pin
// See the Wire tutorial for pinouts for each Arduino
// http://arduino.cc/en/reference/wire
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

float prevaltm;
float startaltm;
int startaltmcount = 0;
float altm;
int altmcount = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit_MPL3115A2 test!");
}

void loop() {
  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
  }
  
  //float pascals = baro.getPressure();
  // Our weather page presents pressure in Inches (Hg)
  // Use http://www.onlineconversion.com/pressure.htm for other units
  //Serial.print(pascals/3377); Serial.println(" Inches (Hg)");


  
  altm = baro.getAltitude();
  // Calculate rate of descent
  if ((prevaltm - altm) > 1) {
      Serial.println("the drone is falling");
    }
    
  Serial.print(prevaltm); Serial.println(" meters (last)");
  Serial.print(altm); Serial.println(" meters");
  Serial.print(startaltm); Serial.println("starting altitude");
  Serial.print(altm - startaltm); Serial.println(" altitude above ground");
  // Get starting altitude value
  if (startaltmcount == 5) {
    startaltm = altm;
    
    }
  
  // Get previous altitude value
  if (altmcount != 0) {
     prevaltm = altm;
     altmcount = 0;
    }

  //float tempC = baro.getTemperature();
  //Serial.print(tempC); Serial.println("*C");

  altmcount = 1;
  startaltmcount++;
  delay(300);
}
