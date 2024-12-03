/**
 * Sets up the initial display on the LCD
 */
void initializeDisplay() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("> ");
}

/**
 * Adds a digit to the input string and displays it on the LCD
 * @param digit The digit to add
 * @param input The current input string
 */
void addDigit(char digit, String &input) {
  input += digit;
  lcd.print(digit);
}

/**
 * Processes the submitted answer
 * @param input The complete input string
 * @return The entered number as an integer
 */
int submitAnswer(const String &input) {
  int result = input.toInt();
  lcd.clear();
  lcd.print("Number entered:");
  lcd.setCursor(0, 1);
  lcd.print(result);
  delay(2000);  // Display the result for 2 seconds
  return result;
}


void displayWakeUpMessage() {
  // print a fun message
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Good morning, ");
  lcd.setCursor(0,1);
  lcd.print("mathematician!");

  // show the message for 2 seconds (at minimum)
  delay(2000);   
}


/**
 * Clears the current answer and resets the display
 */
void clearAnswer(String equation_str) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(equation_str);
  lcd.setCursor(0, 1);
  lcd.print("> ");
  
}
