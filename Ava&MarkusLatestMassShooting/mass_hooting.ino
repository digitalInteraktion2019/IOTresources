//lys
#include <Adafruit_NeoPixel.h>
#define NUM_PIXELS 8
Adafruit_NeoPixel pixels(NUM_PIXELS, D7, NEO_GRB | NEO_KHZ800);

//wifi
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>


const char* ssid     = "Markus  - iPhone";
const char* password = "Avalover";
const char* host = "www.dayssincethelastmassshooting.com";

void setup() {
  pixels.begin();
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

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

int value = 0;

void setColor(uint32_t color) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(200);
  }
}
void loop() {
  delay(5000); //run every 5 seconds
  int linecounter = 0;
  String vindchillString;
  float vindchill;
  String line = "";
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClientSecure client;
  client.setInsecure();
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(2000); //wait a while to let the server respond (increase on shitty connections)

  // Read all the lines of the reply from server
  
  while (client.available()) {
    yield();
    line = client.readStringUntil('\n'); //cariage return as delimiter
    linecounter++;
    if (String(line).indexOf("single-digit") > 0) {
      Serial.print(linecounter);
      Serial.println(": " + line);
      vindchillString = line.substring(29, 32);
    }
    delay(1000);
  }

  if (line != "") //if data exists
  {
    vindchill = vindchillString.toFloat(); //parse the string as a float.
    Serial.println("String: " + String(vindchillString)); //test report
    Serial.println("float: " + String(vindchill)); //test report
  }

  Serial.println();
  Serial.println("closing connection");

  if(vindchill>=0 && vindchill <3){setColor(pixels.Color(255, 0, 0));}
  if(vindchill>=3 && vindchill <8){setColor(pixels.Color(255, 20, 0));}
  if(vindchill>=8 && vindchill <15){setColor(pixels.Color(255, 50, 0));}
  if(vindchill>=15 && vindchill <22){setColor(pixels.Color(255, 100, 0));}
  if(vindchill>=22 && vindchill <31){setColor(pixels.Color(255, 150, 0));}
  if(vindchill>=31 && vindchill <61){setColor(pixels.Color(230, 230, 0));}
  if(vindchill>61){setColor(pixels.Color(0, 255, 0));}
}
