// Eliza's Modular Microphone Code
// CC1101 Datasheet: https://www.ti.com/lit/ds/symlink/cc1101.pdf?ts=1711001249939&ref_url=https%253A%252F%252Fwww.google.com%252F
// Ultrasound Datasheet: https://media.digikey.com/pdf/Data%20Sheets/Adafruit%20PDFs/4019_Web.pdf
// Microphone Datasheet: https://cdn.sparkfun.com/datasheets/Sensors/Sound/CEM-C9745JAD462P2.54R.pdf
//
// Modular microphone implementation

#include "microphone.h"

Microphone::Microphone(int sensor, int led){
    sensorPin = sensor;

    ledPin = led;
    pinMode(ledPin, OUTPUT);

    //default delay time is 500
    debugLightLength = 500;
    
}

//setter function to edit the threshold of the microphone
void Microphone::setThreshold(int lower, int upper){
    lowerthreshold = lower;
    higherthreshold = upper;
}

//set just the lower threshold
void Microphone::setLowerThreshold(int lower){
    lowerthreshold = lower;
}

//set just the higher threshold
void Microphone::setUpperThreshold(int upper){
    higherthreshold = upper;
}

//set debug light delay
void Microphone::setDebugLightLength(int delay){
    debugLightLength = delay;
}

//read value from the sensor 
int Microphone::readValue(){
    int sensorValue = analogRead(sensorPin);
    return sensorValue;
}

//reads value from the sensor and prints to the serial monitor
int Microphone::readValueDebug(){
    int sensorValue = analogRead(sensorPin);
    Serial.println(sensorValue);
    return sensorValue;
}

//uses value to set debug light
void Microphone::triggerDebugLight(int value){
    if (value < lowerthreshold || value > higherthreshold) {     
        // sound above certain level: LED on
        digitalWrite(ledPin, HIGH);
        delay(debugLightLength);
        digitalWrite(ledPin, LOW); 
    }else {
        // sound normal: LED off
        digitalWrite(ledPin, LOW); 
    }
}
