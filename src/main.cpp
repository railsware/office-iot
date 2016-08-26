#include "Arduino.h"
#include <Wire.h>
#include <stdio.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "cozir.h"
#include "ThingSpeak.h"

const int RX_PIN  = D5;
const int TX_PIN  = D6;
const int RST_PIN = D0;
const int INF_PIN = D8;

const char* ssid     = WIFI_SSID;
const char* password = WIFI_SECRET;

unsigned long myChannelNumber = TS_UID;
const char* myWriteAPIKey     = TS_TOKEN;

SoftwareSerial nss(RX_PIN, TX_PIN, false, 256);
WiFiClient client;
COZIR czr(nss);

char output[64], buf[256];

void blink(int times = 1) {
  for(int i = 0; i < times; i++) {
    digitalWrite(INF_PIN, HIGH);
    delay(300);
    digitalWrite(INF_PIN, LOW);
    delay(100);
  }
}

void setup() {
  pinMode(RST_PIN, INPUT);
  pinMode(INF_PIN, OUTPUT);
  pinMode(TX_PIN, OUTPUT);
  pinMode(RX_PIN, INPUT);

  blink();

  Serial.begin(9600);
  nss.begin(9600);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    blink();
  }

  czr.SetOperatingMode(CZR_POLLING);

  ThingSpeak.begin(client);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if (digitalRead(RST_PIN) == HIGH) {
    czr.CalibrateFreshAir();
    blink(6);
    delay(2000);
  }

  float t = czr.Celsius();
  float h = czr.Humidity();
  int c = czr.CO2();
  int digi = czr.GetDigiFilter();

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  ThingSpeak.setField(3, c);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  Serial.print("Celcius : ");Serial.println(t);
  Serial.print("Humidity : ");Serial.println(h);
  Serial.print("CO2 : ");Serial.println(c);
  Serial.print("Digital Filter : ");Serial.println(digi);
  Serial.println("");

  blink(2);

  delay(20000);

  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }

}
