#define CURL_STATICLIB
#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <iterator>
#include <algorithm>

#include "WebServer.h"
#include "curl/curl.h"

#ifdef _DEBUG
#	pragma comment (lib, "curl/libcurl.lib")
#else
#	pragma comment (lib, "curl/libcurl_a.lib")
#endif

using namespace std;

std::string contents;
std::string token;

static int writer(char* data, size_t size, size_t nmemb,
	std::string* writerData)
{
	if (writerData == NULL)
		return 0;

	writerData->append(data, size * nmemb);

	return size * nmemb;
}

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

void ScrapeWebsite() {
	
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = nullptr;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.edoganci.dk/");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &contents);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	//std::cout << contents << endl;
}

void LookForData() {
	std::string startDelim = "<img src=\"";
	std::string stopDelim = "\" class=";
	token = get_str_between_two_str(contents, startDelim, stopDelim);
}

void SendDataToJS() {

}

void WebServer::onMessageReceived(int clientSocket, const char* msg, int length)
{
	cout << "Received message from client" << endl;
	cout << "Parsing out the client's request string" << endl;
	std::istringstream iss(msg);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	
	std::string webPage = "<h1>404 Not Found</h1>";
	std::string htmlFile = "/index.html";
	int errorCode = 404;

	cout << "Getting name of GET request" << endl;
	if (parsed.size() >= 3 && parsed[0] == "GET")
	{
		htmlFile = parsed[1];

		cout << "Received no index reference, returning to index.html" << endl;
		if (htmlFile == "/")
		{
			htmlFile = "/index.html";
		}
	}

	cout << "Finding root folder" << endl;
	std::ifstream f(".\\wwwroot" + htmlFile);

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
	
	ScrapeWebsite();
	LookForData();

}

void WebServer::onClientConnected(int clientSocket){}
void WebServer::onClientDisconnected(int clientSocket){}