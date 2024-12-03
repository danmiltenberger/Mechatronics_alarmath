#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc; // Create an instance of the RTC_DS1307 class

void setup() {
    Serial.begin(9600); // Initialize serial communication
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1); // Halt if RTC is not found
    }

    // Uncomment these lines to set the date and time initially
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to compile time
    // rtc.adjust(DateTime(2024, 11, 25, 15, 30, 0)); // Set specific date and time (Year, Month, Day, Hour, Minute, Second)

    Serial.println("RTC initialized.");
}

void loop() {
    DateTime now = rtc.now(); // Get current time from RTC

    // Print time to Serial Monitor
    Serial.print("Time: ");
    Serial.print(now.year(), DEC); // Year
    Serial.print("-");
    Serial.print(now.month(), DEC); // Month
    Serial.print("-");
    Serial.print(now.day(), DEC); // Day
    Serial.print(" ");
    Serial.print(now.hour(), DEC); // Hour
    Serial.print(":");
    Serial.print(now.minute(), DEC); // Minute
    Serial.print(":");
    Serial.print(now.second(), DEC); // Second
    Serial.println();

    delay(1000); // Wait for 1 second before repeating
} 