#define sensorPin P1_4   
#define ledPin P8_2
#define lowerBound 1000
#define upperBound 3000
#define DEBUG true

int sensorValue = 0;

#include "microphone.h"

Microphone mic(sensorPin, ledPin);


void setup() {
  Serial.begin(9600);
  mic.setThreshold(lowerBound, upperBound);
}

void loop() {
  if(DEBUG){
    sensorValue = mic.readValueDebug();
    mic.triggerDebugLight(sensorValue);
  }else{
    sensorValue = mic.readValue();
  }
  
}