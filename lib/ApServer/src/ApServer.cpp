#include <Arduino.h>
#include "ApServer.h"
#include "Pages.h"

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
    request->send_P(200, "text/html", CAPTIVE_PAGE);
  }
};

ApServer::ApServer() : _dnsServer(), _server(80), networks(0), readyToConnect(false)
{
  ssid = "";
  password = "";
}

bool ApServer::networkSelected()
{
  return this->readyToConnect && (this->ssid != "");
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

void ApServer::setSsid(const String ssid)
{
  Serial.println(ssid);
  ApServer::ssid = ssid;
}

void ApServer::setPassword(const String password)
{
  Serial.println(password);

  this->password = password;
}

void ApServer::setReadyToConnect(bool readyToConnect)
{
  this->readyToConnect = readyToConnect;
}

void ApServer::handleHome(AsyncWebServerRequest *request)
{
  char html[2000];
  strcpy(html, MAIN_PAGE_1);
  String st = "";

  for (int i = 0; i < this->networks; i++)
  {
    Serial.println(WiFi.SSID(i));
    st += "<option ";
    st += "value='" + WiFi.SSID(i) + "'";
    st += " >";
    st += WiFi.SSID(i);
    st += "</option>";
  };

  strcat(html, st.c_str());
  strcat(html, MAIN_PAGE_2);
  request->send(200, "text/html", html);
}

void ApServer::setup()
{
  setupAp();

  _dnsServer.start(53, "*", WiFi.softAPIP());

  scanNetworks();

  _server.on("/home", HTTP_GET, [this](AsyncWebServerRequest *request)
             { this->handleHome(request); });

  _server.on("/connect", HTTP_POST, [this](AsyncWebServerRequest *request)
             {
    Serial.println("POST request received");
    Serial.println(request->params());
    if (request->hasParam("ssid", true) && request->hasParam("password", true))
    {
      Serial.println("SSID and password received");
      this->setSsid(request->getParam("ssid", true)->value());
      this->setPassword(request->getParam("password", true)->value());
      this->setReadyToConnect(true);
    };
    request->send(200, "text/plain", "OK"); });

  this->networks = WiFi.scanNetworks();

  _server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  _server.begin();
}

void ApServer::handleNextRequest()
{
  _dnsServer.processNextRequest();
}