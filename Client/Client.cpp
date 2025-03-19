#include "Client.h"

void Error_message(const char* nameFunction);
void Client_handler();

SOCKET connection;

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

	connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connection == INVALID_SOCKET)
		Error_message("socket");

	if (connect(connection, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
		Error_message("connect");

	std::cout << "Connected to server\n";

	std::thread th(Client_handler);
	th.detach();

	std::string msg;
	while (true) {
		std::getline(std::cin, msg);
		std::size_t msg_lenght = msg.size();

		send(connection, (char*)&msg_lenght, sizeof(msg_lenght), NULL);
		send(connection, msg.c_str(), msg_lenght, NULL);
	}

	closesocket(connection);
	WSACleanup();
	system("pause");
	return 0;
}

void Error_message(const char* nameFunction) {
	std::cout << nameFunction << " function failed with error = " << WSAGetLastError() << std::endl;
	WSACleanup();
	exit(EXIT_FAILURE);
}

void Client_handler() {
	std::size_t msg_lenght;

	while (true) {
		recv(connection, (char *)&msg_lenght, sizeof(msg_lenght), NULL);
	
		char* msg = new char[msg_lenght + 1];
		msg[msg_lenght] = '\0';

		recv(connection, msg, msg_lenght, NULL);
		 
		std::cout << msg << "\n";

		delete[] msg;
	}
}