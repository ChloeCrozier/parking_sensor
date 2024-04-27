// Chloe Crozier
// 3.21.2024 V0
// This is a modular program that allows the user to choose if they want to transmit or send data between a connected cc1101 radio and anoter mcu radio
// I heavily used the RX/TX code we used in class to write this program
// I changed the program to send 1 in the first packet, 2 in the second packet... and n in the nth packet until some final count
// I stored these ints in a 8 bit binary format in the rx and tx buffers (which is why I added a limit to nums below 256)
// I also added unit tests to ensure that the data being sent between launchpads was accurrately sent and received
// CC1101 Datasheet: https://www.ti.com/lit/ds/symlink/cc1101.pdf?ts=1711001249939&ref_url=https%253A%252F%252Fwww.google.com%252F
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
// Included based on custommodule from class
#include "minunit.h"

// Set a macro to determine if we are entering receiving (RX) or transmitting (TX) mode
#define RECEIVING_MODE true
// Set a macro for when we want to turn on a unit testing mode
#define DEBUG_MODE true
// Set a macro for the number of packets I want to send in transmitting mode
#define MAX_PACKETS_SENT 10

// Initialize IO objects
RadioRX radioRX;
RadioTX radioTX;
int i;

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

//  This tests verifys the correct number of packets were received, the value of the last packet received, and the total sum of packets received
static char *test_receivedPackets() {
  int expectedSum = 0;
  for(int i = 1; i <= MAX_PACKETS_SENT; i++) {
    expectedSum += i;
  }
  
  int i = 0;
  // I originally did this based on time, but I figured that an infinite loop of it missing a packet can serve as a failed test case
  while(i < MAX_PACKETS_SENT) {
    radioRX.receivePacket();
    if (radioRX.getReceivedStatus()) {
        radioRX.printBuffer();
        radioRX.printPacketInfo();
        Serial.println();
        i++;
    }
  }

  // Ensure that the acutal final val, count, and sum match the expected values
  mu_assert("Error: The number of packets received does not match the number of packets sent", radioRX.getNumPacketsReceived() == MAX_PACKETS_SENT);
  Serial.print("Actual number of packets received ");
  Serial.println(radioRX.getNumPacketsReceived());
  mu_assert("Error: The value of the last packet sent does not match the number of packets sent", radioRX.getPacketVal() == MAX_PACKETS_SENT);
  Serial.print("Actual value of the final packet received ");
  Serial.println(radioRX.getPacketVal());
  mu_assert("Error: The sum of the received packet values does not match the expected sum", radioRX.getPacketSum() == expectedSum);
  Serial.print("Actual sum of all received packet vals ");
  Serial.println(radioRX.getPacketSum());

  return NULL;
}

// This was used in the customemodule example we went over in class
int tests_run = 0;
int tests_passed = 0;
int assertions = 0;

// Run the test
static char *all_tests() {
    Serial.println();
    mu_run_test(test_receivedPackets);
    
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
        // Declare what packet number we are sending
        Serial.print("Sending packet #");
        Serial.println(i);
        // Send the data with the val of the current nth packet
        radioTX.sendData(i++); // send 1 packet of data with a value of 1
        radioTX.printBuffer();
        radioTX.printPacketInfo();

        // If we max out the number per size of buffer (8 bits for a max of 256), end the sending
        if((i > MAX_PACKETS_SENT) || (i > 255)){
          suspend();
        }

        // Delay for simplification
        delay(radioTX.getSendInterval());
        Serial.println();
      }
    }
}
