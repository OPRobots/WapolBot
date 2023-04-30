#include <Arduino.h>

#include "src/leds/leds.h"
#include "src/menu/menu.h"
#include "src/motors/motors.h"
#include "src/buttons/buttons.h"
#include "src/sensors/sharp/sharps.h"
#include "src/sensors/qre1113/qre1113.h"

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