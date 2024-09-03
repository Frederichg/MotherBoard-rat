/* Jacob 2024
 * Test for Stepper Motor
 * This code adjusts the pulse duration based on the chosen microstepping.
 *
 * Pins:
 * ENA - Enable pin for stepper driver (connected to pin 8)
 * DIR - Direction pin for stepper driver (connected to pin 7)
 * PUL - Pulse pin for stepper driver (connected to pin 6)
 * micro - Microstepping value (can be 1, 2, 4, 8, 16, 32)
 */
#include <Arduino.h>

//Borad V1, ENA GPB7, DIR GPB6, PUL GPB5
//Board V2, ENA 10, DIR 5, PUL 18
#define ENA 10  //8      // Enable pin for stepper driver
#define DIR 5   //7      // Direction pin for stepper driver
#define PUL 18  //6      // Pulse pin for stepper driver
#define micro 1   // Can be 1, 2, 4, 8, 16, 32

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(PUL, OUTPUT);
  digitalWrite(ENA, HIGH); // Disable motor
  digitalWrite(DIR, LOW); // Clockwise when looking from top
  digitalWrite(PUL, LOW); // Start out LOW
}

void loop() {
  digitalWrite(ENA, LOW);
  delay(100);
  for (int x = 0; x < 200 * micro; x++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(500 / micro);
    digitalWrite(PUL, LOW);
    delayMicroseconds(500 / micro);
  }
  delay(100);
  digitalWrite(ENA, HIGH);
  delay(1000);
}

