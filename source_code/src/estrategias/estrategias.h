#ifndef ESTRATEGIAS_H
#define ESTRATEGIAS_H
#include <Arduino.h>

////
#define velocidadMaxima 250 

#define NUM_SENSORES 3
extern long lectura[NUM_SENSORES];
//Variables PID
extern float kp; //2
extern float kd; //0
extern float ki;
extern unsigned int sensores[NUM_SENSORES];
extern int posicion;
extern int posicion_anterior;
extern int proporcional;
extern int derivada;
extern int integral;
extern int correccion;
extern int velocidad;

extern long millisPID;
extern int tiempoPID; //40

extern bool prints;


extern int lastSeen;

extern void estrategiaPID();
extern int proporcionalPesos();
extern int proporcionalSimple();
extern void movimiento(int pos, int correccion, int velI, int velD);
extern int limitSpeed(int speed);

////

extern void estrategiaMirarAdelante();
extern void estrategiaMirarAtras();
extern void estrategiaMirarLadoDerecha();
extern void estrategiaMirarLadoIzquierda();
extern void estrategiaCaja();

#endif