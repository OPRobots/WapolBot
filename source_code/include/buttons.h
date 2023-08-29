#ifndef BUTTONS_H
#define BUTTONS_H
#include <Arduino.h>

#define leftButton 2
#define rightButton 12

extern int buttonState; 
extern int buttonPushCounter;
extern int lastButtonState;

void buttonsSetup();
void buttonsTest();
void buttonsReset();
int buttonsRightPressed();
int buttonsLeftPressed();
void buttonPressCount(int val);
int buttonsGetCount();

#endif