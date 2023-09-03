#include "ChatServer.h"

ChatServer::ChatServer()
{
    _dataBase.openData();
    _dataBase.createDataTable();
    
}

ChatServer::~ChatServer()
{
    _dataBase.closeData();
    _server.Shutdown();
}

void ChatServer::startServer()
{
    std::cout << "Hello World!\n";
   
    std::string message;
    std::string response;
    std::cout << "Start server...\n";
    _networkStatus = _server.Listen();
    while (!_networkStatus) {

        if (_networkStatus == 0)  _networkStatus = _server.Read(message);

        std::cout << "\t" << message << " >>\t";
        response = commandHandler(message);
        std::cout << "<< " << response << "\n";

        if (_networkStatus == 0)  _networkStatus = _server.Write(response);


    }
    
}

std::vector<std::string> ChatServer::split(const std::string& s, char seperator)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));
        output.push_back(substring);
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos - prev_pos));
    return output;
}
std::string ChatServer::commandHandler(const std::string& input)
{
    std::vector<std::string> args = split(input, ':');

    if ("isUserExist" == args[0]) {
        if (_dataBase.isUserExist(args[1])) {
            return "true";
        }
        return "false";
    }
    else if ("isUserLogedIn" == args[0]) {
        if (_dataBase.isUserLoggedIn(args[1])) {
            return "true";
        }
        return "false";
    }
    else if ("login" == args[0])
    {
        if (_dataBase.verificationDataUser(args[1], args[2]))
        {
            _dataBase.userLogin(args[1]);
            return "true";
        }
        return "false";
    }
    else if ("logout" == args[0])
    {
        _dataBase.userLogout(args[1]);
        return "logout:" + args.at(1);
    }
    else if ("regUser" == args[0])
    {
        _dataBase.insertDataUser(args[1], args[2], args[3]);
        return "regUser:" + args[1] + args[2] + args[3];
    }
    else if ("delUser" == args[0])
    {
        _dataBase.deleteUser(args[1]);
        return "delUser:" + args[1];
    }
    else if ("regAdmin" == args[0])
    {
        _dataBase.insertDataAdmin(args[1], args[2], args[3]);
        return "regAdmin:" + args[1] + args[2] + args[3];
    }
    else if ("sendMessage" == args[0])
    {
        return "sendMessage: " + _dataBase.insertDataMessage(args[1], args[2], args[3]);
    }
    else if ("getMessages" == args[0])
    {
        return _dataBase.getMessagesBetweenUsers(args[1], args[2]);
    }
    else if ("getAllMessages" == args[0])
    {
        return _dataBase.getAllMessages();
    }
    else if ("getSharedMessages" == args[0])
    {
        return  _dataBase.getSharedMessages();
    }
    else if ("getUsersCount" == args[0])
    {
        return std::to_string(split(_dataBase.readDataUsersLogin("all"), ':').size());
    }
    else if ("getMessagesCount" == args[0])
    {
        std::string tmp = _dataBase.readDataMessagesCount();
        if(!tmp.empty())
            return std::to_string(split(tmp, ':').size());
        else
            return "0";

    }
    else if ("getUsers" == args[0])
    {
        return _dataBase.readDataUsersLogin(args[1]);
    }
    else if ("getUserInfo" == args[0])
    {
        if ("name" == args[2])
        {
            return _dataBase.readUserInfo(args[1], args[2]);
        }
        else if ("user_type" == args[2])
        {
            return _dataBase.readUserInfo(args[1], args[2]);
        }
        else if ("user_password" == args[2])
        {
            return _dataBase.readUserInfo(args[1], args[2]);
        }
        else if ("loggedin" == args[2])
        {
            return _dataBase.readUserInfo(args[1], args[2]);
        }
        return "error:unknown:" + args[2];
    }
    else if ("makedb" == args[0])
    {
        _dataBase.createDataTable();
        return "makedb:" + args.at(1);
    }

    return "commandHandler response " + args.at(0);
}
