#include "DHT.h"
 
#define DHTPIN1 2     // what pin we're connected to
#define DHTPIN2 3     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
 
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
 
void setup() {
  Serial.begin(9600); 
  dht1.begin();
  dht2.begin();
}
 
void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
 
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();

  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h1) || isnan(t1) || isnan(h2) || isnan(t2)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Humidity1: "); 
  Serial.print(h1);
  Serial.print(" %\t");
  Serial.print("Temperature1: "); 
  Serial.print(t1);
  Serial.println(" *C ");

   Serial.print("Humidity2: "); 
  Serial.print(h2);
  Serial.print(" %\t");
  Serial.print("Temperature2: "); 
  Serial.print(t2);
  Serial.println(" *C ");
 
}
