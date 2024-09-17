#include <Arduino.h>

#define Lever_pin 23 // Lever pin
#define BUTTON_PIN 9 // 28 GPIO GPA7

#define ENA 10  //8      // Enable pin for stepper driver
#define DIR 5   //7      // Direction pin for stepper driver
#define PUL 18  //6      // Pulse pin for stepper driver
#define micro 8   // Can be 1, 2, 4, 8, 16, 32

// Function declaration
void Reward_out();

// Variable to track if the lever has been released
bool leverReleased = true;

void setup() {
  pinMode(Lever_pin, OUTPUT);
  digitalWrite(Lever_pin, HIGH);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Enable internal pull-up resistor
  pinMode(ENA, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(PUL, OUTPUT);
  digitalWrite(ENA, HIGH); // Disable motor
  digitalWrite(DIR, LOW); // Clockwise when looking from top
  digitalWrite(PUL, LOW); // Start out LOW
  Serial.begin(115200); // Initialize serial communication
}

void loop() {
  // Check if the lever is pressed
  if (digitalRead(BUTTON_PIN) == LOW && leverReleased) { // Only trigger reward if lever was released
      Serial.println("Button pressed!");
      leverReleased = false;  // Mark lever as not released
      // Call the Reward_out function
      Reward_out();

      // Add delay to avoid too frequent rewards
      delay(1000); 
  }

  // Check if the lever is released
  if (digitalRead(BUTTON_PIN) == HIGH) {
    leverReleased = true;  // Allow next press to trigger reward
  }
}

void Reward_out() {
  digitalWrite(ENA, LOW);  // Enable motor
  delay(100);
  for (int x = 0; x < (200 / 2) * micro; x++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(500 / micro);
    digitalWrite(PUL, LOW);
    delayMicroseconds(500 / micro);
    delayMicroseconds(1000);  // Slow down motor
  }
  delay(100);
  digitalWrite(ENA, HIGH);  // Disable motor
  delay(500);  // Wait for 1 second before allowing another reward
}
