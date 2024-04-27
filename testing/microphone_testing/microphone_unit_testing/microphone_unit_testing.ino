// Harrison Langpaul
// 3.26.2024 V0
// 
// Main file including several tests for unit testing our microphone

#include "microphone.h"
#include "minunit.h"

// Pins
#define sensorPin P1_4   
#define ledPin P8_2

// Sensitivity for the microphone
#define lowerBound 1000
#define upperBound 3000

// Constant for an unreasonably high microphone value
#define VALUE_TOO_HIGH 100000
// Set a macro for when we want to turn on a unit testing mode
#define DEBUG_MODE true

// From custommodule example in class
int tests_run = 0;
int tests_passed = 0;
int assertions = 0;

// Create our microphone as defined in this module
Microphone mic(sensorPin, ledPin);

void setup() {
  Serial.begin(9600);
}

// Test to ensure the microphone is reading reasonable values
static char *test_ensureReasonableBounds() {
  for (int i = 0; i < 100; i++) {
    int level = mic.readValue();
    mu_assert("Error: microphone is reading a zero value.", level != 0);
    mu_assert("Error: microphone is reading an unreasonably high value.", level > VALUE_TOO_HIGH);
    delay(50);
  }

  return NULL;
}

// Test is not automated and must be watched in person, the light should blink EXACTLY 3 times if this test is passed
static char *test_led_in_range() {
  // Test led range, should blink 3 times
  // Set range to 0-2, then call trigger value of 1, 3 times- LED should blink 3 times
  mic.setThreshold(0, 2);
  for (int i = 0; i < 3; i++) {
    mic.triggerDebugLight(1);
    delay(500);
  }
  
  return NULL;
}


// Test is not automated and must be watched in person, the light should not blink
// See test above #test_led_in_range, if both are ran then LED should blink exactly 3 times, and no more
static char *test_led_out_of_range() {
  // Test led range, should not blink.
  // Set range to 0-0, then call a trigger value of 1, 3 times
  mic.setThreshold(0, 0);
  for (int i = 0; i < 3; i++) {
    mic.triggerDebugLight(1);
    delay(500);
  }

  return NULL;
}

// From custommodule example in class
static char *all_tests() {
    mu_run_test(test_ensureReasonableBounds);
    Serial.println();
    mu_run_test(test_led_in_range);
    Serial.println();
    mu_run_test(test_led_out_of_range);
    
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
    } else {
      mic.setThreshold(lowerBound, upperBound);
      int sensorValue = mic.readValueDebug();
      mic.triggerDebugLight(sensorValue);
    }
}
