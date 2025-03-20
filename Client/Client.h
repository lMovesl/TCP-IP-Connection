#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <thread>
#include <string>

void Error_message(const char* name_function);
void Client_handler();
