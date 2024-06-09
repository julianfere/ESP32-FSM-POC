#include <ApServer.h>
#include <SimpleFSM.h>
#include <Leds.h>

ApServer apServer;
SimpleFSM machine;

int tries = 12;

#pragma region State functions
void setupApServer()
{
  turnOn("blue");
  apServer.setup();
}

void connectToLocalNetwork()
{
  tries = 12;
  WiFi.mode(WIFI_STA);
  wl_status_t state = WiFi.begin(apServer.ssid.c_str(), apServer.password.c_str());

  Serial.print("Attempting to connect");
  while (state != WL_CONNECTED && tries > 0)
  {
    blinkLed("green");
    tries--;
    Serial.print(".");
    delay(1000);

    state = WiFi.status();
  }
}

void logAndSleep()
{
  turnOn("green");
  Serial.println("Connected to network");
  delay(5000);
}

void logTransition(String to)
{
  Serial.println("Transitioning to " + to);

  for (int i = 0; i < 10; i++)
  {
    blinkLed("yellow");
    delay(200);
  }
}
#pragma endregion

#pragma region States

State SETUP_SERVER_STATE = State("Ap Server", setupApServer);
State CONNECT_TO_NETWORK_STATE = State("Connect to Wifi", connectToLocalNetwork);
State LOG_AND_SLEEP_STATE = State("Log and wait", NULL, logAndSleep);

#pragma endregion

#pragma region Triggers and Transitions
enum Triggers
{
  SETUP_AP = 1,
  CONNECT_TO_WIFI = 2,
  LOG_AND_SLEEP = 3,
};

Transition transitions[] = {
    Transition(&SETUP_SERVER_STATE, &CONNECT_TO_NETWORK_STATE, CONNECT_TO_WIFI, []()
               { logTransition("CONNECT_TO_WIFI"); }),
    Transition(&CONNECT_TO_NETWORK_STATE, &LOG_AND_SLEEP_STATE, LOG_AND_SLEEP, []()
               { logTransition("LOG_AND_SLEEP"); }),
    Transition(&CONNECT_TO_NETWORK_STATE, &SETUP_SERVER_STATE, SETUP_AP, []()
               { logTransition("SETUP_AP"); }),
};

#pragma endregion

int num_transitions = sizeof(transitions) / sizeof(Transition);

#pragma region Setup and Loop

void setup()
{
  Serial.begin(115200);
  machine.add(transitions, num_transitions);
  machine.setInitialState(&SETUP_SERVER_STATE);
  setupPins();
}

void loop()
{
  apServer.handleNextRequest();
  machine.run();

  if ((apServer.networkSelected() && machine.isInState(&SETUP_SERVER_STATE) && tries > 0))
  {
    machine.trigger(CONNECT_TO_WIFI);
  }

  if ((WiFi.status() == WL_CONNECTED) && machine.isInState(&CONNECT_TO_NETWORK_STATE))
  {
    machine.trigger(LOG_AND_SLEEP);
  }

  if (machine.isInState(&CONNECT_TO_NETWORK_STATE) && WiFi.status() == WL_DISCONNECTED && tries == 0)
  {
    Serial.println("Failed to connect to network");
    for (int i = 0; i < 10; i++)
    {
      blinkLed("red");
      delay(200);
    }
    machine.trigger(SETUP_AP);
  }
}

#pragma endregion