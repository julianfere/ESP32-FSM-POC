#include <Arduino.h>
#include "Leds.h"

int blink = 0;
int blinkInterval = 100;

void setupPins()
{
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
}

void turnOff()
{
  analogWrite(PIN_RED, LOW);
  analogWrite(PIN_GREEN, LOW);
  analogWrite(PIN_BLUE, LOW);
}

void turnOn(LedColors color)
{
  int rgb[3] = {0, 0, 0};

  switch (color)
  {
  case LedColors::WHITE:
    rgb[0] = WHITE_C[0];
    rgb[1] = WHITE_C[1];
    rgb[2] = WHITE_C[2];
    break;
  case LedColors::RED:
    rgb[0] = RED_C[0];
    rgb[1] = RED_C[1];
    rgb[2] = RED_C[2];
    break;
  case LedColors::GREEN:
    rgb[0] = GREEN_C[0];
    rgb[1] = GREEN_C[1];
    rgb[2] = GREEN_C[2];
    break;
  case LedColors::BLUE:
    rgb[0] = BLUE_C[0];
    rgb[1] = BLUE_C[1];
    rgb[2] = BLUE_C[2];
    break;
  case LedColors::YELLOW:
    rgb[0] = YELLOW_C[0];
    rgb[1] = YELLOW_C[1];
    rgb[2] = YELLOW_C[2];
    break;
  case LedColors::CYAN:
    rgb[0] = CYAN_C[0];
    rgb[1] = CYAN_C[1];
    rgb[2] = CYAN_C[2];
    break;
  default:
    rgb[0] = WHITE_C[0];
    rgb[1] = WHITE_C[1];
    rgb[2] = WHITE_C[2];
    break;
  }

  analogWrite(PIN_RED, rgb[0]);
  analogWrite(PIN_GREEN, rgb[1]);
  analogWrite(PIN_BLUE, rgb[2]);
}

void blinkLed(LedColors color)
{
  if (blink == 0)
  {
    turnOn(color);
    blink = 1;
  }
  else
  {
    turnOff();
    blink = 0;
  }
}