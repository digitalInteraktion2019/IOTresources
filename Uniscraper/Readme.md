# UniScraper
###### Af Daniel, Mathias og Samuel 

#### Ide pitch:
Vi har fundet ud af, at selvbetjening er meget let at udnytte, og ville derfor lave et program, som scrapede dem for deres data.

#### Oversigt:
- "Scraper" fra selvbetjening.aarhustech.dk
- LCD-skærm til at vise data
- Rotary Encoder til at indtaste navn
- Push button til at sætte alt igang

#### Steps:
- Brugeren: Vælger navn, via. sensor samt LCD-skærm
- Arduino: Sender navnet via. en POST request til selvbetjening.aarhustech.dk API (måske/måske ikke uden tilladelse)
- API: Sender response med en liste over personer der har navnet som blev sendt samt data over personerne (I det brugeren vælger et fuldt navn, sender serveren kun en person tilbage i response)
- Arduino: Filtrerer det modtagne data for det sendte navn's Uni-Login navn, herefter skrives Uni-Login navnet på LCD-Skærmen
- Brugeren: ***Udnytter Uni-Login navn***

#### Sourcecode:

Setup:
``` c
//Tilslutter vores ESP8266 D1 mini til vores Wifi
WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
```


Loop:
``` c
//Payload
String input = "{\"prefixText\":\"" + nameSelected + "\",\"count\":150,\"contextKey\":\"all|0|0\"}";
  
//POST request
client.print(String("POST ") + page + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json;charset=UTF-8" + "\r\n" +
               "Connection: close\r\n" +
               "Content-Length: " + input.length() + "\r\n\r\n" +
               input + "\r\n");

//Sætter data = sidste besked i Response (som altid er en lang JSON string med data)
while (client.connected()) {
      data = client.readString();
    }
    Serial.print(data);
```

#### Data Eksempel:
![](https://github.com/digitalInteraktion2019/IOTresources/blob/master/Uniscraper/Ressourcer/DataEksempel.PNG)