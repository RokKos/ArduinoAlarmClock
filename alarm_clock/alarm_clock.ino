// --- Library includes ---
#include <LiquidCrystal.h>
#include <Time.h>

// --- Custom types ---
typedef unsigned char uchr;

// --- Constants ---

// -- LCD display
const uchr kRSPin = 12;
const uchr kEnablePin = 11;
const uchr kD4Pin = 5;
const uchr kD5Pin = 4;
const uchr kD6Pin = 3;
const uchr kD7Pin = 2;

const uchr kLCDWidth = 16;
const uchr kLCDHeight = 2;


// Setup 
LiquidCrystal lcd(kRSPin, kEnablePin,kD4Pin ,kD5Pin, kD6Pin, kD7Pin);

void setup() {
  lcd.begin(kLCDWidth,kLCDHeight);
  lcd.print("LOVE you ALL <3");

  Serial.begin(9600);
  while (!Serial) { }
  Serial.println("Hello Computer");

}

void loop() {
  lcd.setCursor(0,1);
  char time_format[100];
  sprintf(time_format, "Time %02d:%02d:%02d", hour(), minute(), second());
  lcd.print(time_format);
}
