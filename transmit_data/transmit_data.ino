//This is the transmitter for accl and gyro data

//Include all libraries
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include "I2Cdev.h"
#include "MPU6050.h"

//I2C initialization
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

/// Set up the accelerometer and gyro 
MPU6050 accelgyro;

/// Set up the altimeter 
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

// Define variables for acc/gyro
// same scale as johnny five
float scale = 16384.0;
float gyroscale = 131.0;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t payload[5];

#define OUTPUT_READABLE_ACCELGYRO

#define LED_PIN 13
bool blinkState = false;

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.  
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(57600);
  Serial.setTimeout(500);
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  // set acc/gyro to +/- 8G sensitivity
  // make sure this is the same as johnny five
  accelgyro.setFullScaleAccelRange(0);

  // initialize MPU6050
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  
  // verify connection for MPU6050
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  
  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  //if ( role == role_ping_out )
  {
    //radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  //else
  {
    //radio.openWritingPipe(pipes[1]);
    //radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
}

void loop(void)
{

  // check altimeter

  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
  }

  // setup MPU6050
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // set and print all values to validate
  Serial.print("Acc x ");
  ax = ax/scale;
  Serial.print(ax);
  Serial.print("Acc y ");
  ay = ay/scale;
  Serial.print(ay);
  Serial.print("Acc z ");
  az = az/scale;
  Serial.println(az);
  Serial.print("Gyro x ");
  gx = gx/gyroscale;
  Serial.print(gx);
  Serial.print("Gyro y ");
  gy = gy/gyroscale;
  Serial.print(gy);
  Serial.print("Gyro z ");
  gz = gz/gyroscale;
  Serial.println(gz);

  // build payload
  //TODO make this into a loop later
  payload[0] = ax;
  payload[1] = ay;
  payload[2] = az;
  payload[3] = gx;
  payload[4] = gy;
  payload[5] = gz;
  
  //
  // Transmitter Role.  Repeatedly send acceleration and gyro values
  //

  if (role == role_ping_out)
  {
    // First, stop listening so we can talk.
    radio.stopListening();

    //TODO delete later
    // Take the time, and send it.  This will block until complete
    //unsigned long time = millis();
    //printf("Now sending %lu...",time);
    bool ok = radio.write( &payload, sizeof(payload) );
    
    if (ok)
      printf("SUCCESSFUL");
    else
      printf("FAILED\n\r");

    // Start listening for new data 
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      //unsigned long got_time;
      //radio.read( &got_time, sizeof(unsigned long) );

      // Spew it
      //printf("Got response %lu, round-trip delay: %lu\n\r",got_time,millis()-got_time);
    }

    // Try again 1s later
    delay(1000);
  }

  //
  // Change roles
  //

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      // Become the primary transmitter (ping out)
      role = role_ping_out;
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]);
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");
      
      // Become the primary receiver (pong back)
      role = role_pong_back;
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
    }
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
