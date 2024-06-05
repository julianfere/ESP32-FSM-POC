#include <ApServer.h>
#include <StateMachine.h>

ApServer apServer;
StateMachine machine;

State *S0 = machine.addState(&setupApServer);
State *S1 = machine.addState(&connectToLocalNetwork);
State *S2 = machine.addState(&logAndSleep);

void setup()
{
  Serial.begin(115200);
  apServer.setup();

  S0->addTransition(&transitionFromS0ToS0, S0);
  S0->addTransition(&transitionFromS0ToS1, S1);
  S1->addTransition(&transitionFromS1toS2, S1);
}

void loop()
{
  apServer.handleNextRequest();
  machine.run();
}

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
  WiFi.mode(WIFI_STA);
  wl_status_t state = WiFi.begin(apServer.ssid.c_str(), apServer.password.c_str());

  while (state != WL_CONNECTED)
  {
    Serial.print("Attempting to connect");
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