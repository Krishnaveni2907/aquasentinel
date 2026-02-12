#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#define PIR_PIN   D5
#define RAIN_PIN  D6

const char* ssid = "annasofiya";
const char* password = "annasofiya";
const char* pushToken = "o.6F4rW1In4ESj02acLou1csZCZ5LMB667";

unsigned long lastPush = 0;
const unsigned long cooldown = 1000; 

void sendPush(String message) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  if (https.begin(client, "https://api.pushbullet.com/v2/pushes")) {
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Access-Token", pushToken);

    String payload = "{";
    payload += "\"type\":\"note\",";
    payload += "\"title\":\"Washroom Alert 🚨\",";
    payload += "\"body\":\"" + message + "\"";
    payload += "}";

    https.POST(payload);
    https.end();
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  bool waterDetected = (digitalRead(RAIN_PIN) == LOW); 
  bool motionDetected = (digitalRead(PIR_PIN) == HIGH);

  if (waterDetected && millis() - lastPush > cooldown) {

    if (motionDetected) {
      sendPush("Water is leaking and the person is inside the restroom 🚿");
    } else {
      sendPush("Water is leaking and the person is NOT in the restroom ⚠️");
    }

    lastPush = millis();
  }
}
