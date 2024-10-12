#include "control.h"

static bool starting = false;

/**
 * @brief Establece el estado de iniciando carrera.
 *
 * @param starting Indica si la carrera está iniciando.
 */
void set_starting(bool _starting) {
  starting = _starting;
}
/**
 * @brief Comprueba si la carrera está iniciando.
 *
 * @return true Iniciando.
 * @return false En espera o iniciada.
 */
bool is_starting() {
  return starting;
}
