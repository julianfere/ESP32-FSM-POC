#include <Arduino.h>
#include <DHT.h>

#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#define DHTPIN 4
#define DHTTYPE DHT11
#define READINGS 10

class TemperatureSensor
{
public:
  TemperatureSensor();
  void read();
  float getTemperature();
  float getHumidity();
  float getAverageTemperature();
  float getAverageHumidity();
  float isAverageReady();
  void reset();

private:
  DHT dht;
  float temperature;
  float humidity;
  float averageTemperature;
  float averageHumidity;
  int readings;
};
#endif
