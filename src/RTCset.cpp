///Attention l'environnement n'est pas setté pour le moment


#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "SPI.h"

RTC_DS3231 rtc;  // Use this for DS3231 RTC
// For DS1307, you can use RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);

  // Check if the RTC is connected
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Display message to prompt for time entry
  Serial.println("Please enter the date and time in the following format:");
  Serial.println("YYYY MM DD HH MM SS");
  Serial.println("For example: 2024 09 10 12 34 56");
  
  // Wait for serial input
  while (!Serial.available()) {
    // Wait until input is available
  }

  // Variables to hold date and time
  int year, month, day, hour, minute, second;
  
  // Read the date and time from Serial input
  year = Serial.parseInt();
  month = Serial.parseInt();
  day = Serial.parseInt();
  hour = Serial.parseInt();
  minute = Serial.parseInt();
  second = Serial.parseInt();
  
  // Adjust the RTC with the entered date and time
  rtc.adjust(DateTime(year, month, day, hour, minute, second));
  Serial.println("RTC time set successfully!");
}

void loop() {
  // Nothing to do here
}
