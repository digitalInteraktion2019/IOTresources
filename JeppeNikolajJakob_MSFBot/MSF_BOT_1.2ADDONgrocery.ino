//-----------------------------------------------
/*
 * This code sends data from the Wemos to a Google sheets.
 * 
 */
//-----------------------------------------------
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "StackThunk.cpp"
String readString;
const char* ssid = "Bjergs IPhone";
const char* password = "kode1234";
const char* host = "script.google.com";
const int httpsPort = 443;
// Use WiFiClientSecure class to create TLS connection
BearSSL::WiFiClientSecure client;
// SHA1 fingerprint of the certificate, don't care with your GAS service
const char* fingerprint = "96383360d46b84c932674944f227d87c331a355a";
String GAS_ID = "AKfycbyaa7QwORUgl1aY7vPPNU7NCeVa2ML4qR5w6kP0OYuCWp3iS74";   // Replace by your GAS service id 
void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
}

void loop() {

  Serial.print("connecting to ");
  Serial.println(host);

Serial.printf("Using fingerprint '%s'\n", fingerprint);
client.setFingerprint(fingerprint);
  
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
    
  String string_item = "jordbaer";
  String string_place =  "coop"; 
  String string_price =  "9.97kr"; 
  String url = "/macros/s/" + GAS_ID + "/exec?item=" + string_item + "&place=" + string_place + "&price=" + string_price;
  Serial.print("requesting url: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "host: " + host + "\r\n" +
         "User-Agent: curl/7.55.1\r\n" +
         "Accept */*\r\n" +
         "Connection: close\r\n\r\n");
         
delay(5000);

  Serial.println("request sent");
  while (client.connected()) {
  String line = client.readStringUntil('\n');
  yield();
  if (line == "\r") {
    Serial.println("headers received");
    break;

  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
  Serial.println("esp8266/Arduino CI successfull!");
  } else {
  Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}
  }
  }

