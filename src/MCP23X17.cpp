#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

#define SENSOR_PIN 7 // Define the sensor pin
#define LED_PIN 8    // Define the LED pin

Adafruit_MCP23X17 mcp;

void setup() {
  Serial.begin(115200);

  // Initialize the MCP23017
  if (!mcp.begin_I2C(0x20)) { // Default I2C address is 0x20
    Serial.println("Error initializing MCP23017.");
    while (1);
  }

  // Configure the pin as input and output
  mcp.pinMode(SENSOR_PIN, INPUT);
  mcp.pinMode(LED_PIN, OUTPUT);

  // Ensure pull-up resistors are globally disabled (manually check library documentation for any helper methods)
  // This step will be skipped if unsupported in the library version being used.
}

void loop() {
  // Read the sensor state
  int sensorState = mcp.digitalRead(SENSOR_PIN);

  // Print the sensor state
  Serial.print("Sensor state: ");
  Serial.println(sensorState);

  // Control the LED based on the sensor state
  if (sensorState == HIGH) {
    Serial.println("Turning LED OFF");
    mcp.digitalWrite(LED_PIN, LOW); // Turn off the LED
  } else {
    Serial.println("Turning LED ON");
    mcp.digitalWrite(LED_PIN, HIGH); // Turn on the LED
  }

  delay(500);
}
