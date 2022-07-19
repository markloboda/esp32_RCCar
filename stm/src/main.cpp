#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "secrets.h"

// wifi connection
const char *ssid = secret_ssid;         // replace
const char *password = secret_password; // replace

AsyncWebServer server(80);

// Motor properties
/// Motor A
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;
/// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

// defining functions
void setSpeed(int speed)
{
  dutyCycle = speed;
  ledcWrite(pwmChannel, dutyCycle); // set duty cycle
}

void moveForward()
{
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
}

void moveBackward()
{
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
}

void stop()
{
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
}

void onBodyThrottle(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  // parse data with arduino json lib
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, data);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  // get the value of the "speed" field
  int speed = doc["value"];
  Serial.println(speed);
}

void onBodySteering(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  // parse data with arduino json lib
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, data);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  // get the value of the "steering" field
  int steering = doc["value"];
  Serial.println(steering);
}

void setup()
{
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);

  // initializing
  Serial.begin(115200);

  // wifi connection
  WiFi.begin(ssid, password);

  // wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  // start server with responses

  server.on("/controls/throttle", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
  }, NULL, onBodyThrottle);

  server.on("/controls/steering", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
  }, NULL, onBodySteering);

  server.begin();
}

void loop()
{
}
