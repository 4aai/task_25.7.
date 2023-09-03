#pragma once
#include "consts.h"

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class NetworkClient
{
public:
	//NetworkClient(const SOCKET& SOCKET);
	NetworkClient();
	~NetworkClient();
	int reConnect();
	int Connect();
	int Read(std::string& message);
	int Write(const std::string& message);
	int Shutdown();
private:
	std::vector<std::string> _history;
	WSADATA _WSADATA;
	int _iResult = 0;
	SOCKET _C_SOCKET = 0;
	struct sockaddr_in _C_ADRESS {};
	char _R_BUF[BUF]{};

};

