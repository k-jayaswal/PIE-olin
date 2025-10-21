#include <Servo.h>

// Create Servo objects for pan and tilt
Servo servo;  
Servo servo2;

// Sensor and LED setup
int sensorPin = A0;       // IR sensor analog input
int ledPin = 13;          // Onboard LED (optional)
int sensorValue = 0;      // Stores sensor reading
int outputValue = 0;      // Placeholder (unused)

// Servo position variables
int pos = 0;              
int pos2 = 180;           
int sensing = 1;          // Flag (unused)

void setup() {
  servo.attach(6);        // Pan servo on digital pin 6
  servo2.attach(3);       // Tilt servo on digital pin 3
  Serial.begin(9600);     // Start serial communication
  pinMode(ledPin, OUTPUT);
}

void loop() {
  sensorValue = analogRead(sensorPin);  // Initial sensor read

  // Sweep tilt servo from 180 to 90 degrees
  for (pos2 = 180; pos2 >= 90; pos2 -= 2) {
    
    // Sweep pan servo from 0 to 90 degrees
    for (pos = 0; pos <= 90; pos += 2) {
      servo.write(pos);           // Update pan angle
      servo2.write(pos2);         // Update tilt angle
      sensorValue = analogRead(sensorPin);  // Read sensor
      Serial.print(servo.read()); Serial.print(" ");
      Serial.print(servo2.read()); Serial.print(" ");
      Serial.println(sensorValue);          // Send data
      delay(200);                           // Wait
    }

    pos2 -= 5;  // Additional decrement for tilt

    // Sweep pan servo back from 90 to 0 degrees
    for (pos = 90; pos >= 0; pos -= 2) {
      servo.write(pos);
      servo2.write(pos2);
      sensorValue = analogRead(sensorPin);
      Serial.print(servo.read()); Serial.print(" ");
      Serial.print(servo2.read()); Serial.print(" ");
      Serial.println(sensorValue);
      delay(200);
    }
  }
}