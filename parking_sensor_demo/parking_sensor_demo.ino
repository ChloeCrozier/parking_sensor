// CPSC 6820 - Team Park Sharks
// 3.25.2024 V0

/*
 * This file is designed to control the main flow for the ParkSharks parking sensor
 */

// Button to control system status (On or Off)
const byte ON_OFF_BUTTON = 0;
// Button to control debug status (Toggles LED parking indicator)
const byte DEBUG_BUTTON = 0;

byte systemRunning = 1;
byte debugMode = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(ON_OFF_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ON_OFF_BUTTON), onOffPress, CHANGE);
  pinMode(DEBUG_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEBUG_BUTTON), debugPress, CHANGE);
}

void loop() {
  if (systemRunning) {
    
  }
}

void onOffPress() {
  systemRunning = !systemRunning;
}

void debugPress() {
  debugMode = !debugMode;
}
