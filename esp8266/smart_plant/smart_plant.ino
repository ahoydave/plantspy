#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define ANA_M_READ A0
#define DIGI_M_READ D5
#define DIGI_PUMP D6
#define DIGI_M_POWER D1
#define SLEEP_TIME 60000
#define WATER_TIME 500
#define POWER_READ_DELAY 100

double analogValue = 0.0;
int digitalValue = 0;
double analogVolts = 0.0;
unsigned long timeHolder = 0;
bool pumpOn = false;

// WiFi parameters to be configured
const char* ssid = "MY SSID THIS IS NOT MY ACTUAL SSID";
const char* password = "MY WIFI PASSWORD THIS IS NOT THE ACTUAL PASSWORD";
const char* host = "MY SWEET HOST URI";

void setup() {
  // put your setup code here, to run once:
  pinMode(ANA_M_READ, INPUT);
  pinMode(DIGI_M_READ, INPUT);
  pinMode(DIGI_PUMP, OUTPUT);
  pinMode(DIGI_M_POWER, OUTPUT);
  Serial.begin(115200);
  Serial.println("Go Water Plant Robot!");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Read moisture and decide if we should pump
  digitalWrite(DIGI_M_POWER, true);
  delay(POWER_READ_DELAY);
  analogValue = analogRead(ANA_M_READ);
  digitalValue = digitalRead(DIGI_M_READ);
  digitalWrite(DIGI_M_POWER, false);
  pumpOn = digitalValue == 1;

  // Pump if we need to
  if (pumpOn) {
    digitalWrite(DIGI_PUMP, true);
    delay(WATER_TIME);
    digitalWrite(DIGI_PUMP, false);
  }

  // Send debug serial data
  Serial.print("Analog raw: ");
  Serial.println(analogValue);
  Serial.print("Digital raw: ");
  Serial.println(digitalValue);
  Serial.print("Pump on?: ");
  Serial.println(pumpOn);
  
  // Send an update to our monitoring service
  if (WiFi.status() == WL_CONNECTED) {
    // We need to tell the HTTP client to ignore the SSL certificate
    // This is because we (might be) connecting over HTTPS and our
    // device doesn't have any stored certificates
    // Don't send your credit card details like this :)
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    http.begin(client, host);
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<256> doc;
    doc["analogValue"] = analogValue;
    doc["pumpOn"] = pumpOn;
    String json = "";
    serializeJson(doc, json);
    int httpCode = http.POST(json);
    Serial.println(json);
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
  
  delay(SLEEP_TIME);
}
