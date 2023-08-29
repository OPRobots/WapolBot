#ifndef SHARPS_H
#define SHARPS_H
#include <Arduino.h>
#include <SharpIR.h>

/*
sensores  1-derecha
sharp     6-izquierda
          5-centro
*/ 
#define pinSharpCentro A5 
#define pinSharpIzquierda A6
#define pinSharpDerecha A1 

extern SharpIR sharpCentro;
extern SharpIR sharpIzquierda;
extern SharpIR sharpDerecha;

int sharpCentroDistancia();
int sharpIzquierdoDistancia();
int sharpDerechoDistancia();

int limitedDistance(int val);

bool sharpCentroDetectado();
bool sharpIzquierdoDetectado();
bool sharpDerechoDetectado();

bool sharpCentroCerca();
bool sharpIzquierdoCerca();
bool sharpDerechoCerca();


#endif

/*
Serial.begin(9600);

Serial.print("Distancia: ");
Serial.print(sharpIzquierdoDistancia());
Serial.print(" - ");
Serial.print(sharpCentroDistancia());
Serial.print(" - ");
Serial.print(sharpDerechoDistancia());
Serial.println(" cm");
delay(1000);
*/