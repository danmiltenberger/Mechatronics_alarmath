#include <Keypad.h>
#include <LiquidCrystal.h>
#include <TimeLib.h>

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;

// note that this is a rough workaround for issues with the rows and columns, the keypad 
// doesn't actually look like this. BUT! It works and that's good enough for a school project
char hexaKeys[ROWS][COLS] = {
  {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};
byte colPins[ROWS] = {44, 46, 48, 50}; 
byte rowPins[COLS] = {45, 47, 49, 51}; 

// create instance of the Keypad class with the keys, pins, rows, columns defined above
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


// LCD Setup
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// Define a struct to hold the different settings values
struct Settings {
  bool only_count_corrects_in_a_row;
  int num_correct_max;
  int alarm_hour_24;
  int alarm_min;
  int alarm_sec;
};

// Create instance of settings struct
Settings setn;

// Create instance of time comparisons
int current_time_sec;
int alarm_time_sec;
String alarm_str;


// Define a struct to hold both the equation string and the result
typedef struct {
  String equation;
  int result;
} EquationResult;


// Buzzer setup
const int buzzer = 13; 


/**
 * Main function to handle keypad input
 * @return The number entered by the user
 */
bool show_eqn_get_user_answer(EquationResult eq, int num_correct, int num_correct_max) {
  String input = "";
  lcd.clear();
  String equation_str = eq.equation + " = " + "   cr:" + String(num_correct) + "/" + String(num_correct_max);
  lcd.print(equation_str);
  lcd.setCursor(0, 1);
  lcd.print("> ");
  
  while (true) {
    char key = keypad.getKey();
    
    if (key) {
      if (key >= '0' && key <= '9') {
        // If a number key is pressed, add it to the input
        addDigit(key, input);
      } 
      else if (key == 'D') {
        // 'D' key submits the answer
        if (input.length() > 0) {
          int userAnswer = input.toInt();
          lcd.clear();
          if (userAnswer == eq.result) {
            lcd.print("Correct!");
            delay(2000);
            return true;
          } else {
            lcd.print("Incorrect!");
            lcd.setCursor(0, 1);
            lcd.print("Answer: " + String(eq.result));
            delay(2000);
            return false;
          }
        }
      } 
      else if (key == '*') {
        // '*' key clears the current input
        clearAnswer(equation_str);
        input = "";
      }
    }
  }
}


/**
 * Main function to the math alarm section of the program
 * Get an equation, get the user input, and track how many correct answers
 * @return nothing, the program should go back to the clock. Only closes when enough correct answers are submitted
 */
void handleEquations(Settings settings) {
  // instantiate a counter for the number of correct responses
  int num_correct = 0;
  bool is_correct = false;

  // read the settings
  int num_correct_max = settings.num_correct_max;
  bool is_only_counting_num_correct_in_a_row = settings.only_count_corrects_in_a_row;
  
  // begin a forever loop
  while (true){

    // get some equation and the expected result
    EquationResult eq;
    eq = generate_NdotN_equation();

    // prompt the user to answer the equation
    is_correct = show_eqn_get_user_answer(eq, num_correct, num_correct_max);

    // increase the number of correct answers
    if (is_correct) {
      num_correct = num_correct + 1;
    }

    // handle if we want some number of total correct, or some number of correct in a row
    else {
      // If we want some number correct in a row, clear the counter when an answer is wrong
      if (is_only_counting_num_correct_in_a_row){
        num_correct = 0;
      }

      // otherwise no change
      else {
        num_correct = num_correct; 
      }
    }

    // handle if the user has answered all of the math equations properly
    if (num_correct == num_correct_max){
      displayWakeUpMessage();

      // leave the loop, likely going back to the clock
      break;     
    }
  }
}


void alarmNoise(int delay_ms) {
  // make the buzzer go beep-beep-beep-beep until a key has been pressed 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("press btn to snz");

  while(true){
    tone(buzzer, 1000);     // Send 1KHz sound signal...
    delay(delay_ms);        // wait some delay
    noTone(buzzer);         // Stop sound...
    delay(delay_ms);        // wait some delay

    // look for key input. if key input, break out of the alarm
    char any_key = keypad.getKey();
    if (any_key) {
      Serial.print(any_key);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("snoozed");

      break;
    }
  }
}




void setup() {
  // Initialize a 16x2 LCD
  lcd.begin(16, 2); 

  // Initialize serial connection
  Serial.begin(9600);

  // Initialize random seed
  randomSeed(analogRead(0));

  // Initialize buzzer
  pinMode(buzzer, OUTPUT); 

  // Initialize the settings
  setn.only_count_corrects_in_a_row = true;
  setn.num_correct_max = 3;

  // alarm at 7:30 am
  setn.alarm_hour_24 = 7;
  setn.alarm_min = 30;
  setn.alarm_sec = 0;
  alarm_time_sec = setn.alarm_hour_24*60*60 + setn.alarm_min*60 + setn.alarm_sec;
  alarm_str = "Alm: " + formatDigits(setn.alarm_hour_24) + ":" + formatDigits(setn.alarm_min) + ":" + formatDigits(setn.alarm_sec);


  // Set the initial time 5 seconds before the alarm goes off
  int hour = 7;
  int minute = 29;
  int second = 55;
  int day = 13;
  int month = 11;
  int year = 2024;
  setTime(hour, minute, second, day, month, year); 

}




void loop() {
  // update display
  write_date_time_to_display(alarm_str);

  // start math alarm based off of the time
  current_time_sec = get_current_time();
  if (current_time_sec == alarm_time_sec) {
    alarmNoise(500);
    handleEquations(setn);
  }

  // // handle different inputs from the 'clock' screen
  // char key = keypad.getKey();
  // if (key) {    
  //   // start math alarm if the "B" key is pressed (for the in class demo)
  //   if (key == 'B') {  
  //     handleEquations(setn);
  //   }

  //   // allow the user to change settings
  //   if (key == 'C') {
  //     setn = modifySettings(setn);
  //   }
  // }
}


