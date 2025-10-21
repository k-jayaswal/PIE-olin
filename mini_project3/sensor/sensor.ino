#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(3);

// Sensors: Right to Left = A3, A2, A1, A0
const int S1 = A0;  // Left outer
const int S2 = A1;  // Left middle
const int S3 = A2;  // Right middle
const int S4 = A3;  // Right outer

const int threshold = 200;  // Adjust to your surface

// Base speeds (super slow)
const int baseLeftSpeed = 35;
const int baseRightSpeed = 35;

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  Serial.println("=== Line Follower with Spin Recovery ===");

  leftMotor->setSpeed(baseLeftSpeed);
  rightMotor->setSpeed(baseRightSpeed);
}

void loop() {
  int rawS1 = analogRead(S1);
  int rawS2 = analogRead(S2);
  int rawS3 = analogRead(S3);
  int rawS4 = analogRead(S4);

  int s1 = (rawS1 < threshold) ? 1 : 0;
  int s2 = (rawS2 < threshold) ? 1 : 0;
  int s3 = (rawS3 < threshold) ? 1 : 0;
  int s4 = (rawS4 < threshold) ? 1 : 0;

  // Sensor debug output
  Serial.print("Raw: ");
  Serial.print(rawS1); Serial.print(" ");
  Serial.print(rawS2); Serial.print(" ");
  Serial.print(rawS3); Serial.print(" ");
  Serial.println(rawS4);

  Serial.print("Binary: ");
  Serial.print(s1); Serial.print(s2); Serial.print(s3); Serial.println(s4);

  // === MAIN LOGIC ===

  if ((s1 == 1 && s2 == 1 && s3 == 1 && s4 == 0) || (s1 == 1 && s2 == 1 && s3 == 0 && s4 == 0)) {
    while (true) {
      sharpRightSpin();
      rawS1 = analogRead(S1);
      rawS2 = analogRead(S2);
      s1 = (rawS1 < threshold) ? 1 : 0;
      s2 = (rawS2 < threshold) ? 1 : 0;
      if (s1 == 0 && s2 == 0) break;
    }
  } else if ((s1 == 0 && s2 == 1 && s3 == 1 && s4 == 1) || (s1 == 0 && s2 == 0 && s3 == 1 && s4 == 1)) {
    while (true) {
      sharpLeftSpin();
      rawS3 = analogRead(S3);
      rawS4 = analogRead(S4);
      s3 = (rawS3 < threshold) ? 1 : 0;
      s4 = (rawS4 < threshold) ? 1 : 0;
      if (s3 == 0 && s4 == 0) break;
    }
  } else if (s2 == 0 && s3 == 0) {
    goForward();
  } else if (s2 == 1 && s3 == 0) {
    softLeftTurn();
  } else if (s2 == 0 && s3 == 1) {
    softRightTurn();
  } else {
    searchForLine();
  }
}

// === MOVEMENT FUNCTIONS ===

void goForward() {
  leftMotor->setSpeed(baseLeftSpeed);
  rightMotor->setSpeed(baseRightSpeed);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

void turnLeft() {
  leftMotor->setSpeed(15);
  rightMotor->setSpeed(40);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

void turnRight() {
  leftMotor->setSpeed(40);
  rightMotor->setSpeed(15);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

void sharpRightSpin() {
  for (int i = 0; i < 5; i++) {
    leftMotor->setSpeed(30);     // slow backward
    rightMotor->setSpeed(100);    // fast forward
    leftMotor->run(FORWARD);
    rightMotor->run(BACKWARD);
    delay(75);
  }
}

void sharpLeftSpin() {
  for (int i = 0; i < 5; i++) {
    leftMotor->setSpeed(100);     // fast forward
    rightMotor->setSpeed(30);    // slow backward
    leftMotor->run(BACKWARD);
    rightMotor->run(FORWARD);
    delay(75);
  }
}

void softRightTurn() {
  leftMotor->setSpeed(50);
  rightMotor->setSpeed(20);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

void softLeftTurn() {
  leftMotor->setSpeed(20);
  rightMotor->setSpeed(50);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

void searchForLine() {
  leftMotor->setSpeed(25);
  rightMotor->setSpeed(25);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

void hellaSharpRight() {
  leftMotor->setSpeed(85);
  rightMotor->setSpeed(0);
  leftMotor->run(BACKWARD);
  rightMotor->run(RELEASE);
  delay(300);
}

void hellaSharpLeft() {
  leftMotor->setSpeed(0);
  rightMotor->setSpeed(85);
  leftMotor->run(RELEASE);
  rightMotor->run(BACKWARD);
  delay(300);
}