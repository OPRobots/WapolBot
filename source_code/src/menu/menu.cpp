#include "menu.h"
#include "../leds/leds.h"
#include "../buttons/buttons.h"
#include "../estrategias/estrategias.h"

int option = -1;
int position = 1;

void menu1(){
  buttonPressCount(5);
  while(option == -1){
    switch (position) {
      case 1:
        ledsLoading(LOW);
        ledsUpArrow(HIGH);
        break;
      case 2:
        ledsLoading(LOW);
        ledsDownArrow(HIGH);
        break;
      case 3:
        ledsLoading(LOW);
        ledsRightArrow(HIGH);
        break;
      case 4:
        ledsLoading(LOW);        
        ledsLeftArrow(HIGH);
        break;
      case 5:
        ledsLoading(LOW);
        ledsGreen(HIGH);
        break;
      
      default:
        break;
    }
    //position = buttonsGetCount();
    if(buttonsLeftPressed()){
      //option = position;
      position++;
      delay(200);
      if(position==6){
        position = 1;
      }
    }
    if(buttonsRightPressed()){
      ledsCuentaAtras();
      option = 0;
      switch (position) {
        case 1:
          ledsBuiltIn(HIGH);
          estrategiaMirarAdelante();
          break;
        case 2:
          ledsBuiltIn(HIGH);
          estrategiaMirarAtras();
          break;
        case 3:
          ledsBuiltIn(HIGH);
          estrategiaMirarLadoDerecha();
          break;
        case 4:
          ledsBuiltIn(HIGH);
          estrategiaMirarLadoIzquierda();
          break;
        case 5:
          estrategiaCaja();
          option = -1;
        default:
          break;
      }
    }
  }
}