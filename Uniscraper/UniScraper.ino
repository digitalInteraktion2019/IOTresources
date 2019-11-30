#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
 
char ssid[] = "Test";
const char* password =  "hackerman";

const String host = "selvbetjening.aarhustech.dk";
const String page = "/WebServices/CalSearch.asmx/SearchAll";

String data;

void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 int timer = 10000;
void loop() {
 timer++;
 if(timer >= 10000) {
  String nameSel = "Daniel";
  Serial.println("Getting data for: " + nameSel);
  getData(nameSel);
  timer = 0;
 }
 Serial.print(timer);
}

void getData(String nameSelected) {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    WiFiClientSecure client;
    client.setInsecure();
    
   if (!client.connect(host, 443)) {
    Serial.println("connection failed");
    return;
  }

  Serial.println("Connected boiiii");

    String input = "{\"prefixText\":\"" + nameSelected + "\",\"count\":150,\"contextKey\":\"all|0|0\"}";

    
    client.print(String("POST ") + page + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json;charset=UTF-8" + "\r\n" +
               "Connection: close\r\n" +
               "Content-Length: " + input.length() + "\r\n\r\n" +
               input + "\r\n");

    Serial.println("request sent");
    //String data;
    while (client.connected()) {
      data = client.readString();
    }
    Serial.print(data);
  }
}
