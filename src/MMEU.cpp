#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define AO_PIN 9  // Lever input pin
#define LEVER_PIN 23  // Lever control output

#define ENA 10  // Enable pin for stepper driver
#define DIR 5   // Direction pin for stepper driver
#define PUL 18  // Pulse pin for stepper driver
#define micro 8 // Can be 1, 2, 4, 8, 16, 32

void Reward_out();
LiquidCrystal_I2C lcd(0x27, 16, 4);  // Set the LCD I2C address, columns, and rows

unsigned int totalPresses = 0;  // Total number of lever presses
unsigned int rewardCount = 0;  // Number of rewards given
unsigned int pressesSinceLastReward = 0;  // Number of presses since the last reward
unsigned int requiredPressesForNextReward = 1;  // Initial number of presses required for the first reward
unsigned long lastPressTime = 0;  // Time of the last lever press
unsigned long currentTime = 0;  // Current time
unsigned long lastDisplayUpdate = 0;  // Last time the display was updated
bool leverWasPressed = false;  // Tracks if the lever was pressed previously to detect release
bool stopped = false;  // To track if the program has stopped

void setup() {
  pinMode(AO_PIN, INPUT_PULLUP);  // Set AO_PIN as input with internal pull-up resistor
  pinMode(LEVER_PIN, OUTPUT);  // Lever control output
  digitalWrite(LEVER_PIN, HIGH);  // Extend the lever

  pinMode(ENA, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(PUL, OUTPUT);
  digitalWrite(ENA, HIGH); // Disable motor
  digitalWrite(DIR, LOW); // Clockwise when looking from top
  digitalWrite(PUL, LOW); // Start out LOW

  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on the backlight
  lcd.home();  // Go home
  lcd.print("Rewards: 0");
  lcd.setCursor(0, 1);
  lcd.print("Next: 1");
  lcd.setCursor(0, 2);
  lcd.print("Since last: 0");
  lcd.setCursor(0, 3);
  lcd.print("Total: 0 Secs: 0");

  Serial.begin(115200);  // Initialize serial communication
  Serial.println("Setup complete. Waiting for lever press...");
}

void updateLCD() {
  lcd.setCursor(9, 0);
  lcd.print("    ");  // Clear previous reward count
  lcd.setCursor(9, 0);
  lcd.print(rewardCount);
  lcd.setCursor(6, 1);
  lcd.print("    ");  // Clear previous next reward press count
  lcd.setCursor(6, 1);
  lcd.print(requiredPressesForNextReward);
  lcd.setCursor(12, 2);
  lcd.print("    ");  // Clear previous presses since last reward count
  lcd.setCursor(12, 2);
  lcd.print(pressesSinceLastReward);
  lcd.setCursor(7, 3);
  lcd.print("    ");  // Clear previous total press count
  lcd.setCursor(7, 3);
  lcd.print(totalPresses);
  unsigned long secondsSinceLastPress = (millis() - lastPressTime) / 1000;
  lcd.setCursor(11, 3);
  lcd.print("Secs:");
  lcd.print(secondsSinceLastPress);
}

void loop() {
  if (stopped) {
    return;  // If the program is stopped, do nothing
  }

  currentTime = millis();
  int sensorValue = digitalRead(AO_PIN);  // Read the value from the pin

  // Print the current value of AO_PIN to the Serial Monitor for debugging
  Serial.print("AO_PIN value: ");
  Serial.println(sensorValue);

  // Detect the first press of the lever
  if (sensorValue == LOW && !leverWasPressed) {
    Serial.println("Lever pressed!");
    leverWasPressed = true;  // Mark the lever as pressed
    totalPresses++;
    pressesSinceLastReward++;
    lastPressTime = millis();

    if (pressesSinceLastReward >= requiredPressesForNextReward) {
      rewardCount++;
      pressesSinceLastReward = 0;
      Reward_out();

      // Update required presses for the next reward
      requiredPressesForNextReward = round(pow(rewardCount + 1, 1.2));
    }

    // Update the LCD display
    updateLCD();
  }

  // Detect when the lever is released
  if (sensorValue == HIGH && leverWasPressed) {
    Serial.println("Lever released!");
    leverWasPressed = false;  // Reset the lever press status
  }

  // Update the LCD display every second
  if (currentTime - lastDisplayUpdate >= 1000) {
    lastDisplayUpdate = currentTime;
    updateLCD();
  }

  // Check if more than 5 minutes (300 seconds) have passed since the last lever press
  if ((millis() - lastPressTime) >= 300000) {
    stopped = true;
    digitalWrite(LEVER_PIN, LOW);  // Retract the lever
    updateLCD();  // Final update to show stopped state
  }

  delay(50);  // Short delay to prevent too fast loop iteration
}

void Reward_out() {
  Serial.println("Reward triggered: Motor running...");
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
  delay(1000);  // Wait before allowing another reward
  Serial.println("Reward finished: Motor stopped.");
}
