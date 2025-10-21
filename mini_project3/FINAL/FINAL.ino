#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(3);

// sensors: right to left = A3, A2, A1, A0
const int S1 = A0;  // left outer
const int S2 = A1;  // left middle
const int S3 = A2;  // right middle
const int S4 = A3;  // right outer

const int threshold = 200;

// base speed (can be updated live)
int baseSpeed = 35;

// live motor speeds
int currentLeftSpeed = baseSpeed;
int currentRightSpeed = baseSpeed;

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  leftMotor->setSpeed(baseSpeed);
  rightMotor->setSpeed(baseSpeed);
}

void loop() {
  checkSerialCommands();

  // raw sensor readings
  int rawS1 = analogRead(S1);
  int rawS2 = analogRead(S2);
  int rawS3 = analogRead(S3);
  int rawS4 = analogRead(S4);

  // binary conversion
  int s1 = (rawS1 < threshold) ? 1 : 0;
  int s2 = (rawS2 < threshold) ? 1 : 0;
  int s3 = (rawS3 < threshold) ? 1 : 0;
  int s4 = (rawS4 < threshold) ? 1 : 0;

  // print 6 values: 4 sensors + 2 motor speeds
  Serial.print(rawS1); Serial.print(" ");
  Serial.print(rawS2); Serial.print(" ");
  Serial.print(rawS3); Serial.print(" ");
  Serial.print(rawS4); Serial.print(" ");
  Serial.print(currentLeftSpeed); Serial.print(" ");
  Serial.println(currentRightSpeed);
  delay(100);

  // MAIN LOGIC
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

// MOVEMENT FUNCTIONS

void goForward() {
  currentLeftSpeed = baseSpeed;
  currentRightSpeed = baseSpeed;
  leftMotor->setSpeed(currentLeftSpeed);
  rightMotor->setSpeed(currentRightSpeed);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

void softLeftTurn() {
  currentLeftSpeed = baseSpeed - 15;
  currentRightSpeed = baseSpeed + 15;
  leftMotor->setSpeed(currentLeftSpeed);
  rightMotor->setSpeed(currentRightSpeed);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

void softRightTurn() {
  currentLeftSpeed = baseSpeed + 15;
  currentRightSpeed = baseSpeed - 15;
  leftMotor->setSpeed(currentLeftSpeed);
  rightMotor->setSpeed(currentRightSpeed);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

void sharpRightSpin() {
  for (int i = 0; i < 5; i++) {
    currentLeftSpeed = baseSpeed - 5;
    currentRightSpeed = baseSpeed + 50;
    leftMotor->setSpeed(currentLeftSpeed);
    rightMotor->setSpeed(currentRightSpeed);
    leftMotor->run(BACKWARD);
    rightMotor->run(FORWARD);
    delay(75);
  }
}

void sharpLeftSpin() {
  for (int i = 0; i < 5; i++) {
    currentLeftSpeed = baseSpeed + 50;
    currentRightSpeed = baseSpeed - 5;
    leftMotor->setSpeed(currentLeftSpeed);
    rightMotor->setSpeed(currentRightSpeed);
    leftMotor->run(FORWARD);
    rightMotor->run(BACKWARD);
    delay(75);
  }
}

void searchForLine() {
  currentLeftSpeed = baseSpeed - 10;
  currentRightSpeed = baseSpeed - 10;
  leftMotor->setSpeed(currentLeftSpeed);
  rightMotor->setSpeed(currentRightSpeed);
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
}

// SERIAL COMMAND HANDLER

void checkSerialCommands() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.startsWith("set base ")) {
      baseSpeed = input.substring(9).toInt();
      Serial.println("Updated baseSpeed to " + String(baseSpeed));
    }
  }
}