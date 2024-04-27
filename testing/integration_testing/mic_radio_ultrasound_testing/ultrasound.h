// Chloe Crozier
// 4.27.2024 V0
// This is a program that functions as an integration test between the ultrasound, microphone, and radio modules
// I heavily used the RX/TX code we used in class to write this program and I directly used my ultrasound, microphone, and radio unit test code
// I changed this code to test that a receiving can get a 10 bit representation of the current measured distance and also the state of whether a object is parked or not
// The microhpone units testts are not fully automated and must be watched in person, but we could always just send the LED h/l values over the radio and verify that way
// CC1101 Datasheet: https://www.ti.com/lit/ds/symlink/cc1101.pdf?ts=1711001249939&ref_url=https%253A%252F%252Fwww.google.com%252F
// Ultrasound Datasheet: https://media.digikey.com/pdf/Data%20Sheets/Adafruit%20PDFs/4019_Web.pdf
// Microphone Datasheet: https://cdn.sparkfun.com/datasheets/Sensors/Sound/CEM-C9745JAD462P2.54R.pdf
//
// Header file for ultrasound class

#ifndef Ultrasound_H
#define Ultrasound_H

// This is what the github repo included to use a bunch of the prexisitng functions
#include "Arduino.h"

class Ultrasound
{
  public:
    Ultrasound(int output, int input, int parkingSpaceSize, int detectingCycles);
    long measureTime(void);
    long measureDist(long duration);
    bool isOccupied(long distance);
    void isParked(long distance);
    bool isValidDist(long distance);
    int getCycleLength(void);
    int getObjectCount(void);
    bool getParkedState(void);
    long getTimeParked(void);
    double getParkingSpaceDist(void);
    int getMinDetectionCycles(void);
  private:
    int triggerPin;
    int echoPin;
    int cycleLength;    
    int timeSending;
    double parkingSpaceDist;
    int minDetectionCycles;
    bool parkedState;
    int stateCounter;
    int objectCount;
    long timeParked;
};

#endif
