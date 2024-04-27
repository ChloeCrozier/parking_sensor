/*Eliza Sorber 
****** This is a basic file that shows the microphone functionality. 
****** The microphone we are using is this one from Sparkfun: https://www.sparkfun.com/products/12758
****** I used examples from https://www.instructables.com/Electret-microphone/ and https://github.com/zippy/sound_alarm
        and https://learn.sparkfun.com/tutorials/electret-mic-breakout-board-hookup-guide#resources-and-going-further- 
****** see modular_microphone directory for the modular version of this code. 
****** This code is measuring sound above a arbitrary threshold values. In our final product, 
        we plan on using our envelope detector circuit to measure sound above a certain threshold
        so this code is simply a fall back to use for testing while we troubleshoot our hardware
        and solder our pcb 
*/

#define sensorPin P1_4   
#define ledPin P8_2
int sensorValue = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
  // read from sensor
  sensorValue = analogRead(sensorPin); 

  Serial.println(sensorValue);
  
  if (sensorValue < 1000 || sensorValue > 3000) {     
    // sound above certain level: LED on
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW); 
  }  
  else {
    // sound normal: LED off
    digitalWrite(ledPin, LOW); 
  }   
}