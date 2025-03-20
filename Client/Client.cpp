#include "Client.h"

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

	std::cout << "Client connected to server\n";

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

void Error_message(const char* name_function) {
	std::cout << name_function << " function failed with error = " << WSAGetLastError() << std::endl;
	WSACleanup();
	exit(EXIT_FAILURE);
}

void Client_handler() {
	std::size_t msg_lenght = 0;
	std::size_t sender_lenght = 0;

	while (true) {
		//sender
		recv(connection, (char*)&sender_lenght, sizeof(sender_lenght), NULL);
		char* sender = new char[sender_lenght + 1];
		sender[sender_lenght] = '\0';
		recv(connection, sender, sender_lenght, NULL);

		//message
		recv(connection, (char *)&msg_lenght, sizeof(msg_lenght), NULL);
		char* msg = new char[msg_lenght + 1];
		msg[msg_lenght] = '\0';
		recv(connection, msg, msg_lenght, NULL);
		 
		std::cout << sender << ": " << msg << "\n";

		delete[] sender;
		delete[] msg;
	}
}