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
            case 7:
                ledsLoading(LOW);
                ledsUpArrow(HIGH);
                ledsGreen(HIGH);

                break;
            case 8:
                ledsLoading(LOW);
                ledsLeftRight(HIGH);
                ledsGreen(HIGH);
                break;

            default:
                break;
        }
        // position = buttonsGetCount();
        if (buttonsLeftPressed()) {
            position++;
            delay(200);
            if (position == 9) {
                position = 1;
            }
        }
        if (buttonsRightPressed()) {
            ledsCuentaAtras();
            ledsLoading(LOW);
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
                    ledsBuiltIn(HIGH);
                    estrategiaCaja();
                    break;
                case 6:
                    ledsBuiltIn(HIGH);
                    estrategiaRadar();
                    break;
                case 7:
                    ledsBuiltIn(HIGH);
                    estrategiaSharps();
                    break;
                case 8:
                    ledsBuiltIn(HIGH);
                    estrategiaQre1113();
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