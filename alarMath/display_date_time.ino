#include <TimeLib.h>

// documentation:
// https://github.com/PaulStoffregen/Time


// write the current time str (line 1) and alarm str (line 2) to the 16x2 LCD display
void write_date_time_to_display(String alarm_str) {
  // Clear the LCD
  lcd.clear();
  
  // // Display the date on the first row
  // lcd.setCursor(0, 0);
  // lcd.print(formatDate());
  

  
  // Display the time on the first row
  lcd.setCursor(0, 0);
  lcd.print(formatTime());

  // Display the alarm on the second row
  lcd.setCursor(0, 1);
  lcd.print(alarm_str);
  
  // Update every second
  delay(1000);
}


// read the RTC and return the current time as a big number of seconds
int get_current_time(){
  int current_time_sec = rtc.now().hour()*60*60 + rtc.now().minute()*60 + rtc.now().second();
  return current_time_sec;
}


String formatDate() {
  // formatted to standard ISO YYYY-MM-DD
  // updated to use the RTC
  return String(rtc.now().year()) + "-" + String(rtc.now().month()) + "-" + String(rtc.now().day());
}

String formatTime() {
  // updated to use the RTC
  return "Now: " + formatDigits(rtc.now().hour()) + ":" + formatDigits(rtc.now().minute()) + ":" + formatDigits(rtc.now().second());
}

String formatDigits(int digits) {
  if (digits < 10) {
    return "0" + String(digits);
  }
  return String(digits);
}