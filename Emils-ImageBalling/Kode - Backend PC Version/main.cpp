#include "WebServer.h"
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fstream>

#include <vector>
#include <valarray>
#include <complex>

#include <limits>
#include <algorithm>

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