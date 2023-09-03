#pragma once
//#include"Func.h"
#include"mysql.h"
#include<string>
#include<iostream>

class DBWorker
{
    MYSQL mysql;
    MYSQL_RES* res;
    MYSQL_ROW row;

public:
    DBWorker();
    ~DBWorker();

    void openData();
    void createDataTable();
    bool insertDataUser(const std::string& name, const std::string& login, const std::string& password);
    bool insertDataAdmin(const std::string& name, const std::string& login, const std::string& password);
    bool verificationDataUser(const std::string& login, const std::string& password);
    bool insertDataMessage(const std::string& from, const std::string& to, const std::string& text);
    void userLogin(const std::string& login);
    void userLogout(const std::string& login);
    void deleteUser(const std::string& login);
    std::string readDataUsersLogin(const std::string& login);
    std::string getAllMessages();
    std::string getMessagesBetweenUsers(const std::string& login, const std::string& login2);
    bool isUserExist(const std::string& login_to);
    bool isUserLoggedIn(const std::string& login_to);
    std::string readUserInfo(const std::string& login_to, const std::string& type);
    std::string getSharedMessages();
    std::string readDataMessagesCount();
    void closeData();
};
