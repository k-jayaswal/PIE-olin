// LED pins (must be PWM-capable pins to control brightness via potentiometer)
int led1 = 5; // red
int led2 = 6; // orange
int led3 = 9; // yellow
int led4 = 10; // green
int led5 = 11; // blue

// button variables
int button = 13; // pushbutton pin
int pattern = 1; // current LED pattern
int prev_button = HIGH; // previous button state
int curr_button = HIGH; // current button state

void setup() {
  // set LEDs as outputs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  // set button as input with pull-up resistor
  pinMode(button, INPUT_PULLUP); 

  // start serial monitor for debugging
  Serial.begin(9600);
}

void loop() {
  // read potentiometer (analog input)
  int pot = analogRead(A0);
  // map potentiometer value to brightness 
  int brightness = map(pot, 0, 1023, 0, 255);
  // Serial.println(brightness); // print brightness for testing

  // read button state
  curr_button = digitalRead(button);

  // detect button press and change pattern
  if (prev_button == HIGH && curr_button == LOW) {
    pattern = pattern + 1;     // go to next pattern
    if (pattern > 6) {         // loop back after 6
      pattern = 1;
    }
    delay(200); // general delay 
  }
  prev_button = curr_button;

  // choose LED behavior based on pattern number
  
  // pattern 1: biker is braking
  if (pattern == 1) { 
    // all LEDs on with brightness
    analogWrite(led1, brightness);
    analogWrite(led2, brightness);
    analogWrite(led3, brightness);
    analogWrite(led4, brightness);
    analogWrite(led5, brightness);
  }

  // pattern 2: biker is at rest (not "driving")
  else if (pattern == 2) {
    // all LEDs off
    analogWrite(led1, 0);
    analogWrite(led2, 0);
    analogWrite(led3, 0);
    analogWrite(led4, 0);
    analogWrite(led5, 0);
  }

  // pattern 3: biker is "driving"
  else if (pattern == 3) {
    // all LEDs blink together
    analogWrite(led1, brightness);
    analogWrite(led2, brightness);
    analogWrite(led3, brightness);
    analogWrite(led4, brightness);
    analogWrite(led5, brightness);
    delay(300);
    analogWrite(led1, 0);
    analogWrite(led2, 0);
    analogWrite(led3, 0);
    analogWrite(led4, 0);
    analogWrite(led5, 0);
    delay(300);
  }

  // pattern 4: biker is turning left
  else if (pattern == 4) {
    // LEDs light up one by one (red to blue)
    analogWrite(led1, brightness); delay(150); analogWrite(led1, 0);
    analogWrite(led2, brightness); delay(150); analogWrite(led2, 0);
    analogWrite(led3, brightness); delay(150); analogWrite(led3, 0);
    analogWrite(led4, brightness); delay(150); analogWrite(led4, 0);
    analogWrite(led5, brightness); delay(150); analogWrite(led5, 0);
  }

  // pattern 5: biker is turning right
  else if (pattern == 5) {
    // LEDs light up one by one (blue to red)
    analogWrite(led5, brightness); delay(150); analogWrite(led5, 0);
    analogWrite(led4, brightness); delay(150); analogWrite(led4, 0);
    analogWrite(led3, brightness); delay(150); analogWrite(led3, 0);
    analogWrite(led2, brightness); delay(150); analogWrite(led2, 0);
    analogWrite(led1, brightness); delay(150); analogWrite(led1, 0);
  }

  // pattern 6: biker has emergency lights on 
  else if (pattern == 6) {
    // only first and last LED blink alternately
    analogWrite(led5, brightness); delay(150); analogWrite(led5, 0);
    analogWrite(led1, brightness); delay(150); analogWrite(led1, 0);
  }
}