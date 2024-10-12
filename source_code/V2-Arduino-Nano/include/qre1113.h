#ifndef QRE1113_H
#define QRE1113_H
#include <Arduino.h>

// sensores 0-7 suelo
#define pinQRE1113Derecha A0 
#define pinQRE1113Izquierda A7

int qre1113Izquierdo();
int qre1113Derecho();

int qre1113IzquierdoMapeado();
int qre1113DerechoMapeado();

bool qre1113IzquierdoBlanco();
bool qre1113DerechoBlanco();

bool qre1113Blancos();

#endif

/*
Serial.begin(9600);

Serial.print("Sensor suelo: ");
Serial.print(qre1113IzquierdoMapeado());
Serial.print(" - ");
Serial.println(qre1113DerechoMapeado());
delay(1000);
*/