// // Define a struct to hold both the equation string and the result
// typedef struct {
//   String equation;
//   int result;
// } EquationResult;

// // Now you can declare variables like this:
// EquationResult myResult;


EquationResult generate_NdotN_equation() {
  // Generate two random numbers between 1 and 9
  int num1 = random(1, 10);
  int num2 = random(1, 10);
  
  // Create the equation string
  String equationStr = String(num1) + "*" + String(num2);
  
  // Calculate the result
  int result = num1 * num2;
  
  // Return both the equation string and the result
  return {equationStr, result};
}


// EquationResult generate_NNdotNN_equation() {
//   // Generate two random numbers between 0 and 100
//   int num1 = random(0, 101);
//   int num2 = random(0, 101);
  
//   // Create the equation string
//   String equationStr = "";
  
//   // Add the first number, ensuring it's two digits
//   if (num1 < 10) {
//     equationStr += "0";
//   }
//   equationStr += String(num1);
  
//   // Add the "x" separator
//   equationStr += "*";
  
//   // Add the second number, ensuring it's two digits
//   if (num2 < 10) {
//     equationStr += "0";
//   }
//   equationStr += String(num2);
  
//   // Calculate the result
//   int result = num1 * num2;
  
//   // Return both the equation string and the result
//   return {equationStr, result};
// }


// /**
//  * Main function to handle keypad input
//  * @return The number entered by the user
//  */
// bool show_eqn_get_user_answer(EquationResult eq, int num_correct, int num_correct_max) {
//   String input = "";
//   lcd.clear();
//   lcd.print(eq.equation + " = " + "   cr:" + String(num_correct) + "/" + String(num_correct_max));
//   lcd.setCursor(0, 1);
//   lcd.print("> ");
  
//   while (true) {
//     char key = keypad.getKey();
    
//     if (key) {
//       if (key >= '0' && key <= '9') {
//         // If a number key is pressed, add it to the input
//         addDigit(key, input);
//       } 
//       else if (key == 'D') {
//         // 'D' key submits the answer
//         if (input.length() > 0) {
//           int userAnswer = input.toInt();
//           lcd.clear();
//           if (userAnswer == eq.result) {
//             lcd.print("Correct!");
//             delay(2000);
//             return true;
//           } else {
//             lcd.print("Incorrect!");
//             lcd.setCursor(0, 1);
//             lcd.print("Answer: " + String(eq.result));
//             delay(2000);
//             return false;
//           }
//         }
//       } 
//       else if (key == '*') {
//         // '*' key clears the current input
//         clearAnswer();
//         input = "";
//       }
//     }
//   }
// }



// /**
//  * Main function to the math alarm section of the program
//  * Get an equation, get the user input, and track how many correct answers
//  * @return nothing, the program should go back to the clock. Only closes when enough correct answers are submitted
//  */
// void handleEquations(Settings settings) {
//   // instantiate a counter for the number of correct responses
//   int num_correct = 0;
//   bool is_correct = false;

//   // read the settings
//   int num_correct_max = settings.num_correct_max;
//   bool is_only_counting_num_correct_in_a_row = settings.only_count_corrects_in_a_row;
  
//   // begin a forever loop
//   while (true){

//     // get some equation and the expected result
//     EquationResult eq;
//     eq.equation = "8x5";
//     eq.result = 40;

//     // prompt the user to answer the equation
//     is_correct = show_eqn_get_user_answer(eq, num_correct, num_correct_max);

//     // increase the number of correct answers
//     if (is_correct) {
//       num_correct = num_correct + 1;
//     }

//     // handle if we want some number of total correct, or some number of correct in a row
//     else {
//       // If we want some number correct in a row, clear the counter when an answer is wrong
//       if (is_only_counting_num_correct_in_a_row){
//         num_correct = 0;
//       }

//       // otherwise no change
//       else {
//         num_correct = num_correct; 
//       }
//     }

//     // handle if the user has answered all of the math equations properly
//     if (num_correct == num_correct_max){
//       displayWakeUpMessage();

//       // leave the loop, likely going back to the clock
//       break;     
//     }
//   }
// }