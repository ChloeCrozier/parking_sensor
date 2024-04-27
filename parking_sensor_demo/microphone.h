/*
Eliza Sorber
Modular version of microphone.ino 
See comment block in that file for sources
*/


#ifndef MICROPHONE_H
#define MICROPHONE_H


class Microphone
{
  public:
    Microphone(int sensor, int led);
    void setThreshold(int low, int high);
    int readValue();
    int readValueDebug();
    void triggerDebugLight(int value);
    void setDebugLightLength(int delay);
    void setLowerThreshold(int lower);
    void setUpperThreshold(int upper);
    
  private:
    int sensorPin;
    int micLEDPin;
    int lowerthreshold;
    int higherthreshold;
    int debugLightLength;

};

#endif
