#include "ThingSpeak.h"
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1050)

const char* ssid = "ssid";   // your network SSID (name) 
const char* pass = "password";   // your network password
WiFiClient  client;

//ThingSpeak Channel
unsigned long myChannelNumber = channel_id;
const char * myWriteAPIKey = "WRITE-API-KEY";

//BME280
Adafruit_BME280 bme;
float temperature, humidity, pressure, altitude;



void setup() {
  Serial.begin(115200);  //Initialize serial
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
     Serial.print(".");
     delay(5000);     
  } 
  Serial.println("\nConnected.");
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  bme.begin(0x76);
}

void loop() {
  temperature = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  humidity = bme.readHumidity();
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);  
  // set the fields with the values
  Serial.print("Temp (deg celsius):");
  Serial.println(temperature);
  ThingSpeak.setField(1, temperature);
  Serial.print("Pressure (hPa):");
  Serial.println(pressure);
  ThingSpeak.setField(2, pressure);
  Serial.print("Humidity (%):");
  Serial.println(humidity);
  ThingSpeak.setField(3, humidity);
  Serial.print("Altitude (m):");
  Serial.println(altitude);
  ThingSpeak.setField(4, altitude);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(20000); // Wait 20 seconds to update the channel again
}
