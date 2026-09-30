// ------------------------------------------------------
//   MOTOR CONTROL + DHT11 SENSOR + OLED DISPLAY
// ------------------------------------------------------

// ---------------------
// Motor Pin Definitions
// ---------------------
#define ENA 10
#define IN1 8
#define IN2 9

#define ENB 11
#define IN3 12
#define IN4 13

// ---------------------
// OLED + DHT Libraries
// ---------------------
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------------
// Motor Functions
// ---------------------
void forward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void backward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void turnLeft(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void turnRight(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// ---------------------
// Display + Sensor Update
// ---------------------
void updateDisplay() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);

  if (isnan(h) || isnan(t)) {
    display.println("Error!");
    display.display();
    return;
  }

  display.println("Temperatr:");
  display.print(t, 1);
  display.print((char)247); 
  display.println("C");

  display.setCursor(0, 32);
  display.println("Humidity:");
  display.print(h, 1);
  display.println("%");

  display.display();
}

// ---------------------
// Setup
// ---------------------
void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // OLED Init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    for (;;) ;
  }
  display.clearDisplay();
  display.setTextColor(WHITE);

  // DHT Init
  dht.begin();

  // Startup delay (car begins after 3 seconds)
  delay(3000);
}

// ------------------------------------------------------
//                  PREDEFINED SEQUENCE
// ------------------------------------------------------
//
// IMPORTANT: After each movement, we call updateDisplay()
// so the screen updates while the car moves.
// ------------------------------------------------------

void loop() {

  // 1. Move slowly forward for 3 seconds
  forward(120);
  for (int i = 0; i < 3; i++) {
    updateDisplay();
    delay(1000);
  }

  // 2. Stop 2 seconds
  stopMotors();
  for (int i = 0; i < 2; i++) {
    updateDisplay();
    delay(1000);
  }

  // 3. Turn right for 1 second
  turnRight(160);
  updateDisplay();
  delay(1000);

  // 4. Move forward 2 seconds
  forward(180);
  for (int i = 0; i < 2; i++) {
    updateDisplay();
    delay(1000);
  }

  // 5. Stop 1 second
  stopMotors();
  updateDisplay();
  delay(1000);

  // 6. Turn right 1 second
  turnRight(160);
  updateDisplay();
  delay(1000);

  // 7. Move forward 3 seconds
  forward(180);
  for (int i = 0; i < 3; i++) {
    updateDisplay();
    delay(1000);
  }

  // 8. Stop for 1.5 seconds
  stopMotors();
  updateDisplay();
  delay(1500);

  // 9. Turn right 1 second
  turnRight(160);
  updateDisplay();
  delay(1000);

  // 10. Move forward 3 seconds
  forward(180);
  for (int i = 0; i < 3; i++) {
    updateDisplay();
    delay(1000);
  }

  // 11. FINAL STOP FOREVER
  stopMotors();

  // Continue showing temperature/humidity forever
  while (true) {
    updateDisplay();
    delay(1500);
  }
}


