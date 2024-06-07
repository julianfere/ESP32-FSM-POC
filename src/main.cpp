#include <ApServer.h>
#include <SimpleFSM.h>

ApServer apServer;
SimpleFSM machine;

int tries = 12;

void setupApServer()
{
  apServer.setup();
}

bool transitionFromS0ToS0()
{
  return true;
}

bool transitionFromS0ToS1()
{
  return apServer.networkSelected();
}

void connectToLocalNetwork()
{
  tries = 12;
  WiFi.mode(WIFI_STA);
  wl_status_t state = WiFi.begin(apServer.ssid.c_str(), apServer.password.c_str());

  Serial.print("Attempting to connect");
  while (state != WL_CONNECTED && tries > 0)
  {
    tries--;
    Serial.print(".");
    delay(1000);

    state = WiFi.status();
  }
}

bool transitionFromS1toS2()
{
  return WiFi.status() == WL_CONNECTED;
}

void logAndSleep()
{
  Serial.println("Connected to network");
  delay(5000);
}

void logTransition(String to)
{
  Serial.println("Transitioning to " + to);
}

State SETUP_SERVER_STATE = State("setupApServer", setupApServer);
State CONNECT_TO_NETWORK_STATE = State("connectToLocalNetwork", connectToLocalNetwork);
State LOG_AND_SLEEP_STATE = State("logAndSleep", logAndSleep);

enum triggers
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

int num_transitions = sizeof(transitions) / sizeof(Transition);

void setup()
{
  Serial.begin(115200);
  machine.add(transitions, num_transitions);
  machine.setInitialState(&SETUP_SERVER_STATE);
}

void loop()
{
  apServer.handleNextRequest();
  machine.run();

  if ((apServer.networkSelected() && machine.isInState(&SETUP_SERVER_STATE)))
  {
    machine.trigger(CONNECT_TO_WIFI);
  }

  if ((WiFi.status() == WL_CONNECTED) && machine.isInState(&CONNECT_TO_NETWORK_STATE))
  {
    Serial.println("Connected to network");
    machine.trigger(LOG_AND_SLEEP);
  }

  if (machine.isInState(&CONNECT_TO_NETWORK_STATE) && WiFi.status() == WL_DISCONNECTED && tries == 0)
  {
    Serial.println("Failed to connect to network");
    machine.trigger(SETUP_AP);
  }
}
