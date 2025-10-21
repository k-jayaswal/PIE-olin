#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Motor Setup (Adafruit Motor Shield)
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor3 = AFMS.getMotor(3);  // left motor (M3)
Adafruit_DCMotor *myMotor4 = AFMS.getMotor(4);  // right motor (M4)

const int baseSpeed = 150;  // base speed (0–255)

// Sensor Setup
const int sensorPins[4] = {A0, A1, A2, A3};  // IR sensor colors: 
int sensorValues[4];

// PID Setup
float Kp = 1.2; // correction is based on "off-path-ness"
float Ki = 0.0; // correction for long term errors
float Kd = 0.5; // correction based on how fast error is changing

float error = 0, previousError = 0, integral = 0;

// calibration
const int tapeValue = 42;     // lightest surface tape 
const int carpetValue = 513;  // darkest surface carpet

// ADJUST for sensor internal distance
float weights[4] = {-5.0, -2.0, 2.0, 5.0};

void setup() {
  Serial.begin(9600);
  AFMS.begin();  

  myMotor3->setSpeed(baseSpeed);
  myMotor4->setSpeed(baseSpeed);
  myMotor3->run(FORWARD);
  myMotor4->run(FORWARD);

  Serial.println("Line Follower Started (Adafruit Shield)");
}

void loop() {
  // initializes variables
  float weightedSum = 0;
  float total = 0;

  // read sensors & normalize
  for (int i = 0; i < 4; i++) {
    int raw = analogRead(sensorPins[i]);
    sensorValues[i] = raw;

    // converst raw readings for values between 0 and 1
    float norm = (float)(raw - tapeValue) / (carpetValue - tapeValue);
    norm = constrain(norm, 0.0, 1.0);
    float detect = 1.0 - norm;  // darker = higher detect value

    weightedSum += detect * weights[i];
    total += detect;

    // prints raw and normalized values for each sensor
    Serial.print("S"); Serial.print(i);
    Serial.print(":"); Serial.print(raw);
    Serial.print(" ("); Serial.print(detect, 2); Serial.print(")  ");
  }
  Serial.println();

  // compute error
  if (total > 0.05)
    error = weightedSum / total;
  else
    error = 0;  // no line detection

  // PID control
  integral += error;
  integral = constrain(integral, -100, 100);
  float derivative = error - previousError;
  float output = Kp * error + Ki * integral + Kd * derivative;
  previousError = error;

  // convert PID output to motor speeds
  int leftSpeed  = baseSpeed - output * 40;
  int rightSpeed = baseSpeed + output * 40;

  leftSpeed  = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  // apply speeds using Adafruit Motor Shield
  myMotor3->setSpeed(leftSpeed);
  myMotor4->setSpeed(rightSpeed);
  myMotor3->run(FORWARD);
  myMotor4->run(FORWARD);

  // debug output
  Serial.print("Err: "); Serial.print(error, 2);
  Serial.print(" Out: "); Serial.print(output, 2);
  Serial.print(" L: "); Serial.print(leftSpeed);
  Serial.print(" R: "); Serial.println(rightSpeed);

  delay(50);
}
