#include "Arduino.h"
#include "Keypad.h"

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {36, 37, 38, 39}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {34, 35, 32}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(115200);
}
   
void loop(){
  char key = keypad.getKey();
   
  if (key != NO_KEY){
    Serial.println(key);
  }
}