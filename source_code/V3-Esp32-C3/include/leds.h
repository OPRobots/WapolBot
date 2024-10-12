#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "pinout.h"

enum LEDS {
  RGB_LEFT = 2,
  RGB_TOP = 1,
  RGB_RIGHT = 0
};

void set_led(enum LEDS led, int r, int g, int b);
void clear_led(enum LEDS led);

void blink_led(enum LEDS led, int r, int g, int b, int ms);
void rainbow_led(enum LEDS led);
void rainbow_leds();

void clear_leds();
void set_leds(int r, int g, int b);
void blink_leds(int r, int g, int b, int ms);




void ledsGreen(uint8_t val);
void ledsUp(uint8_t val);
void ledsLeft(uint8_t val);
void ledsRight(uint8_t val);
void ledsLeftRight(uint8_t val);
void ledsLoading(uint8_t val);
void ledsCuentaAtras();

#endif // LEDS_H