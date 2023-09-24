#include "MultiClientChat.h"
#include <string>
#include <iostream>

using namespace std;

void MultiClientChat::onClientConnected(int clientSocket) {
	string msg = "SOCKET #" + to_string(clientSocket) + " connected\r\n";
	cout << msg;
	broadcastToClients(clientSocket, msg.c_str(), msg.size());

	// Відсилаємо привітання з підключенням
	string welcomeMessage = "Welcome!\r\n";
	sendToClient(clientSocket, welcomeMessage.c_str(), welcomeMessage.size());
};

void MultiClientChat::onClientDisconnected(int clientSocket) {
	string msg = "SOCKET #" + to_string(clientSocket) + " disconnected\r\n";
	cout << msg;
	broadcastToClients(clientSocket, msg.c_str(), msg.size());
};

void MultiClientChat::onMessageReceived(int clientSocket, const char* msg, int length) {
	// Відправляємо всім клієнтам повідомлення яке надіслав вхідний клієнт
	string response = "SOCKET #" + to_string(clientSocket) + " " + msg + "\r\n";
	broadcastToClients(clientSocket, response.c_str(), response.size());
};