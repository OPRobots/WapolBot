#ifndef LEDS_H
#define LEDS_H
#include <Arduino.h>

#define ledUp 9
#define ledDown 8
#define ledLeft 7
#define ledRight 10
#define ledGreen 4
#define ledBuiltIn LED_BUILTIN


void ledsSetup();
void ledsTest();
void ledsGreen(uint8_t val);
void ledsBuiltIn(uint8_t val);
void ledsUp(uint8_t val);
void ledsDown(uint8_t val);
void ledsLeft(uint8_t val);
void ledsRight(uint8_t val);
void ledsRightArrow(uint8_t val);
void ledsLeftArrow(uint8_t val);
void ledsUpArrow(uint8_t val);
void ledsDownArrow(uint8_t val);
void ledsUpDown(uint8_t val);
void ledsLeftRight(uint8_t val);
void ledsLoading(uint8_t val);
void ledsCuentaAtras();

#endif