#pragma once
#include <iostream>
#include <iomanip>
#include <memory>
#include "User.h"
#include <vector>

#include "NetworkClient.h"
#include "NetworkServer.h"

class Chat
{
public:
	Chat();
	~Chat();

	void startChat();

	void createUser(const std::string& name, const std::string& login, const std::string& password);
	void createAdminUser(const std::string& name, const std::string& login, const std::string& password);
	void deleteUser(const std::string& login);
	void sendMessageTo(const std::string& login, const std::string& message);
	void sendMessageToAll(const std::string& message);
	std::vector<std::string> printUsersList();
	void printMessages();
	bool login(const std::string& login, const std::string& password);
	void logOff();
	void justExit();
	bool isCurrentUserLogedIn();
	bool isCurrentUserAdmin() const;
	bool isUserExist(const std::string& login);
	size_t getUsersCount() const;
	size_t getMessagesCount() const;
	void printMessagesForUser(const std::string& login);
	void printMessagesForAdmin();
	std::string sendMessageToServer(const std::string& request);
	void updateUserInfo(const std::string& login);
	std::vector<std::string> split(const std::string& s, char seperator);
	
private:
	User _currentUser;
	NetworkClient _client;
	size_t _networkStatus{};

};

