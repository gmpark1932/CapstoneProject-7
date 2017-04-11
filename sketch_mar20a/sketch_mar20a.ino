/*
 * 2017/3/15  Kyuho Kim
 * Template for using Thing Speak Service
 */

const char* host_thingspeak = "api.thingspeak.com";
String url_mission4 = "/update?api_key=XX8CPF5JAM7KFAAZ";
String url_mission5 = "/update?api_key=3RZ0UFQDK1K47R9E";

const char* host_nodejs = "163.239.78.84";
String url_mission7 = "/ading?";

const int httpPort = 80;
int interval = 60000;

float x = 0.0;

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <math.h>

#define ONE_WIRE_BUS D4

const char* ssid = "ReleaseWifi";   // Your own ssid here
const char* password = "10161016";  // Your own password here

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

/* sending to url for payload */
void delivering(const char* host,String url,String payload) { 
  WiFiClient client;
  Serial.print("connecting to "); 
  Serial.println(host);
  if (!client.connect(host, httpPort)) {
    Serial.print("connection failed: ");
    Serial.println(payload);
    return;
  }
  String getheader = "GET "+ String(url) +"&"+ String(payload) +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266 Gyungmin");  
  client.println("Host: " + String(host));  
  client.println("Connection: close");  
  client.println();

  Serial.println(getheader);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("Done cycle.");
}

/* Mission #4 */
String working() { 
  x = x + 0.1; 
  float r;

  r = 3 * (1 - cos(x));
  
  return(String("field1=")+String(r));
}

void send_workingValue(){
    String payload = working();
     delivering(host_thingspeak,url_mission4,payload);
}

/* Mission #5 */
float getTemperature() {
Serial.println("In GetTem\n");
  
  float temp;
  do {
    sensors.requestTemperatures(); 
    temp = sensors.getTempCByIndex(0);
    Serial.println(temp);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  Serial.println(temp);
  return temp;
}

void send_temperature(){
  char temperatureString[6];
  float temperature = getTemperature();

  dtostrf(temperature, 2, 2, temperatureString);
  // send temperature to the serial console
  Serial.println(temperatureString);

  String payload = "field1=" + String(temperature);

  delivering(host_thingspeak,url_mission5,payload);
  
}

void connect_ap() {
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n Got WiFi, IP address: ");
  Serial.println(WiFi.localIP());  
}

void setup() {
  Serial.begin(115200);
  connect_ap();

  Serial.println("ESPArduinoThingSpeak.cpp - 2017/3/15");
}


unsigned long mark = 0;
void loop() {
     send_workingValue(); // Mission #4
     send_temperature(); // Mission #5 

     delay(6000);
  
}
