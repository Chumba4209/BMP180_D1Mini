#include <Arduino.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>

// -------- WiFi --------
const char* ssid = "Robotics Institute of Kenya";
const char* password = "RObOT1C&#357";

// -------- Flask Server --------
String serverURL = "http://192.168.1.56:5000/api/bmp180";

// -------- OLED --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -------- BMP180 --------
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);

  // I2C init for D1 Mini
  Wire.begin(D2, D1); // SDA, SCL

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED init failed");
    while (1);
  }
  display.clearDisplay();
  display.display();

  // BMP180
  if (!bmp.begin()) {
    Serial.println("BMP180 not found!");
    while (1);
  }
}

void loop() {

  float temp = bmp.readTemperature();      // °C
  float pressure = bmp.readPressure();     // Pa
  float altitude = bmp.readAltitude();     // meters

  // -------- Serial --------
  Serial.printf(
    "Temp: %.2f C | Pressure: %.2f Pa | Altitude: %.2f m\n",
    temp, pressure, altitude
  );

  // -------- OLED --------
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("BMP180");

  display.setCursor(0, 20);
  display.printf("Temp: %.2f C", temp);

  display.setCursor(0, 35);
  display.printf("Press: %.0f Pa", pressure);

  display.setCursor(0, 50);
  display.printf("Alt: %.2f m", altitude);

  display.display();

  // -------- Send to MongoDB --------
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;

    http.begin(client, serverURL);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{"
                      "\"temperature\":" + String(temp) +
                      ",\"pressure\":" + String(pressure) +
                      ",\"altitude\":" + String(altitude) +
                      "}";

    int httpResponseCode = http.POST(jsonData);

    Serial.print("Server response: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0) {
      Serial.println(http.getString());
    }

    http.end();
  }

  delay(3000);
}
