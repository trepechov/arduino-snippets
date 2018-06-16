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

#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define RSTPIN D0

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "ae1ac49356d142f69dc3a1f066628831";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Trepechov";
char pass[] = "azimitaka2";

const int sleepSeconds = 1 * 60; // Sleep to 10 minuntes

Adafruit_BMP085 bmp;

float temperature, pressure;
// float humidity, temperature, heatIndex;
// char str_humidity[10], str_temperature[10], str_heatIndex[10];

// DHT dht(DHTPIN, DHTTYPE);

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor() {

  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();

  if (isnan(temperature) || isnan(pressure)) {
    Serial.println("Failed to read from BMP180 sensor!");
    return;
  }

  Serial.printf("Temperature:\t%0.2f *C\nPressure:\t%f hPa\n\n", temperature, pressure);

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V6, temperature);
  Blynk.virtualWrite(V7, pressure);
}

void setup()
{
  Serial.begin(9600);
  pinMode(RSTPIN, WAKEUP_PULLUP);
  Blynk.begin(auth, ssid, pass);


  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
  Blynk.run();
  Blynk.syncVirtual(V4);
  sendSensor();

  Serial.printf("Sleep for %i seconds\n\n", sleepSeconds);
  ESP.deepSleep(sleepSeconds * 1000000);
}

int someValue;
BLYNK_WRITE(V4) 
{
   someValue = param.asInt();
   Serial.printf("V4 %i\n\n", someValue);
}

void loop()
{
  //  Blynk.run();
  //  timer.run();
}

