#include <Arduino.h>

#include "control.h"
#include "menu.h"
#include "pinout.h"
#include "sensors.h"


#include "leds.h"
#include "menu.h"
#include "motors.h"
#include "buttons.h"
#include "pid.h"

void setup() {
  init_components();
  pidSetup();
  setVelD(getVel());
  setVelI(getVel());
  
  //Serial.begin(9600);
  near = false;
}



void loop() {



  //test mando y leds
  /*
  clear_leds();
  if (is_starting()){
    set_leds(0, 255, 0);
  }else{
    clear_leds();
  }
  */
  
  //test sensores
  
  //filtro_sensores();
  //print_sensores_2();
  

  
  //test motores
 /*
  if(is_starting()){
    motoresSusto();
    delay(500);
  }else{
    motoresStop();
  }*/
  
  
  
  //enablePrintsPid();

  menu1();

}




