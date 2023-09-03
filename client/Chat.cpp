#include "Chat.h"
#include "consts.h"

Chat::Chat(){}
Chat::~Chat() {}

// функция которая частично фиксит неправильнный ввод пользователя
void checkInput(int& menu, int screen)
{
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore();
        menu = screen;
    }
}
void Chat::startChat()
{
    system("chcp 1251");
    int _menuState = 0;
    std::string chatWithUserLogin;
    std::string last_server_response; // for admin debug console
    do
    {
        system("CLS");
        std::cout << "waiting for server...\n";
        _networkStatus = _client.Connect();
        if (_networkStatus == 1) _client.reConnect();
    } while (_networkStatus != 0);
    while (_menuState != -1)
    {
        std::string _login;
        std::string _password;
        std::string _name;
        std::string _msg;
        
        if (_networkStatus != 0) {
            _networkStatus = _client.Connect();
        }
        system("CLS");
        if (_networkStatus != 0) {
            std::cout << "off-line\t";
        }
        else {
            std::cout << "\033[96mon-line\033[0m\t";
        }
        std::cout << "Chat version: " << CHAT_VERSION;
        if (isCurrentUserLogedIn())
        {
            if (isCurrentUserAdmin()) std::cout << "\033[96m" << chat_god << "\033[0m";
            std::cout << "\nYour Login: \033[93m" << _currentUser.getLogin() << "\033[0m";
            std::cout << "\nYour Name: \033[93m" << _currentUser.getName() << "\033[0m";
        }
        std::cout << "\nUsers: \033[93m" << sendMessageToServer("getUsersCount") << "\033[0m"
            << " Messages: \033[93m" << sendMessageToServer("getMessagesCount") << "\033[0m\n\n";

        std::vector<std::string> tmp_users;

        switch (_menuState)
        {
            //==================================================================================
        case MENU_FIRST_SCREEN:

            if (isCurrentUserLogedIn())
            {
                _menuState = MENU_CHAT_SCREEN;
                break;
            }

            std::cout << "Menu:\n";
            std::cout << "1. Login\n";
            std::cout << "2. Registration\n";
            std::cout << "3. Exit\n";
            std::cout << "input menu > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_FIRST_SCREEN);
            if (_menuState < 1 || 3 < _menuState) _menuState = MENU_FIRST_SCREEN;
            break;
            //==================================================================================
        case MENU_LOGIN_SCREEN:

            std::cout << "Login:\n";
            std::cout << "input Login > "; std::cin >> _login;
            if (!isUserExist(_login))
            {   // если пользователя не существует, предлагаем зарегистрироваться
                std::cout << "User " << _login << " dosn't exist enter name and password to registration:\n";
                std::cout << "input Name > "; std::cin >> _name;
                std::cout << "input Password > "; std::cin >> _password;
                
                createUser(_name, _login, _password);
                // авто вход
                login(_login, _password);
            }
            else // если пользователь уже существует то просим войти
            {    // приветствуем пользователя по имени
                _currentUser.setLogin(_login);
                if (isCurrentUserLogedIn())
                {
                    updateUserInfo(_login);
                    _menuState = MENU_CHAT_SCREEN;
                    break;
                }
                std::cout << "Hello, " << sendMessageToServer("getUserInfo:" + _currentUser.getLogin() + ":name") << "\n";
                do
                {
                    std::cout << "input Password > "; std::cin >> _password;
                } while (!login(_login, _password));
            }
            _menuState = MENU_CHAT_SCREEN;
            break;
            //==================================================================================
        case MENU_REGISTRATION_SCREEN:

            std::cout << "Registration:\n";
            std::cout << "input Login > "; std::cin >> _login;
            if (!isUserExist(_login)) // если пользователя нету то регистрация
            {
                std::cout << "input Name > "; std::cin >> _name;
                std::cout << "input Password > "; std::cin >> _password;

                createUser(_name, _login, _password);
                // авто вход
                login(_login, _password);
            }
            else // если пользователь уже существует то просим войти
            {    // приветствуем пользователя по имени
                _currentUser.setLogin(_login);
                if (isCurrentUserLogedIn())
                {
                    updateUserInfo(_login);
                    _menuState = MENU_CHAT_SCREEN;
                    break;
                }
                std::cout << "Hello, " << sendMessageToServer("getUserInfo:" + _currentUser.getLogin() + ":name") << "\n";
                do
                {
                    std::cout << "input Password > "; std::cin >> _password;
                } while (!login(_login, _password));
            }
            _menuState = MENU_CHAT_SCREEN;
            break;
            //==================================================================================
        case MENU_EXIT_PROGRAM: _menuState = -1; break;
            //==================================================================================
        case MENU_CHAT_SCREEN:

            std::cout << "Chat:\n";
            std::cout << "1. General chat\n";
            std::cout << "2. Show users to chating\n";
            std::cout << "3. Log out to World\n";
            std::cout << "4. Exit without logout\n";


            if (isCurrentUserAdmin())
            {
                std::cout << "63. \033[93mDebug console \033[30m\033[106m[experimental]\033[0m\n";
                std::cout << "127. Delete users\n";
                std::cout << "1337. Show all messages\n";
            }
            std::cout << "input menu > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_CHAT_SCREEN);

            switch (_menuState)
            {
            case 1: _menuState = MENU_CHAT_WITH_ALL_USERS_SCREEN; break;
            case 2: _menuState = MENU_SELECT_USER_SCREEN; break;
            case 3: logOff(); _menuState = MENU_FIRST_SCREEN; break;
            case 4: justExit(); _menuState = MENU_FIRST_SCREEN; break;
            case MENU_DEBUG_CONSOLE: break;
            case MENU_CHAT_ADMIN_SCREEN: break; // удаление пользователей
            case MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES: break; // все сообщения
            default: _menuState = MENU_CHAT_SCREEN; break;
            }
            break;
            //==================================================================================
        case MENU_SELECT_USER_SCREEN:

            /*std::vector<std::string> users = getUsersList();
            for (size_t i = 0; i < users.size(); i++)
            {
                std::cout << i + ". " + users[i] + "\n";
            }*/
            std::cout << "select user to chat with him:\n";
            tmp_users = printUsersList();
            std::cout << "____________________________________________\n";
            std::cout << "type ID or -1 to Back\n";
            std::cout << "input user id > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_SELECT_USER_SCREEN);

            if (0 <= _menuState && _menuState < tmp_users.size())
            {
                chatWithUserLogin = tmp_users[_menuState];

                _menuState = MENU_CHAT_WITH_SCREEN;
            }
            else
            {
                if (_menuState == -1) _menuState = MENU_CHAT_SCREEN;
                else                  _menuState = MENU_SELECT_USER_SCREEN;
            }
            break;
            //==================================================================================
        case MENU_CHAT_WITH_SCREEN:

            std::cout << "Chat with " << chatWithUserLogin << ":\n";
            printMessagesForUser(chatWithUserLogin);

            std::cout << "____________________________________________\n";
            std::cout << " 1. send message\n-1. back\n";
            std::cout << "input menu > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_CHAT_WITH_SCREEN);

            switch (_menuState)
            {
            case MENU_CHAT_WITH_SCREEN: break;
            case -1: chatWithUserLogin.clear(); _menuState = MENU_SELECT_USER_SCREEN; break;
            case 1:

                std::cout << "input message > ";
                std::getline(std::cin >> std::ws, _msg);
                sendMessageTo( chatWithUserLogin, _msg);
                _menuState = MENU_CHAT_WITH_SCREEN;

                break;
            default: _menuState = MENU_SELECT_USER_SCREEN; break;
            }
            break;
            //==================================================================================
        case MENU_CHAT_WITH_ALL_USERS_SCREEN:

            std::cout << "Chat with all users:\n";
            printMessages();

            std::cout << "____________________________________________\n";
            std::cout << " 1. send message\n-1. back\n";
            std::cout << "input menu > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_CHAT_WITH_ALL_USERS_SCREEN);

            switch (_menuState)
            {
            case MENU_CHAT_WITH_ALL_USERS_SCREEN: break;
            case -1: _menuState = MENU_CHAT_SCREEN; break;
            case 1:

                std::cout << "input message > ";
                std::getline(std::cin >> std::ws, _msg); // ввод сообщения с пробелами
                // отправка сообщения всем зарегистрированным пользователям
                sendMessageToAll(_msg);
                _menuState = MENU_CHAT_WITH_ALL_USERS_SCREEN;

                break;
            default: _menuState = MENU_CHAT_SCREEN; break;
            }
            break;
            //==================================================================================
        case MENU_CHAT_ADMIN_SCREEN:

            std::cout << "select user to delete by id:\n";
            tmp_users = printUsersList();
            std::cout << "____________________________________________\n";
            std::cout << "type ID or -1 to Back\n";
            std::cout << "input user id > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_CHAT_ADMIN_SCREEN);

            if (0 <= _menuState && _menuState < tmp_users.size())
            {
                /*if (!getUserByID(_menuState)->isUserAdmin())
                {
                    deleteUser(_menuState); tmp_users[_menuState]
                }*/
                sendMessageToServer("delUser:" + tmp_users[_menuState]);
                _menuState = MENU_CHAT_ADMIN_SCREEN;
            }
            else
            {
                if (_menuState == -1) _menuState = MENU_CHAT_SCREEN;
                else                  _menuState = MENU_CHAT_ADMIN_SCREEN;
            }
            break;
            //==================================================================================
        case MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES:

            std::cout << "Messages: \n";
            printMessagesForAdmin();
            std::cout << "___________________________________________________________\n";
            std::cout << "-1 to Back\n";
            std::cout << "input menu > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES);

            if (_menuState == -1) _menuState = MENU_CHAT_SCREEN;
            else                  _menuState = MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES;

            break;
            //==================================================================================
        
        case MENU_DEBUG_CONSOLE:
            
                
            std::cout << "[DEBUG_CONSOLE] for exit type '-1'\n";
            std::cout << "[examle] 'getUserInfo:admin:name'\n";
            std::cout << "[examle] 'getUserInfo:admin:user_type'\n";
            std::cout << "___________________________________________________________\n\n";
                
            if (_networkStatus == 0)
            {
                if (_networkStatus == 0) {

                    std::cout << "last response: " << last_server_response << "\n";
                    std::cout << "input message > ";
                    std::getline(std::cin >> std::ws, _msg);
                    if (_msg == "-1")
                    {
                        _menuState = MENU_CHAT_SCREEN; break;
                    }
                    _networkStatus = _client.Write(_msg);
                    std::cout << "waiting for response... ";
                }
                if (_networkStatus == 0)
                    _networkStatus = _client.Read(last_server_response);
            }
            else
            {
                _client.Shutdown();
                _menuState = MENU_CHAT_SCREEN;
            }
            
            break;
            //==================================================================================
        default: _menuState = MENU_FIRST_SCREEN; break;
        }
    }
}

void Chat::createUser(const std::string& name, const std::string& login, const std::string& password)
{
    sendMessageToServer("regUser:" + name + ":" + login + ":" + password);
}

void Chat::createAdminUser(const std::string& name, const std::string& login, const std::string& password)
{
    sendMessageToServer("regAdmin:" + name + ":" + login + ":" + password);
}

void Chat::deleteUser(const std::string& login)
{
    sendMessageToServer("delUser:"+ login );
}

bool Chat::isUserExist(const std::string& login) 
{
    if ("true" == sendMessageToServer("isUserExist:" + login)) return true;
    return false;
}

void Chat::sendMessageTo(const std::string& login, const std::string& message)
{
    sendMessageToServer("sendMessage:" + _currentUser.getLogin() + ":" + login + ":" + message);
}

void Chat::sendMessageToAll(const std::string& message)
{
    sendMessageToServer("sendMessage:" + _currentUser.getLogin() + ":all:" + message);
}

std::vector<std::string> Chat::printUsersList()
{
    std::vector<std::string> users = split(sendMessageToServer("getUsers:" + _currentUser.getLogin()), ':');
    for (size_t i = 0; i < users.size(); i++)
    {
        std::cout << "[" + std::to_string(i) + "] " + users[i] + "\n";
    }
    return users;
}

void Chat::printMessagesForUser(const std::string& login)
{
    std::cout << sendMessageToServer("getMessages:" + _currentUser.getLogin() + ":" + login);
}

void Chat::printMessages()
{
    std::cout << sendMessageToServer("getSharedMessages");
}


void Chat::printMessagesForAdmin()
{
    std::cout << sendMessageToServer("getAllMessages");
}

bool Chat::login(const std::string& login, const std::string& password)
{
    if ("true" == sendMessageToServer("login:" + login + ":" + password)) 
    {
        updateUserInfo(login);
        return true; 
    }
    return false;
}

void Chat::updateUserInfo(const std::string& login)
{
    _currentUser.setLogin(login);
    _currentUser.setName(sendMessageToServer("getUserInfo:" + _currentUser.getLogin() + ":name"));
    if ("admin" == sendMessageToServer("getUserInfo:" + _currentUser.getLogin() + ":user_type"))
    {
        _currentUser.setUserAdmin(true);
    }
}

void Chat::logOff()
{
    sendMessageToServer("logout:" + _currentUser.getLogin());
    _currentUser.clear();
}

void Chat::justExit()
{
    _currentUser.clear();
}

bool Chat::isCurrentUserLogedIn()
{
    if (_currentUser.getLogin().empty()) return false;
    if("true" == sendMessageToServer("isUserLogedIn:" + _currentUser.getLogin()) )
    {
        return true;
    }
    return false;
}

bool Chat::isCurrentUserAdmin() const
{
    return _currentUser.isUserAdmin();
}


size_t Chat::getUsersCount() const
{
    return 1;// _users.size();
}

size_t Chat::getMessagesCount() const
{
	return 1;
}

std::string Chat::sendMessageToServer(const std::string& request)
{
    if (_networkStatus == 0) {
        _networkStatus = _client.Write(request);
    }
    std::string response;
    if (_networkStatus == 0)
        _networkStatus = _client.Read(response);

    return response;
}

std::vector<std::string> Chat::split(const std::string& s, char seperator)
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