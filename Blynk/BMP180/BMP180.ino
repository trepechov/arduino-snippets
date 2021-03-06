/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define RSTPIN D0
#define BATPIN A0

char auth[] = "ae1ac49356d142f69dc3a1f066628831";
char ssid1[] = "MB";
char ssid2[] = "Trepechov";
char pass[] = "azimitaka2";

int sleepMinutes = 5; // Sleep to 5 minunte
float temperature, pressure;
Adafruit_BMP085 bmp;

bool connectWiFi(char ssid[], char pass[], int retry) 
{
  Serial.printf("Connecting to %s", ssid);
  WiFi.begin(ssid, pass);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (i++ > retry)
    {
      WiFi.disconnect();
      Serial.println(" Can't connect to network!");
      return false;
    }
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" Connected!");
  blink(3);
  return true;
}

bool connectBlynk(int retry)
{
  Blynk.config(auth);
  int i = 0;
  while(Blynk.connect() != true)
  {
    if (i++ > retry)
    {
      Serial.println(" Can't connect to Blynk service");
      return false;  
    }
    Serial.print(".");
    delay(500);
  }
  Serial.println("Conected!");
  return true;
}

void processSensor()
{
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100;

  if (isnan(temperature) || isnan(pressure)) {
    Serial.println("Failed to read from BMP180 sensor!");
    return;
  }

  Serial.printf("\nTemperature:\t%0.1f *C\nAir Pressure:\t%0.0f hPa\n\n", temperature, pressure); 
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V6, temperature);
  Blynk.virtualWrite(V8, pressure);
}

float readBattery()
{
  float v = analogRead(BATPIN);
  return v;
}

void blink(int i)
{
  int j;
  for (j = 0; j < i; j++)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }
}

void sleep(int minutes)
{
  Serial.printf("Sleep for %i minutes\n\n", minutes);
  //blink(sleepMinutes);
  ESP.deepSleep(minutes * 60 * 1000000);
}

BLYNK_WRITE(V4) 
{
   sleepMinutes = param.asInt();
   Serial.printf("Read V4 slider value - %i\n\n", sleepMinutes);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("");
  pinMode(LED_BUILTIN, OUTPUT);  
  pinMode(RSTPIN, WAKEUP_PULLUP);
  digitalWrite(LED_BUILTIN, HIGH);
  bool connected = false;
  
  connected = connectWiFi(ssid1, pass, 7);
  connected = connected || connectWiFi(ssid2, pass, 7);
  connected = connected && connectBlynk(4);
  
  if (connected)
  {
    if (!bmp.begin())
    {
      Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    }
    Blynk.run();
    Blynk.syncVirtual(V4);
    processSensor();
    Blynk.virtualWrite(V2, readBattery());
  }

  sleep(sleepMinutes);
}

void loop()
{
  //  Blynk.run();
  //  timer.run();
}
