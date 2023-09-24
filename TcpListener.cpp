#include "TcpListener.h";
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

int TcpListener::init()
{
	// Ініціалізація winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		return wsOk;
	}

	// Створюємо сокет
	i_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (i_socket == INVALID_SOCKET) {
		return WSAGetLastError();
	}

	// Привязуємо ip адресу і порт до сокету
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(i_port);
	inet_pton(AF_INET, i_ipAddress, &hint.sin_addr);

	if (bind(i_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
		return WSAGetLastError();
	}

	// Заставляємо сокет слухати цю адресу і порт
	if (listen(i_socket, SOMAXCONN) == SOCKET_ERROR) {
		return WSAGetLastError();
	}

	FD_ZERO(&i_master);
	FD_SET(i_socket, &i_master);
	return 0;
}

int TcpListener::run()
{
	while (true) {
		// копіюємо наш master тому що select змінює дані в ньому, а ми не хочемо втрачати наш listening socket
		fd_set copy = i_master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET socket = copy.fd_array[i];
			if (socket == i_socket) {
				// Приймаємо конекшн від клієнта
				SOCKET client = accept(i_socket, nullptr, nullptr);
				// Записуємо його в наш fd_set
				FD_SET(client, &i_master);
				onClientConnected(client);
			}
			else {
				char buffer[4096];
				ZeroMemory(buffer, 4096);

				// Отримуємо повідомлення від клієнта, якщо 0, то значить що клієнт відключився, очищаємо його
				int receivedBytes = recv(socket, buffer, 4096, 0);
				if (receivedBytes <= 0) {
					onClientDisconnected(socket);

					closesocket(socket);
					FD_CLR(socket, &i_master);
				}
				else {
					// Відправляємо всім клієнтам повідомлення яке надіслав вхідний клієнт
					onMessageReceived(socket, buffer, receivedBytes);
				}
			}
		}
	}
	FD_CLR(i_socket, &i_master);
	closesocket(i_socket);

	// Виключення winsock
	WSACleanup();
	return 0;
}


void TcpListener::sendToClient(int clientSocket, const char* msg, int length) {
	send(clientSocket, msg, length + 1, 0);
};

void TcpListener::broadcastToClients(int sendingClient, const char* msg, int length) {
	for (int i = 0; i < i_master.fd_count; i++) {
		SOCKET outSocket = i_master.fd_array[i];
		if (outSocket != i_socket && outSocket != sendingClient) {
			sendToClient(outSocket, msg, length);
		}
	}
};

void TcpListener::onClientConnected(int clientSocket) {};

void TcpListener::onClientDisconnected(int clientSocket) {};

void TcpListener::onMessageReceived(int clientSocket, const char* msg, int length) {};