/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need Adafruit DHT sensor libraries:
    https://github.com/adafruit/Adafruit_Sensor
    https://github.com/adafruit/DHT-sensor-library

  App project setup:
    Value Display widget attached to V5
    Value Display widget attached to V6
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define RSTPIN D0
#define DHTPIN D4          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "b111708168014bb0abbbf2b567d51ef4";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "MB";
char pass[] = "azimitaka2";

const int sleepSeconds = 10*60;

float humidity, temperature, heatIndex;
char str_humidity[10], str_temperature[10], str_heatIndex[10];

DHT dht(DHTPIN, DHTTYPE);

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);
  
  // Convert the floats to strings and round to 2 decimal places
  dtostrf(humidity, 1, 2, str_humidity);
  dtostrf(temperature, 1, 2, str_temperature);
  dtostrf(heatIndex, 1, 2, str_heatIndex);
  Serial.printf("\nHumidity:    %s %%\nTemperature: %s *C\nHeat index:  %s *C\n\n", str_humidity, str_temperature, str_heatIndex);
  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, humidity);
  Blynk.virtualWrite(V6, temperature);
  Blynk.virtualWrite(V7, heatIndex);
}

void setup()
{
  Serial.begin(9600);
  pinMode(RSTPIN, WAKEUP_PULLUP);
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  Blynk.run();
  sendSensor();

  Serial.printf("Sleep for %d seconds\n\n", sleepSeconds);
  ESP.deepSleep(sleepSeconds * 1000000);
}

void loop()
{
//  Blynk.run();
//  timer.run();
}

