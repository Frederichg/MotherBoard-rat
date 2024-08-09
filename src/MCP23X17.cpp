#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

#define LED_PIN 7 // GPIO A7

Adafruit_MCP23X17 mcp;

void setup() {
  if (!mcp.begin_I2C(0x20)) { // I2C address. Can be 0x20 to 0x27, depending on the A0, A1, and A2 pins. If all 0, the address is 0x20.
    Serial.println("Error.");
    while (1);
  }
  mcp.pinMode(LED_PIN, OUTPUT);
}
void loop() {
  mcp.digitalWrite(LED_PIN, HIGH);
  delay(500);
  mcp.digitalWrite(LED_PIN, LOW);
  delay(500);
}