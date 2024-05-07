/*
Park Sharks
Modular version of microphone.ino 
See comment block in that file for sources
*/

#define SAMPLES_SIZE 15
#define VOLUME_BOUND 550

#ifndef MICROPHONE_H
#define MICROPHONE_H

#include "Arduino.h"

class Microphone
{
  public:
    Microphone(int sensor, int led);
    int32_t getThreshold(void);
    int32_t readValue();
    int32_t readValueDebug();
    bool valueIsInRange(int value);
    void triggerDebugLight(int value);
    void setDebugLightLength(int delay);
    void setThreshold(int thresh);
    void setCycleLength(int);
    int getCycleLength(void);
    int getSampleIndex(void);
    
  private:
    int sensorPin;
    int micLEDPin;
    int32_t threshold;
    int debugLightLength;
    int cycleLength;
    int ledPin;
    int32_t samples[SAMPLES_SIZE];
    int samplesSize;
    int sampleIndex;

};

#endif
