
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLQg4WADn-"
#define BLYNK_DEVICE_NAME "SIRUSERV"

#define BLYNK_FIRMWARE_VERSION        "0.3.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"

//============================================================================================
// Pin sensor
#include <DHT.h>
#define DHTTYPE DHT11     // DHT 11
#define DHTPIN 15         //D8 DHT11
DHT dht(DHTPIN, DHTTYPE);
#define pinSensor1 5      // Flame Sensor1 Digital
#define pinSensor2 4      // Flame Sensor1 Analog
#define pinSensor3 14     // Flame Sensor2 Digital
#define pinSensor4 12     // Flame Sensor2 Analog
#define pinSensor5 16     // Pir Motion1
#define pinSensor6 13     // Pir Motion2 
WiFiClient client;
BlynkTimer timer;

//============================================================================================

//============================================================================================
bool eventTrigger = false;
int bacasensor;
int BacaAnalog;
int BacaAnalog2;
int bacasensor2;
int ledsensor;
int ledsensor1;
//============================================================================================

//============================================================================================
//nodemcu to APP BLYNK
WidgetLCD lcd1(V4);
WidgetLCD lcd2(V6);
WidgetLED led1(V5);
WidgetLED led2(V7);
//============================================================================================

//============================================================================================
//ThingSpeak Config
String host = "api.thingspeak.com";
String writeAPIKey = "NVRSZB3EYKLVBZ25"; //Ganti API Key di Channel Masing Masing
String request_string;
//============================================================================================

//============================================================================================
//DHT 11 SENSOR
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");

    return;
  }
  Serial.print("Suhu : ");
  Serial.println(t);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
}
//============================================================================================

//============================================================================================
// sensor api
void flame_sensor() {
  lcd1.print(0, 0, " SENSOR 1 ");
  int bacasensor = digitalRead(pinSensor1);
  if (bacasensor == LOW )  {
    Blynk.logEvent("fs");

    Blynk.logEvent("fs", String("Warning") + bacasensor);
    lcd1.print(0, 1, "ADA API");
  } else {
    lcd1.print(0, 1, "TIDAK ADA API");
  }

  lcd2.print(0, 0, " SENSOR 2 ");
  int bacasensor2 = digitalRead(pinSensor3);
  if (bacasensor2 == LOW )  {
    Blynk.logEvent("fs1");

    Blynk.logEvent("fs1", String("Warning") + bacasensor2);
    lcd2.print(0, 1, "ADA API");
  } else {
    lcd2.print(0, 1, "TIDAK ADA API");
  }
}
//============================================================================================

//============================================================================================
//pir motion
void pir_motion() {
  int ledsensor = digitalRead(pinSensor5);
  if (ledsensor == HIGH) {
    led1.on();
  } else {
    led1.off();
  }

  int ledsensor1 = digitalRead(pinSensor6);
  if (ledsensor1 == HIGH) {
    led2.on();
  } else {
    led2.off();
  }
}
//============================================================================================


//==========================================================================================
//mengirim data ke thingspeak
void kirim_data_thingspeak() {
  BacaAnalog = analogRead(pinSensor2);
  BacaAnalog2 = analogRead(pinSensor4);
  if (client.connect(host, 80))
  {
    request_string = "/update?key=" + writeAPIKey
                     + "&field1=" + BacaAnalog
                     + "&field2=" + BacaAnalog2;

    Serial.println(String("GET ") + request_string + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");

    client.print(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();

    while (client.available() == 0)
    {
      if (millis() - timeout > 5000)
      {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("Closing connection");
  }

}
//==========================================================================================

void setup()
{
  Serial.begin(115200);
  delay(100);
  pinMode(pinSensor1, INPUT);
  pinMode(pinSensor2, INPUT);
  pinMode(pinSensor3, INPUT);
  pinMode(pinSensor4, INPUT);
  pinMode(pinSensor5, INPUT);
  pinMode(pinSensor6, INPUT);
  BlynkEdgent.begin();

  timer.setInterval(2000L, sendSensor);
  timer.setInterval(2000L, flame_sensor);
  timer.setInterval(2000L, pir_motion);
  timer.setInterval(2000L, kirim_data_thingspeak);
}

void loop() {
  delay(1000);
  lcd1.clear();
  lcd2.clear();
  BlynkEdgent.run();
  timer.run();
}
