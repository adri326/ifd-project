#include <Arduino.h>

#define SERIAL_SPEED 115200

String buffer;
void process_serial3_events() {
  while (Serial3.available()) {
    char c = Serial3.read();
    Serial.print(c); // forward to serial
    buffer += c;
    if (c == ']') {
      // handle command
      buffer = "";
    }
  }
}

void setup() {
  Serial.begin(SERIAL_SPEED);

  Serial.println("Hello, world!");
}

void loop() {
  process_serial3_events();
  delay(100);
}
