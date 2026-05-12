#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN D5
#define TEMP_PIN A0

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(123);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address might be 0x3F on some displays

void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1); // SDA, SCL

  // LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // ADXL345
  if (!accel.begin()) {
    lcd.setCursor(0, 1);
    lcd.print("ADXL NOT Found!");
    while (1);
  }
  accel.setRange(ADXL345_RANGE_16_G);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  delay(1000);
  lcd.clear();
}

void loop() {
  // TEMP
  int analogValue = analogRead(TEMP_PIN);
  float voltage = analogValue * 3.3 / 1024.0;
  float temperatureC = voltage * 100;

  // ADXL345
  sensors_event_t event;
  accel.getEvent(&event);

  float x = abs(event.acceleration.x);
  float y = abs(event.acceleration.y);
  float z = abs(event.acceleration.z);

  bool fallDetected = (x > 12 || y > 12 || z > 12);  // simple threshold

  // LCD Output
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatureC);
  lcd.print(" C   ");

  lcd.setCursor(0, 1);
  if (fallDetected) {
    lcd.print("Fall Detected!  ");
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    lcd.print("No Fall         ");
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(500);
}