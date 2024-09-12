#include <Arduino.h>

#define Lever_pin 23 // Lever pin
#define BUTTON_PIN 9 // 28 GPIO GPA7

#define ENA 10  //8      // Enable pin for stepper driver
#define DIR 5   //7      // Direction pin for stepper driver
#define PUL 18  //6      // Pulse pin for stepper driver
#define micro 8   // Can be 1, 2, 4, 8, 16, 32
//11 sept 2024, STK 8 with off, off, off,  on, on, off

// Function declaration
void Reward_out();
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;   // the debounce time; increase if the output flickers


void setup() {
  pinMode(Lever_pin, OUTPUT);
  digitalWrite(Lever_pin, HIGH);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Enable internal pull-up resistor
  pinMode(ENA, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(PUL, OUTPUT);
  digitalWrite(ENA, HIGH); // Disable motor
  digitalWrite(DIR, HIGH); // Clockwise when looking from top
  digitalWrite(PUL, LOW); // Start out LOW
  Serial.begin(115200); // Initialize serial communication
}

void loop() {
  static bool buttonState = HIGH;  // the current reading from the input pin
  static bool lastButtonState = HIGH;  // the previous reading from the input pin

  // read the state of the switch into a local variable:
  int reading = digitalRead(BUTTON_PIN);

  // check to see if you just pressed the button
  // (i.e. the input went from HIGH to LOW), and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only trigger the function if the new button state is LOW
      if (buttonState == LOW) {
        Serial.println("Button pressed!");
        Reward_out();
      }
    }
  }

  // save the reading. Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

  digitalWrite(Lever_pin, HIGH);
  delay(50);
  digitalWrite(Lever_pin, LOW);
  delay(50);
}

void Reward_out() {
  digitalWrite(ENA, LOW);
  delay(100);
  for (int x = 0; x < (200 / 2) * micro; x++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(500 / micro);
    digitalWrite(PUL, LOW);
    delayMicroseconds(500 / micro);
  }
  delay(100);
  digitalWrite(ENA, HIGH);
  delay(1000);
}