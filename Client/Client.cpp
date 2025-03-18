#include "Client.h"

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
	inet_pton(AF_INET, "127.0.0.1", &sockaddr.sin_addr);

	SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		std::cout << "Failed connect to server!" << std::endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	std::cout << "Connected to server\n";

	system("pause");
	return 0;
}