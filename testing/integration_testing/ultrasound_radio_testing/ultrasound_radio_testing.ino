// Chloe Crozier
// 3.22.2024 V0
// This is a program that functions as an integration test between the ultrasound and radio modules
// I heavily used the RX/TX code we used in class to write this program and I directly used my ultrasound and radio unit test code
// I changed this code to test that a receiving can get a 10 bit representation of the current measured distance and also the state of whether a object is parked or not
// CC1101 Datasheet: https://www.ti.com/lit/ds/symlink/cc1101.pdf?ts=1711001249939&ref_url=https%253A%252F%252Fwww.google.com%252F
// Ultrasound Datasheet: https://media.digikey.com/pdf/Data%20Sheets/Adafruit%20PDFs/4019_Web.pdf
//
// Main file to run tests

// These headfiles were used in RX/TX
#include <cc1101.h>
#include <macros.h>
#include <pins.h>
#include <registers.h>
// Include the RX and TX header files
#include "RadioRX.h"
#include "RadioTX.h"
// use the header file we wrote for this module
#include "Ultrasound.h"
// Included based on custommodule from class
#include "minunit.h"

// Set a macro to determine if we are entering receiving (RX) or transmitting (TX) mode
#define RECEIVING_MODE true
// Set a macro for when we want to turn on a unit testing mode
#define DEBUG_MODE true
// Set the trigger pin (we have it connected to P6_2)
#define TRIGGER P6_2
// Set the echo return pin (we have it connected to P6_3)
#define ECHO P6_3
// Set a macro for the number of packets I want to send in transmitting mode
#define MAX_PACKETS_SENT 10
// Set a macro in cm for test verification and also parking spaces are around 5 meters (500 cm)
#define PARKING_SPACE_DIST 15
// Set a macro to determine the minimun number of cycles needer to consider an object occupying the spot as parked
#define PARKING_DETECTION_CYCLES 5

// Initialize IO objects
RadioRX radioRX;
RadioTX radioTX;
int i;
// From the modular ultrasound_unit_testing code
long duration;
long distance;
Ultrasound ultrasound(TRIGGER, ECHO, PARKING_SPACE_DIST, PARKING_DETECTION_CYCLES);

void setup() {
  // These lines were in common between both RX and TX's setup functions
  Serial.begin(9600);
  Radio.Init();
  Radio.SetDataRate(4); // Needs to be the same in Tx and Rx
  Radio.SetLogicalChannel(5); // Needs to be the same as receiver
  
  // RX setup
  if(RECEIVING_MODE) {
    radioRX.setRX();
  }
  // TX setup
  else {
    radioTX.setTX();
    // set send index as 1 (the i'th packet is sent with a value of i) for the first packet
    i = 1;
  }
}

//  This test verifies if the parking status bit (index 0 of buffer) accurately detects if the car is parked after sending the last packet
static char *test_parkingStatus() {
  int i = 0;
  int stateCount = 0;
  // I originally did this based on time, but I figured that an infinite loop of it missing a packet can serve as a failed test case
  while(i < MAX_PACKETS_SENT) {
    radioRX.receivePacket();
    if (radioRX.getReceivedStatus()) {
        radioRX.printBuffer();
        radioRX.printPacketInfo();
        Serial.println();
        i++;

        // Count the number of consecutive times the object is occupying the spot
        if(radioRX.getPacketVal() <= (PARKING_SPACE_DIST / 2.0)) {
          stateCount++;
        }
        // Reset the count if it is no longer detected
        else {
          stateCount = 0;
        }
        // Test to see if the radio receives any invalid data
        mu_assert("Error: The value of the last packet received was above the valid ultrasound sensor range", radioRX.getPacketVal() < 450);
        mu_assert("Error: The value of the last packet received was below the valid ultrasound sensor range", (radioRX.getPacketVal() > 2) || (radioRX.getPacketVal() == 0));
    }
  }
  // Ensure that the acutal count matches the expected values
  mu_assert("Error: The number of packets received does not match the number of packets sent", radioRX.getNumPacketsReceived() == MAX_PACKETS_SENT);
  Serial.print("Actual number of packets received ");
  Serial.println(radioRX.getNumPacketsReceived());
  // Ensure that it measured enough values correctly to be able to determine if an object has been occupying the spot long enough to be considered parked
  mu_assert("Error: The final parking state was not calcualted correctly due to invalid values", (stateCount >= PARKING_DETECTION_CYCLES) == radioRX.getParkedState());
  Serial.print("Actual final parking state ");
  Serial.println(stateCount >= PARKING_DETECTION_CYCLES);

  return NULL;
}

// This was used in the customemodule example we went over in class
int tests_run = 0;
int tests_passed = 0;
int assertions = 0;

// Run the test
static char *all_tests() {
    Serial.println();
    mu_run_test(test_parkingStatus);
    
    return NULL;
}


void loop() {
    // If we want to run the unit tests, we can set DEBUG_MODE to true
    if(DEBUG_MODE) {      
      // I kept all of this the same as the module we used in class and the ultrasound unit tests I wrote
      char *result = all_tests();
      Serial.print("\nTests run: ");
      Serial.println(tests_run);
      
      if (result != 0) {
        Serial.print("Tests passed: ");
        Serial.println(tests_passed);
      } else {
        Serial.println("ALL TESTS PASSED\n");
      }
      
      Serial.println();
      suspend();
    }
    // If we want to run the normal functionality testing, we can set DEBUG_MODE to false
    else {
      // If we are receiving
      if(RECEIVING_MODE) {
        // Call the function to see if a receive has been triggered
        radioRX.receivePacket();
        // If the status stats we have received a packet, proceed
        if (radioRX.getReceivedStatus()) {
            radioRX.printBuffer();
            radioRX.printPacketInfo();
            Serial.println();
        }
      }
      // If we are transmitting
      else {
        // If we have already sent the max number of packets
        if(i >= MAX_PACKETS_SENT){
          Serial.print("Done sending ");
          Serial.print(i);
          Serial.print(" out of ");
          Serial.print(MAX_PACKETS_SENT);
          Serial.println(" packets");
          suspend();
        }
        // If the binary representation of distance is larger than the max size of buffer (9 bits for a max of 2^9 - 1 -> 511 plus one for the parking status)
        else if(distance > 511) {
          Serial.println("Distance was too large to send over 10 bit buffer");
          distance = 0;
          suspend();
        }
        // If we are sending a valid distance and haven't compelted all of the sends
        else {
          // Get the time and distance traveled
          duration = ultrasound.measureTime();
          distance = ultrasound.measureDist(duration);
    
          if(ultrasound.isValidDist(distance)){
             // Run isParked (void) to update the parkedState bool and print the details of the current state
             ultrasound.isParked(distance);
          }
          else {
            distance = 0;
          }
          
          // Declare what packet number we are sending
          Serial.println();
          Serial.print("Sending packet #");
          Serial.println(i++);
          
          // Send the data with the val of the current nth packet
          radioTX.sendData(distance, ultrasound.getParkedState()); // send a packet of data with a value of distance
          radioTX.printBuffer();
          radioTX.printPacketInfo();
  
          // Delay for simplification
          delay(radioTX.getSendInterval());
        }
        Serial.println("-------------------------------------------------------------------------------");
        Serial.println();
        Serial.println();
      }
    }
}
