#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

#define LED_PIN 15 // GPIO A7
#define LED_PIN2 14
//Attention sur le plan p.ex. GPB7 on doit appelé la pin 15

Adafruit_MCP23X17 mcp;

void setup() {
  if (!mcp.begin_I2C(0x20)) { // I2C address. Can be 0x20 to 0x27, depending on the A0, A1, and A2 pins. If all 0, the address is 0x20.
    Serial.println("Error.");
    while (1);
  }
  mcp.pinMode(LED_PIN, OUTPUT);
  mcp.pinMode(LED_PIN2, OUTPUT);
}
void loop() {
  mcp.digitalWrite(LED_PIN, HIGH);
  mcp.digitalWrite(LED_PIN2, HIGH);
  delay(500);
  mcp.digitalWrite(LED_PIN, LOW);
  mcp.digitalWrite(LED_PIN2, HIGH);
  delay(500);
}