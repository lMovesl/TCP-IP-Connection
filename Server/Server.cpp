#include "Server.h"

std::vector<SOCKET> connections;

std::mutex mtx;

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

	while (true) {
		newConnection = accept(sListen, (SOCKADDR*)&sockaddr, &sockSize);

		if (newConnection == INVALID_SOCKET) {
			Error_message("accept");
		}
		else {
			std::cout << "User" << newConnection << " connected!\n";

			{
				std::lock_guard<std::mutex> lock(mtx);
				connections.emplace_back(newConnection);
			}

			std::thread th(Client_handler, newConnection);
			th.detach();
		}
	}

	for (SOCKET conn : connections)
		closesocket(conn);

	WSACleanup();
	system("pause");
	return 0;
}

void Error_message(const char* name_function) {
	std::cout << name_function << "function failed with error = " << WSAGetLastError() << std::endl;
	WSACleanup();
	exit(EXIT_FAILURE);
}

void Client_handler(SOCKET conn) {
	std::size_t msg_lenght = 0;
	std::size_t sender_lenght = 0;

	while (true) {
		if (recv(conn, (char*)&msg_lenght, sizeof(msg_lenght), NULL) == SOCKET_ERROR) {
			std::cout << "User" << conn << " disconnected\n";
			std::lock_guard<std::mutex> lock(mtx);
			closesocket(conn);
			connections.erase(std::find(connections.begin(), connections.end(), conn));
			return;
		}

		char* msg = new char[msg_lenght + 1];
		msg[msg_lenght] = '\0';

		recv(conn, msg, msg_lenght + 1, NULL);

		std::string sender = "user" + std::to_string(conn);
		sender_lenght = sender.size();

		{
			std::lock_guard<std::mutex> lock(mtx);
			for (SOCKET curr_conn : connections) {
				if (curr_conn == conn)
					continue;

				send(curr_conn, (char*)&sender_lenght, sizeof(sender_lenght), NULL);
				send(curr_conn, sender.c_str(), sender_lenght, NULL);

				send(curr_conn, (char*)&msg_lenght, sizeof(msg_lenght), NULL);
				send(curr_conn, msg, msg_lenght, NULL);
			}
		}

		delete[] msg;
	}
}