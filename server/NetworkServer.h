#pragma once

#define PORT                                34567
#define BUF                                 4094

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string>
#include <vector>
#pragma comment (lib, "Ws2_32.lib")

class NetworkServer
{
public:
	NetworkServer();
	~NetworkServer();
	int Listen();
	int Read(std::string& message);
	int Write(const std::string& message);
	int Shutdown();

private:
	WSADATA _WSADATA;

	int _iResult = 0;
	SOCKET _L_SOCKET = 0, _C_SOCKET = 0;
	sockaddr_in _L_ADRESS{}, _C_ADRESS{};
	char _R_BUF[BUF]{};


};

