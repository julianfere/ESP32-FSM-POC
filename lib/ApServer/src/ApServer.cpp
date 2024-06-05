#include <Arduino.h>
#include "ApServer.h"

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request)
  {
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", "<h1> Hello World!</h1>");
  }
};

ApServer::ApServer() : _dnsServer(), _server(80), networks(0), readyToConnect(false)
{
  ssid = "";
  password = "";
}

bool ApServer::networkSelected()
{
  return readyToConnect && (ssid != "");
}

void ApServer::setupAp()
{
  WiFi.softAP("ESP32-Access-Point", "123456789");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void ApServer::scanNetworks()
{
  networks = WiFi.scanNetworks();
}

void ApServer::setup()
{
  setupAp();

  _dnsServer.start(53, "*", WiFi.softAPIP());

  scanNetworks();

  _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send_P(200, "text/html", "<h1> Hello World!</h1>"); }); // Render a page with select input to choose a network

  _server.on("/connect", HTTP_POST, [](AsyncWebServerRequest *request)
             {
              if (request->hasParam("ssid", true) && request->hasParam("password", true)) [&]{
                ssid = request->getParam("ssid", true)->value();
                password = request->getParam("password", true)->value();
                readyToConnect = true;
              };
              request->send(200, "text/plain", "OK"); });

  _server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  _server.begin();
}

void ApServer::handleNextRequest()
{
  _dnsServer.processNextRequest();
}