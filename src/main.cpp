#include <ApServer.h>
#include <SimpleFSM.h>
#include <Leds.h>
#include <Ticker.h>
#include <TemperatureSensor.h>

#pragma region Declaration

#define WIFI_TIME_OUT_SECONDS 120
#define READ_INTERVAL 10

ApServer apServer;
SimpleFSM machine;
Ticker ticker;
TemperatureSensor sensor;

int tries = 12;

enum Triggers
{
  SETUP_AP = 1,
  CONNECT_TO_WIFI = 2,
  OPERATIONAL = 3,
  WIFI_TIMED_OUT = 4,
};

#pragma endregion

#pragma region Ticker handler

void handleWifiTimeout()
{
  machine.trigger(WIFI_TIMED_OUT);
}

void handleReadFromSensor()
{
  for (int i = 0; i < 10; i++)
  {
    blinkLed(LedColors::CYAN);
    delay(100);
  }

  sensor.read();

  Serial.println("Temperature: " + String(sensor.getTemperature()) + " Humidity: " + String(sensor.getHumidity()));
}

#pragma endregion

#pragma region State functions
void setupApServer()
{
  turnOn(LedColors::BLUE);
  apServer.setup();
  ticker.once(WIFI_TIME_OUT_SECONDS, handleWifiTimeout);
}

void onEnterConnectToLocalNetwork()
{
  ticker.detach();
  tries = 12;
  WiFi.mode(WIFI_STA);
  wl_status_t state = WiFi.begin(apServer.ssid.c_str(), apServer.password.c_str());
}

void connectToLocalNetwork()
{
  Serial.print("Attempting to connect");

  if (WiFi.status() == WL_CONNECTED)
  {
    machine.trigger(OPERATIONAL);
    return;
  };

  tries--;
  Serial.print(".");
  delay(500);
  blinkLed(LedColors::GREEN);
}

void onEnterOperational()
{
  turnOn();
  ticker.attach(READ_INTERVAL, handleReadFromSensor);
}

void operational()
{
  if (sensor.isAverageReady())
  {
    Serial.println("---------------- Average Ready ----------------");
    Serial.println("Temperature: " + String(sensor.getAverageTemperature()) + " Humidity: " + String(sensor.getAverageHumidity()));
    Serial.println("-----------------------------------------------");
    sensor.reset();
  }
}

void onExitOperational()
{
  ticker.detach();
  turnOff();
}

void logTransition(String to)
{
  Serial.println("Transitioning to " + to);

  for (int i = 0; i < 10; i++)
  {
    blinkLed(LedColors::YELLOW);
    delay(200);
  }
}

void wifiTimedOut()
{
  blinkLed(LedColors::YELLOW);
  delay(100);
}
#pragma endregion

#pragma region States

State SETUP_SERVER_STATE = State("Ap Server", setupApServer);
State CONNECT_TO_NETWORK_STATE = State("Connect to Wifi", onEnterConnectToLocalNetwork, connectToLocalNetwork);
State OPERATIONAL_STATE = State("Operational", onEnterOperational, operational, onExitOperational);
State WIFI_TIMED_OUT_STATE = State("Wifi timed out", NULL, wifiTimedOut);

#pragma endregion

#pragma region Transitions

Transition transitions[] = {
    Transition(&SETUP_SERVER_STATE, &CONNECT_TO_NETWORK_STATE, CONNECT_TO_WIFI, []()
               { logTransition("CONNECT_TO_WIFI"); }),
    Transition(&CONNECT_TO_NETWORK_STATE, &OPERATIONAL_STATE, OPERATIONAL, []()
               { logTransition("OPERATIONAL"); }),
    Transition(&CONNECT_TO_NETWORK_STATE, &SETUP_SERVER_STATE, SETUP_AP, []()
               { logTransition("SETUP_AP"); }),
    Transition(&SETUP_SERVER_STATE, &WIFI_TIMED_OUT_STATE, WIFI_TIMED_OUT, []()
               { logTransition("WIFI_TIMED_OUT"); })};

#pragma endregion

#pragma region Utils

bool failedToConnectToWifi()
{
  return WiFi.status() == WL_DISCONNECTED && tries == 0;
}

#pragma endregion

#pragma region Setup and Loop

void setup()
{
  sensor = TemperatureSensor();
  int num_transitions = sizeof(transitions) / sizeof(Transition);
  Serial.begin(115200);
  machine.add(transitions, num_transitions);
  machine.setInitialState(&SETUP_SERVER_STATE);
  setupPins();
  Serial.println(machine.getDotDefinition());
}

void loop()
{
  apServer.handleNextRequest();
  machine.run();

  if ((apServer.networkSelected() && machine.isInState(&SETUP_SERVER_STATE) && tries > 0))
  {
    machine.trigger(CONNECT_TO_WIFI);
  }

  if (machine.isInState(&CONNECT_TO_NETWORK_STATE) && failedToConnectToWifi())
  {
    Serial.println("Failed to connect to network");
    for (int i = 0; i < 10; i++)
    {
      blinkLed(LedColors::RED);
      delay(200);
    }
    machine.trigger(SETUP_AP);
  }

  if ((WiFi.status() == WL_CONNECTED) && machine.isInState(&CONNECT_TO_NETWORK_STATE))
  {
    machine.trigger(OPERATIONAL);
  }
}

#pragma endregion