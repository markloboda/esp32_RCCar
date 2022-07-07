#include <Arduino.h>
#include <string>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "global.h"

const char* ssid = "";  //replace
const char* password =  ""; //replace

AsyncWebServer server(80);

void setup() {
  // initializing
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  // wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  // start server with responses
  server.on("/light/on", HTTP_GET, [](AsyncWebServerRequest *request) { request ->
    send(200, "text/plain","lights turned on");
    Serial.println("light on");
  });

  server.on("/light/off", HTTP_GET, [](AsyncWebServerRequest *request) { request ->
    send(200, "text/plain","lights turned off");
    Serial.println("light off");
  });

  server.begin();
}

void loop() {}