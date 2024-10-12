#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>

#include "pinout.h"
#include "motors.h"
#include "sensors.h"

void set_starting(bool starting);
bool is_starting();

#endif