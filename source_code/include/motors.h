#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>

#define MotorIalante 5
#define MotorIatras 6

#define MotorDalante 11
#define MotorDatras 3

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

#endif