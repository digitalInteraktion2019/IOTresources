#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <FastLED.h>
#define LED_PIN     D2
#define NUM_LEDS    1

const char* ssid     = "Joakims S10";
const char* password = "12345679";

const char* host = "caniflyakite.com";


CRGB leds[NUM_LEDS];


void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(D2, OUTPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  leds[0] = CRGB(200, 200, 0);
  FastLED.show();

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

void loop() {
  delay(10000); //run every 5 seconds
  int linecounter=0;
  String answer;
  String line="";
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClientSecure client;
  client.setInsecure();
  const int httpPort = 443;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/check/text"; //i need something here

  Serial.print("POSTing to URL: ");
  Serial.println(url);

  // This will send the request to the server
 // client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");


  client.println(String("POST ") + url + " HTTP/1.1");
  client.println("Host: " + String(host));
  client.println("User-Agent: curl/7.58.0");
  client.println("Accept: */*");
  client.println("Content-Type: application/json");
  client.println("Content-Length: 27");
  client.println();

  client.write("{\"input\":\"Aarhus, denmark\"}");
  
  delay(1000); //wait a while to let the server respond (increase on shitty connections)

  // Read all the lines of the reply from server
  while(client.available()){
    line = client.readStringUntil('\n'); //cariage return as delimiter
    //Serial.println(line);
    linecounter++;
    if(linecounter==11) answer=line.substring(13,15);  //magic numbers galore :P
  }

  if(line!="") //if data exists
  {
    Serial.print("Answer = " + answer);

    if(answer == "Ye") {
      leds[0] = CRGB(0, 255, 0);
      FastLED.show();
      Serial.println("Yes");
    }
    else if (answer == "No") {
      leds[0] = CRGB(255, 0, 0);
      FastLED.show();
      Serial.println("No");
    }
    else {
      leds[0] = CRGB(200, 200, 200);
      FastLED.show();
      Serial.println("Null");
    }
    
  }

  Serial.println();
  Serial.println("closing connection");
}
