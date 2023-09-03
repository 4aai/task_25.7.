#include "NetworkServer.h"


NetworkServer::NetworkServer()
{
    _iResult = WSAStartup(MAKEWORD(2, 2), &_WSADATA);
}

NetworkServer::~NetworkServer()
{
    closesocket(_C_SOCKET);
    WSACleanup();
}


int NetworkServer::Listen()
{
    if (_iResult != 0)
    {
        std::cout << "[Server WSAStartup error]: " << _iResult << std::endl;
        return 1;
    }

    _L_ADRESS.sin_addr.s_addr = htonl(INADDR_ANY);
    _L_ADRESS.sin_family = 2;
    _L_ADRESS.sin_port = htons(PORT);
    _L_SOCKET = socket(2, 1, 0);
    std::cout << "[socket Listen] " << _L_SOCKET << std::endl;
    if (_L_SOCKET == -1)
    {
        std::cout << "[создание socket error] " << _L_SOCKET << std::endl;
        return 1;
    }
    _iResult = bind(_L_SOCKET, (struct sockaddr*)&_L_ADRESS, sizeof(_L_ADRESS));
    std::cout << "[bind Listen] " << _iResult << std::endl;
    if (_iResult == -1)
    {
        std::cout << "[bind error] " << _iResult << std::endl;
        closesocket(_L_SOCKET);
        return 1;
    }
    _iResult = listen(_L_SOCKET, 5);
    std::cout << "[listen Listen] " << _iResult << std::endl;
    if (_iResult == -1)
    {
        std::cout << "[listen error] " << _iResult << std::endl;
        closesocket(_L_SOCKET);
        return 1;
    }
    _C_SOCKET = accept(_L_SOCKET, 0, 0);
    std::cout << "[accept Listen] " << _iResult << std::endl;
    if (_C_SOCKET == -1)
    {
        std::cout << "[accept error] " << _iResult << std::endl;
        closesocket(_L_SOCKET);
        return 1;
    }
    closesocket(_L_SOCKET);
    return 0;
}

int NetworkServer::Read(std::string& message)
{
    _iResult = recv(_C_SOCKET, _R_BUF, sizeof(_R_BUF), 0);
    std::cout << "[recv Read] " << _iResult << std::endl;
    std::string str;
    if (_iResult <= 0)
    {
        std::cout << "[recv error] " << _iResult << std::endl;
        return 1;
    }
    else
    {
        str = std::string(_R_BUF);
        if (str == "exit")
        {
            std::cout << "[connection closed]" << std::endl;
            return 1;
        }
    }

    message = str;
    memset(_R_BUF, 0, sizeof(_R_BUF));
    return 0;
}

int NetworkServer::Write(const std::string& message)
{
    std::string str;

        str = message;
    strcpy_s(_R_BUF, str.c_str());

    _iResult = send(_C_SOCKET, _R_BUF, sizeof(_R_BUF), 0);
    std::cout << "[send Write] " << _iResult << std::endl;
    if (_iResult <= 0)
    {
        std::cout << "[send error] " << _iResult;
        return 1;
    }
    memset(_R_BUF, 0, sizeof(_R_BUF));
    if (message == "exit") return 1;

    return 0;
}

int NetworkServer::Shutdown()
{
    _iResult = shutdown(_C_SOCKET, 0x01);
    std::cout << "[shutdown] " << _iResult << std::endl;
    if (_iResult == -1)
    {
        std::cout << "[shutdown error] " << _iResult << std::endl;
        _iResult = 0;
        return 1;
    }
    return 0;
}
