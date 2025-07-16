#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define DHTPIN 3
#define DHTTYPE DHT11 // Change to DHT22 if you're using that

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  dht.begin();

  // LoRa init
  LoRa.setPins(10, 9, 2); // NSS, RESET, DIO0

  if (!LoRa.begin(868300000)) {
    Serial.println("LoRa init failed. Check your shield.");
    while (1);
  }

  Serial.println("LoRa Sender (Node A) with DHT Sensor Ready.");
}

void loop() {
  float temperature = dht.readTemperature(); // °C
  float humidity = dht.readHumidity();       // %

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  String message = "Temp: " + String(temperature, 1) + "°C | Hum: " + String(humidity, 1) + "%";

  Serial.print("Sending: ");
  Serial.println(message);

  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  delay(5000);
}
