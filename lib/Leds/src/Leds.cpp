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

void turnOn(String color)
{
  int rgb[3] = {0, 0, 0};

  if (color == "white")
  {
    rgb[0] = WHITE[0];
    rgb[1] = WHITE[1];
    rgb[2] = WHITE[2];
  }
  else if (color == "red")
  {
    rgb[0] = RED[0];
    rgb[1] = RED[1];
    rgb[2] = RED[2];
  }
  else if (color == "green")
  {
    rgb[0] = GREEN[0];
    rgb[1] = GREEN[1];
    rgb[2] = GREEN[2];
  }
  else if (color == "blue")
  {
    rgb[0] = BLUE[0];
    rgb[1] = BLUE[1];
    rgb[2] = BLUE[2];
  }
  else if (color == "yellow")
  {
    rgb[0] = YELLOW[0];
    rgb[1] = YELLOW[1];
    rgb[2] = YELLOW[2];
  }
  else
  {
    rgb[0] = WHITE[0];
    rgb[1] = WHITE[1];
    rgb[2] = WHITE[2];
  }

  analogWrite(PIN_RED, rgb[0]);
  analogWrite(PIN_GREEN, rgb[1]);
  analogWrite(PIN_BLUE, rgb[2]);
}

void blinkLed(String color)
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