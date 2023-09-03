#include "DBWorker.h"


DBWorker::DBWorker() = default;
DBWorker::~DBWorker() = default;

void DBWorker::openData()
{
    mysql_init(&mysql);
    if (&mysql == nullptr)
    {
        std::cout << "Ошибка: дискритор соединения MySQL не создан." << std::endl;
        exit(1);
    }
    MYSQL* connect = mysql_real_connect(&mysql, "localhost", "root", "", "chat", 0, NULL, 0);
    if (connect == nullptr)
    {
        std::cout << "Невозможно подключится к базе данных. Ошибка:  " << mysql_error(&mysql) << std::endl;
        exit(1);
    }
}

void DBWorker::createDataTable()
{
    mysql_query(&mysql, "create table if not exists users( \
                        id INT AUTO_INCREMENT primary key not null,\
                        name varchar(200) not null,\
                        login varchar(200) unique not null,\
                        user_password varchar(200),\
                        user_type varchar(200)  NOT NULL  DEFAULT 'user',\
                        loggedin BOOLEAN DEFAULT false)");


    if(!isUserExist("admin"))
        mysql_query(&mysql, "insert into users (id, name, login, user_password, user_type)\
                            values (default, 'God', 'admin', 'root', 'admin')");
    if (!isUserExist("all"))
        mysql_query(&mysql, "insert into users (id, name, login, user_password, user_type)\
                            values (default, 'all', 'all', 'super_secure_password', 'shared_chat')");
    if (!isUserExist("user"))
        mysql_query(&mysql, "insert into users (id, name, login, user_password, user_type)\
                            values (default, 'just_user', 'user', 'simple', 'user')");


    mysql_query(&mysql, "create table if not exists messages (\
                        id INT AUTO_INCREMENT primary key not null,\
                        from_id integer, foreign key (from_id) references users(id) on delete cascade on update cascade, \
                        to_id integer, foreign key (to_id) references users(id) on delete cascade on update cascade, \
                        text varchar(500),\
                        date timestamp NOT NULL DEFAULT current_timestamp())");

}

bool DBWorker::insertDataUser(const std::string& name, const std::string& login, const std::string& password)
{
    const std::string str = "insert into users (id, name, login, user_password) values \
     (default, '" + name + "', '" + login + "', '" + password + "')";

    mysql_query(&mysql, str.c_str());
    if (mysql_errno(&mysql))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool DBWorker::insertDataAdmin(const std::string& name, const std::string& login, const std::string& password)
{
    const std::string str = "insert into users (id, name, login, user_password, user_type) values \
     (default, '" + name + "', '" + login + "', '" + password + "', 'admin')";

    mysql_query(&mysql, str.c_str());
    if (mysql_errno(&mysql))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool DBWorker::verificationDataUser(const std::string& login, const std::string& password)
{
    const std::string str = "select login, user_password from users where login='" + login + "' and \
	  user_password = '" + password + "'";
    mysql_query(&mysql, str.c_str());
    res = mysql_store_result(&mysql);
    if (res)
    {
        if (mysql_fetch_row(res))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool DBWorker::insertDataMessage(const std::string& from, const std::string& to, const std::string& text)
{
    const std::string str = "insert into messages (id, from_id, to_id, text) values (default, \
	 (select id from users where login = '" + from + "'), (select id from users where \
     login ='" + to + "'), '" + text + "')";
    mysql_query(&mysql, str.c_str());

    if (mysql_errno(&mysql))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void DBWorker::userLogin(const std::string& login)
{
    const std::string str = "UPDATE `users` SET `loggedin` = '1' WHERE `users`.`login` = '" + login + "';";

    mysql_query(&mysql, str.c_str());
}

void DBWorker::userLogout(const std::string& login)
{
    const std::string str = "UPDATE `users` SET `loggedin` = '0' WHERE `users`.`login` = '" + login + "';";

    mysql_query(&mysql, str.c_str());
}
void DBWorker::deleteUser(const std::string& login)
{
    const std::string str = "DELETE FROM users WHERE `users`.`id` = (select id from users WHERE login = '" + login + "')";

    mysql_query(&mysql, str.c_str());
}
//bool DBWorker::insertDataMessageAll(const std::string& from, const std::string& text)
//{
//    const std::string str = "insert into messagesAll (id, from_id, text) values (default, \
//	 (select id from users where login = '" + from + "'), '" + text + "')";
//
//    mysql_query(&mysql, str.c_str());
//
//    if (mysql_errno(&mysql))
//    {
//        return false;
//    }
//    else
//    {
//        return true;
//    }
//}

std::string DBWorker::readDataUsersLogin(const std::string& login)
{
    std::string str = "SELECT login  FROM users where login != '" + login + "'";
    if (login != "all") str = "SELECT login FROM users where login != '" + login + "' and login != 'all'";
    mysql_query(&mysql, str.c_str());
    std::string tmp;
    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
       {
           for (auto i = 0; i < mysql_num_fields(res); i++)
            {
                tmp = tmp + row[i] + ":";
            }
        }
    }
    tmp.pop_back();
    return tmp;
}

std::string DBWorker::readDataMessagesCount()
{
    const std::string str = "SELECT id FROM messages ";
    mysql_query(&mysql, str.c_str());
    std::string tmp;
    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            for (auto i = 0; i < mysql_num_fields(res); i++)
            {
                tmp = tmp + row[i] + ":";
            }
        }
    }
    if(!tmp.empty()) tmp.pop_back();
    return tmp;
}

bool DBWorker::isUserExist(const std::string& login_to)
{
    const std::string str = "SELECT login FROM users ";
    mysql_query(&mysql, str.c_str());
    res = mysql_store_result(&mysql);
    if (res)
    {
        while (row = mysql_fetch_row(res))
        {
            for (auto i = 0; i < mysql_num_fields(res); i++)
            {
                if (row[i] == login_to)
                    return true;
            }
        }
        return false;
    }
    else
    {
        return false;
    }
}

bool DBWorker::isUserLoggedIn(const std::string& login_to)
{
    const std::string str = "SELECT loggedin FROM users WHERE login = '" + login_to + "'";
    mysql_query(&mysql, str.c_str());
    res = mysql_store_result(&mysql);
    std::string tmp;
    if (res)
    {
        while (row = mysql_fetch_row(res))
        {
            for (auto i = 0; i < mysql_num_fields(res); i++)
            {
                tmp = row[i];
            }
        }

    }
    if (tmp == "1") return true;
    return false;
}

std::string DBWorker::readUserInfo(const std::string& login_to, const std::string& type)
{
    const std::string str = "SELECT " + type + " FROM users WHERE login = '" + login_to + "'";
    mysql_query(&mysql, str.c_str());
    res = mysql_store_result(&mysql);
    std::string tmp;
    if (res)
    {
        while (row = mysql_fetch_row(res))
        {
            for (auto i = 0; i < mysql_num_fields(res); i++)
            {
                tmp = row[i];
            }
        }

    }
    return tmp;
}
std::string DBWorker::getSharedMessages()
{
    const std::string str = "SELECT date, login, text FROM `messages` , `users` WHERE \
        (messages.to_id = (select id from users WHERE login = 'all')) and (users.id = from_id)\
        ORDER BY messages.date";
    std::string tmp;
    mysql_query(&mysql, str.c_str());

    if (res = mysql_store_result(&mysql))
    {

        while (row = mysql_fetch_row(res))
        {
            for (auto i = 0; i < mysql_num_fields(res); i++)
            {
                if ((i + 1) % 3 == 0) tmp = tmp + row[i];
                else tmp = tmp + row[i] + ": ";
            }
            tmp = tmp + "\n";
        }
    }

    return tmp;
}

std::string DBWorker::getAllMessages()
{
    const std::string str = "SELECT date, login, text FROM `messages` , `users` WHERE \
        users.id = from_id\
        ORDER BY messages.date";
    std::string tmp;
    mysql_query(&mysql, str.c_str());

    if (res = mysql_store_result(&mysql))
    {

        while (row = mysql_fetch_row(res))
        {
            for (auto i = 0; i < mysql_num_fields(res); i++)
            {
                if ((i + 1) % 3 == 0) tmp = tmp + row[i];
                else tmp = tmp + row[i] + ": ";
            }
            tmp = tmp + "\n";
        }
    }

    return tmp;
}
std::string DBWorker::getMessagesBetweenUsers(const std::string& login, const std::string& login2)
{
 
    const std::string str = "SELECT date, login, text FROM `messages` , `users` WHERE \
                            ((messages.from_id = (select id from users WHERE login = '" + login + "')\
                            and messages.to_id = (select id from users WHERE login = '" + login2 + "'))\
                            OR(messages.from_id = (select id from users WHERE login = '" + login2 + "')\
                            and messages.to_id = (select id from users WHERE login = '" + login + "')))\
                            and (users.id = from_id)\
                            ORDER BY messages.date";

    std::string tmp;
    mysql_query(&mysql, str.c_str());

    if (res = mysql_store_result(&mysql))
    {

        while (row = mysql_fetch_row(res))
        {
            for (auto i = 0; i < mysql_num_fields(res); i++)
            {
                if ((i+1) % 3 == 0) tmp = tmp + row[i];
                else tmp = tmp + row[i] + ": ";
            }
            tmp = tmp + "\n";
        }
    }

    return tmp;
}

void DBWorker::closeData()
{
    mysql_close(&mysql);
}