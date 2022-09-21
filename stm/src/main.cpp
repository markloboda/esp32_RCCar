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

////////////////////
// Motor properties
////////////////////
/// Motor A
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;
/// Setting PWM properties
const int freq = 30000;
const int dcPwmChannel = 1;
const int resolution = 8;
int dutyCycle = 0;

////////////////////
// Servo motor properties
////////////////////
int servoPin = 18;
int servoPwmChannel = 2;
int minServoVal = 1638;
int maxServoVal = 7864;

////////////////////
// defining functions
////////////////////
void setSpeed(int speed)
{
  if (speed > 0)
  {
    // spin motor forward
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
  }
  else if (speed < 0)
  {
    // make speed into a positive number
    speed = -speed;
    // spin motor backward
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
  }
  else
  {
    // stop motor
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
  }
  // set speed (duty cycle)
  speed += 150; // add offset (dutyCycle = 150 for 0% speed)
  dutyCycle = speed;
  ledcWrite(dcPwmChannel, dutyCycle); // set duty cycle
}

void setSteering(int steering)
{
  // steering is in degrees 0 to 180
  // convert to PWM count
  int steeringPwmCount = map(steering, 0, 180, minServoVal, maxServoVal);
  // set steering to servoPwmChannel
  ledcWrite(servoPwmChannel, steeringPwmCount);
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
  Serial.print("Speed chanage:");
  Serial.println(speed);
  // set the speed of the motor
  setSpeed(speed);
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
  Serial.print("Steering chanage:");
  Serial.println(steering);
  // set steering of servo
  setSteering(steering);
}

void setup()
{
  ////////////////////
  // servo setup
  ////////////////////
  ledcSetup(servoPwmChannel, 50, 16);
  ledcAttachPin(servoPin, servoPwmChannel);

  ////////////////////
  // motor setup
  ////////////////////
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(dcPwmChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, dcPwmChannel);

  // initializing
  Serial.begin(115200);

  // wifi connection
  WiFi.begin(ssid, password);
  // change the variables ssid and password to your wifi credentials

  // wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  // start server with responses

  server.on(
      "/controls/throttle", HTTP_POST, [](AsyncWebServerRequest *request)
      { request->send(200, "text/plain", "OK"); },
      NULL, onBodyThrottle);

  server.on(
      "/controls/steering", HTTP_POST, [](AsyncWebServerRequest *request)
      { request->send(200, "text/plain", "OK"); },
      NULL, onBodySteering);

  server.begin();
}

void loop()
{
}
