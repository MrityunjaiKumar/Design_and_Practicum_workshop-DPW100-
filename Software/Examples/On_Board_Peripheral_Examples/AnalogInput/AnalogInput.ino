
int sensor = 34;      // select the input pin 
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  Serial.begin(115200);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensor);
  //Printing the value from the sensor
  Serial.println(sensorValue);
  delay(1000);
}
