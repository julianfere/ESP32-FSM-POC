#include <Arduino.h>

#ifndef LEDS_H
#define LEDS_H

#define PIN_RED 23   // GPIO23
#define PIN_GREEN 22 // GPIO22
#define PIN_BLUE 21  // GPIO21

enum LedColors
{
  WHITE,
  RED,
  GREEN,
  BLUE,
  YELLOW,
  CYAN
};

const int WHITE_C[3] = {255, 255, 255};
const int RED_C[3] = {255, 0, 0};
const int GREEN_C[3] = {0, 255, 0};
const int BLUE_C[3] = {0, 0, 255};
const int YELLOW_C[3] = {255, 255, 0};
const int CYAN_C[3] = {0, 255, 255};

void setupPins();
void turnOn(LedColors color = LedColors::WHITE);
void blinkLed(LedColors color = LedColors::WHITE);
void turnOff();
#endif