#include <Keypad.h>
#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <Wire.h>
#include <RTClib.h>


// Keypad setup -----------------------------------------
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
 {'1', '2', '3', 'A'},
 {'4', '5', '6', 'B'},
 {'7', '8', '9', 'C'},
 {'*', '0', '#', 'D'}
};
// in order from left  to right:
byte r1 = 31; // red
byte r2 = 33; // orange
byte r3 = 35; // yellow
byte r4 = 37; // green
byte c1 = 39; // blue
byte c2 = 41; // purple
byte c3 = 43; // white
byte c4 = 45; // black

byte colPins[COLS] = {c1, c2, c3, c4}; 
byte rowPins[ROWS] = {r1, r2, r3, r4}; 

// create instance of the Keypad class with the keys, pins, rows, columns defined above
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
// Keypad setup -----------------------------------------

// LCD Setup --------------------------------------------
const int rs = 2; // blue
const int en = 3; // yellow
const int d4 = 4; // orange
const int d5 = 5; // brown
const int d6 = 6; // orange
const int d7 = 7; // purple
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// LCD Setup --------------------------------------------


// Define a struct to hold the different settings values
struct Settings {
  bool only_count_corrects_in_a_row;
  int num_correct_max;
  float millisec_per_question;
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
const int buzzer = 13; // white


// RTC setup
RTC_DS3231 rtc; // create class


/**
 * Main function to handle keypad input
 * @return The number entered by the user
 */
bool show_eqn_get_user_answer(EquationResult eq, int num_correct, Settings setn) {
  // unpack the settings struct
  int num_correct_max = setn.num_correct_max;
  float time_limit_per_q_ms = setn.millisec_per_question;


  String input = "";
  lcd.clear();
  String equation_str = eq.equation + " = " + "   cr:" + String(num_correct) + "/" + String(num_correct_max);
  lcd.print(equation_str);
  lcd.setCursor(0, 1);
  lcd.print("> ");
  
  // user must answer in some amount of time
  float start_time_ms = millis();    // counts milliseconds that the program has been running in total

  while (true) {
    // end loop variables:
    float elapsed_time_ms = millis() - start_time_ms;
    if (elapsed_time_ms >= time_limit_per_q_ms) {
      lcd.setCursor(0, 0);
      lcd.print("Outta time!");
      lcd.setCursor(0, 1);
      lcd.print("Ans: " + String(eq.result));
      delay(4*1000);

      // to add insult to injury:
      alarmNoise(500);
      return false;
    }

    // get key input
    char key = keypad.getKey();
    if (key) {

      // any key press resets the countdown timer
      start_time_ms = millis();

      Serial.print(key);
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
            lcd.setCursor(0, 0);
            lcd.print("Correct!");
            delay(2*1000);
            return true;
          } else {
            lcd.setCursor(0, 0);
            lcd.print("Incorrect!");
            lcd.setCursor(0, 1);
            lcd.print("Ans: " + String(eq.result));
            delay(4*1000);
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
  
  // loop continuously
  while (true){

    // get some equation and the expected result
    EquationResult eq;
    eq = generate_NdotN_equation();

    // prompt the user to answer the equation
    is_correct = show_eqn_get_user_answer(eq, num_correct, settings);

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
  lcd.print("hold btn to snz");

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
  setn.millisec_per_question = 10*1000;


  // Initialize real time clock (RTC) -----------------------------
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

  // set the RTC to whatever time the computer says
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));   

  // create the initial alarm (at 7:30am)
  setn.alarm_hour_24 = 7;
  setn.alarm_min = 30;
  setn.alarm_sec = 0;
  alarm_time_sec = setn.alarm_hour_24*60*60 + setn.alarm_min*60 + setn.alarm_sec;
  alarm_str = "Alm: " + formatDigits(setn.alarm_hour_24) + ":" + formatDigits(setn.alarm_min) + ":" + formatDigits(setn.alarm_sec);
  // Initialize real time clock (RTC) -----------------------------
}


// Function for in class demo
void alarmInFiveSeconds() {
  // count down
  write_date_time_to_display("Alarm in 5");
  delay(1000);
  write_date_time_to_display("Alarm in 4");
  delay(1000);
  write_date_time_to_display("Alarm in 3");
  delay(1000);
  write_date_time_to_display("Alarm in 2");
  delay(1000);
  write_date_time_to_display("Alarm in 1");
  delay(1000);

  // beep the beeper
  alarmNoise(500);

  // run the equations
  handleEquations(setn);
}



void loop() {
  // update display
  write_date_time_to_display(alarm_str);


  // start math alarm based off of the time
  current_time_sec = get_current_time();

  // if there are five seconds left, display a countdown and then do the math program
  if (current_time_sec-5 == alarm_time_sec) {
    alarmInFiveSeconds();
  }

  // for in class demo, set the alarm 5 seconds ahead of current time when 'B' key is pressed
  char key = keypad.getKey();
  if (key) {    // if the key press exists
    if (key == 'B'){
      alarmInFiveSeconds();
    }
  }

}
