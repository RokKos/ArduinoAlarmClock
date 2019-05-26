//#define DEBUG_VERBOSE

// --- Library includes ---
#include <LiquidCrystal.h>
#include <Time.h>

#include "Pitches.h"


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


//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int noteDurations[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

bool not_playing_ = true;


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
  not_playing_ = true;

}

void loop() {
  ReadStartingTime();

  int key = analogRead(A0);
  Serial.println(key);
  
  lcd.setCursor(0,1);
  sprintf(time_format_, "Time %02d:%02d:%02d", hour(), minute(), second());
  lcd.print(time_format_);


  // TODO: Custom Queue implementation
  if (not_playing_ && key > 500) {
    not_playing_ = false;
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(8, melody[thisNote], noteDuration);
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(8);
    }  
    not_playing_ = true;
  }
  
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
