// CPSC 6820 - Team Park Sharks
// 3.25.2024 V0
// Module that will be used for our final prototpye demo

/*
   HEADER FILES
*/
#include <cc1101.h>
#include <macros.h>
#include <pins.h>
#include <registers.h>
// Include the RX and TX header files
#include "radioRX.h"
#include "radioTX.h"
// use the header file we wrote for the ultrasound
#include "ultrasound.h"
// Include the headerfule for the mic module
#include "microphone.h"

/*
   PINS
*/
// Button to control system status (On or Off)
#define ON_OFF_BUTTON P4_2
// Button to control debug status (Toggles LED parking indicator)
#define DEBUG_BUTTON P4_1
//microphone
#define MICROPHONE_PIN P1_4
// Set the trigger pin (we have it connected to P6_2)
#define TRIGGER P6_2
// Set the echo return pin (we have it connected to P6_3)
#define ECHO P6_3
// Envelope detector pin
#define ENVELOPE_PIN P3_5
// LEDs
#define RED_PIN P8_1
#define BLUE_PIN P8_2
#define YELLOW_PIN P8_3


/*
   CONSTANTS
*/
// Set a macro in cm for test verification and also parking spaces are around 5 meters (500 cm)
#define PARKING_SPACE_DIST 200
// Set a macro to determine the minimun number of cycles needer to consider an object occupying the spot as parked
#define PARKING_DETECTION_CYCLES 5
// Radio Channel
#define RADIO_CHANNEL 5
// Radio Data Rate
#define RADIO_DATA_RATE 4

/*
   TIMERS
*/
// Interval for running ultrasound check when in slow mode
#define ULTRASOUND_SLOW 1000
// Interval for running ultrasound check when in fast mode
#define ULTRASOUND_FAST 200
// Default length of microphone cycles
#define MICROPHONE_CYCLE 50 // Will sample every cycle
// Default length of microphone cycles
#define RADIO_CYCLE 3500

/*
    OPTIONS
*/
// Set a macro to determine if we are entering receiving (RX) or transmitting (TX) mode
#define RECEIVING_MODE true
// Set a macro for when we want to turn on a unit testing mode
#define DEBUG_MODE true

// Initialize radio objects
RadioRX radioRX;
RadioTX radioTX;

// Define state vars
byte systemRunning = 1;
byte debugMode = 1;

// Define component objects
Ultrasound ultrasound(TRIGGER, ECHO, PARKING_SPACE_DIST, PARKING_DETECTION_CYCLES);
Microphone microphone(MICROPHONE_PIN, YELLOW_PIN);

// Timers
int timer_m = 0; // Microphone
int timer_u = 0; // Ultrasound
int timer_r = 0; // Radio (Sending)
int tickLength = 25;

int micDelay = 0;

// Parking state
bool oldState = false;
bool newState;

void setup() {
  Serial.begin(9600);

  // Setup radio
  Radio.Init();
  Radio.SetDataRate(RADIO_DATA_RATE); // Needs to be the same in Tx and Rx
  Radio.SetLogicalChannel(RADIO_CHANNEL); // Needs to be the same as receiver

  // RX setup
  if (RECEIVING_MODE) {
    radioRX.setRX();
    return;
  }

  // Code below should only be run for transmitter (AKA Parking Sensor)
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);

  // TX setup
  radioTX.setTX();

  pinMode(ON_OFF_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ON_OFF_BUTTON), onOffPress, RISING);
  pinMode(DEBUG_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEBUG_BUTTON), debugPress, RISING);

  //interrupt for envelope detector
  pinMode(ENVELOPE_PIN, INPUT_PULLUP);

  int sensorValue = microphone.readValueDebug(); // Why is this here??
  microphone.triggerDebugLight(sensorValue);

  ultrasound.setCycleLength(ULTRASOUND_FAST);
  microphone.setCycleLength(MICROPHONE_CYCLE);
}

void loop() {
  // RECEIVING mode is for HUB device
  if (RECEIVING_MODE) {
    radioRX.receivePacket();
    if (radioRX.getReceivedStatus()) {
      if (debugMode) radioRX.printBuffer();

      int value = radioRX.getPacketVal();
      bool parkingStatus = radioRX.getParkedState();
      Serial.print("The parking sensor has been ");
      Serial.print(parkingStatus ? "occupied" : "available");
      Serial.print(" for ");
      Serial.print(value);
      Serial.print(" seconds.\n");
    }
    return;
  }

  // Below is all logic for transmitting mode (AKA parking sensor)
  if (!systemRunning) return;

  timer_u += tickLength;
  timer_m += tickLength;
  timer_r += tickLength;

  // Microphone tick
  if (timer_m >= microphone.getCycleLength()) {
    timer_m %= microphone.getCycleLength();

    int value = debugMode ? microphone.readValueDebug() : microphone.readValue();

    if (microphone.valueIsInRange(value) && oldState) {
      if (debugMode) digitalWrite(YELLOW_PIN, HIGH);
      ultrasound.setCycleLength(ULTRASOUND_FAST);

    } else if (!microphone.valueIsInRange(value) && oldState) {
      if (debugMode) digitalWrite(YELLOW_PIN, LOW);
      ultrasound.setCycleLength(ULTRASOUND_SLOW);

    } else {
      if (debugMode) digitalWrite(YELLOW_PIN, LOW);
      ultrasound.setCycleLength(ULTRASOUND_FAST);
    }

  }

  // Ultrasound tick
  if (timer_u >= ultrasound.getCycleLength()) {
    timer_u %= ultrasound.getCycleLength();

    long duration = ultrasound.measureTime();
    long distance = ultrasound.measureDist(duration);

    oldState = ultrasound.getParkedState();

    // update parked state
    ultrasound.isParked(distance);

    newState = ultrasound.getParkedState();

    if (!oldState && newState) {
      // Car is now parked
      if (debugMode) if (debugMode) digitalWrite(RED_PIN, HIGH);
      if (debugMode) if (debugMode) digitalWrite(BLUE_PIN, LOW);
      ultrasound.setCycleLength(ULTRASOUND_SLOW);
    } else if (oldState && !newState) {
      // Car is no longer parked
      if (debugMode) if (debugMode) digitalWrite(RED_PIN, LOW);
      if (debugMode) if (debugMode) digitalWrite(BLUE_PIN, HIGH);
      ultrasound.setCycleLength(ULTRASOUND_FAST);
    }

    // Radio tick (Triggered inside of ultrasound tick)
    if (timer_r >= RADIO_CYCLE) {
      Serial.println("Radio tick");

      timer_r %= RADIO_CYCLE;
      int parkedTimeInSeconds = (ultrasound.getTimeParked() / 1000);
      if (parkedTimeInSeconds > 511) parkedTimeInSeconds = 511;
      radioTX.sendData(parkedTimeInSeconds, newState);
    }
  }

  delay(tickLength);
}

void onOffPress() {
  systemRunning = !systemRunning;
  Serial.print("SYSTEM RUNNING was set to: ");
  Serial.println(systemRunning);
}

void debugPress() {
  debugMode = !debugMode;
  Serial.print("DEBUG MODE was set to: ");
  Serial.println(systemRunning);
}
