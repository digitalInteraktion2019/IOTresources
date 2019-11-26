#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

//Function declarations:
void handleRoot();
void handleToggle();
void handleNotFound();

//WIFI SETTINGS:
const char* ssid     = "FBI12";
const char* pass     = "123456yg";
//Batt:
static unsigned battPin=A0;
unsigned long ADCfiltered=1; //default to 0V
float vBatt=14;
unsigned int filterBeta=10;

// NTP settings
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//init http server on port 80:
ESP8266WebServer server(80);

//hardware stuff:
const unsigned int relayPin = D1;
bool lightState=true; // True=on , false=off


void setup() {
  Serial.begin(115200);
  WiFi.hostname("lighthouse");
  WiFi.mode(WIFI_STA); //neccesary?
  WiFi.begin(ssid, pass);
  pinMode(relayPin,OUTPUT);
  digitalWrite(relayPin,lightState);

  for(int a=0; a<=filterBeta; a++) ADCfiltered=((ADCfiltered*filterBeta)+analogRead(battPin))/(filterBeta+1); //low pass filtering of ADC0

  vBatt=(float)14/(float)1023*(float)ADCfiltered; //ADC -> voltage

  delay(500);

  //Serial.print("ADCfiltered @:");
  //Serial.println(ADCfiltered);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("WiFi up!");
  Serial.print("  IPv4: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("lighthouse")) {
    Serial.println("MDNS responder started, see you at http://lighthouse.local");
  }

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.onNotFound(handleRoot);

  timeClient.begin();
  server.begin();
  Serial.println("HTTP server started");

}

//only make conversions every 1/4 seconds, to make time for web stuff
unsigned long lastConversion = 0;
unsigned long conversionDelay = 250; //ms between ADC conversions
void loop() {

if (millis()> lastConversion+conversionDelay){
  for(int a=0; a<=filterBeta; a++) ADCfiltered=((ADCfiltered*filterBeta)+analogRead(battPin))/(filterBeta+1); //low pass filtering of ADC0
  vBatt=(float)14/(float)1023*(float)ADCfiltered; //ADC -> voltage
  lastConversion=millis();
  // Serial.print("ADCfiltered @:");
  // Serial.println(ADCfiltered);
  // Serial.print("vBatt @:");
  // Serial.println(vBatt);
}





if(vBatt<11) digitalWrite(relayPin,LOW);

server.handleClient();

yield();

}

//http stuff:
void handleRoot() {
  timeClient.update();

  String dayOfWeek = daysOfTheWeek[timeClient.getDay()];
  Serial.println(dayOfWeek);
  
  if(dayOfWeek == "Wednesday") {
      Serial.println("client trying to access /");
      //server.send(200, "text/plain", "hello from esp8266!");
      server.sendContent("HTTP/1.1 200 OK\r\n"); //send new p\r\nage
      server.sendContent("Content-Type: text/html\r\n");
      server.sendContent("\r\n");
      server.sendContent("<HTML>\r\n");
      server.sendContent("<HEAD>\r\n");
      server.sendContent("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"); //zoom to width of window
      server.sendContent("<meta name='apple-mobile-web-app-capable' content='yes' />\r\n");
      server.sendContent("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />\r\n");
      server.sendContent("<link rel='stylesheet' type='text/css' href='https://moore.dk/doorcss.css' />\r\n"); //External CSS
      server.sendContent("<TITLE>Is it wednesday?</TITLE>\r\n");
      server.sendContent("</HEAD>\r\n");
      server.sendContent("<BODY>\r\n");
      if(lightState) server.sendContent("<H1>Is it wednesday?</H1>\r\n");
      else server.sendContent("<H1>Lighthouse is off.</H1>\r\n");
      server.sendContent("<hr />\r\n");
      //server.sendContent("<br />\r\n");
      server.sendContent("<br />\r\n");
      if(lightState) server.sendContent("<iframe width=560 height=315 src=https://www.youtube.com/embed/du-TY1GUFGk?autoplay=1&enable_js=1 frameborder=0 allow=autoplay; autoplay; encrypted-media; gyroscope; picture-in-picture allowfullscreen></iframe>\r\n");
      else server.sendContent("<a href=\"/toggle\"\">Light that lighthouse up!</a>\r\n");
      server.sendContent("<br />\r\n");
      if(!lightState) server.sendContent("<H3>The lighthouse takes about 7 seconds to heat up</H3>");
      server.sendContent("</BODY>\r\n");
      server.sendContent("</HTML>\r\n");    
    } else {
      Serial.println("client trying to access /");
      //server.send(200, "text/plain", "hello from esp8266!");
      server.sendContent("HTTP/1.1 200 OK\r\n"); //send new p\r\nage
      server.sendContent("Content-Type: text/html\r\n");
      server.sendContent("\r\n");
      server.sendContent("<HTML>\r\n");
      server.sendContent("<HEAD>\r\n");
      server.sendContent("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"); //zoom to width of window
      server.sendContent("<meta name='apple-mobile-web-app-capable' content='yes' />\r\n");
      server.sendContent("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />\r\n");
      server.sendContent("<link rel='stylesheet' type='text/css' href='https://moore.dk/doorcss.css' />\r\n"); //External CSS
      server.sendContent("<TITLE>Is it wednesday?</TITLE>\r\n");
      server.sendContent("</HEAD>\r\n");
      server.sendContent("<BODY>\r\n");
      server.sendContent("<H1>it is ");
      server.sendContent(String(dayOfWeek));
      server.sendContent("</H1>");   
      server.sendContent("<hr />\r\n");
      //server.sendContent("<br />\r\n");
      server.sendContent("<br />\r\n");
      server.sendContent("<br />\r\n");
      if(!lightState) server.sendContent("<H3>The lighthouse takes about 7 seconds to heat up</H3>");
      server.sendContent("</BODY>\r\n");
      server.sendContent("</HTML>\r\n");  
      }
}

void handleToggle(){

  lightState=!lightState;
  digitalWrite(relayPin,lightState);
  if(lightState)  Serial.println("light on..");
  else Serial.println("light off..");
  //reply to client with redirect to root, to update status of light in browser
  server.sendContent("HTTP/1.1 303 See Other\r\n");
    server.sendContent("Location: /\r\n");
    server.sendContent("\r\n"); //EOT
}
