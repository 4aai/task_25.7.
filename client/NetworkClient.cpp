#include "NetworkClient.h"


NetworkClient::NetworkClient()
{
    _iResult = WSAStartup(MAKEWORD(2, 2), &_WSADATA);
}

NetworkClient::~NetworkClient()
{
    closesocket(_C_SOCKET);
    WSACleanup();
}

int NetworkClient::reConnect()
{
    closesocket(_C_SOCKET);
    WSACleanup();
    _iResult = WSAStartup(MAKEWORD(2, 2), &_WSADATA);
    return 0;
}

int NetworkClient::Connect()
{
    if (_iResult != 0)
    {
        std::cout << "[Client WSAStartup error]: " << _iResult
            << std::endl;
        return 1;
    }

    _C_SOCKET = socket(2, 1, 0);
    if (_C_SOCKET == -1)
    {
        std::cout << "[create socket error]" << std::endl;
        return 1;
    }

    _C_ADRESS.sin_addr.s_addr = inet_addr(SERVER_HOST);
    _C_ADRESS.sin_family = AF_INET;
    _C_ADRESS.sin_port = htons(PORT);
    _iResult = connect(_C_SOCKET, (struct sockaddr*)&_C_ADRESS, sizeof(_C_ADRESS)); 

    if (_iResult == -1)
        _C_SOCKET = -1;
    if (_C_SOCKET == -1)
    {
        std::cout << "[servers not found]" << std::endl;
        return 1;
    }
    return 0;
}

int NetworkClient::Read(std::string& message)
{
    _iResult = recv(_C_SOCKET, _R_BUF, sizeof(_R_BUF), 0);
    std::string str;
    if (_iResult <= 0)
    {
        std::cout << "[recv error]" << std::endl;
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

int NetworkClient::Write(const std::string& message)
{
    std::string str;
    str = message;
    if (str.size() > BUF) str = str.substr(0, BUF -1 );
    strcpy_s(_R_BUF, str.c_str());

    _iResult = send(_C_SOCKET, _R_BUF, sizeof(_R_BUF), 0);
    if (_iResult <= 0)
    {
        std::cout << "[send error]";
        return 1;
    }
    memset(_R_BUF, 0, sizeof(_R_BUF));
    if (message == "exit") return 1;
    return 0;
}

int NetworkClient::Shutdown()
{
    _iResult = shutdown(_C_SOCKET, 0x01);
    if (_iResult == -1)
    {
        std::cout << "[shutdown error]" << std::endl;
        _iResult = 0;
        return 1;
    }
    return 0;
}
