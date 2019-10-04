#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* ssid = "<SSID>";
const char* password =  "<PASSWORD>";

// Defining NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

void connect_wifi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
}

void setup()
{
  // Initialize Serial Monitor
  Serial.begin(115200);
  connect_wifi();  
}
 
void loop()
{
  timeClient.setTimeOffset(5.5*3600);
  while(!timeClient.update()) {
    timeClient.forceUpdate();
    Serial.println("NTP update");
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);
  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  delay(1000);
}
