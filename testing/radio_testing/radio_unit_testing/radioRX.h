// Chloe Crozier
// 3.21.2024 V0
// This is a modular program that allows the user to choose if they want to transmit or send data between a connected cc1101 radio and anoter mcu radio
// I heavily used the RX/TX code we used in class to write this program
// I changed the program to send 1 in the first packet, 2 in the second packet... and n in the nth packet until some final count
// I stored these ints in a 8 bit binary format in the rx and tx buffers (which is why I added a limit to nums below 256)
// I also added unit tests to ensure that the data being sent between launchpads was accurrately sent and received
// CC1101 Datasheet: https://www.ti.com/lit/ds/symlink/cc1101.pdf?ts=1711001249939&ref_url=https%253A%252F%252Fwww.google.com%252F
// 
// Radio receiving header file

#ifndef RadioRX_H
#define RadioRX_H

#include <Arduino.h>

#define INT_SIZE 8

class RadioRX
{
  public:
    RadioRX();
    void setRX();
    void receivePacket();
    int getNumPacketsReceived();
    int getPacketSize();
    int getPacketVal();
    void printBuffer();
    void printPacketInfo();
    bool getReceivedStatus();
    long getPacketSum();
  private:
    int numPacketsReceived;
    int packetSize;
    int packetVal;
    byte RX_buffer[INT_SIZE];
    bool packetReceived;
    long packetSum;
};

#endif
