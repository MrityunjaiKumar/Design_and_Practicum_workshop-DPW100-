#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Wi-Fi Scanning
int n=0;

void setup() {
  Serial.begin(112500);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  Serial.println("scan start");
  n = WiFi.scanNetworks();
  display.setCursor(0,0);
  display.println("scan done");
  display.display();
  delay(1500);
  display.clearDisplay();
  display.setCursor(0,0);
    if (n == 0) {
        display.println("no networks found");
    } else if(n>0){
        display.println("Networks found.");
    }
  display.display();
  delay(1500);
  display.clearDisplay();
}

  void loop()
{
    if(n>0){
         for (int i = 0; i <= n; ++i) {
            // Print SSID for each network found
            
            display.setCursor(0, i*10);
            display.print(WiFi.SSID(i));
            display.display();
        }
    }
    delay(10000);
    //display.clearDisplay();
}
