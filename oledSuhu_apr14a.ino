#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define DHTPIN 2
#define DHTTYPE DHT11
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

#include <LiquidCrystal.h>
LiquidCrystal lcd(22,23,5,18,19,21);

void setup() {
  // put your setup code here, to run once:
  dht.begin();

  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Temperature");

  lcd.setCursor(0,1);
  lcd.print("Sensor");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
 
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.clear();
  lcd.print("Temperature");
  lcd.setCursor(0,1);
  lcd.print(t);
  lcd.print("C");
  delay(2000);
  lcd.clear();
  lcd.print("Humidity");
  lcd.setCursor(0,1);
  lcd.print(h);
  lcd.print("%");
  delay(2000);
}
