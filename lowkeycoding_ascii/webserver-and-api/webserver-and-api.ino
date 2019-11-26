// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <string>
const char* ssid     = "INSERT_SSID";
const char* password = "INSERT_PASSWORD";

ESP8266WebServer server(80);
HTTPClient http;

void handleRoot() {
 String s = "<HTML> <head> <script> const ascii = () => { var string = document.getElementById('text').value; var xhr = new XMLHttpRequest(); xhr.open('GET', '/art?string=' + string + '&charlen=20', true); xhr.onload = function (e) { if (xhr.readyState === 4) { if (xhr.status === 200) { console.log(xhr.responseText); document.getElementById('result-value').innerText=xhr.responseText; } else { console.error(xhr.statusText); } } }; xhr.onerror = function (e) { console.log(xhr.statusText); }; xhr.send(null); } </script> <script src='https://kit.fontawesome.com/74596594bf.js' crossorigin='anonymous'></script> <style> body { font-family: 'Montserrat', sans-serif; padding: 0; margin: 0; background-image: linear-gradient(to right, #cb2d3e, #ef473a); } .container { position: relative; top: 50%; text-align: center; } .head { color: white; font-size: 28px; line-height: 40px; text-align: center; }  .searchBox { display: inline-block; background: #2f3640; height: 40px; border-radius: 40px; padding: 10px; margin-bottom: 20px; } .searchInput{ width:0px; } .result:empty { display: none; } pre { text-align: center; margin: 0 auto; background: #2f3640; border-radius: 40px; padding-left: 20px; padding-right: 20px; color: #00FF00; width: 80%; white-space: pre; text-align : left; margin-bottom: 20px; } pre:empty { display: none; } .searchBox:hover>.searchInput { width: 240px; padding: 0 6px; }  .searchBox:hover>.searchButton { background: white; color: #2f3640; }  .searchInput:focus { width: 240px; padding: 0 6px; }  .searchInput:focus>.searchButton { background: white; color: #2f3640; }  .searchButton { color: white; float: right; width: 40px; height: 40px; border-radius: 50%; background: #2f3640; display: flex; justify-content: center; align-items: center; transition: 0.4s; }  .searchInput { border: none; background: none; outline: none; float: left; padding: 0; color: #00FF00; font-size: 16px; transition: 0.4s; line-height: 40px; width: 0px; }  @media screen and (max-width: 620px) { .searchBox:hover>.searchInput { width: 150px; padding: 0 6px; } .searchInput:focus { width: 150px; padding: 0 6px; } } </style> <style> @import url('https://fonts.googleapis.com/css?family=Montserrat&display=swap'); </style> </head> <body> <div class='container'> <div class='head'> <h1>Ascii art generator</h1> </div> <div class='searchBox'> <input class='searchInput' id='text' type='text' name='' placeholder='Input text'> <button class='searchButton' onclick='ascii()'> <i class='fas fa-long-arrow-alt-right'></i> </button> </div> <div class='result'> <pre id='result-value'></pre> </div> </div> </body> </HTML>";
 server.send(200, "text/html", s); //Send web page
}

void handleArt() {
  int charlen = 27;
  String string;
  string = server.arg(0);
  charlen = server.arg(1) != "" ? server.arg(1).toInt() : charlen;
  if(string.length()+1 < charlen){
    getArt(string);
  } else {
   int amount = floor(((float)string.length()+1)/charlen);
   for(int i = 0; i  < amount+1; i++){
    String sf = "";
    if((string.length()+1)-i*charlen < charlen){
      sf = string.substring(i*charlen, string.length()+1);
    } else {
      sf = string.substring(i*charlen, (i*charlen)+charlen);
    }
    Serial.println("String: " + sf);
    getArt(sf);
   }
  }
}
void setup(void){
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/art", handleArt);
  server.begin();
  Serial.println("HTTP server started");
}
void loop(void){
  server.handleClient();
}
String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
    
}
void getArt(String text){
    http.begin("http://artii.herokuapp.com/make?text="+urlencode(text));  //Specify request destination
    int httpCode = http.GET();
    if(httpCode>0){
      String s = http.getString();
      //server.send(200, "text/plain", s);
      server.sendContent(s+"\r\n");
    }  
}
unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}