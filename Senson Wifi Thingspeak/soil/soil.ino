
/* Arduino+esp8266 thingSpeak example  
 * Example name = "Write temperature and humidity to Thingspeak channel"
 * Created by Ilias Lamprou
 * Updated Oct 30 2016
 * 
 * Download latest Virtuino android app from the link:https://play.google.com/store/apps/details?id=com.virtuino_automations.virtuino&hl
 * Video tutorial link: https://youtu.be/4XEe0HY0j6k
 * Contact address for questions or comments: iliaslampr@gmail.com
 */

// Code to use SoftwareSerial
#include <SoftwareSerial.h>
SoftwareSerial espSerial =  SoftwareSerial(2,3);      // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to esp8266 module TX pin   -  connect the arduino TX pin to esp8266 module RX pin

int sensorPin = A5;
int lowMoisturePit = 4;

String apiKey = "MFQFJFYWL7C0SJU1";     // replace with your channel's thingspeak WRITE API key
String ssid="Trepechov";    // Wifi network SSID
String password ="azimitaka2";  // Wifi network password

boolean DEBUG=true;

//======================================================================== showResponce
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (espSerial.available()){
        c=espSerial.read();
        if (DEBUG) Serial.print(c);
      }
    }
}

//========================================================================
boolean thingSpeakWrite(float value3){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "api.thingspeak.com";                                 // api.thingspeak.com
  cmd += "\",80";
  espSerial.println(cmd);
  if (DEBUG){
    Serial.println(cmd);
    Serial.println(espSerial.read());
  }
  
  /*
  if(espSerial.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }*/
  
  // prepare GET string
  String getStr = "GET /update?api_key=" + apiKey;
  getStr +="&field3=" + String(value3);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=" + String(getStr.length());
  espSerial.println(cmd);
  if (DEBUG){
    Serial.println(cmd);
    Serial.println(espSerial.read());
  }
  
  delay(100);
  
 // if(espSerial.find(">")){
    espSerial.print(getStr);
    if (DEBUG){
      Serial.println(getStr);
      Serial.println(espSerial.read());
    }
 // }
  /*else{
    espSerial.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }*/
  return true;
}

int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 1023, 380, 0, 100);
  return percentValue;
}
//================================================================================ setup
void setup() {
  DEBUG=true;           // enable debug serial
  pinMode(lowMoisturePit, OUTPUT);
  Serial.begin(9600);
  
  espSerial.begin(115200);  // enable software serial
                          // Your esp8266 module's speed is probably at 115200. 
                          // For this reason the first time set the speed to 115200 or to your esp8266 configured speed 
                          // and upload. Then change to 9600 and upload again
  
  //espSerial.println("AT+RST");         // Enable this line to reset the module;
  //showResponse(1000);

  //espSerial.println("AT+UART_CUR=9600,8,1,0,0");    // Enable this line to set esp8266 serial speed to 9600 bps
  //showResponse(1000);
  
  espSerial.println("AT+CWMODE=1");   // set esp8266 as client
  showResponse(1000);

  espSerial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password
  showResponse(5000);

   if (DEBUG) {
    //Serial.println("Setup completed");
    //Serial.println(espSerial.println("AT+CIPSTATUS"));
   }
}


// ====================================================================== loop
void loop() {

  // Read sensor values
  float raw = analogRead(sensorPin);
  float perc = convertToPercent(raw); 
  if (DEBUG) Serial.println("Soil moisture: "+String(raw)+" raw");
  if (DEBUG) Serial.println("Soil moisture: "+String(perc)+" %");

  if (perc < 50) {
    digitalWrite(lowMoisturePit, HIGH);
  }
  else {
    digitalWrite(lowMoisturePit, LOW);
  }
  thingSpeakWrite(perc);
  
    
  // thingspeak needs 15 sec delay between updates,     
  delay(20000);  
}





