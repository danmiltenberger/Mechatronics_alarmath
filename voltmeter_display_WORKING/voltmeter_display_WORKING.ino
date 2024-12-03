// Logan's code from Lab 1 for voltmeter

// Including library for the LCD
#include <LiquidCrystal.h>


// Initializing the LCD connection pins
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);


// Initializing variable for voltage reading
float input_voltage = 0.0;


// Desired measuring frequency, in Hz
int measuringFrequency = 3;


void setup(){
  // Initializing LCD display
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Voltmeter");
  // Delay before reading voltage
  delay(300);
}

void loop(){
  // Reading value for voltage
  int analog_value = analogRead(A0);

  // Converting from Arduino to volts
  input_voltage = (analog_value*5.0)/1024.0;
  
  // If voltage is too low, then voltmeter reads as 0
  if (input_voltage < 0.1){
    input_voltage = 0.0;
  }

  // Printing voltage to display
  lcd.setCursor(0,1);
  lcd.print(input_voltage);
  lcd.print(" V");
  // Setting frequency of measurement
  delay(round(1000/measuringFrequency));
}




