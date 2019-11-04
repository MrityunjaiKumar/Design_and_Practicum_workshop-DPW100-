/* 
  Uses interrup method check the new incoming messages, and print via serial
  in 115200 baud rate.
*/
#include <SPI.h>
#include <LoRa.h>

#define SCK     18    // GPIO5  -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    23   // GPIO27 -- SX127x's MOSI
#define SS      5   // GPIO18 -- SX127x's CS
#define RST     27   // GPIO14 -- SX127x's RESET
#define DI0     16   // GPIO26 -- SX127x's IRQ(Interrupt Request)

#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6
#define PABOOST false


void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND,PABOOST)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  // do nothing
}

void onReceive(int packetSize)
{
  // received a packet
  Serial.print("Received packet '");
  // read packet
  for (int i = 0; i < packetSize; i++)
  {
    Serial.print((char)LoRa.read());
  }
  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
}
