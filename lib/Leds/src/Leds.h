#include <Arduino.h>

#ifndef LEDS_H
#define LEDS_H

#define PIN_RED 23   // GPIO23
#define PIN_GREEN 22 // GPIO22
#define PIN_BLUE 21  // GPIO21

const int WHITE[3] = {255, 255, 255};
const int RED[3] = {255, 0, 0};
const int GREEN[3] = {0, 255, 0};
const int BLUE[3] = {0, 0, 255};
const int YELLOW[3] = {255, 255, 0};

void setupPins();
void turnOn(String color = "white");
void turnOff();
void blinkLed(String color = "white");
#endif