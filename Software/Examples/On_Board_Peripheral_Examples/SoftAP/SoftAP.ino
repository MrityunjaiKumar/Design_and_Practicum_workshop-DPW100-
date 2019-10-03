#include <WiFi.h>
 
const char *ssid = "MyESP32AP";
const char *password = "testpassword";
 
void setup() {
 
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
 
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
 
}
 
void loop() {}
