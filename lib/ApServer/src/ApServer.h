/*
  ApServer.h - Library for creating an Access Point with a web server.
*/
#ifndef ApServer_h
#define ApServer_h
#include <Arduino.h>
#include <DNSServer.h>
#include "ESPAsyncWebServer.h"
#include <WiFi.h>

class ApServer
{

public:
  ApServer();
  void setup();
  void handleNextRequest();
  bool networkSelected();
  String ssid;
  String password;

private:
  DNSServer _dnsServer;
  AsyncWebServer _server;
  bool readyToConnect;
  int networks;
  void setupAp();
  void scanNetworks();
};
#endif