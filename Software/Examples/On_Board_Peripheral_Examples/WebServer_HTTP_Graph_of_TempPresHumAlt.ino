//Edited by Shivam Kumar (eMail: 2016ucs0022@iitjammu.ac.in) from the original "WebServer_HTTP_graph"
/*
 * This code serves the realtime graph and values of 
 * temperature, pressure, humidity and altitude through 
 * a web server.
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


Adafruit_BME280 bme;
float currTemp, currPres, currHum, currAlt;
float avgTemp, avgPres, avgHum, avgAlt;
int numObs=0; //number of observations till now
#define SEALEVELPRESSURE_HPA (1013.25)

const int valCount = 9;
float neg = -300; //a big negative number
float posi = 1000000.0; //a big positive number

float tempr[valCount];
float prssr[valCount];
float humid[valCount];
float altit[valCount];

int tempPos = -1;
int presPos = -1;
int humPos = -1;
int altPos = -1;

float minTemp=posi, minPres=posi, minHum=posi, minAlt=posi;
float maxTemp=neg, maxPres=neg, maxHum=neg, maxAlt=neg;


const char *ssid = "YOUR_SSID";
const char *password = "THE_PASSWORD";
WebServer server(80);


const int led = 2;



void handleRoot() {
	digitalWrite(led, 1);

	char temp[2500];
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	currTemp = bme.readTemperature();
	currHum = bme.readHumidity();
	currPres = bme.readPressure() / 100.0F;
	currAlt = bme.readAltitude(SEALEVELPRESSURE_HPA);

	avgTemp = float(((avgTemp*numObs)+currTemp)/float(numObs+1));
	avgPres = float(((avgPres*numObs)+currPres)/float(numObs+1));
	avgHum = float(((avgHum*numObs)+currHum)/float(numObs+1));
	avgAlt = float(((avgAlt*numObs)+currAlt)/float(numObs+1));
	numObs++;

	minTemp = (currTemp<minTemp)?(currTemp):(minTemp);
	minPres = (currPres<minPres)?(currPres):(minPres);
	minHum = (currHum<minHum)?(currHum):(minHum);
	minAlt = (currAlt<minAlt)?(currAlt):(minAlt);

	maxTemp = (currTemp>maxTemp)?(currTemp):(maxTemp);
	maxPres = (currPres>maxPres)?(currPres):(maxPres);
	maxHum = (currHum>maxHum)?(currHum):(maxHum);
	maxAlt = (currAlt>maxAlt)?(currAlt):(maxAlt);

	String tempSvg = drawTempGraph();
	String presSvg = drawPresGraph();
	String humSvg = drawHumGraph();
	String altSvg = drawAltGraph();

	String finalHtml = "";


	snprintf(temp, 2500, 
"<html>\
	<head>\
		<meta http-equiv='refresh' content='5'/>\
		<title>IoT-A-Sync Sensors Output</title>\
		<style>\
			body { background-color: #c7fff7; font-family: Arial, Helvetica, Sans-Serif; Color: #070f27; }\
			h1 {text-align:center;}\
			p {text-align:center;}\
			.divClass {margin: 10px; padding: 15px;}\
			.img {border-width: 1px; border-style: solid;}\
			.innerDiv {display: inline-block; max-height: 250px; max-width: 37.5%%; margin: 10px; padding: 10px;}\
			.clearfix {clear: both;}\
		</style>\
	</head>\
	<body>\
		<h1>Real-Time Remote Monitoring</h1>\
		<p>\
			<b>Uptime:</b> %02d Hrs %02d Mins %02d Secs\
			<br>\
			<b>Location:</b> IIT Jammu\
		</p>\
		\
		<div class=\"divClass\">\
			<h2> Temperature </h2>\
			", hr, min%60, sec%60);

	finalHtml += temp;
	finalHtml += tempSvg;

	snprintf(temp, 2500, 
"<div class=\"innerDiv\">\
				<b>Current Temperature:</b> %.2f<sup>o</sup> C\
				<br>\
				<b>Average Temperature for uptime:</b> %.2f<sup>o</sup> C\
				<br>\
				<b>Maximum Temperature for uptime:</b> %.2f<sup>o</sup> C\
				<br>\
				<b>Minimum Temperature for uptime:</b> %.2f<sup>o</sup> C\
				<br>\
				<br>\
				<u>Limits</u>: -45.0<sup>o</sup> C to 105.0<sup>o</sup> C\
			</div>\
		</div>\
		<div class=\"clearfix\"></div>\
		<div class=\"divClass\">\
			<h2> Pressure </h2>\
			", currTemp, avgTemp, maxTemp, minTemp);

	finalHtml += temp;
	finalHtml += presSvg;

	snprintf(temp, 2500, 
"<div class=\"innerDiv\">\
				<b>Current Pressure:</b> %.2f hPa\
				<br>\
				<b>Average Pressure for uptime:</b> %.2f hPa\
				<br>\
				<b>Maximum Pressure for uptime:</b> %.2f hPa\
				<br>\
				<b>Minimum Pressure for uptime:</b> %.2f hPa\
				<br>\
				<br>\
				<u>Limits</u>: 300 hPa to 1100 hPa\
			</div>\
		</div>\
		<div class=\"clearfix\"></div>\
		<div class=\"divClass\">\
			<h2> Humidity </h2>\
			", currPres, avgPres, maxPres, minPres);

	finalHtml += temp;
	finalHtml += humSvg;

	snprintf(temp, 2500, 
"<div class=\"innerDiv\">\
				<b>Current Humidity:</b> %.2f%%\
				<br>\
				<b>Average Humidity for uptime:</b> %.2f%%\
				<br>\
				<b>Maximum Humidity for uptime:</b> %.2f%%\
				<br>\
				<b>Minimum Humidity for uptime:</b> %.2f%%\
				<br>\
				<br>\
				<u>Limits</u>: 0%% to 100%%\
			</div>\
		</div>\
		<div class=\"clearfix\"></div>\
		<div class=\"divClass\">\
			<h2> Altitude </h2>\
			", currHum, avgHum, maxHum, minHum);

	finalHtml += temp;
	finalHtml += altSvg;

	snprintf(temp, 2500, 
"<div class=\"innerDiv\">\
				<b>Current Altitude:</b> %.2f m\
				<br>\
				<b>Average Altitude for uptime:</b> %.2f m\
				<br>\
				<b>Maximum Altitude for uptime:</b> %.2f m\
				<br>\
				<b>Minimum Altitude for uptime:</b> %.2f m\
				<br>\
				<br>\
				<u>Limits</u>: 0 m to 2000 m\
			</div>\
		</div>\
		<div class=\"clearfix\"></div>\
		\
	</body>\
</html>", currAlt, avgAlt, maxAlt, minAlt);

	finalHtml += temp;

	server.send(200, "text/html", finalHtml);

	digitalWrite(led, 0);
}





String drawTempGraph() {
	if(tempPos<valCount-1){
		tempr[tempPos+1] = currTemp;

		String out = "";
		out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"700\" height=\"250\" class=\"img\">\n";
		out += "<rect width=\"700\" height=\"250\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
		out += "<g stroke=\"black\">\n";
		out += "<line x1=\"0\" y1=\"175\" x2=\"700\" y2=\"175\" stroke-width=\"3\"/>";

		char temp[100];

		int x = 30;
		for(int i=0; i<=tempPos; i++){
			sprintf(temp, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke-width=\"1\" />\n", x, (525 - 5*tempr[i])/3.0, x + 80, (525 - 5*tempr[i + 1])/3.0);
			out += temp;
			sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (525 - 5*tempr[i])/3.0, 2);
			out += temp;

			x += 80;
		}

		sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (525 - 5*tempr[tempPos+1])/3.0, 2);
		out += temp;
		out += "</g>\n</svg>\n";

		tempPos++;

		// server.send(200, "image/svg+xml", out);
		return out;
	}
	else{
		for(int i=0; i<valCount-1; i++){
			tempr[i] = tempr[i+1];
		}
		tempr[valCount-1] = currTemp;

		String out = "";
		out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"700\" height=\"250\" class=\"img\">\n";
		out += "<rect width=\"700\" height=\"250\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
		out += "<g stroke=\"black\">\n";
		out += "<line x1=\"0\" y1=\"175\" x2=\"700\" y2=\"175\" stroke-width=\"3\"/>";

		char temp[100];
		for (int x = 30; x < 670; x += 80) {
			sprintf(temp, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke-width=\"1\" />\n", x, (525 - 5*tempr[(x-30)/80])/3.0, x + 80, (525 - 5*tempr[(x-30)/80 + 1])/3.0);
			out += temp;
			sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (525 - 5*tempr[(x-30)/80])/3.0, 2);
			out += temp;
		}
		sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", 670, (525 - 5*tempr[valCount-1])/3.0, 2);
		out += temp;

		out += "</g>\n</svg>\n";
		// server.send(200, "image/svg+xml", out);
		return out;
	}
}

String drawPresGraph(){
	if(presPos<valCount-1){
		prssr[presPos+1] = currPres;

		String out = "";
		out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"700\" height=\"250\" class=\"img\">\n";
		out += "<rect width=\"700\" height=\"250\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
		out += "<g stroke=\"black\">\n";

		char temp[100];

		int x = 30;
		for(int i=0; i<=presPos; i++){
			sprintf(temp, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke-width=\"1\" />\n", x, (5500 - 5*prssr[i])/16.0, x + 80, (5500 - 5*prssr[i+1])/16.0);
			out += temp;
			sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (5500 - 5*prssr[i])/16.0, 2);
			out += temp;

			x += 80;
		}

		sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (5500 - 5*prssr[presPos+1])/16.0, 2);
		out += temp;
		out += "</g>\n</svg>\n";
		
		presPos++;

		// server.send(200, "image/svg+xml", out);
		return out;
	}
	else{
		for(int i=0; i<valCount-1; i++){
			prssr[i] = prssr[i+1];
		}
		prssr[valCount-1] = currPres;

		String out = "";
		out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"700\" height=\"250\" class=\"img\">\n";
		out += "<rect width=\"700\" height=\"250\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
		out += "<g stroke=\"black\">\n";

		char temp[100];
		for (int x = 30; x < 670; x += 80) {
			sprintf(temp, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke-width=\"1\" />\n", x, (5500 - 5*prssr[(x-30)/80])/16.0, x + 80, (5500 - 5*prssr[(x-30)/80 + 1])/16.0);
			out += temp;
			sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (5500 - 5*prssr[(x-30)/80])/16.0, 2);
			out += temp;
		}
		sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", 670, (5500 - 5*prssr[valCount-1])/16.0, 2);
		out += temp;

		out += "</g>\n</svg>\n";
		// server.send(200, "image/svg+xml", out);
		return out;
	}
}

String drawHumGraph(){
	if(humPos<valCount-1){
		humid[humPos+1] = currHum;

		String out = "";
		out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"700\" height=\"250\" class=\"img\">\n";
		out += "<rect width=\"700\" height=\"250\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
		out += "<g stroke=\"black\">\n";

		char temp[100];

		int x = 30;
		for(int i=0; i<=humPos; i++){
			sprintf(temp, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke-width=\"1\" />\n", x, (500 - 5*humid[i])/2.0, x + 80, (500 - 5*humid[i+1])/2.0);
			out += temp;
			sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (500 - 5*humid[i])/2.0, 2);
			out += temp;

			x += 80;
		}

		sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (500 - 5*humid[humPos+1])/2.0, 2);
		out += temp;
		out += "</g>\n</svg>\n";
		
		humPos++;

		//server.send(200, "image/svg+xml", out);

		return out;
	}
	else{
		for(int i=0; i<valCount-1; i++){
			humid[i] = humid[i+1];
		}
		humid[valCount-1] = currHum;

		String out = "";
		out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"700\" height=\"250\" class=\"img\">\n";
		out += "<rect width=\"700\" height=\"250\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
		out += "<g stroke=\"black\">\n";

		char temp[100];
		for (int x = 30; x < 670; x += 80) {
			sprintf(temp, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke-width=\"1\" />\n", x, (500 - 5*humid[(x-30)/80])/2.0, x + 80, (500 - 5*humid[(x-30)/80 + 1])/2.0);
			out += temp;
			sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (500 - 5*humid[(x-30)/80])/2.0, 2);
			out += temp;
		}
		sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", 670, (500 - 5*humid[valCount-1])/2.0, 2);
		out += temp;

		out += "</g>\n</svg>\n";
		// server.send(200, "image/svg+xml", out);
		return out;
	}
}

String drawAltGraph(){
	if(altPos<valCount-1){
		altit[altPos+1] = currAlt;

		String out = "";
		out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"700\" height=\"250\" class=\"img\">\n";
		out += "<rect width=\"700\" height=\"250\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
		out += "<g stroke=\"black\">\n";

		char temp[100];

		int x = 30;
		for(int i=0; i<=altPos; i++){
			sprintf(temp, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke-width=\"1\" />\n", x, (2000 - altit[i])/8.0, x + 80, (2000 - altit[i+1])/8.0);
			out += temp;
			sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (2000 - altit[i])/8.0, 2);
			out += temp;

			x += 80;
		}

		sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (2000 - altit[altPos+1])/8.0, 2);
		out += temp;
		out += "</g>\n</svg>\n";
		
		altPos++;

		// server.send(200, "image/svg+xml", out);
		return out;
	}
	else{
		for(int i=0; i<valCount-1; i++){
			altit[i] = altit[i+1];
		}
		altit[valCount-1] = currAlt;

		String out = "";
		out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"700\" height=\"250\" class=\"img\">\n";
		out += "<rect width=\"700\" height=\"250\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
		out += "<g stroke=\"black\">\n";

		char temp[100];
		for (int x = 30; x < 670; x += 80) {
			sprintf(temp, "<line x1=\"%d\" y1=\"%f\" x2=\"%d\" y2=\"%f\" stroke-width=\"1\" />\n", x, (2000 - altit[(x-30)/80])/8.0, x + 80, (2000 - altit[(x-30)/80 + 1])/8.0);
			out += temp;
			sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", x, (2000 - altit[(x-30)/80])/8.0, 2);
			out += temp;
		}
		sprintf(temp, "<circle cx=\"%d\" cy=\"%f\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />", 670, (2000 - altit[valCount-1])/8.0, 2);
		out += temp;

		out += "</g>\n</svg>\n";
		// server.send(200, "image/svg+xml", out);
		return out;
	}
}





void handleNotFound() {
	digitalWrite(led, 1);

	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}

	server.send(404, "text/plain", message);

	digitalWrite(led, 0);
}







void setup(void) {

	for(int i=0; i<valCount; i++){
		tempr[i] = neg;
		prssr[i] = neg;
		humid[i] = neg;
		altit[i] = neg;
	}
	avgTemp = tempr[0];
	avgPres = prssr[0];
	avgHum = humid[0];
	avgAlt = altit[0];

	bme.begin(0x76);

	pinMode(led, OUTPUT);
	digitalWrite(led, 0);

	Serial.begin(115200);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	if (MDNS.begin("esp32")) {
		Serial.println("MDNS responder started");
	}

	server.on("/", handleRoot);
	/*
	server.on("/temperature.svg", drawTempGraph);
	server.on("/pressure.svg", drawPresGraph);
	server.on("/humidity.svg", drawHumGraph);
	server.on("/altitude.svg", drawAltGraph);
	*/
	/*server.on("/inline", []() {
		server.send(200, "text/plain", "this works as well");
	});*/
	server.onNotFound(handleNotFound);
	server.begin();

	Serial.println("HTTP server started");

}



void loop(void) {
	server.handleClient();
}