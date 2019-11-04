#include "common.h"
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude;

#define SEALEVELPRESSURE_HPA (1050)

void bme_setup()
{
	// put your setup code here, to run once:
	delay(100);
	bme.begin(0x76);
}

void bme_loop()
{
	// put your main code here, to run repeatedly:
	temperature = bme.readTemperature();
	humidity = bme.readHumidity();
	pressure = bme.readPressure() / 100.0F;
	altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

	Serial.print("Temperature = ");
	Serial.print(temperature);
	Serial.println(" *C");

	// Convert temperature to Fahrenheit
	/*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/

	Serial.print("Pressure = ");
	Serial.print(pressure);
	Serial.println(" hPa");

	Serial.print("Approx. Altitude = ");
	Serial.print(altitude);
	Serial.println(" m");

	Serial.print("Humidity = ");
	Serial.print(humidity);
	Serial.println(" %");

	Serial.println();
	delay(1000);
}

String get_BME_payload()
{

	String message= "";

	temperature = bme.readTemperature();
	humidity = bme.readHumidity();
	pressure = bme.readPressure() / 100.0F;
	altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

	// 	Serial.print("Temperature = ");
	// 	Serial.print(temperature);
	// 	Serial.println(" *C");

	// 	// Convert temperature to Fahrenheit
	// 	/*Serial.print("Temperature = ");
	//   Serial.print(1.8 * bme.readTemperature() + 32);
	//   Serial.println(" *F");*/

	// 	Serial.print("Pressure = ");
	// 	Serial.print(pressure);
	// 	Serial.println(" hPa");

	// 	Serial.print("Approx. Altitude = ");
	// 	Serial.print(altitude);
	// 	Serial.println(" m");

	// 	Serial.print("Humidity = ");
	// 	Serial.print(humidity);
	// 	Serial.println(" %");

	// 	Serial.println();

	//Encoder function
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject &root = jsonBuffer.createObject();
	root["dev_id"] = "demo";
	JsonObject& iotasync_data = root.createNestedObject("iotasync_data");
	iotasync_data["temp"] = String(temperature);
	iotasync_data["alt"] = String(altitude);
	iotasync_data["humidity"] = String(humidity);
	iotasync_data["pressure"] = String(pressure);



	char JSONmessageBuffer[200];

	root.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
	message += String(JSONmessageBuffer);

	// StaticJsonBuffer<200> jsonBuffer2;
	// JsonObject &root_out = jsonBuffer2.createObject();


	// char JSONmessageBuffer2[200];

	// root_out.printTo(JSONmessageBuffer2, sizeof(JSONmessageBuffer2));
	// message_outer += String(JSONmessageBuffer2);
	Serial.println(message);
	return message;
}