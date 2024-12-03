#include <TimeLib.h>

// documentation:
// https://github.com/PaulStoffregen/Time


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


int get_current_time(){
  int current_time_sec = hour()*60*60 + minute()*60 + second();
  return current_time_sec;
}

String formatDate() {
  // formatted to standard ISO YYYY-MM-DD
  return String(year()) + "-" + String(month()) + "-" + String(day());
}

String formatTime() {
  return "Now: " + formatDigits(hour()) + ":" + formatDigits(minute()) + ":" + formatDigits(second());
}

String formatDigits(int digits) {
  if (digits < 10) {
    return "0" + String(digits);
  }
  return String(digits);
}