#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#include "pinout.h"
#include "buttons.h"
#include "leds.h"

/**
 * @brief Valor máximo y mínimo.
 *
 */
#define SENSORS_MAX 4095.0
#define SENSORS_MIN 0

void filtro_sensores2();

int sensor1_analog();
int sensor2_analog();
int sensor3_analog();

void print_sensores_1();
void print_sensores_2();

bool qre1113IzquierdoBlanco();
bool qre1113DerechoBlanco();

bool qre1113Blancos();

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

#endif // SENSORS_H