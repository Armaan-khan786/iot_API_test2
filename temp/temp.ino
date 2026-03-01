/************ BLYNK DETAILS ************/
#define BLYNK_TEMPLATE_ID "TMPL3YI9LIFNS"
#define BLYNK_TEMPLATE_NAME "temperature monitor"
#define BLYNK_AUTH_TOKEN "AV6Yyn81W7kNA723S4Y0asvrV2eUu6nC"

/************ LIBRARIES ************/
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/************ WIFI DETAILS ************/
char ssid[] = "@armaan_khxn__";
char pass[] = "786786786j";

/************ LCD + SENSOR ************/
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define LM35 34

BlynkTimer timer;

/************ SEND TEMPERATURE ************/
void sendTemperature()
{
  int sensorValue = analogRead(LM35);

  float voltage = sensorValue * (3.3 / 4095.0);
  float temperature = voltage * 100.0;

  Blynk.virtualWrite(V0, temperature);

  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(0,1);
  lcd.print(temperature);
  lcd.print(" C   ");

  Serial.print("TEMP:");
  Serial.println(temperature);
}

/************ SEND WIFI RSSI ************/
void sendWiFiStrength()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    int rssi = WiFi.RSSI();

    Blynk.virtualWrite(V1, rssi);

    Serial.print("RSSI:");
    Serial.println(rssi);
  }
}

/************ CONNECTION STATUS ************/
void checkConnection()
{
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WIFI_CONNECTED");
  else
    Serial.println("WIFI_DISCONNECTED");

  if (Blynk.connected())
    Serial.println("BLYNK_CONNECTED");
  else
    Serial.println("BLYNK_DISCONNECTED");
}

void setup()
{
  Serial.begin(115200);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Connecting...");

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("WAITING_WIFI");
  }

  Serial.println("WIFI_CONNECTED");

  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();

  if (Blynk.connected())
    Serial.println("BLYNK_CONNECTED");

  lcd.clear();
  lcd.print("Connected!");

  timer.setInterval(1000L, sendTemperature);
  timer.setInterval(3000L, sendWiFiStrength);
  timer.setInterval(5000L, checkConnection);
}

void loop()
{
  Blynk.run();
  timer.run();
}