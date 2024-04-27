// Chloe Crozier
// 3.20.2024 V0
// This is a modular version of my original functionality testing code from ultrasound_functionality.ino
// The code/design is based on the ultrasonic grove module from the launchpad_kit repo on github
// Repo: https://github.com/Seeed-Studio/LaunchPad_Kit/tree/master/Grove_Modules/ultrasonic_ranger// Here is the implementation of the ultrasound class functions.
// Sensor Part: https://www.adafruit.com/product/4007#technical-details
// 
// Here are the implementations of the ultrasound class functions from ultrasound.h

#include "Ultrasound.h"

// Constructor to intialize the I/O pins and time duration vars
Ultrasound::Ultrasound(int output, int input) {
  triggerPin = output;
  pinMode(triggerPin, OUTPUT);
  
  echoPin = input;
  pinMode(echoPin,INPUT);

  cycleLength = 1500;
  
  // Shorter times are better for pulseIn() and this is measured in microseconds (us)
  timeSending = 15;
  
  // Parking spaces are around 5 meters (500 cm)
  parkingSpaceDist = 16;
  
  // Number of measurements needed to determine if the dectected object can be considered as parked
  minDetectionCycles = 5;
  
  // This var is used to ensure that a spot is only occupied once the object is dectected for n consecutive measurements
  stateCounter = 0;
  
  // Set the parked state to false (no object present long enough to be considered parked)
  parkedState = false;

  // Set the object coutner to 0
  objectCount = 0;

  // Set timeParked to 0
  timeParked = 0;
}

// This is the same method I used in ultrasound_functionality.ino
long Ultrasound::measureTime(void) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(timeSending);
  digitalWrite(triggerPin, LOW);
  
  return pulseIn(echoPin, HIGH);
}

// This is the same formula from ultrasound_functionality.ino, but I removed the readInTime part after find better performance with out it
long Ultrasound::measureDist(long duration) {
  return (((duration * 34300) / 2) / 1000000);
}

bool Ultrasound::isValidDist(long distance) {
  // If the distance is out side of the accurate/reasonable datasheet ranges, print what the inaccurate value was
  if ((distance < 2) || (distance > 450)) {
      Serial.print("An inaccurate distance of ");
      Serial.print(distance);
      Serial.println(" was measured. Next cycle starting.");
      return false;
  }
  // If the distance is within an accurate range, print the valid values
  else {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
      return true;
   }
}

// New function that returns true if an object is detected half-way into a parking spot
bool Ultrasound::isOccupied(long distance) {
  // If the object is measured at least half way into the parking spot
  if (distance <= (parkingSpaceDist / 2.0)) {
    Serial.println("State: Occupied");
    return true;
  } else {
    Serial.println("State: Unoccupied");
    return false;
  }
}

// New function that delares an object as parked once it is detected at least minDetectionCycles number of times
void Ultrasound::isParked(long distance) {
  bool occupiedState = isOccupied(distance);
  // If the object has been detected, but not long enough to be parked
  if (occupiedState && !parkedState) {
    stateCounter++;
    if (stateCounter >= minDetectionCycles) {
      stateCounter = 0;
      parkedState = true;
      objectCount++;
      timeParked = millis();
      Serial.print("Car #");
      Serial.print(getObjectCount());
      Serial.println(" has parked");
    }
  }
  // If the detected object has been sensed long enough to be considered parked
  else if (occupiedState && parkedState) {
    Serial.print("Car #");
    Serial.print(getObjectCount());
    Serial.print(" has been parked for ");
    // When printing the time parked, I make sure to include the time of  minDetectionCycles cycles
    Serial.print((getTimeParked() + (minDetectionCycles * (cycleLength))) / 1000.0);
    Serial.println(" seconds");
  }
  // If the object is not detected, but it was previously parked
  else if (!occupiedState && parkedState) {
      Serial.print("Car #");
      Serial.print(getObjectCount());
      Serial.println(" has left the spot");
    parkedState = false;
  }
  // If there is no detected object or previously parked car
  else {
//    Serial.println("No change, the spot is still unoccupied");
    stateCounter = 0;
  }
}

// New function that returns the length of the input/output cycle (time between send bursts)
int Ultrasound::getCycleLength(void) {
    return cycleLength;
}

// New function to return the parked state
bool Ultrasound::getParkedState(void) {
    return parkedState;
}

// New function to return the number of objects that have parked
int Ultrasound::getObjectCount(void) {
    return objectCount;
}

// New function that returns the current time minus the tiem of the last park
long Ultrasound::getTimeParked(void) {
    return (millis() - timeParked);
}

double Ultrasound::getParkingSpaceDist(void) {
  return parkingSpaceDist;
}

int Ultrasound::getMinDetectionCycles(void) {
  return minDetectionCycles;
}
