#include <Arduino.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>

MQTTClient mqtt_client;
WiFiClient wifi_client;

#include "credentials.cpp"

const char MQTT_TOPIC_IN[] = "/IFD2/IN";
const char MQTT_TOPIC_OUT[] = "/IFD2/OUT";

void connect() {
  Serial.println("[DBG:ESP:Trying to connect to wifi]");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("[DBG:ESP:...]");
    delay(100);
  }

  Serial.println("[DBG:ESP:Wifi connected, trying to connect to MQTT...]");
  while (!mqtt_client.connect(BOARDNAME, USERNAME, PASSWORD)) {
    Serial.println("[DBG:ESP:...]");
    delay(100);
  }
  Serial.println("[DBG:ESP:MQTT connected!]");
  mqtt_client.subscribe(MQTT_TOPIC_IN);
}

void message_received(String &topic, String &payload) {
  Serial.print("[FWD:");
  Serial.print(topic);
  Serial.print(":");
  Serial.print(payload);
  Serial.println("]");
}

String serial_buffer = "";
void process_serial_events() {
  while (Serial.available()) {
    char c = Serial.read();
    serial_buffer += c;
    if (c == ']') {
      mqtt_client.publish(MQTT_TOPIC_OUT, serial_buffer);
      serial_buffer = "";
    }
  }
}

unsigned long last_alive = 0;
void send_alive() {
  if (millis() - last_alive > 10000) {
    last_alive = millis();
    mqtt_client.publish("/esp/alive", BOARDNAME);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("[DBG:ESP:Welcome!]");
  WiFi.begin(SSID, WIFI_PASS);

  mqtt_client.begin(HOSTNAME, 1883, wifi_client);
  mqtt_client.onMessage(message_received);
  connect();
}

void loop() {
  mqtt_client.loop();
  if (!mqtt_client.connected()) {
    connect();
  }
  process_serial_events();
  send_alive();
}
