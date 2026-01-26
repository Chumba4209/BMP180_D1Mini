#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>

//WiFi credentials
const char* ssid = "Lukrasta";
const char* password = "Cycy12345";

// Flask server
const char* serverURL = "http://192.168.100.4:5000/api/bmp180";

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\nStarting...");
  Wire.begin(D2, D1); // I2C for D1 Mini (SDA=D2, SCL=D1)

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

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
  Serial.println("Setup complete!");
}

void loop() {
  float temp = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude();

  // Serial output
  Serial.printf("Temp: %.2f C | Pressure: %.0f1 Pa | Altitude: %.2f m\n", 
                temp, pressure, altitude);

  // OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.println("BMP180 Sensor");
  
  display.setCursor(0, 20);
  display.printf("Temp: %.2f C", temp);
  
  display.setCursor(0, 35);
  display.printf("Press: %.0f Pa", pressure);
  
  display.setCursor(0, 50);
  display.printf("Altgit: %.2f m", altitude);
  
  display.display();

  // Send to server
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverURL);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"temperature\":" + String(temp, 2) +
                      ",\"pressure\":" + String(pressure, 2) +
                      ",\"altitude\":" + String(altitude, 2) + "}";

    int httpResponseCode = http.POST(jsonData);
    Serial.print("Server response: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.println("POST failed");
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected!");
  }

  delay(5000);
}
