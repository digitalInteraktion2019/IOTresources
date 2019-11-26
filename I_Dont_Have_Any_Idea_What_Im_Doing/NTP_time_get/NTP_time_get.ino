
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NTPClient.h>

//Function declarations:
void handleRoot();



//WIFI SETTINGS:
const char* ssid     = "SSID";
const char* password = "Password";

//Batt:
static unsigned battPin = A0;
unsigned long ADCfiltered = 1; //default to 0V
float vBatt = 14;
unsigned int filterBeta = 10;

//NPT Stuff:
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
String day;
String hour;
String minute;
String second;


//init http server on port 80:
ESP8266WebServer server(80);

//hardware stuff:
const unsigned int relayPin = D1;



void setup() {
  Serial.begin(115200);
  WiFi.hostname("Clock");
  WiFi.mode(WIFI_STA); //neccesary?
  WiFi.begin(ssid, password);
  pinMode(relayPin, OUTPUT);
  //digitalWrite(relayPin, squareState);

  for (int a = 0; a <= filterBeta; a++) ADCfiltered = ((ADCfiltered * filterBeta) + analogRead(battPin)) / (filterBeta + 1); //low pass filtering of ADC0
  vBatt = (float)14 / (float)1023 * (float)ADCfiltered; //ADC -> voltage

  delay(500);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //npt timeclient prep
  timeClient.begin();
  //}
  Serial.println();
  Serial.print("WiFi up!");
  Serial.print("  IPv4: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("clock")) {
    Serial.println("MDNS responder started, see you at http://clock.local");
  }

  server.on("/", handleRoot);
  server.on("/time",handleTime);/*
  server.on("/minute",minute);
  server.on("/second",second);*/
  server.onNotFound(handleRoot);

  server.begin();
  Serial.println("HTTP server started");

}

//only make conversions every 1/4 seconds, to make time for web stuff
unsigned long lastConversion = 0;
unsigned long conversionDelay = 250; //ms between ADC conversions
void loop() {

  if (millis() > lastConversion + conversionDelay) {
    //for (int a = 0; a <= filterBeta; a++) ADCfiltered = ((ADCfiltered * filterBeta) + analogRead(battPin)) / (filterBeta + 1); //low pass filtering of ADC0
    //vBatt = (float)14 / (float)1023 * (float)ADCfiltered; //ADC -> voltage
    lastConversion = millis();
    // Serial.print("ADCfiltered @:");
    // Serial.println(ADCfiltered);
    // Serial.print("vBatt @:");
    // Serial.println(vBatt);
    timeClient.update();
    day = (String)daysOfTheWeek[timeClient.getDay()];
    hour = (String)int(timeClient.getHours());
    minute = (String)int(timeClient.getMinutes());
    second = (String)int(timeClient.getSeconds());

  }

  //if (vBatt < 11) digitalWrite(relayPin, LOW);

  server.handleClient();

  yield();

}
void handleTime(){
  server.sendContent(hour + " : " + minute + " : " + second);
}

//http stuff:
void handleRoot() {

  Serial.println("client trying to access /");
  //server.send(200, "text/plain", "hello from esp8266!");

  //update the time
  timeClient.update();

  //  httpout += "<meta http-equiv=\"Refresh\" content=\"30\"/>";
  //  server.send(404, "text/html", httpout);

  server.sendContent("<HTML>\r\n");
  server.sendContent("<HEAD>\r\n");
  server.sendContent("<script>\r\n");
  
  server.sendContent("function httpGet(URL){");
  server.sendContent("var xhr = new XMLHttpRequest();\r\n"); //start Request
  server.sendContent("xhr.open('GET', URL, true); \r\n"); //get url
  server.sendContent("xhr.onload = function (e) { \r\n"); 
  server.sendContent("if (xhr.readyState === 4) { \r\n"); //check if it has processed through all 5 stages
  server.sendContent("if (xhr.status === 200) { \r\n"); //check if the response from server is a ok
  server.sendContent("console.log(xhr.responseText); \r\n");
  server.sendContent("document.getElementById('time').innerText=xhr.responseText;\r\n "); //set time to string gotten from arduino
  server.sendContent("} else { console.error(xhr.statusText);\r\n } } }\r\n ");
  server.sendContent("xhr.onerror = function (e) { \r\n");
  server.sendContent("console.log(xhr.statusText); } \r\n");
  server.sendContent("xhr.send(null);  }\r\n");

 
  server.sendContent("window.setInterval(function(){httpGet('/time');}, 500); \r\n");
  server.sendContent("</script>\r\n");
  server.sendContent("</HEAD>\r\n");
  
  server.sendContent("<body>\r\n");
  server.sendContent("<p>" + day + "</p>" + "\r\n");
  server.sendContent("<p id=\"time\"></p>\r\n");
  server.sendContent("</body>\r\n");
  server.sendContent("</HTML>\r\n");

}
