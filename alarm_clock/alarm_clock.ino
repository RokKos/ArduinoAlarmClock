//#define DEBUG_VERBOSE

// --- Library includes ---
#include <LiquidCrystal.h>
#include <Time.h>

// --- Custom types ---
typedef unsigned char uchr;
typedef unsigned int uint;

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

// -- Time -- 
const uchr kYear = 19;
const uchr kMonth = 5;
const uchr kDay = 23;

char time_format_[14];

bool reading_time_ = true;
uchr time_bit_recived_ = 0;
uchr hours_ = 0;
uchr minutes_ = 0;
uchr seconds_ = 0;

// -- Serial -- 
const uint kPortNumber = 9600;

// -- String --
const char kLoveMessage[16] = "LOVE you ALL <3";
const uchr kZero = 48;


// Setup 
LiquidCrystal lcd(kRSPin, kEnablePin,kD4Pin ,kD5Pin, kD6Pin, kD7Pin);

void setup() {
  lcd.begin(kLCDWidth,kLCDHeight);
  lcd.print(kLoveMessage);
  
  Serial.begin(kPortNumber);
  while (!Serial) { }
#ifdef DEBUG_VERBOSE  
  Serial.println("Hello Computer");
#endif
  reading_time_ = true;
  

}

void loop() {
  ReadStartingTime();
  
  lcd.setCursor(0,1);
  sprintf(time_format_, "Time %02d:%02d:%02d", hour(), minute(), second());
  lcd.print(time_format_);
}


void ReadStartingTime(){
  if (reading_time_ && Serial.available() > 0) {
    int inc_byte = Serial.read();
    
#ifdef DEBUG_VERBOSE  
    Serial.print("I received: ");
    Serial.println(inc_byte);
#endif

    ParseTimeRecieved(inc_byte); 
    if (time_bit_recived_ >= 6) {
      reading_time_ = false;
      setTime(hours_,minutes_,seconds_,kDay,kMonth,kYear);
    }   
  }
}

void ParseTimeRecieved(uchr inc_byte) {
  switch(time_bit_recived_){
    case 0:
      hours_ += 10 * (inc_byte - kZero);
      break;

    case 1:
      hours_ += inc_byte - kZero;
      break;
      
    case 2:
      minutes_ += 10 * (inc_byte - kZero);
      break;

    case 3:
      minutes_ += inc_byte - kZero;
      break;

    case 4:
      seconds_ += 10 * (inc_byte - kZero);
      break;

    case 5:
      seconds_ += inc_byte - kZero;
      break;
     
    default:
      Serial.print("Error: Too much bytes send");
      break;
    }
    
    time_bit_recived_++;
}
