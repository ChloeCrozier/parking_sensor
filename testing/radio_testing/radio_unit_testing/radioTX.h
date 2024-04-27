// Chloe Crozier
// 3.21.2024 V0
// This is a modular program that allows the user to choose if they want to transmit or send data between a connected cc1101 radio and anoter mcu radio
// I heavily used the RX/TX code we used in class to write this program
// I changed the program to send 1 in the first packet, 2 in the second packet... and n in the nth packet until some final count
// I stored these ints in a 8 bit binary format in the rx and tx buffers (which is why I added a limit to nums below 256)
// I also added unit tests to ensure that the data being sent between launchpads was accurrately sent and received
// CC1101 Datasheet: https://www.ti.com/lit/ds/symlink/cc1101.pdf?ts=1711001249939&ref_url=https%253A%252F%252Fwww.google.com%252F
// 
// Radio transmission header file

#ifndef RadioTX_H
#define RadioTX_H

#include <Arduino.h>

// This is the size of an integer
#define INT_SIZE 8

class RadioTX
{
  public:
    RadioTX();
    void setTX(void);
    void sendData(int num);
    int getPacketVal(void);
    int getSendInterval(void);
    void printBuffer(void);
    void printPacketInfo(void);
    int getPacketSize();
  private:
    byte TX_buffer[INT_SIZE]; 
    int packetVal;
    int sendInterval;
};

#endif
