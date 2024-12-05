#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

// Define a struct to hold alarm time
struct Alarm_time {
  int hour;
  int minute;
  int second;
};

// Declare an instance of Alarm_time for the alarm
Alarm_time alarm = {7, 30, 0}; // Initial alarm time set to 7:30:00

void setup() {
  Serial.begin(9600);

  // handle no RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // handle no RTC power
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Set initial alarm
  setAlarm(alarm);
}

void loop() {
  // Check if it's time for the alarm
  bool alarm_is_happening = is_Alarm();
}

// Function to set an alarm
void setAlarm(Alarm_time at) {
  Serial.print("Setting alarm for ");
  Serial.print(at.hour);
  Serial.print(":");
  Serial.print(at.minute);
  Serial.print(":");
  Serial.println(at.second);
}


// Function to check if it's time for the alarm
bool is_Alarm() {
  DateTime now = rtc.now();

  if (now.hour() == alarm.hour && now.minute() == alarm.minute && now.second() == alarm.second) {
    Serial.println("ALARM! It's time!");
    delay(1000); // Prevent multiple triggers within the same second
    
    return true;
  }
  else{
    return false;
  }
}

// Function to set the alarm five seconds ahead of current time (for demo)
void setAlarmFiveSecondsAhead() {
  DateTime now = rtc.now();
  DateTime futureTime = now + TimeSpan(0, 0, 0, 5); // Add 5 seconds

  alarm.hour = futureTime.hour();
  alarm.minute = futureTime.minute();
  alarm.second = futureTime.second();

  Serial.print("Alarm set for: ");
  Serial.print(alarm.hour);
  Serial.print(":");
  Serial.print(alarm.minute);
  Serial.print(":");
  Serial.println(alarm.second);
}