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
  //Serial.print("on");
  digitalWrite(Lever_pin, HIGH);
  //delay(2000);

  if (digitalRead(BUTTON_PIN) == LOW) { // Check for LOW state
      Serial.println("Button pressed!");
      // Call the Reward_out function
      Reward_out();

  //digitalWrite(Lever_pin, LOW);
  delay(1000);
   }

}

void Reward_out() {
  digitalWrite(ENA, LOW);
  delay(100);
  for (int x = 0; x < (200/2) * micro; x++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(500 / micro);
    digitalWrite(PUL, LOW);
    delayMicroseconds(500 / micro);
    delayMicroseconds(1000);  //It is a way to slow it down
  }
  delay(100);
  digitalWrite(ENA, HIGH);
  delay(1000);
  
}