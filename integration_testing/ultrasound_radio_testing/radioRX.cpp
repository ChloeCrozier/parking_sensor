// Chloe Crozier
// 3.22.2024 V0
// This is a program that functions as an integration test between the ultrasound and radio modules
// I heavily used the RX/TX code we used in class to write this program and I directly used my ultrasound and radio unit test code
// I changed this code to test that a receiving can get a 10 bit representation of the current measured distance and also the state of whether a object is parked or not
// CC1101 Datasheet: https://www.ti.com/lit/ds/symlink/cc1101.pdf?ts=1711001249939&ref_url=https%253A%252F%252Fwww.google.com%252F
// Ultrasound Datasheet 
//
// Radio receiving implementation

#include "RadioRX.h"
// These were used in RX/TX
#include <cc1101.h>
#include <macros.h>
#include <pins.h>
#include <registers.h>

// Constructor to set the buffer, packet sum, status light, etc...
RadioRX::RadioRX() {
  numPacketsReceived = 0;
  packetSize = 0;
  packetVal = 0;
  packetSum = 0;
  packetReceived = false;
  parkedState = 0;

  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);

  // Initialize RX_buffer
  for (int i = 0; i < INT_SIZE; i++) {
    RX_buffer[i] = 0;
  }
}

// Function to delcare that we are going to be using the radio for receiving data
void RadioRX::setRX() {
  Serial.println("Entering Receiving Mode...");
  Radio.SetTxPower(1);
  Radio.RxOn();
}

// Function to receive a 8-bit bianry representation of num in a buffer
void RadioRX::receivePacket() {
  // Reset packet information from a possible previosuly received packet
  packetSize = 0;
  packetVal = 0;

  // If the radio has detected a packet to receive
  if (Radio.CheckReceiveFlag()) {
    digitalWrite(RED_LED, HIGH);
    numPacketsReceived++;

    // Get the size and content of the incoming packet
    packetSize = Radio.ReceiveData(RX_buffer);

  
    // If the packet exceeds the size of the buffer
    if (packetSize > INT_SIZE) {
      Serial.println("Packet received is too big to fit into the buffer");
      packetReceived = false;
    }
    else {
      // ensures that a valid packet has been received
      packetReceived = true;
      // Calculate decimal value of the binary in the buffer
      for (int i = 1; i < packetSize; i++) {
        packetVal += RX_buffer[i] << (packetSize - 1 - i);
      }
      // Add it to the sum
      packetSum += packetVal;
      parkedState = RX_buffer[0];
    }

    if(packetVal == 0){
      Serial.println("\nThe received packet contained a 0 meaning that the ultrasound sensor had an invalid reading");
    }

    // According to RX, the ReceiveData function stops the 'polling', so we need to turning that back on
    Radio.RxOn();

    // Add a delay before turning the light off, becuase the light was flickering too quickly
    delay(100);
    digitalWrite(RED_LED, LOW);
  }
  // If we did not receive a packet
  else {
    packetReceived = false;
  }
}

// Function to print the buffer
void RadioRX::printBuffer() {
  Serial.print("RX_buffer contents: ");
  for (int i = 0; i < packetSize; i++) {
    Serial.print(RX_buffer[i]);
    if (i < packetSize - 1) {
      Serial.print(", ");
    }
  }
  Serial.println();
}

// Function to print the sent packet summary (size, value, etc...)
void RadioRX::printPacketInfo() {
  Serial.print("A packet of ");
  Serial.print(packetSize);
  Serial.print(" bits was received with a value of ");
  Serial.println(packetVal);
}

// Function to get the count of packets currently received
int RadioRX::getNumPacketsReceived() {
  return numPacketsReceived;
}

// Function to get the size of the last packet received
int RadioRX::getPacketSize() {
  return packetSize;
}

// Function to get the value of the last packet received
int RadioRX::getPacketVal() {
  return packetVal;
}

// Function to get the status of whether a packet is currently beign received
bool RadioRX::getReceivedStatus() {
  return packetReceived;
}

// Function to get the current running packet value sum
long RadioRX::getPacketSum() {
  return packetSum;
}

// Function to return the parked state
int RadioRX::getParkedState() {
  return parkedState;
}
