#ifndef PINOUT_H
#define PINOUT_H

#include <Arduino.h>

#include "motors.h"
#include "rc5.h"

/**
 * @brief Pin de los LEDs
 *
 */
#define NEOPIXEL 20

/**
 * @brief Pin del módulo de inicio
 *
 */
#define MOD_START 21

/**
 * @brief Pin de Boton
 *
 */
#define BTN_1 6

/**
 * @brief Pines de Motores
 *
 */
#define MOTOR_RIGHT_A 2
#define MOTOR_RIGHT_B 8
#define MOTOR_LEFT_A 7
#define MOTOR_LEFT_B 10

/**
 * @brief Pines de Sensores
 *
 */
#define S_RIVAL_I 1
#define S_RIVAL_C 3
#define S_RIVAL_D 4

#define S_LINEA_I 0
#define S_LINEA_D 5

void init_components();


#endif // PINOUT_H