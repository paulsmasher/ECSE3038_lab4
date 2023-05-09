#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

const char* HOST = "ecse-three-led-api.onrender.com";

const int LED_PIN_1 = 9;
const int LED_PIN_2 = 10;
const int LED_PIN_3 = 11;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("");

  Serial.println("Loading");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String http_response;

    String url = "https://" + String(HOST) + "/api/state";
    http.begin(url);
    http.addHeader("X-API-Key", API_KEY);

    int http_response_code = http.GET();

    Serial.println("");
    Serial.println("");

    if (http_response_code > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(http_response_code);

      Serial.print("Response from server: ");
      http_response = http.getString();
      Serial.println(http_response);
    } else {
      Serial.print("Error code: ");
      Serial.println(http_response_code);
    }
    http.end();

    StaticJsonDocument<1024> doc;

    DeserializationError error = deserializeJson(doc, http_response);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    bool light_switch_1 = doc["light_switch_1"];
    bool light_switch_2 = doc["light_switch_2"];
    bool light_switch_3 = doc["light_switch_3"];

    Serial.println("");
    Serial.println(light_switch_1);
    Serial.println(light_switch_2);
    Serial.println(light_switch_3);
    digitalWrite(LED_PIN_1, light_switch_1);
    digitalWrite(LED_PIN_2, light_switch_2);
    digitalWrite(LED_PIN_3, light_switch_3);
    Serial.println("LED states updated");

  } else {
    return;
  }
}