#include <Wire.h>
#include <DS3231.h>

DS3231 rtc;
DateTime dt;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  
  // Uncomment the line below to set the RTC to the date and time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  displayDateTime();
  delay(1000);
}

void displayDateTime() {
  dt = rtc.now();
  
  Serial.print("Date: ");
  Serial.print(dt.year(), DEC);
  Serial.print('/');
  Serial.print(dt.month(), DEC);
  Serial.print('/');
  Serial.print(dt.day(), DEC);
  
  Serial.print("  Time: ");
  Serial.print(dt.hour(), DEC);
  Serial.print(':');
  Serial.print(dt.minute(), DEC);
  Serial.print(':');
  Serial.println(dt.second(), DEC);
}
