#include "menu.h"

#include "buttons.h"
#include "estrategias.h"
#include "leds.h"

bool inMenu = true;
int position = 1;

void menu1() {
    if (inMenu) {
        buttonPressCount(5);
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
                ledsUp(HIGH);
                ledsGreen(HIGH);
                break;
            case 6:
                ledsLoading(HIGH);
                break;

            default:
                break;
        }
        // position = buttonsGetCount();
        if (buttonsLeftPressed()) {
            position++;
            delay(200);
            if (position == 7) {
                position = 1;
            }
        }
        if (buttonsRightPressed()) {
            ledsCuentaAtras();
            switch (position) {
                case 1:
                    ledsBuiltIn(HIGH);
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
                    break;
                case 6:
                    estrategiaRadar();
                    break;
                default:
                    break;
            }
            inMenu = false;
        }
    }else{
      estrategiaBase();
    }
}