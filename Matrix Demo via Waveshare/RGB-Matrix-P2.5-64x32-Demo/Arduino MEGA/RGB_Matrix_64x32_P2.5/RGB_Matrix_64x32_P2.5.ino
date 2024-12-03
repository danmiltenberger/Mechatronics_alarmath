#include "RGBmatrixPanel.h"
#include "bit_bmp.h"
#include "fonts.h"
#include <string.h>
#include <stdlib.h>
#define CLK 11 
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3





RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

void setup() {

  matrix.begin();

  // draw a pixel in solid white
  matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7));
  delay(500);

  // fix the screen with black
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
  delay(500);



void loop() {
  
}



void clear_line(int lineNumber) {
    // Define the height of each line (adjust based on your font size)
    const int LINE_HEIGHT = 8; // Example height for each line in pixels

    // Clear the specified line by filling it with black color
    int display_px_width = 64;
    matrix.fillRect(0, lineNumber * LINE_HEIGHT, display_px_width, LINE_HEIGHT, matrix.Color333(0, 0, 0)); 
}



void display_text(int x, int y, char *str, const GFXfont *f, int color, int pixels_size)
{
  matrix.setTextSize(pixels_size);// size 1 == 8 pixels high
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves
  matrix.setFont(f);      //set font
  matrix.setCursor(x, y);
  matrix.setTextColor(color);
  matrix.println(str);
}

void Demo_1()
{
  display_text(1, 13, "text", NULL, matrix.Color333(0, 100, 255), 2); // this text need to be printed slightly larger and over the 2 displays, not duplicated.
}

void Reginit()
{
    Serial.write("Connecting RGB pins");
    pinMode(24, OUTPUT); //R1
    pinMode(25, OUTPUT); //G1
    pinMode(26, OUTPUT); //B1
    pinMode(27, OUTPUT); //R2
    pinMode(28, OUTPUT); //G2
    pinMode(29, OUTPUT); //B2
    pinMode(CLK, OUTPUT);
    pinMode(OE, OUTPUT);
    pinMode(LAT, OUTPUT);

    Serial.write("Connecting clock etc pins");
    digitalWrite(OE, HIGH);
    digitalWrite(LAT, LOW);
    digitalWrite(CLK, LOW);
    int MaxLed = 64;

    int C12[16] = {0, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1};
    int C13[16] = {0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 0,  0, 0, 0, 0, 0};

    for (int l = 0; l < MaxLed; l++)
    {
        int y = l % 16;
        digitalWrite(24, LOW);
        digitalWrite(25, LOW);
        digitalWrite(26, LOW);
        digitalWrite(27, LOW);
        digitalWrite(28, LOW);
        digitalWrite(29, LOW);
        if (C12[y] == 1)
        {
          digitalWrite(24, HIGH);
          digitalWrite(25, HIGH);
          digitalWrite(26, HIGH);
          digitalWrite(27, HIGH);
          digitalWrite(28, HIGH);
          digitalWrite(29, HIGH);
        }
        if (l > MaxLed - 12)
        {
            digitalWrite(LAT, HIGH);
        }
        else
        {
            digitalWrite(LAT, LOW);
        }
        digitalWrite(CLK, HIGH);
        delayMicroseconds(2);
        digitalWrite(CLK, LOW);
    }
    digitalWrite(LAT, LOW);
    digitalWrite(CLK, LOW);


    Serial.write("Sending data to control reg 12");
    // Send Data to control register 12
    for (int l = 0; l < MaxLed; l++)
    {
        int y = l % 16;
        digitalWrite(24, LOW);
        digitalWrite(25, LOW);
        digitalWrite(26, LOW);
        digitalWrite(27, LOW);
        digitalWrite(28, LOW);
        digitalWrite(29, LOW);
        if (C13[y] == 1)
        {
            digitalWrite(24, HIGH);
            digitalWrite(25, HIGH);
            digitalWrite(26, HIGH);
            digitalWrite(27, HIGH);
            digitalWrite(28, HIGH);
            digitalWrite(29, HIGH);
        }
        if (l > MaxLed - 13)
        {
            digitalWrite(LAT, HIGH);
        }
        else
        {
            digitalWrite(LAT, LOW);
        }
        digitalWrite(CLK, HIGH);
        delayMicroseconds(2);
        digitalWrite(CLK, LOW);
    }
    digitalWrite(LAT, LOW);
    digitalWrite(CLK, LOW);

  Serial.write("Reg Init Complete");
}
