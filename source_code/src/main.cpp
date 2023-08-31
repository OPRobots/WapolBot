#include <Arduino.h>

#include "leds.h"
#include "menu.h"
#include "motors.h"
#include "buttons.h"
#include "sharps.h"
#include "qre1113.h"
#include "pid.h"

// runs once
void setup() {
  ledsSetup();
  motorsSetup();
  buttonsSetup();
  pidSetup();
  setVelD(getVel());
  setVelI(getVel());
}

// runs forever
void loop() { 
  menu1();
}