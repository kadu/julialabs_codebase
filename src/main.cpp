#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "ota.h"

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void configuraRotas() {
    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam("message")) {
            message = request->getParam("message")->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  setupOTA();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configuraRotas();
}

void loop() {
  ArduinoOTA.handle();
}