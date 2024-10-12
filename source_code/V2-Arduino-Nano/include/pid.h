#ifndef PID_H
#define PID_H
#include <Arduino.h>

#include "motors.h"
#include "sharps.h"

#define velocidadMaxima 250

#define NUM_SENSORES 3

// Variables PID
extern float kp;
extern float kd;
// extern float ki = 0;

extern int posicion;
extern int posicion_anterior;

extern int proporcional;
extern int derivada;
// extern int integral = 0;

extern int correccion;
extern int velocidad;

extern unsigned long millisPID;
extern int tiempoPID;

extern bool prints;
extern bool noSpeed;

extern int lastSeen;

void pidSetup();
void enablePrintsPid();
void dissableSpeedPid();
void doPid();
int proporcionalPesos();
int proporcionalSimple();
void movimiento(int pos, int correccion, int velI, int velD);
int limitSpeed(int speed);

#endif