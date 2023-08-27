#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

const int DHTPIN = 4; // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22

char ssid[] = "Njeet";    // WiFi Name
char pass[] = "12345678";    // WiFi Password

unsigned long channelID = 2252713;    // Your ThingSpeak Channel ID
const char *apiKey = "EOTO9LSTOT80KF24";    // Your ThingSpeak API Key

WiFiClient client;
DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(9600);
  connectWiFi();
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() 
{
  delay(1000);

  // Read DHT22 values
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float temperature = event.temperature;
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.println("°C");

  float humidity = event.relative_humidity;
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  if (!isnan(temperature) && !isnan(humidity)) {
    sendDataToThingSpeak(temperature, humidity);
  } else {
    Serial.println("Failed to read DHT22 values");
  }
}

void connectWiFi() 
{
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println("WiFi Connected");
}

void sendDataToThingSpeak(float temperature, float humidity) 
{
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int httpCode = ThingSpeak.writeFields(channelID, apiKey);

  if (httpCode == 200) 
  {
    Serial.println("Data sent to ThingSpeak");
  } 
  
  else 
  {
    Serial.println("Error sending data to ThingSpeak");
  }
}
