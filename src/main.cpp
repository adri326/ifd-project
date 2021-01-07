#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_RESET_PIN 5
#define LINE_LENGTH 11

#define SERIAL_SPEED 115200

#define INPUT_PIN_A 6
#define INPUT_PIN_B 7
#define MIN_DELAY 700
#define MAX_ALLOWED 50
#define MAX_PEOPLE 100

#define AVERAGE_CUSTOMERS_DELAY 1000

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

bool pin_a = false;
bool pin_b = false;

bool first_pin = false;
bool obstructed = false;
unsigned long obstruction_end = 0;
unsigned long obstruction_start = 0;

unsigned long last_oled_update;
unsigned long last_average_customers_update;

unsigned int number_of_people = 0;
unsigned long arrivals = 0;

unsigned long average_customers_sigma = 0;
unsigned long average_customers_n = 0;
double average_customers = 0.0;

void update_oled();

Adafruit_SSD1306 OLED;
void setup_oled() {
  OLED = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET_PIN);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // may need to use 0x3D?
    Serial.println("SSD1306 allocation failed");
    return;
  }

  update_oled();

  Serial.println("Set up oled display");
}

void update_oled() {
  OLED.clearDisplay();
  OLED.setTextSize(2);
  OLED.setTextWrap(false);

  last_oled_update = millis();

  // Current number of people in the shop
  OLED.setCursor(0, 0);
  OLED.setTextColor(WHITE, BLACK);
  OLED.print("P: ");
  OLED.print(number_of_people);
  OLED.print("/");
  OLED.print(MAX_ALLOWED);
  if (number_of_people >= MAX_ALLOWED) OLED.print("!");

  // Average arrival rate (arrivals / clock)
  OLED.setCursor(0, 16);
  OLED.print("L: ");
  double lambda = (double)arrivals / (double)millis() * 1000; // people per second
  OLED.print((unsigned long)(lambda * 60));
  OLED.print("/60\"");

  // Average number of people in the shop
  OLED.setCursor(0, 32);
  OLED.print("A: ");
  char buffer[6];
  sprintf(buffer, "%.1f", average_customers);
  OLED.print(buffer);
  OLED.print("/");
  OLED.print(MAX_ALLOWED);
  if (average_customers > MAX_ALLOWED) OLED.print("!");

  // Average time spent in the shop
  OLED.setCursor(0, 48);
  OLED.print("W: ");
  unsigned long average_time_spent = average_customers > 0.1 ? (long)(average_customers / lambda) : 0;
  unsigned long minutes = average_time_spent / 60;
  unsigned long seconds = average_time_spent % 60;
  OLED.print(minutes);
  OLED.print("'");
  OLED.print(seconds);
  OLED.print("\"");

  OLED.display();
}

// true means out, false means in
void register_passage(bool direction, unsigned long dt) {
  if (direction) {
    if (number_of_people > 0) number_of_people -= 1;
    Serial.println("Someone came out!");
  } else {
    if (number_of_people <= MAX_PEOPLE) number_of_people += 1;
    arrivals += 1;
    Serial.println("Someone came in!");
  }

  update_oled();
}

void update_average_customers() {
  average_customers_sigma += number_of_people;
  average_customers_n += 1;
  if (average_customers_n > 0) {
    average_customers = (float)average_customers_sigma / (float)average_customers_n;
  }
}

void setup() {
  Serial.begin(SERIAL_SPEED);
  pinMode(INPUT_PIN_A, INPUT_PULLUP);
  pinMode(INPUT_PIN_B, INPUT_PULLUP);

  Serial.println("Hello, world!");
  setup_oled();
}

void loop() {
  process_serial3_events();
  int raw_a = digitalRead(INPUT_PIN_A);
  int raw_b = digitalRead(INPUT_PIN_B);

  pin_a = raw_a > 0;
  pin_b = raw_b > 0;

  if (obstructed && pin_a && pin_b) { // obstruction end
    obstruction_end = millis();
  } else if (!obstructed && !(pin_a && pin_b)) {
    if (millis() - obstruction_end >= MIN_DELAY) {
      first_pin = pin_a;
      register_passage(first_pin, millis() - obstruction_start);
      obstruction_start = millis();
    }
  }
  obstructed = !(pin_a && pin_b);

  if (millis() - last_oled_update > 1000) {
    update_oled();
  }

  if (millis() - last_average_customers_update > AVERAGE_CUSTOMERS_DELAY) {
    update_average_customers();
  }

  delay(10);
}
