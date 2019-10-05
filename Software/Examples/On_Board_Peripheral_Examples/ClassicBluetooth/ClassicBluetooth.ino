#include "BluetoothSerial.h" //Header File for Serial Bluetooth

BluetoothSerial bt; //Object for Bluetooth

int incoming;
int LED = 2;

void setup() {
  Serial.begin(115200);
  bt.begin("ESP32_LED_Control"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");
  pinMode (LED, OUTPUT);//Specify that LED pin is output
}

void loop() {
  
  if (bt.available()) //Check if we receive anything from Bluetooth
  {
    incoming = bt.read(); //Read what we recevive 
    Serial.print("Received:"); 
    Serial.println(incoming);
    if (incoming == 49)
        {
        digitalWrite(LED, HIGH);
        bt.println("LED turned ON");
        }
        
    if (incoming == 48)
        {
        digitalWrite(LED, LOW);
        bt.println("LED turned OFF");
        }     
  }
  delay(20);
}
