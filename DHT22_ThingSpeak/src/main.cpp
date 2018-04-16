#include "DHT.h"
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"


#define DHTPIN 4     // IMPORTANT D2 on NodeMCU is GPIO 4

// Uncomment whatever type you're using
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

const char *ssid =  "xxxxxx"; // Your wifi ssid
const char *pass =  "xxxxxx";       //Your wifi password

unsigned long myChannelNumber = 12345;
const char * myWriteAPIKey = "xxxxx";

const char* server = "api.thingspeak.com";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void updateThingSpeak(float t, float h){
  ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  delay(20*1000);
  ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);
  delay(20*1000);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();
  ThingSpeak.begin(client);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

  if (client.connect(server, 80)) {
    updateThingSpeak(t,h);    
  }
}



