#include "Arduino.h"
#include <Keypad.h>
#include <Adafruit_BusIO_Register.h>

const byte ROWS = 4; 
const byte COLS = 3; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {5, 18, 23, 19};
// Connecter les colonnes du clavier aux broches du ESP32
byte colPins[COLS] = {4, 9, 10};


Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(115200);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
    delay(1000);
  }
}