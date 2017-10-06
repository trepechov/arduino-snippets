#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
/************************* SETUP *********************************/
const bool LOGGING = false;
const char * wifiSSID = "Trepechov";
const char * wifiPassword = "azimitaka2";
const int connectionRetrys = 30;
/************************* Other settings ****************************/
ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
void setup()
{
  Serial.begin(115200);
  WiFiMulti.addAP(wifiSSID, wifiPassword);
  Serial.println("");
  Serial.print("Connecting Wifi");
  for (int i = 0; i < connectionRetrys; i++)
  {
    Serial.print(".");
    if (WiFiMulti.run() == WL_CONNECTED)
    {
      Serial.println(" connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      return;
    }
    delay(1000);
  }
}

void loop()
{
}
