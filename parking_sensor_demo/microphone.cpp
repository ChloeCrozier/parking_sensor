/*
Park Sharks
Modular version of microphone.ino 
See comment block in that file for sources
*/
#include "microphone.h"

Microphone::Microphone(int sensor, int led){
    sensorPin = sensor;

    ledPin = led;
    pinMode(ledPin, OUTPUT);

    //default delay time is 500
    debugLightLength = 500;
    cycleLength = 200;
    sampleIndex = 0;
    
}

//set debug light delay
void Microphone::setDebugLightLength(int delay){
    debugLightLength = delay;
}

//reads value from the sensor, updates bound and samples array
int32_t Microphone::readValue() {
    int32_t sensorValue = analogRead(sensorPin);
    int32_t sensorValueSquared = sq(sensorValue - 2048);
    
    samples[sampleIndex++] = sensorValueSquared;
    sampleIndex %= SAMPLES_SIZE;

     int32_t total = 0;
     for (int i = 0; i < SAMPLES_SIZE; i++) {
       total += samples[i];
     }
     threshold = sqrt(total / SAMPLES_SIZE);
    
    return sensorValue;
}

//reads value from the sensor and prints to the serial monitor
int32_t Microphone::readValueDebug(){
    int32_t sensorValue = readValue();
    Serial.print("Microphone read: ");
    Serial.print(sensorValue);
    Serial.print(", Current Threshold: ");
    Serial.print(threshold);
    Serial.print("\n");
    
    return sensorValue;
}

// Test if value is in range
bool Microphone::valueIsInRange(int value){
    if (threshold > 500) {     
        return true;
    }
    return false;
}

//uses value to set debug light
void Microphone::triggerDebugLight(int value){
    if (valueIsInRange(value)) {     
        // sound above certain level: LED on
        digitalWrite(ledPin, HIGH);
        delay(debugLightLength);
        digitalWrite(ledPin, LOW);
    } else {
        // sound normal: LED off
        digitalWrite(ledPin, LOW); 
    }
}

void Microphone::setCycleLength(int c) {
  cycleLength = c;
}

//getter function to get the current threshold of the microphone
int32_t Microphone::getThreshold(){
    return threshold;
}

int Microphone::getSampleIndex(){
    return sampleIndex;
}

int Microphone::getCycleLength(void){
  return cycleLength;
}
