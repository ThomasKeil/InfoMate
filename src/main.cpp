#include <SPI.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include "config.h"

#include "wifi.xbm"
#include "alert.xbm"


String infoUrl = INFOURL;

unsigned int nextUpdate = 0;

unsigned short int paintsWithoutRefresh = 0;

boolean bme280_present = true;

U8G2_IL3820_V2_296X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/D5, /* data=*/D7, /* cs=*/D8, /* dc=*/D2, /* reset=*/D3);

Adafruit_BME280 bme; // use I2C interface
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

unsigned char image_bits[DISPLAY_BYTES+1];

HTTPClient http;

void connectWifi() 
{
  const uint8_t *font = u8g2_font_helvR12_tf;

  u8g2.clearDisplay();
  u8g2.firstPage();
  u8g2.setFont(font);   

  do
  {
    u8g2.drawXBMP(0, 32, wifi_width, wifi_height, wifi_bits);
        u8g2.drawStr(wifi_width + 15, 64, "Verbinde mit WLAN");

  } while (u8g2.nextPage());


  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  u8g2.clearDisplay();
}

u8g2_uint_t get_x_for_centered_text(const char *text, const u8g2_uint_t area_width, const uint8_t *font) {
  u8g2_uint_t text_width;
  u8g2.setFont(font);
  text_width = u8g2.getStrWidth(text);
  return (area_width / 2) - (text_width / 2);
}

void alert(const char *text)
{
  Serial.print("Fehler: ");
  Serial.println(text);

  const uint8_t *font = u8g2_font_helvR12_tf;

  u8g2.clearDisplay();

  u8g2.firstPage();
  u8g2.setFont(font);
  do
  {
    u8g2.drawXBMP(0, 32, alert_width, alert_height, alert_bits);
    u8g2.drawStr(alert_width + 5, 58, text);
  } while (u8g2.nextPage());
}

void setup(void)
{
  Serial.begin(115200);
  u8g2.begin();

  Wire.begin(D6 /* SDA */, D1 /* SCL */);

  if (!bme.begin(0x76)) {
    Serial.println(F("Could not find a valid BME280 sensor."));
    bme280_present = false;
  }
  
  connectWifi();
}

void loop(void)
{
  unsigned int now = millis();

  if (now > nextUpdate)
  {
    nextUpdate = now + ERROR_WAIT; // Default wait is 5 minutes, e.g. in case of an error
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("WiFi Disconnected");
      connectWifi();
    }

    String serverPath = infoUrl + "?unitname=" + UNITNAME;

    if (bme280_present) {
      sensors_event_t temp_event, humidity_event;
      bme_temp->getEvent(&temp_event);
      bme_humidity->getEvent(&humidity_event);
      serverPath = serverPath + "&temperature=" + temp_event.temperature + "&humidity=" + humidity_event.relative_humidity;
    }

    int payloadLength = 0;

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET();
    if (httpResponseCode == 200)
    {

      String payload = http.getString();

      if (payload.startsWith("XBM: ")) {
        payload.remove(0, 5);
        payloadLength = payload.length();

        if (payloadLength < DISPLAY_BYTES) {
          Serial.println("Payload too short, trying again...");
          nextUpdate = now + TRY_AGAIN_WAIT; // Try again in 2 Seconds;
        } else {
          for (int i=0; i<DISPLAY_BYTES; i++) {
            image_bits[i] = payload.charAt(i);
          }

          if (paintsWithoutRefresh > 10) {
            u8g2.clearDisplay();
            paintsWithoutRefresh = 0;
          }
          paintsWithoutRefresh++;
          
          u8g2.firstPage();
          do
          {
            u8g2.drawXBM(0, 0, u8g2.getDisplayWidth(), u8g2.getDisplayHeight(), image_bits);
          } while (u8g2.nextPage());

          nextUpdate = now + SUCCESS_WAIT;
        }
      } else if (payload.startsWith("Error: ")) { // Error?
        payload.remove(0, 7);
        alert(payload.c_str());
      } else {
        alert("Unbekannter Fehler");
      }
    }
    else // Status != 200
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      alert("Serverfehler");
    }
    http.end();
  }
}
