#include "Server.h"

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

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	if (sListen == INVALID_SOCKET)
		error_message("socket");

	if (bind(sListen, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
		error_message("bind");

	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR)
		error_message("listen");

	int sockSize = sizeof(sockaddr);
	SOCKET newConnection = accept(sListen, (SOCKADDR*)&sockaddr, &sockSize);
	if (newConnection == INVALID_SOCKET) {
		error_message("accept");
	}
	else {
		std::cout << "Client connected\n";
	}

	system("pause");
	return 0;
}

void error_message(const char* nameFunction) {
	std::cout << nameFunction << "function failed with error = " << WSAGetLastError() << std::endl;
	WSACleanup();
	exit(EXIT_FAILURE);
}