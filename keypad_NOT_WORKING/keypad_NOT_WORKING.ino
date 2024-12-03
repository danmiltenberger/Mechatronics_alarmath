#include <Key.h>
#include <Keypad.h>

#include <Key.h>
// NOTE - must use downloaded keypad zip file included in the drive
// Ribbon / Sketch / Include Library / Zip File
#include <Keypad.h>

#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

// this is how the keypad actually looks
// char hexaKeys[ROWS][COLS] = {
//   {'1', '2', '3', 'A'},
//   {'4', '5', '6', 'B'},
//   {'7', '8', '9', 'C'},
//   {'*', '0', '#', 'D'}
// };


// this is my terrible recreation so the code can work. I've tried reversing the cols, rows, order, everything
// eventually it's easier to force the code to conform to reality
char hexaKeys[ROWS][COLS] = {
  {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};
byte colPins[ROWS] = {22, 24, 26, 28}; 
byte rowPins[COLS] = {30, 32, 34, 36}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
}
