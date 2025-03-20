#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <thread>
#include <string>
#include <mutex>

void Error_message(const char* name_function);
void Client_handler(SOCKET conn);
