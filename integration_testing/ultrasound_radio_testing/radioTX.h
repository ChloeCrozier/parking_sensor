// Chloe Crozier
// 3.22.2024 V0
// This is a program that functions as an integration test between the ultrasound and radio modules
// I heavily used the RX/TX code we used in class to write this program and I directly used my ultrasound and radio unit test code
// I changed this code to test that a receiving can get a 10 bit representation of the current measured distance and also the state of whether a object is parked or not
// CC1101 Datasheet: https://www.ti.com/lit/ds/symlink/cc1101.pdf?ts=1711001249939&ref_url=https%253A%252F%252Fwww.google.com%252F
// Ultrasound Datasheet 
//
// Radio transmission header file

#ifndef RadioTX_H
#define RadioTX_H

#include <Arduino.h>

// Changed from 8 to be able to hold up to dist = 450, so this goes up to 2^9 - 1, plus one bit for the parking status
#define INT_SIZE 10

class RadioTX
{
  public:
    RadioTX();
    void setTX(void);
    void sendData(int num, bool state);
    int getPacketVal(void);
    int getSendInterval(void);
    void printBuffer(void);
    void printPacketInfo(void);
    int getPacketSize(void);
  private:
    byte TX_buffer[INT_SIZE]; 
    int packetVal;
    int sendInterval;
};

#endif
