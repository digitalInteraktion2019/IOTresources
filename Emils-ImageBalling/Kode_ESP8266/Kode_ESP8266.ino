#include <iostream>
#include <string>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>´

String website = "http://www.edoganci.dk/";
const char* ssid = "NETVÆRKS NAVN";
const char* password = "NETVÆRKS KODE";
int port = 8080;

char html[] PROGMEM = {"<!DOCTYPE html>\n<html>\n\n<head>\n    <meta charset=\"utf-8\" />\n    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\n    <style>\n        @import url('https://fonts.googleapis.com/css?family=Zilla+Slab&display=swap');\n        body {\n            display: flex;\n            flex-direction: row;\n            justify-content: center;\n            align-items: top\n            min-height: 100vh;\n            font-family: 'Zilla Slab', serif;\n            font-size: 4em;\n            background: #efefef;\n            letter-spacing: -1px;\n        }\n        html, body {\n\t\t  width:  100%;\n\t\t  height: 100%;\n\t\t  margin: 0;\n\t\t}\n        \n        quote::before {\n            content: '\\201C';\n        }\n        \n        quote::after {\n            content: '\\201D';\n        }\n        \n        ins {\n            color: rgba(86, 175, 155, .75);\n            cursor: help;\n            position: relative;\n        }\n        \n        ins::before {\n            content: '(correction)';\n            display: block;\n            pointer-events: none;\n            position: absolute;\n            color: #659b82;\n            top: -2em;\n            left: 0;\n            font-size: .5em;\n            opacity: 0;\n            transition: .5s ease;\n        }\n        \n        ins:hover del {\n            max-width: 9em;\n        }\n        \n        ins:hover::before {\n            opacity: 1;\n            top: -1.2em;\n        }\n        \n        ins del {\n            color: rgba(255, 107, 107, .75);\n            display: inline-block;\n            overflow: hidden;\n            max-width: 0;\n            vertical-align: bottom;\n            transition: .5s ease;\n        }\n        \n        ins del::before {\n            content: '\\00a0';\n            visibility: hidden;\n        }\n    </style>\n</head>\n\n<body onLoad=\"init();\">\n    <div>\n        <p class=\"sleep\" >Emils IOT-P<span class=\"sleep__letters\"><sup>Z</sup><sub>Z</sub><sup>Z</sup></span>roject...</p>\n    </div>\n\n    <canvas id=\"myCanvas\" width=\"600\" height=\"450\" style=\"position:absolute; bottom:75px; border:1px solid #000000; z-index:3\"></canvas>\n\n</body>\n\n</html>\n\n<script> \n\tvar context;\n\tvar x=100;\n\tvar y=200;\n\tvar dx=5;\n\tvar dy=5;\n\tvar canvasX = 600;\n\tvar canvasY = 450;\n\tvar imglink = \"http://www.edoganci.dk/\" + document.title;\n\tvar actualImg = new Image();\n\tvar pattern;\n\tfunction init()\n\t  {\n\t    context = myCanvas.getContext('2d');\n\t    setInterval(draw, 10);\n\t    actualImg.src = imglink;\n\t    actualImg.onload = function(){\n\t    \tpattern = context.createPattern(this, \"repeat\");\n\t\t    context.beginPath();\n\t\t    \n\t\t    context.fillStyle=pattern;\n\t\t    context.arc(100,100,75,0,Math.PI*2,true); context.closePath();\n\t\t    context.fill();\n\t    }\n\t  }\n\tfunction draw()\n\t{\n\t\t context.clearRect(0,0, canvasX,canvasY);\n\t\t context.beginPath();\n\t\t context.fillStyle = pattern;\n\t\t context.arc(x,y,75,0,Math.PI*2,true);\n\t\t context.closePath();\n\t\t context.fill();\n\t\tif( x<0 || x>canvasX) dx=-dx; \n\t\tif( y<0 || y>canvasY) dy=-dy; \n\t\tx+=dx; \n\t\ty+=dy;\n\t}\n \n</script>"};
String webPage;
String token;
String actualToken;
boolean Scraped = false;

ESP8266WebServer webServer(port);
HTTPClient http;

String get_my_string(String startDelim, String stopDelim, String myString){
  int firstChar = myString.indexOf(startDelim) + startDelim.length();
  int secondChar = myString.indexOf(stopDelim);

  return myString.substring(firstChar, secondChar);
}

void createWebServer(){
  webServer.send(200, "text/html", actualToken + html);
}

void scrapeWebsite(){
  Serial.println("Scraping website " + website);
  http.begin(website);
  int httpCode = http.GET();
  if (httpCode > 0){
      Serial.println("Assigning variable with scraped data:");
      webPage = http.getString();
    }
  //Serial.println(webPage);
}

void filterData(){
  token = get_my_string("<img src=\"", "\" class=", webPage);
  actualToken = "<title>" + token + " </title>";
  Serial.println("token: " + token);
  Serial.println("actual token" + actualToken);
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){ delay(500); Serial.print("Waiting for connection..\n");}
  scrapeWebsite();
  filterData();
  
  webServer.on("/", createWebServer);
  webServer.begin();
  
  Serial.print("Webserver started on: ");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.print(port);
}

void loop() {
  delay(10);
  webServer.handleClient();
}
