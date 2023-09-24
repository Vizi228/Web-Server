#pragma once

#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

class TcpListener {
public:
	// Конструктор
	TcpListener(const char* ipAddress, int port) :
		i_ipAddress(ipAddress), i_port(port) {}

	// Ініціалізація слухача
	int init();
	// Запуск слухача
	int run();
protected:
	virtual void onClientConnected(int clientSocket);
	virtual void onClientDisconnected(int clientSocket);
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);

	void sendToClient(int clientSocket, const char* msg, int length);
	void broadcastToClients(int sendingClient, const char* msg, int length);
private:
	const char* i_ipAddress; // Ip адреса на якій буде запущений наш сервер
	int i_port; // Порт сервера
	int i_socket; // Внутрішній File Descriptor для сокета слухача
	fd_set i_master; // Список всіх сокетів
};