// Chloe Crozier
// 3.19.2024 V0
// This simple code just prints the detected distance of an object infron of the ultrasound sensor to the serial monitor
// I based my code structure on the ultrasonic grove module from the launchpad_kit repo on github (references within code comments)
// Repo: https://github.com/Seeed-Studio/LaunchPad_Kit/tree/master/Grove_Modules/ultrasonic_ranger
// This is the first program I wrote, so it is very simple, but I used it to get ideas for unit tests and modular design
// Sensor Part: https://www.adafruit.com/product/4007#technical-details

// Set the trigger pin (we have it connected to P6_2)
#define TRIGGER P6_2
// Set the echo return pin (we have it connected to P6_3)
#define ECHO P6_3
int timeSending;
int readInTime;

void setup() {
  // Initialize so I can see the output on the serial monitor
  Serial.begin(9600);
  // Set the trigger as output and the echo pin as the returning input
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  timeSending = 15;
  readInTime = 0;
}

void loop() {
  // In Ultrasonic.cpp, duration and distance were two variables declared as longs, so I did that here
  long duration, distance;
  
  // The module started by clearing the output pin to ensure we are sending from a baseline low
  digitalWrite(TRIGGER, LOW);
  // They set a delay for 2 us, so I did the same
  delayMicroseconds(2);
  
  // Now, they set the trigger to high until they wanted to start receiving
  digitalWrite(TRIGGER, HIGH);
  // They set a time where the pin is high as 10 us, so I made another var in case I reference it a lot later
  // Also, pulseIn has more issues the longer the pulse, so I'll keep it short
  delayMicroseconds(timeSending);
  // Set the trigger low to stop sending
  digitalWrite(TRIGGER, LOW);
  
  // Read the input pin with pulseIn: https://energia.nu/reference/en/language/functions/advanced-io/pulsein/
  // This returns the time it takes for the pin to return to LOW (in microseconds -> us)
  // This means that it will start timeing when ECHO first hears back from the TRIGGER and then stops timing when ECHO stops
  duration = pulseIn(ECHO, HIGH);

    // The example module found the dist in centimeters with this: RangeInCentimeters = duration/29/2;
    // distance = (duration / 29) * 0.5; // This works, but I want the formula to look intuitive
  
  // Sound travels at 34300 cm/s and it is traveling twice the dist from send to return
  // time * speed * 0.5 (in seconds and cm) -> duration * (1/10^6) * 34300 * 0.5
  // I subtracted readInTime (0 for now) from duration becuase that is how long it takes to read an analog pin (100 us)
  // According to this source, it takes 100 us to read in: https://energia.nu/reference/en/language/functions/analog-io/analogread/
  distance = (((duration * 34300) / 2) / 1000000);

  // Added for debugging based on the datasheet ranges
  if(distance <= 2 || distance >= 250){
    Serial.println("Inaccurate Range!");
    if(distance < 0){
      distance = 0; 
    }
  }

  // Print the output
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Delay for clarity/verification in reading the output
  delay(1500);
}
