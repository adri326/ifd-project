#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_RESET_PIN 5
#define LINE_LENGTH 11

#define SERIAL_SPEED 115200

#define INPUT_PIN_A A0
#define INPUT_PIN_B A1

#define THRESHOLD 128

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

int raw_a = 0;
int raw_b = 0;

bool pin_a = false;
bool pin_b = false;

// TODO: softwrap

Adafruit_SSD1306 OLED;
void setup_oled() {
  OLED = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET_PIN);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // may need to use 0x3D?
    Serial.println("SSD1306 allocation failed");
    return;
  }

  OLED.clearDisplay();
  OLED.setTextSize(2);
  OLED.setTextWrap(false);
  OLED.setCursor(0, 0);
  OLED.setTextColor(WHITE, BLACK);
  OLED.print("Fuyez,");
  OLED.setCursor(0, 20);
  OLED.print("humains!");
  OLED.setCursor(0, 50);
  OLED.print("   ~WOR~   ");
  OLED.display();

  Serial.println("Set up oled display");
}

void setup() {
  Serial.begin(SERIAL_SPEED);
  raw_a = analogRead(INPUT_PIN_A);
  raw_b = analogRead(INPUT_PIN_B);

  Serial.println("Hello, world!");
  setup_oled();
}

void loop() {
  process_serial3_events();
  raw_a = analogRead(INPUT_PIN_A);
  raw_b = analogRead(INPUT_PIN_B);

  delay(100);
}
