#pragma once
#include <string>

#define CHAT_VERSION                        "0.0.888"

#define MENU_FIRST_SCREEN                   0
#define MENU_LOGIN_SCREEN                   1
#define MENU_REGISTRATION_SCREEN            2
#define MENU_EXIT_PROGRAM                   3
#define MENU_CHAT_SCREEN                    4
#define MENU_SELECT_USER_SCREEN             5
#define MENU_CHAT_WITH_SCREEN               6
#define MENU_CHAT_WITH_ALL_USERS_SCREEN     7
#define MENU_DEBUG_CONSOLE                  63
#define MENU_CHAT_ADMIN_SCREEN              127
#define MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES   1337

const std::string chat_god = R"(
                _______                _______  _______  _______       _______       _______  _______  ______  
      |\     /|(  ___  )|\     /|     (  ___  )(  ____ )(  ____ \     (  ___  )     (  ____ \(  ___  )(  __  \ 
      ( \   / )| (   ) || )   ( |     | (   ) || (    )|| (    \/     | (   ) |     | (    \/| (   ) || (  \  )
       \ (_) / | |   | || |   | |     | (___) || (____)|| (__         | (___) |     | |      | |   | || |   ) |
        \   /  | |   | || |   | |     |  ___  ||     __)|  __)        |  ___  |     | | ____ | |   | || |   | |
         ) (   | |   | || |   | |     | (   ) || (\ (   | (           | (   ) |     | | \_  )| |   | || |   ) |
         | |   | (___) || (___) |     | )   ( || ) \ \__| (____/\     | )   ( |     | (___) || (___) || (__/  )
         \_/   (_______)(_______)     |/     \||/   \__/(_______/     |/     \|     (_______)(_______)(______/
)";

#define SERVER_HOST                         "127.0.0.1"
#define PORT                                34567
#define BUF                                 4094