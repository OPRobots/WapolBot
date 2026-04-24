#include "menu.h"

#include "buttons.h"
#include "estrategias.h"
#include "leds.h"

bool inMenu = true;
int position = 1;

void menu1() {
    static unsigned long rc5_feedback_ms = 0;
    if (inMenu) {
        BTN_STATES btn_state = get_btn_pressed_state();
        switch (position) {
            case 1:
                ledsLoading(LOW);
                ledsUp(HIGH);
                break;
            case 2:
                ledsLoading(LOW);
                ledsLeftRight(HIGH);
                break;
            case 3:
                ledsLoading(LOW);
                ledsRight(HIGH);
                break;
            case 4:
                ledsLoading(LOW);
                ledsLeft(HIGH);
                break;
            case 5:
                ledsLoading(LOW);
                set_led(RGB_TOP, 127, 0, 255);
                break;
            case 6:
                ledsLoading(LOW);
                set_led(RGB_TOP, 100, 100, 0);
                set_led(RGB_RIGHT, 100, 100, 0);
                set_led(RGB_LEFT, 100, 100, 0);
                break;
            case 7:
                ledsLoading(LOW);
                set_led(RGB_LEFT, 0, 0, 100);
                set_led(RGB_RIGHT, 0, 0, 100);
                set_led(RGB_TOP, 0, 0, 100);
                break;
            case 8:
                ledsLoading(LOW);
                set_led(RGB_LEFT, 0, 0, 200);
                set_led(RGB_RIGHT, 0, 0, 200);
                break;
            case 9:
                ledsLoading(LOW);
                set_led(RGB_LEFT, 100, 100, 0);
                set_led(RGB_RIGHT, 100, 100, 0);
                break;

            default:
                break;
        }
        // position = buttonsGetCount();
        if (btn_state == BTN_PRESSED) {
            position++;
            if (position == 10) {
                position = 1;
            }
        }
        if(rc5_is_prog_done()){
            if (rc5_feedback_ms == 0) rc5_feedback_ms = millis();
            blink_led(RGB_TOP, 0, 0, 0, 50);
            if (millis() - rc5_feedback_ms >= 1000) {
                rc5_reset_prog_done();
                rc5_feedback_ms = 0;
            }
        }
        if (btn_state == BTN_LONG_PRESSED || is_starting()) {
            set_starting(true);
            //ledsCuentaAtras();
            ledsLoading(LOW);
            switch (position) {
                case 1:
                    //set_led(RGB_TOP, 0, 255, 0);
                    break;
                case 2:
                    //set_led(RGB_TOP, 0, 255, 0);
                    estrategiaMirarAtras();
                    break;
                case 3:
                    //set_led(RGB_TOP, 0, 255, 0);
                    estrategiaMirarLadoDerecha();
                    break;
                case 4:
                    //set_led(RGB_TOP, 0, 255, 0);
                    estrategiaMirarLadoIzquierda();
                    break;
                case 5:
                    //set_led(RGB_TOP, 0, 255, 0);
                    estrategiaCaja();
                    break;
                case 6:
                    //set_led(RGB_TOP, 0, 255, 0);
                    estrategiaRadar();
                    break;
                case 7:
                    //set_led(RGB_TOP, 0, 255, 0);
                    estrategiaSharps();
                    break;
                case 8:
                    //set_led(RGB_TOP, 0, 255, 0);
                    estrategiaQre1113();
                    break;
                case 9:
                    //set_led(RGB_TOP, 0, 255, 0);
                    estrategiaMotores();
                    break;

                default:
                    break;
            }
            inMenu = false;
        }
    }else{
        if (!is_starting()) {
            motoresStop();
            set_starting(false);
        }else{
            estrategiaBase();
        }
    }
}