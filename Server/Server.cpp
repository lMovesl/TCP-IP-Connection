#include "Server.h"

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
	sockaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sockaddr.sin_port = htons(1111);

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&sockaddr, sizeof(sockaddr));
	listen(sListen, SOMAXCONN);

	int sockSize = sizeof(sockaddr);
	SOCKET newConnection = accept(sListen, (SOCKADDR*)&sockaddr, &sockSize);
	if (newConnection == INVALID_SOCKET) {
		std::cout << "Accept error\n";
	}
	else {
		std::cout << "Client connected\n";
	}

	system("pause");
	return 0;
}
