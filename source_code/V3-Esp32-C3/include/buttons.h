#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#include "pinout.h"

/**
 * @brief Definición de los posibles estados del botón
 *
 */
enum BTN_STATES {
  BTN_RELEASED,
  BTN_PRESSING,
  BTN_PRESSED,
  BTN_LONG_PRESSED,
};

enum BTN_STATES get_btn_pressed_state();
long get_btn_pressing_ms();

#endif // BUTTONS_H