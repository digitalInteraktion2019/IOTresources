/*
Introducing the MSF BOT, the Money Saving Food Bot.
This uses Https get request, to get data from the internet, mainly prices on Onions.
The device calculates the cheapest Onion, and outouts both price and place on an LCD display.

The code uses fingerprints to bypass the Secure client as it is HTTPS and not HTTP.

This Device is ment to sit in the fridge and display the price for essential foods at all times. 
The user will then be able to see the price and place for the cheapest Onion whenever he or he runs out.
 */

//LCD

#include <rgb_lcd.h>
#include <Wire.h>
#include "rgb_lcd.h"


rgb_lcd lcd;

int colorR = 0;
int colorG = 255;
int colorB = 0;

//Real code

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#ifndef STASSID
#define STASSID "Bjergs IPhone"
#define STAPSK  "kode1234"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;


const int httpsPort = 443; //80


//NEMLIG
const char* host1 = "www.nemlig.com";
String url1 = "/loeg-zittauer-2301096";

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char fingerprint1[] PROGMEM = "666101f91494587d521734df02bae89e5ecd37d3";

float Nemligcurrent;
//NEMLIG


//COOP
const char* host2 = "butik.mad.coop.dk";
String url2 = "/frugt-og-groent/groentsager/loeg-og-porrer/danske-loeg-p-5700380269572";

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char fingerprint2[] PROGMEM = "a9716f5657a175381ecca3770bf9f30493eac1dc";

  float Coopcurrent;
//COOP

void setup() {
//LCD

 lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
    lcd.print("MSF Bot");
    lcd.setCursor(0,1);
    lcd.print("Booting...");

//Real code  
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
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wifi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

int value = 0;

void loop() {  
  BearSSL::WiFiClientSecure client;
  delay(5000); //run every 5 seconds
  int linecounter=0;
  String NemligString1;
  String NemligString2;
  float Nemligprice;
  String line1="";
  ++value;

  Serial.print("connecting to ");
  Serial.println(host1);

  //Using fingerprint to get HTTPS certificate
Serial.printf("Using fingerprint '%s'\n", fingerprint1);
client.setFingerprint(fingerprint1);

  // Use WiFiClient class to create TCP connections
  if (!client.connect(host1, httpsPort)) {
    Serial.println("connection failed");

    lcd.clear();
colorR = 255;
colorG = 0;
colorB = 0; 
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("Error 001"); 
  lcd.setCursor(0,1);
  lcd.print("Wifi disconnected"); 
    
    return;
  }

  // We now create a URI for the request

  Serial.print("Requesting URL: ");
  Serial.println(url1);

  // This will send the request to the server
  client.print(String("GET ") + url1 + " HTTP/1.1\r\n" +
               "Host: " + host1 + "\r\n" +
               "User-Agent: chrome/7.55.1\r\n" +
               "Accept: */*\r\n\r\n");
               //"Connection: close\r\n\r\n");
  
  delay(5000); //wait a while to let the server respond (increase on shitty connections)

  // Read all the lines of the reply from server
  while(client.available()){
    yield();
    line1 = client.readStringUntil('\r'); //cariage return as delimiter
    yield();
    //Serial.println(line1);
    yield();
    linecounter++;
    yield();
    //Serial.println(linecounter);
    if(linecounter==284) {
   NemligString1=line1.substring(2022,2023);  //magic numbers galore :P
   NemligString2=line1.substring(2024,2026);
    }  
  }
  if(line1!="") //if data exists
  {
  Nemligprice=(NemligString1.toFloat()+(NemligString2.toFloat()/100)); //parse the string as a float.
  //Serial.println("String1: " +  String(NemligString1)); //test report
  //Serial.println("String2: " +  String(NemligString2)); //test report
  Serial.println("Nemlig: " + String(Nemligprice)); //test report
 
  }



/*


COOP
COOP
COOP
COOP


*/



//COOP
  delay(5000); //run every 5 seconds
  int linecounter2=0;
  String CoopString1;
  String CoopString2;
  float Coopprice = 0;
  String line2="";
  ++value;

  Serial.print("connecting to ");
  Serial.println(host2);

  //Using fingerprint to get HTTPS certificate
Serial.printf("Using fingerprint '%s'\n", fingerprint2);
client.setFingerprint(fingerprint2);

  // Use WiFiClient class to create TCP connections
  if (!client.connect(host2, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request

  Serial.print("Requesting URL: ");
  Serial.println(url2);

  // This will send the request to the server
  client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
               "Host: " + host2 + "\r\n" +
               "User-Agent: chrome/7.55.1\r\n" +
               "Accept: */*\r\n\r\n");
               //"Connection: close\r\n\r\n");
  
  delay(8000); //wait a while to let the server respond (increase on shitty connections)

  // Read all the lines of the reply from server
  while(client.available()){
    yield();
    line2 = client.readStringUntil('\r'); //cariage return as delimiter
    yield();
    //Serial.println(line2);
    yield();
    linecounter2++;
    yield();
        //Serial.println(linecounter2);
    if(linecounter2==164) {
      CoopString1=line2.substring(3673,3675);  //magic numbers galore :P
      CoopString2=line2.substring(3676,3678);
      
    }  
  }
  if(line2!="") //if data exists
  {
  Coopprice=(CoopString1.toFloat()+(CoopString2.toFloat()/100)); //parse the string as a float.
   //Serial.println("String1: " +  String(CoopString1)); //test report
   //Serial.println("String2: " +  String(CoopString2)); //test report
  Serial.println("Coop: " + String(Coopprice)); //test report
  }
  
  Serial.println();
  Serial.println("closing connection");
  
  Serial.println("FIRST");
  Serial.println(String(Coopprice));
  Serial.println(String(Nemligprice));
  Serial.println(" ");
  Serial.println("SECOND");
float zero = 0;
if (Coopprice != zero) {
  Coopcurrent = Coopprice;
}
Serial.println(String(Coopcurrent));

if (Nemligprice != zero) {
  Nemligcurrent = Nemligprice;
}
Serial.println(String(Nemligcurrent));

colorR = 0;
colorG = 0;
colorB = 255;
if (Nemligcurrent <= zero && Coopcurrent > zero) {
lcd.clear();
  lcd.clear();
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("Loeg");
  lcd.setCursor(0,1);
  lcd.print("Coop: " + String(Coopcurrent) + " kr"); 

} else if (Coopcurrent <= zero && Nemligcurrent > zero){
  lcd.clear();
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("Loeg");
  lcd.setCursor(0,1);
  lcd.print("Nemlig: " + String(Nemligcurrent) + " kr"); 
  
} else if (Nemligcurrent <= Coopcurrent) {
  lcd.clear();
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("Loeg");
  lcd.setCursor(0,1);
  lcd.print("Nemlig: " + String(Nemligcurrent) + " kr"); 

} else if (Coopcurrent < Nemligcurrent) {
  lcd.clear();
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("Loeg");
  lcd.setCursor(0,1);
  lcd.print("Coop: " + String(Coopcurrent) + " kr"); 

} else {
  lcd.clear();
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("No data recieved");
  lcd.setCursor(0,1);
  lcd.print("EROOR002"); 
  Serial.println(String(Coopcurrent));
  Serial.println(String(Nemligcurrent));
  Serial.println(String(zero));
}
}

