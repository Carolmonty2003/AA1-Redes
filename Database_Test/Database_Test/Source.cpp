#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statment.h>
#include <cppconn/resultset.h>

#define SERVER "127.0.0.1:3306"
#define USERNAME "root"
#define PASSWORD "enti"
#define DATABASE "videogame"

void ConnectDatabase(sql::Diriver*& driver, sql::Connection*& con) 
{
    try
    {
        driver = get_driver_instance();
        con = driver->connect(SERVER, USERNAME, PASSWORD);
        std::cout << "Connection done." << std::endl;

    }
    catch (sql::SQLException e)
    {
        std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
        system("pause");
        exit(1);
    }
}

void DisconnectDatabase(sql::Connection* con) 
{
    con->close();

    if (con->isClosed()) 
    {
        std::cout << "Connection close." << std::endl;
        delete con;
    }
}

void GetAllUsers(sql::Connection* con) 
{
    try {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT user FROM users");

        std::cout << "Users in the database: " << std::endl;

        while (res->next())
        {
        std::cout << res->getSring("users") << std::endl;
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "Error while fetching users: " << e.what() << std::endl;
    }

    delete res;
    delete stmt;
}

void main()
{
    sql::Driver* driver;
    sql::Connection* con;
    con->setSchema(DATABASE);

    ConnectDatabase(driver, con);
    GetAllUsers(con);
    DisconnectDatabase(con);

    system("pause");
}