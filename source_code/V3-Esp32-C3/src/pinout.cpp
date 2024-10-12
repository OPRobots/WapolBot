#include "pinout.h"

/**
 * @brief Inicializa los componentes del robot.
 *
 */
void init_components() {
  pinMode(NEOPIXEL, OUTPUT);
  pinMode(BTN_1, INPUT_PULLDOWN);

  pinMode(MOTOR_RIGHT_A, OUTPUT);
  pinMode(MOTOR_RIGHT_B, OUTPUT);
  pinMode(MOTOR_LEFT_A, OUTPUT);
  pinMode(MOTOR_LEFT_B, OUTPUT);
  digitalWrite(MOTOR_RIGHT_A, HIGH);
  digitalWrite(MOTOR_RIGHT_B, HIGH);
  digitalWrite(MOTOR_LEFT_A, HIGH);
  digitalWrite(MOTOR_LEFT_B, HIGH);

  
  pinMode(MOD_START, INPUT);
  attachInterrupt(digitalPinToInterrupt(MOD_START), rc5_isr, CHANGE);

  pinMode(S_RIVAL_I, INPUT);
  pinMode(S_RIVAL_C, INPUT);
  pinMode(S_RIVAL_D, INPUT);

  pinMode(S_LINEA_I, INPUT);
  pinMode(S_LINEA_D, INPUT);

  init_motors();
}
