// http://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/station-examples.html

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;
boolean connectioWasAlive = true;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  wifiMulti.addAP("Trepechov", "azimitaka2");
 // wifiMulti.addAP("secondary-network-name", "pass-to-secondary-network");
 // wifiMulti.addAP("tertiary-network-name", "pass-to-tertiary-network");
}

void monitorWiFi()
{
  if (wifiMulti.run() != WL_CONNECTED)
  {
    if (connectioWasAlive == true)
    {
      connectioWasAlive = false;
      Serial.print("Looking for WiFi ");
    }
    Serial.print(".");
    delay(500);
  }
  else if (connectioWasAlive == false)
  {
    connectioWasAlive = true;
    Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
  }
}

void loop()
{
  monitorWiFi();
}
