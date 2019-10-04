#include <Arduino.h>
#define BUILTIN_LED 2
void setup() {
  // put your setup code here, to run once:
pinMode(BUILTIN_LED,OUTPUT);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

digitalWrite(BUILTIN_LED,HIGH);
Serial.println("LED state is ON");
delay(1000);
digitalWrite(BUILTIN_LED,LOW);
Serial.println("LED state is OFF");
delay(1000);
}