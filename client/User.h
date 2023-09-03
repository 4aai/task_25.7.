#pragma once
#include <iostream>
#include <string>

class User
{
public:
	User();
	~User();
	std::string getName() const;
	std::string getLogin() const;
	void setName(const std::string& name);
	void setLogin(const std::string& login);

	bool isUserAdmin() const;
	void setUserAdmin(bool admin);
	void clear();
private:
	bool _isAdmin = false;
	std::string _name;
	std::string _login;
};

