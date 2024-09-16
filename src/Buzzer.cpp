#include <Wire.h>
#include <Adafruit_MCP23X17.h>

// Create MCP23X17 object
Adafruit_MCP23X17 mcp;

#define BUZZER_PIN 2  // GPA7 pin on MCP23X17


// Create MCP23X17 object
Adafruit_MCP23X17 mcp;

#define BUZZER_PIN 7  // GPA7 pin on MCP23X17
#define BUZZER_FREQUENCY 1000  // Frequency in Hertz (adjust as needed)
#define TONE_DELAY (1000000 / (BUZZER_FREQUENCY * 2))  // Calculate the delay for half the period

void setup() {
  // Initialize communication with the MCP23X17
  //if (!mcp.begin_I2C()) {
  //  Serial.println("Error initializing MCP23X17.");
  //  while (1);
  }
  
  // Set GPA7 (BUZZER_PIN) as an output
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Generate tone by toggling the pin at a specified frequency
  for (int i = 0; i < 1000; i++) {  // Generate a tone for a specific duration
    digitalWrite(BUZZER_PIN, HIGH);  // Turn buzzer ON
    delayMicroseconds(TONE_DELAY);       // Wait for half the period
    digitalWrite(BUZZER_PIN, LOW);   // Turn buzzer OFF
    delayMicroseconds(TONE_DELAY);       // Wait for half the period
  }

  delay(1000);  // Pause for 1 second before repeating the alarm
}
