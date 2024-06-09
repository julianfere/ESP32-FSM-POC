#include <Arduino.h>
#include <TemperatureSensor.h>

TemperatureSensor::TemperatureSensor() : dht(DHTPIN, DHTTYPE)
{
  averageHumidity = 0;
  averageTemperature = 0;
  readings = 0;
}

void TemperatureSensor::read()
{
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  averageHumidity += humidity;
  averageTemperature += temperature;
  readings++;
}

float TemperatureSensor::getTemperature()
{
  return temperature;
}

float TemperatureSensor::getHumidity()
{
  return humidity;
}

float TemperatureSensor::getAverageTemperature()
{
  return averageTemperature / readings;
}

float TemperatureSensor::getAverageHumidity()
{
  return averageHumidity / readings;
}

float TemperatureSensor::isAverageReady()
{
  return readings == READINGS;
}

void TemperatureSensor::reset()
{
  averageHumidity = 0;
  averageTemperature = 0;
  readings = 0;
}