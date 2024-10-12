#include "buttons.h"

static int btn_pressed_ms = 0;
static int btn_last_pressed_ms = 0;

/**
 * @brief Obtiene el estado del botón pulsado
 *
 * @return enum BTN_STATES
 */
enum BTN_STATES get_btn_pressed_state() {
  if (millis() - btn_last_pressed_ms < 50) {
    return BTN_RELEASED;
  }
  if (!digitalRead(BTN_1)) {
    // Serial.print("RELEASE ");
    // Serial.println(btn_pressed_ms);
    if (btn_pressed_ms == 0) {
      return BTN_RELEASED;
    } else {
      if (millis() - btn_pressed_ms >= 250) {
        btn_pressed_ms = 0;
        btn_last_pressed_ms = millis();
        return BTN_LONG_PRESSED;
      } else if (millis() - btn_pressed_ms <= 250) {
        btn_pressed_ms = 0;
        if (millis() - btn_pressed_ms < 25) {
          return BTN_RELEASED;
        } else {
          btn_last_pressed_ms = millis();
          return BTN_PRESSED;
        }
      } else {
        return BTN_RELEASED;
      }
    }
  } else {
    // Serial.println("PRESSING");
    if (btn_pressed_ms == 0) {
      btn_pressed_ms = millis();
    }
    return BTN_PRESSING;
  }
}

/**
 * @brief Obtiene el tiempo que lleva pulsado el botón
 * 
 * @return long Tiempo en ms
 */
long get_btn_pressing_ms() {
  return millis() - btn_pressed_ms;
}