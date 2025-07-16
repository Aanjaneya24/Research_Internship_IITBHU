#include <SPI.h>
#include <LoRa.h>

int packetCount = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // LoRa Shield setup (NSS, RESET, DIO0)
  LoRa.setPins(10, 9, 2);

  if (!LoRa.begin(868000000)) {
    Serial.println("LoRa init failed. Check your Dragino shield.");
    while (1);
  }

  Serial.println("LoRa Receiver (Node C) Ready. Listening on 868.000 MHz...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize > 0) {
    String received = "";

    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    int rssi = LoRa.packetRssi();
    packetCount++;

    Serial.print("Packet #");
    Serial.print(packetCount);
    Serial.print(" | RSSI: ");
    Serial.print(rssi);
    Serial.print(" | Message: ");
    Serial.println(received);
  }
}
