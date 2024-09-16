#include <Arduino.h>

#define BUTTON_PIN 9 // 28 GPIO GPA7

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Enable internal pull-up resistor
  Serial.begin(115200); // Initialize serial communication
}

void loop() {
  int currentButtonState = digitalRead(BUTTON_PIN); // Read the current button state
  if (currentButtonState == HIGH) {
    Serial.println("Pin is HIGH (no press)");
  } else if (currentButtonState == LOW) {
    Serial.println("Pin is LOW (press)");
  }
  delay(500);
}


//Circuit Diagram:
    // 3.3V --- 10kΩ resistor --- GPIO 9
    // GPIO 9 --- Button --- Ground (GND)