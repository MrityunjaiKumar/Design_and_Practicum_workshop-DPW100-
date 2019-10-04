#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1050)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_BME280 bme;
float temperature, humidity, pressure, altitude;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  delay(100);
  bme.begin(0x76);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  display.print("Temp = ");
  display.print(bme.readTemperature());
  display.println(" *C");

  display.print("Pressure = ");
  display.print(bme.readPressure() / 100.0F);
  display.println(" hPa");

  display.print("Alt = ");
  display.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  display.println(" m");

  display.print("Humidity = ");
  display.print(bme.readHumidity());
  display.println(" %");

  display.println();

  display.display();
  delay(1000);
}
