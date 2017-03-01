// This code is for the reciever node

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// declare local variables 
int16_t payload[5];

// Set up nRF24L01 radio on pins 9, 10 
RF24 radio(9,10);

// Radio pipe addresses for nodes to communicate
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };


void setup() {
  Serial.begin(57600);
  Serial.println("Beginning NRF to receive data!");

  radio.begin();

  radio.setRetries(15,15);

  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);

  radio.startListening();
  radio.printDetails();
}

void loop() {
  // if there is data
  if (radio.available())
  {
    bool done = false;
    while (!done)
    {
      //Fetch the data 
      done = radio.read( &payload, sizeof(payload));

      //Show payload in the console
      Serial.println(payload[0]);

      //Stop listening to reply
      radio.stopListening();

      // Send a reply back
      radio.write( &payload, sizeof(payload));

      // Start listening for new data
      radio.startListening();
    }
  }

}
