#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

#define SENSOR_PIN 5 // Corresponds to GPA5 la 3e pin from notch A side
//Work well with either 1 or 10k pin to ground when using 5v directly
//Carfull not to forget to connect all ground together

Adafruit_MCP23X17 mcp;

void setup() {
  Serial.begin(115200);

  // Initialize the MCP23017
  if (!mcp.begin_I2C(0x20)) { // Default I2C address is 0x20
    Serial.println("Error initializing MCP23017.");
    while (1);
  }

  // Configure the pin as input
  mcp.pinMode(SENSOR_PIN, INPUT);

  // Ensure pull-up resistors are globally disabled (manually check library documentation for any helper methods)
  // This step will be skipped if unsupported in the library version being used.
}

void loop() {
  // Read the sensor state
  int sensorState = mcp.digitalRead(SENSOR_PIN);

  // Print the sensor state
  if (sensorState == HIGH) {
    Serial.println("HIGH");
  } else {
    Serial.println("LOW");
  }

  delay(500);
}
