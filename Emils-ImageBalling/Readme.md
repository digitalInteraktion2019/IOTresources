##Emils-ImageBalling

### Idé:

- Scraper en hjemmeside (https://www.edoganci.dk).
- Finder noget specifikt (I det her tilfælde billedet på forsiden af hjemmesiden).
- Erstatter webserverens tittel med linket til billedet.
- Webserverens javascript lægger billedet i baggrunden af en canvas.
- Bolden bevæger i canvas og viser et udsnit af billedet.

### Description:
Jeg har både lavet en PC version der kører webserveren og udføre præcist samme opgave og derudover har jeg også implementeret det til Arduino IDE og udført opgaven med esp8266 enheden der blev udleveret.

Nederst på siden er der udsnit af de vigtigste funktioner af kodet og en lille forklaring til hvert udsnit. 

### Demonstration:
- Resultat:

![](https://github.com/digitalInteraktion2019/IOTresources/blob/master/Emils-ImageBalling/Ressourcer/Demo.gif)


### Utilities - PC Version:
- Visual Studio 2019
- Curl (https://curl.haxx.se/)
- Frontend (https://freefrontend.com/)
	- Html/Css/Javascript
- Indbygget WinHttpClient bibliotek (WebServer)

### Utilities - ESP8266 Version:
- Arduino IDE v2
- string, iostream, ESP8266WebServer, ESP8266HTTPClient, ESP8266Wifi libraries
- Frontend ((https://freefrontend.com/))
	- Html/Css/Javascript

### Installation of PC Version:
Guide til installation af programmet:
- Krav:
1) Windows
2) Visual Studio 2019 installeret

- Guide: 
1) Installer filerne ned på computeren
2) Start Visual Studio 2019
3) Inkluder curl/curl.h i Webserver.cpp filen
4) Gå ind på Project -> Linker -> Additional Dependencies og add følgende: 
libcurl.lib
Normaliz.lib
Ws2_32.lib
Wldap32.lib
Crypt32.lib
advapi32.lib
5) Ændre IP til din egen IPv4 addresse, som du kan finde ved at åbne command prompt og skrive "ipconfig"
6) Compile og kør programmet
7) Konsollen ville vise hvilken addresse serveren er startet på, bare kopier det og paste det ind på browseren. 

### Code Samples for Arduino IDE
1) Det her er funktionen som scraper hjemmeside med curl:
```c++
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
```

2) Her sorteres dataen:
``` c++
String get_my_string(String startDelim, String stopDelim, String myString){
  int firstChar = myString.indexOf(startDelim) + startDelim.length();
  int secondChar = myString.indexOf(stopDelim);

  return myString.substring(firstChar, secondChar);
}

void filterData(){
  token = get_my_string("<img src=\"", "\" class=", webPage);
  actualToken = "<title>" + token + " </title>";
  Serial.println("token: " + token);
  Serial.println("actual token" + actualToken);
}
```
3) Her bliver dataen lagt ind på titlen af webserveren og derefter bliver sendt til webserveren:
``` c++
void filterData(){
  token = get_my_string("<img src=\"", "\" class=", webPage);
  actualToken = "<title>" + token + " </title>";
  Serial.println("token: " + token);
  Serial.println("actual token" + actualToken);
}

void createWebServer(){
  webServer.send(200, "text/html", actualToken + html);
}
```

4) Her bliver dataen benyttet af javascriptet på webserveren og billedet bliver brugt som baggrundsbillede til canvas.
``` javascript
	var imglink = "http://www.edoganci.dk/" + document.title;
	var actualImg = new Image();
	var pattern;
	function init()
	  {
	    context = myCanvas.getContext('2d');
	    setInterval(draw, 10);
	    actualImg.src = imglink;
	    actualImg.onload = function(){
	    	pattern = context.createPattern(this, "repeat");
		    context.beginPath();
		    
		    context.fillStyle=pattern;
		    context.arc(100,100,75,0,Math.PI*2,true); context.closePath();
		    context.fill();
	    }
	  }
	function draw()
	{
		 context.clearRect(0,0, canvasX,canvasY);
		 context.beginPath();
		 context.fillStyle = pattern;
		 context.arc(x,y,75,0,Math.PI*2,true);
		 context.closePath();
		 context.fill();
		if( x<0 || x>canvasX) dx=-dx; 
		if( y<0 || y>canvasY) dy=-dy; 
		x+=dx; 
		y+=dy;
	}
```