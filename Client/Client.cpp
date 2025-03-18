#include "Client.h"

void error_message(const char* nameFunction);

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
		error_message("inet_pton");

	SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connection == INVALID_SOCKET)
		error_message("socket");

	if (connect(connection, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
		error_message("connect");

	std::cout << "Connected to server\n";

	system("pause");
	return 0;
}

void error_message(const char* nameFunction) {
	std::cout << nameFunction << "function failed with error = " << WSAGetLastError() << std::endl;
	WSACleanup();
	exit(EXIT_FAILURE);
}