// This is the program that needs to be on the ESP32 to have the NUC
// computer send a reward signal to the stepper motor. The stepper motor will
// turn a certain amount of steps to dispense the reward. 
// The manual advance also working
// Fred 19 sept 2024


#include <Arduino.h>
#include <Wire.h>

#define ENA 10      // Enable pin for stepper driver
#define DIR 5     // Direction pin for stepper driver
#define PUL 18      // Pulse pin for stepper driver
#define micro 8 // Can be 1, 2, 4, 8, 16, 32

#define Switch1 9 // Switch input
#define F_LED 2    // LED to indicate operation

void Reward_out();

void setup() {
  Serial.begin(115200);
  
  pinMode(Switch1, INPUT_PULLUP);
  pinMode(F_LED, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(PUL, OUTPUT);
}

void loop() {
  // Check if there is any serial input
  while (Serial.available()) {
    String command = Serial.readString();
    if (command == "FDR") { // Check if the command is exactly "FDR"
      Serial.println("FDR command received.");
      Reward_out();
    }
  }
  
  // Check switch state independently of serial input
  int switchValue = digitalRead(Switch1);
  if (switchValue == LOW) { // Assuming the switch activation threshold is properly set
    //Serial.println("Switch activated.");
    Reward_out();
  }

  delay(100); // Small delay to stabilize readings and reduce processing load
}

void Reward_out() {
  //Serial.println("Reward triggered: Motor running...");
  digitalWrite(DIR, HIGH);  // Clockwise when looking from top
  digitalWrite(ENA, LOW);  // Enable motor
  delay(100);
  for (int x = 0; x < (200 / 2) * micro; x++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(500 / micro);
    digitalWrite(PUL, LOW);
    delayMicroseconds(500 / micro);
    delay(1);
  }
  delay(100);
  digitalWrite(ENA, HIGH);  // Disable motor
  //delay(1000);  // Wait before allowing another reward
  //Serial.println("Reward finished: Motor stopped.");
}