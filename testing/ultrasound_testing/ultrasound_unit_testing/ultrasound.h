// Chloe Crozier
// 3.20.2024 V0
// This is a modular version of my original functionality testing code from ultrasound_functionality.ino
// The code/design is based on the ultrasonic grove module from the launchpad_kit repo on github
// Repo: https://github.com/Seeed-Studio/LaunchPad_Kit/tree/master/Grove_Modules/ultrasonic_ranger// Here is the implementation of the ultrasound class functions.
// Sensor Part: https://www.adafruit.com/product/4007#technical-details
// 
// Header file for ultrasound class

#ifndef Ultrasound_H
#define Ultrasound_H

// This is what the github repo included to use a bunch of the prexisitng functions
#include "Arduino.h"

class Ultrasound
{
  public:
    Ultrasound(int output, int input);
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
