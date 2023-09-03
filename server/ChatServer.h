#pragma once
#include <iostream>
#include "NetworkServer.h"
#include "DBWorker.h"
#include "mysql.h"

class ChatServer
{
public:
	ChatServer();
	~ChatServer();

	void startServer();


private:
	std::vector<std::string> split(const std::string& s, char seperator);
	std::string commandHandler(const std::string& input);
	NetworkServer _server;
	DBWorker _dataBase;
	size_t _networkStatus{};
};

