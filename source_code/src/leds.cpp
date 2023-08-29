#include "leds.h"

void ledsSetup(){
  pinMode(ledDown, OUTPUT);
  pinMode(ledRight, OUTPUT);
  pinMode(ledLeft, OUTPUT);
  pinMode(ledUp, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBuiltIn, OUTPUT);  
}

void ledsTest(){ 
  digitalWrite(ledDown, HIGH);
  delay(500);
  digitalWrite(ledDown, LOW);
  digitalWrite(ledRight, HIGH);
  delay(500);
  digitalWrite(ledRight, LOW);
  digitalWrite(ledUp, HIGH);
  delay(500);
  digitalWrite(ledUp, LOW);
  digitalWrite(ledLeft, HIGH);
  delay(500);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledGreen, HIGH);
  delay(500);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBuiltIn, HIGH);
  delay(500);
  digitalWrite(ledBuiltIn, LOW);
}

void ledsGreen(uint8_t val){
  digitalWrite(ledGreen, val);
}

void ledsBuiltIn(uint8_t val){
  digitalWrite(ledBuiltIn, val);
}

void ledsUp(uint8_t val){
  digitalWrite(ledUp, val);
}

void ledsDown(uint8_t val){
  digitalWrite(ledDown, val);
}

void ledsLeft(uint8_t val){
  digitalWrite(ledLeft, val);
}

void ledsRight(uint8_t val){
  digitalWrite(ledRight, val);
}

void ledsRightArrow(uint8_t val){
  digitalWrite(ledDown, val);
  digitalWrite(ledRight, val);
  digitalWrite(ledUp, val);
}

void ledsLeftArrow(uint8_t val){
  digitalWrite(ledDown, val);
  digitalWrite(ledLeft, val);
  digitalWrite(ledUp, val);
}

void ledsUpArrow(uint8_t val){
  digitalWrite(ledRight, val);
  digitalWrite(ledLeft, val);
  digitalWrite(ledUp, val);
}

void ledsDownArrow(uint8_t val){
  digitalWrite(ledDown, val);
  digitalWrite(ledLeft, val);
  digitalWrite(ledRight, val);
}

void ledsUpDown(uint8_t val){
  digitalWrite(ledDown, val);
  digitalWrite(ledUp, val);
}

void ledsLeftRight(uint8_t val){
  digitalWrite(ledLeft, val);
  digitalWrite(ledRight, val);
}

void ledsLoading(uint8_t val){
  digitalWrite(ledDown, val);
  digitalWrite(ledLeft, val);
  digitalWrite(ledUp, val);
  digitalWrite(ledRight, val);
}

void ledsCuentaAtras(){
      ledsLoading(LOW);     
      ledsGreen(LOW);   
      ledsUp(HIGH);
      delay(1000);
      ledsUp(LOW);        
      ledsLeft(HIGH);
      delay(1000);
      ledsLeft(LOW);        
      ledsDown(HIGH);
      delay(1000);
      ledsDown(LOW);        
      ledsRight(HIGH);
      delay(1000);
      ledsLoading(HIGH);
      delay(1000);
      ledsGreen(HIGH);
}