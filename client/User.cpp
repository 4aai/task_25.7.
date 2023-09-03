#include "User.h"


User::User() = default;

User::~User() = default;


std::string User::getName() const
{
	return _name;
}

void User::setName(const std::string& name)
{
	_name = name;
}

std::string User::getLogin() const
{
	return _login;
}
void User::setLogin(const std::string& login)
{
	_login = login;
}



bool User::isUserAdmin() const
{
	return _isAdmin;
}

void User::setUserAdmin(bool admin)
{
	_isAdmin = admin;
}


void User::clear()
{
	_name.clear();
	_login.clear();
	_isAdmin = false;
	
}


