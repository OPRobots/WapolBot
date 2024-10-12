#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include "pinout.h"
#include "control.h"

/**
 * @brief Configuración de los motores de tracción.
 * Canales: 0 a 3
 * Frecuencia: 1 kHz
 * Resolución: 10 bits
 * Rango: 0 a 1023
 *
 */
#define PWM_MOTOR_RIGHT_A 0
#define PWM_MOTOR_RIGHT_B 1
#define PWM_MOTOR_LEFT_A 2
#define PWM_MOTOR_LEFT_B 3
#define PWM_MOTORS_HZ 1000
#define PWM_MOTORS_RESOLUTION 10
#define PWM_MOTORS_MAX 1023
#define PWM_MOTORS_MIN 0

void init_motors();



extern int vel;

extern int velD;
extern int velI;

void motorsSetup();

void setVelD(int val);
void setVelI(int val);
void setVel(int val);

int getVelD();
int getVelI();
int getVel();
int getVelBase();

void motorDerechoAdelante();
void motorIzquierdoAdelante();

void motorDerechoAtras();
void motorIzquierdoAtras();

void motorDerechoStop();
void motorIzquierdoStop();

void motoresAdelante();
void motoresAtras();
void motoresStop();

void motoresGiroDerecha();
void motoresGiroIzquierda();

void motoresGiroDerechaCerrado();
void motoresGiroIzquierdaCerrado();

void motoresSusto();
void motoresGirar45Derecha();
void motoresGirar45Izquierda();
void motoresGirar90Derecha();
void motoresGirar90Izquierda();
void motoresGirar180();

#endif // MOTORS_H