// Chloe Crozier
// 3.20.2024 V0
// This is a modular version of my original functionality testing code from ultrasound_functionality.ino
// The code/design is based on the ultrasonic grove module from the launchpad_kit repo on github
// Repo: https://github.com/Seeed-Studio/LaunchPad_Kit/tree/master/Grove_Modules/ultrasonic_ranger// Here is the implementation of the ultrasound class functions.
// Sensor Part: https://www.adafruit.com/product/4007#technical-details
//
// Main file to run tests

// use the header file we wrote for this module
#include "Ultrasound.h"
// Included based on custommodule from class
#include "minunit.h"

// Set the trigger pin (we have it connected to P6_2)
#define TRIGGER P6_2
// Set the echo return pin (we have it connected to P6_3)
#define ECHO P6_3
// Set a macro for when we want to turn on a unti testing mode
#define DEBUG_MODE true


long duration;
long distance;
Ultrasound ultrasound(TRIGGER, ECHO);

void setup() 
{
    Serial.begin(9600);
}


// This was used in the customemodule example we went over in class
int tests_run = 0;
int tests_passed = 0;
int assertions = 0;

// Test to ensure that the inclusive detection threshold is attainable for the sensor's limitations (plus some wiggle room to test edge cases)
static char *test_verifyParkingSpaceDist() {
    double halfOfSpace = ultrasound.getParkingSpaceDist() / 2.0;
    
    // Verify that one minus the halfway point is at/above the minimum detectable distance of 2 cm (
    mu_assert("Error: Parking space dist/2 - 1 below the the range [2, 450]", halfOfSpace + 1 >= 2);
    // This is redundant, but I just want my code to be readable and I'm not sure which method makes more sense
    mu_assert("Error: Parking space dist/2 - 1 below the the range [2, 450]", ultrasound.getParkingSpaceDist() >= 6);
  
    // Verify that one plus the halfway point is at/below the maximum detectable distance of 450 cm
    mu_assert("Error: Parking space dist/2 + 1 is not in the range [2, 450]", halfOfSpace - 1 <= 450);
    // Redundant for the same reason the minimum boundary assert above is
    mu_assert("Error: Parking space dist/2 + 1 is not in the range [2, 450]", ultrasound.getParkingSpaceDist() <= 898);

    return NULL;
}

// Test to ensure that we are getting accurate/reasonable distance readings
static char *test_isValidDist() {
    // Verify that a middle range reading works
    mu_assert("Error: Distance should be valid", ultrasound.isValidDist(100) == true);

    // Verify that past the lower boundary is invalid
    mu_assert("Error: Distance is below the datasheet range and should be invalid", ultrasound.isValidDist(1) == false);

    // Verify that above the upper boundary is invalid
    mu_assert("Error: Distance is above the datasheet range and should be invalid", ultrasound.isValidDist(451) == false);

    return NULL;
}


// Test the isOccupied function to ensure that it can detect an object within a certain inclusive range of a parking space (halfway into the spot for now)
static char *test_isOccupied() {
    double halfOfSpot = ultrasound.getParkingSpaceDist() / 2.0;
    
    // Test when distance is exactly half of parking dist
    mu_assert("Error: Parking space should be occupied", ultrasound.isOccupied(halfOfSpot) == 1);
    // Test when distance is just over half of parking dist
    mu_assert("Error: Parking space should not be occupied (just over min dist)", ultrasound.isOccupied(halfOfSpot + 1) == 0);
    // Test when distance is just under half of parking dist
    mu_assert("Error: Parking space should be occupied (just under min dist)", ultrasound.isOccupied(halfOfSpot - 1) == 1);
    // Test when distance is  at the minimum detectable distance
    mu_assert("Error: Parking space should be occupied (min detectable dist)", ultrasound.isOccupied(2) == 1);
    return NULL;
}

// Test the isParked function to ensure that a object is only marked as parked once it is detected for a minimum number of detection cycles
static char *test_isParked() {
    double dist = ultrasound.getParkingSpaceDist() / 2.0;

    // Loop for the number of min detection cycles to simulate an object that has occupied a space long enough
    for (int i = 0; i < ultrasound.getMinDetectionCycles(); i++) {
        ultrasound.isParked(dist);
    }
    mu_assert("Error: The parkedState of the object should be considered parked", ultrasound.getParkedState() == 1);

    // Reset the state to be unoccupied and unparked
    ultrasound.isParked(dist + 1);

    // Loop for just under the min detection cycles to simulate a senario when a car might have jsut driven near the sensor for a brief time
    for (int i = 0; i < ultrasound.getMinDetectionCycles() - 1; i++) {
        ultrasound.isParked(dist);
    }
    mu_assert("Error: The parkedState of the object should not be considered parked", ultrasound.getParkedState() == 0);
    return NULL;
}

// This is from the custommodule example we used in class, I just added the names of the tests I created
static char *all_tests() {
    mu_run_test(test_isValidDist);
    Serial.println();
    mu_run_test(test_verifyParkingSpaceDist);
    Serial.println();
    mu_run_test(test_isOccupied);
    Serial.println();
    mu_run_test(test_isParked);
    
    return NULL;
}



void loop()
{
    // If we want to run the units tests, we can set DEBUG_MODE to true
    if(DEBUG_MODE) {
      char *result = all_tests();
      Serial.print("\nTests run: ");
      Serial.println(tests_run);
      
      if (result != 0) {
        Serial.print("Tests passed: ");
        Serial.println(tests_passed);
      } else {
        Serial.println("ALL TESTS PASSED\n");
      }
      
      
      Serial.println();
      suspend();
    }
    // If we want to run the normal functionality testing, we can se DEBUG_MODE to false
    else {
      // Get the time and distance traveled
      duration = ultrasound.measureTime();
      distance = ultrasound.measureDist(duration);

      if(ultrasound.isValidDist(distance)){
         // Run isParked (void) to update the parkedState bool and print the details of the current state
         ultrasound.isParked(distance);
      }

      Serial.println();
      delay(ultrasound.getCycleLength());
    }
}

 
