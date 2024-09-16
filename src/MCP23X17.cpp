//This one test the multiplexer
//It test an input direct to the ESP32

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

#define LED_PIN 3 // GPIO A7
#define BUTTON_PIN 9 // 28 GPIO GPA7

Adafruit_MCP23X17 mcp;

void setup() {
  Serial.begin(115200); // Initialize serial communication
  if (!mcp.begin_I2C(0x20)) { // I2C address. Can be 0x20 to 0x27, depending on the A0, A1, and A2 pins. If all 0, the address is 0x20.
    Serial.println("Error.");
    while (1);
  }
  mcp.pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Enable internal pull-up resistor
}

void loop() {
  mcp.digitalWrite(LED_PIN, HIGH);
  delay(50);
  
  //if (digitalRead(BUTTON_PIN) == LOW)
   { // Check for LOW state
    Serial.println(digitalRead(BUTTON_PIN));
  
  mcp.digitalWrite(LED_PIN, LOW);
  delay(50);
  }
}