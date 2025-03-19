#include "Server.h"
#define MAX_CONNECTIONS 100

void Error_message(const char* nameFunction);
void Client_handler(std::size_t index);

std::vector<SOCKET> connections;
std::size_t curr_connections = 0;

int main(int argc, char* arcv[])
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		std::cout << "WSAStartup error" << std::endl;
		exit(EXIT_FAILURE);
	}

	SOCKADDR_IN sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(1111);
	if (inet_pton(AF_INET, "127.0.0.1", &sockaddr.sin_addr) <= 0)
		Error_message("inet_pton");

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	if (sListen == INVALID_SOCKET)
		Error_message("socket");

	if (bind(sListen, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
		Error_message("bind");

	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR)
		Error_message("listen");

	int sockSize = sizeof(sockaddr);
	SOCKET newConnection;
	
	for (std::size_t i = 0; i < MAX_CONNECTIONS; ++i) {
 		newConnection = accept(sListen, (SOCKADDR*)&sockaddr, &sockSize);

		if (newConnection == INVALID_SOCKET) {
			Error_message("accept");
		}
		else {
			std::cout << "Client " << curr_connections << " connected!\n";

			connections.emplace_back(newConnection);
			++curr_connections;
		
			std::thread th(Client_handler, i);
			th.detach();
		}
	}

	for (SOCKET conn : connections)
		closesocket(conn);
	WSACleanup();
	system("pause");
	return 0;
}

void Error_message(const char* nameFunction) {
	std::cout << nameFunction << "function failed with error = " << WSAGetLastError() << std::endl;
	WSACleanup();
	exit(EXIT_FAILURE);
}

void Client_handler(std::size_t index) {
	std::size_t msg_lenght;
	std::string sender = "Client " + index + ':';

	while (true) {
		recv(connections[index], (char *)&msg_lenght, sizeof(msg_lenght), NULL);

		char* msg = new char[msg_lenght + 1];
		msg[msg_lenght] = '\0';

		recv(connections[index], msg, msg_lenght + 1, NULL);
		
		for (size_t i = 0; i < curr_connections; ++i) {
			if (i == index)
				continue;
			send(connections[i], sender.c_str(), sizeof(sender), NULL);
			send(connections[i], (char *)&msg_lenght, sizeof(msg_lenght), NULL);
			send(connections[i], msg, msg_lenght, NULL);
		}

		delete[] msg;
	}
}