///Attention l'enviornnement n'est pas setté pour le moment

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;  // Use this for DS3231 RTC
// For DS1307, you can use RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);

  // Check if the RTC is connected
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // If RTC lost power or is not running, set the date & time at compile time
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    // Uncomment the next line to set the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
    // Alternatively, to set the RTC manually to a specific date & time:
    // rtc.adjust(DateTime(2024, 9, 10, 12, 0, 0));  // Year, Month, Day, Hour, Minute, Second
  }
}

void loop() {
  // Get the current time
  DateTime now = rtc.now();

  // Display the time
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);  // Wait 1 second before reading the time again
}
