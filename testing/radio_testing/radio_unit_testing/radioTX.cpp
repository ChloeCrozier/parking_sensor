// Chloe Crozier
// 3.21.2024 V0
// This is a modular program that allows the user to choose if they want to transmit or send data between a connected cc1101 radio and anoter mcu radio
// I heavily used the RX/TX code we used in class to write this program
// I changed the program to send 1 in the first packet, 2 in the second packet... and n in the nth packet until some final count
// I stored these ints in a 8 bit binary format in the rx and tx buffers (which is why I added a limit to nums below 256)
// I also added unit tests to ensure that the data being sent between launchpads was accurrately sent and received
// CC1101 Datasheet: https://www.ti.com/lit/ds/symlink/cc1101.pdf?ts=1711001249939&ref_url=https%253A%252F%252Fwww.google.com%252F
// 
// Radio transmission implementation

#include "RadioTX.h"
// These were used in RX/TX
#include <cc1101.h>
#include <macros.h>
#include <pins.h>
#include <registers.h>

// Constructor to set the buffer, send interval, status light, etc...
RadioTX::RadioTX() {
  packetVal = 0;
  sendInterval = 1000;
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, 0);
  
  for (int i = 0; i < INT_SIZE; i++) {
    TX_buffer[i] = 0;
  }
}

// Function to delcare that we are going to be using the radio for sending data
void RadioTX::setTX(void) {
  Serial.println("Entering Transmitting Mode...");
  Radio.SetTxPower(0);
}

// Function to send a 8-bit bianry representation of num in a buffer
void RadioTX::sendData(int num) {
  // Signal that we are starting to send
  digitalWrite(GREEN_LED, 1);
  packetVal = num;
  
  // Convert num to binary where the MSB is on the left and the LSB is on the right
  for (int i = 0; i < INT_SIZE; i++) {
    TX_buffer[i] = (num >> (INT_SIZE - 1 - i)) & 1;
  }
  
  // Send the buffer
  Radio.SendData(TX_buffer, INT_SIZE);
  
  // Add a delay before turning the light off, becuase the light was flickering too quickly
  delay(100);
  digitalWrite(GREEN_LED, 0);
}

// Function to print the buffer
void RadioTX::printBuffer(void) {
  Serial.print("TX_buffer contents: ");
  for (int i = 0; i < INT_SIZE; i++) {
    Serial.print(TX_buffer[i]);
    if (i < INT_SIZE - 1) {
      Serial.print(", ");
    }
  }
  Serial.println();
}

// Function to print the sent packet summary (size, value, etc...)
void RadioTX::printPacketInfo(void){
  Serial.print("A packet of ");
  Serial.print(INT_SIZE);
  Serial.print(" bits was sent with a value of ");
  Serial.println(packetVal);
}

// Function to get the value of the last packet sent
int RadioTX::getPacketVal(void) {
  return packetVal;
}

// Function to get the time between sends
int RadioTX::getSendInterval(void) {
  return sendInterval;
}

// Function to get the size of the buffer sent
int RadioTX::getPacketSize() {
  return INT_SIZE;
}
