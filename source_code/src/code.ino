#include <Arduino.h>

#include "leds.h"
#include "menu.h"
#include "motors.h"
#include "buttons.h"
#include "sharps.h"
#include "qre1113.h"

// runs once
void setup() {
  ledsSetup();
  motorsSetup();
  buttonsSetup();
}

// runs forever
void loop() { 
  setVelD(getVel());
  setVelI(getVel());
  menu1();
}