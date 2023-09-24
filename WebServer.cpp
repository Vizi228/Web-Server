#include "WebServer.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

void WebServer::onMessageReceived(int clientSocket, const char* msg, int length) {
	istringstream iss(msg);
	vector<string> parsed((istream_iterator<string>(iss)), istream_iterator<string>());

	string htmlFile = "/index.html";
	string content = "<h1>404 Not Found</h1>";
	int errorCode = 404;

	if (parsed.size() >= 3 && parsed[0] == "GET" && parsed[1] != "/") {
		htmlFile = parsed[1];
	}

	ifstream f(".\\data" + htmlFile);
	if (f.good()) {
		string str((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
		content = str;
		errorCode = 200;
	}
	f.close();

	ostringstream ss;
	ss << "HTTP/1.1 " << errorCode << " OK\r\n";
	ss << "Cache-Control: no-cache, private\r\n";
	ss << "Content-Type: text/html\r\n";
	ss << "Content-Length: " << content.size() << "\r\n";
	ss << "\r\n";
	ss << content;
	string response = ss.str();

	sendToClient(clientSocket, response.c_str(), response.size());
};

void WebServer::onClientConnected(int clientSocket) {
	
};

void WebServer::onClientDisconnected(int clientSocket) {

};