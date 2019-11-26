#include <LiquidCrystal_I2C.h>
#include "ESP8266WiFi.h"
WiFiClient client;
#include <Wire.h>
#include <rgb_lcd.h>
#include "rgb_lcd.h"


const int ledPin = D4;

unsigned long goalMillis = 30000;

unsigned long previousMillis = 0;
float interval = 0;
int ledState = LOW;
int saved_time = 0;
unsigned long currentMillis = 0;

const int colorR = 30;
const int colorG = 30;
const int colorB = 200;

rgb_lcd lcd;


void setup() {
  Serial.begin(57600);
  pinMode(ledPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.print("Iz working");

}

void loop() {


  int n = scan();
  Serial.println(n);

  interval = goalMillis / n;
  Serial.println(interval);

  while (1) {
    yield();
    currentMillis = millis() - saved_time;

    if (currentMillis >= goalMillis) {
      Serial.println("Got this far cheif");
      saved_time = millis();
      break;
    }


    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;



      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }

      // set the LED with the ledState of the variable:
      digitalWrite(ledPin, ledState);




    }
  }
}

int scan() {
  Serial.println("");
  Serial.println("[*]Scan start");
  WiFi.disconnect();
  delay(100);
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  if (n == 0)
    Serial.println("[*] No networks found");
  else
  {
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(WiFi.SSID(i));
      delay(10);
    }
  }
  Serial.println("[*] Scan done");
  Serial.print("[*] Numbers of networks: **");
  Serial.print(n);
  Serial.print("**\n");
  lcd.clear();
  lcd.print(n);
  return n;
}

String encryptionTypeStr(uint8_t authmode) {
  switch (authmode) {
    case ENC_TYPE_NONE:
      return "NONE";
    case ENC_TYPE_WEP:
      return "WEP";
    case ENC_TYPE_TKIP:
      return "TKIP";
    case ENC_TYPE_CCMP:
      return "CCMP";
    case ENC_TYPE_AUTO:
      return "AUTO";
    default:
      return "?";;
  }
}
/*Code lend from:
 * https://blog.robberg.net/category/arduino/
 * https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
 */
