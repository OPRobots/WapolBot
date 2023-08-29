#include "buttons.h"
#include "leds.h"


int buttonState = 0; 
int buttonPushCounter = 0;
int lastButtonState = 0;

void buttonsSetup(){
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
}

void buttonsTest(){
  if(buttonsLeftPressed() && buttonsRightPressed()){
    ledsLeftRight(HIGH);
  }else if(buttonsLeftPressed()){
    ledsLeft(HIGH);
  }else if(buttonsRightPressed()){
    ledsRight(HIGH);
  }else{
    ledsLeftRight(LOW);
  }
}

void buttonsReset(){
  buttonState = 0; 
  buttonPushCounter = 0;
  lastButtonState = 0;
}

int buttonsRightPressed(){
    return 1 - digitalRead(rightButton);
}

int buttonsLeftPressed(){
    return 1 - digitalRead(leftButton);
}

void buttonPressCount(int val){
    
     // read the pushbutton input pin:
  buttonState = buttonsLeftPressed();

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      if (buttonPushCounter == val) {
        //buttonPushCounter = 0;
      }
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
    } else {
      // if the current state is LOW then the button went from on to off:
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  //cuando el total es modulo 4 enciende led
  if (buttonPushCounter %  val == 0) {
    ledsBuiltIn(HIGH);
  }else{
    ledsBuiltIn(LOW);
  }
}

int buttonsGetCount(){
  return buttonPushCounter;
}