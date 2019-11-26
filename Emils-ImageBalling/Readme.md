#Emils-ImageBalling

### Features

- Scraper en hjemmeside (https://www.edoganci.dk).
- Finder noget specifikt (I det her tilfælde billedet på forsiden af hjemmesiden).
- Erstatter webserverens tittel med linket til billedet.
- Hjemmesidens javascript lægger billedet i baggrunden af en canvas.
- Bolden bevæger i canvas og viser et udsnit af billedet.

### Description
Da jeg ikke havde nok til at eksperimentere med den udleverede ESP8266, så valgte jeg istedet at lave en webserver på computeren og udføre opgaven på præcist samme måde, bare uden en ESP8266. Princippet gælder dog stadig for ESP8266 og mit arbejde kan sagtens implementeres til enheden. 

### Demonstration
- Resultat:

![](https://github.com/digitalInteraktion2019/IOTresources/blob/master/Emils-ImageBalling/Ressourcer/Demo.gif)


### Utilities
- Visual Studio 2019
- Curl (https://curl.haxx.se/)
- Frontend (https://freefrontend.com/)
- Html/Css/Javascript
- Indbygget WinHttpClient bibliotek (WebServer)

### Code Samples
1) Det her er funktionen som scraper hjemmeside med curl:
```c++
void ScrapeWebsite() {
	CURL* curl = nullptr;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.edoganci.dk/");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &contents);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}
```

2) Her sorteres dataen:
```
std::string get_str_between_two_str(const std::string& s,
	const std::string& start_delim,
	const std::string& stop_delim)
{
	unsigned first_delim_pos = s.find(start_delim);
	unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
	unsigned last_delim_pos = s.find(stop_delim);

	return s.substr(end_pos_of_first_delim,
		last_delim_pos - end_pos_of_first_delim);
}

void LookForData() {
	std::string startDelim = "<img src=\"";
	std::string stopDelim = "\" class=";
	token = get_str_between_two_str(contents, startDelim, stopDelim);
}
```
3) Her bliver dataen lagt ind på titlen af webserveren og derefter bliver sendt til webserveren:
```
	cout << "Checking if folder opened successfully" << endl;
	if (f.good())
	{
		cout << "Folder opened successfully\n" << endl;
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		std::string sendData = "<title>" + token + " </title>";
		webPage = sendData + str;
		cout << "Grabbed all contents including:\n==========================================================\n" 
			<< webPage << "\n==========================================================\n" << endl;
		errorCode = 200;
	}

	f.close();

	cout << "Sending index.html to client" << endl;
	std::ostringstream oss;
	oss << "HTTP/1.1 " << errorCode << " OK\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: " << webPage.size() << "\r\n";
	oss << "\r\n";
	oss << webPage;
	cout << "Sent the index.html to client" << endl;

	std::string output = oss.str();
	int size = output.size() + 1;

	sendToClient(clientSocket, output.c_str(), size);
```

4) Her bliver dataen benyttet af javascriptet på webserveren og billedet bliver brugt som baggrundsbillede til canvas.
```
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
5) Her er vores main entrypoint til programmet, hvor selve programmet starter op:
```
void main()
{
	int myPort = 8080;
	const char* myActualIP = "192.168.1.227";

	WebServer webServer(myActualIP, myPort);
	std::cout << "Server started on: " << myActualIP << ":" << myPort << std::endl;

	if (webServer.init() != 0)
		return;

	webServer.run();

	system("pause");
}
```