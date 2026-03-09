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

void UpdatePassword(sql::Connection* con, std::string user, std::string newPassword)
{
    try {
        //sql::Statement* stmt = con->createStatement();
        sql::string query = "UPDATE users SET password = ? WHERE user = ?";
        sql::PreparedStatement* stmt = con->preparedStatement();
        stmt->setString(1, newPassword);
        stmt->setString(2, user);
        std::cout << "La query ejecutada es: " << query << std::endl;
        int affected_rows = stmt->executedUpdate(query);
        std::cout << "Number of rows affected: " << affected_rows << std::endl;
        delete stmt;
    }
    catch (sql::SQLException e) {
        std::cout << "Error while updating password " << < e.what() << std::endl;
    }
}

void DeleteByUser(sql::Connection* con, std::string user)
{
    try {
        sql::Statement* stmt = con->createStatement();
        sql::string query = "DELETE FROM users WHERE users = '" + user + "'";
        
        
        std::cout << "Number of users deleted : " << affected_rows << std::endl;


    }
    catch (sql::SQLException e) {
        std::cout << "Error while updating password " << < e.what() << std::endl;
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

void CreateUsers(sql::Connection* con, std::string user, std::string password)
{
    try {
        sql::Statement* stmt = con->createStatement();
        sql::string query = "INSERT INTO users (users,password) VALUES ('" + user + "', '" + password + "' )";

        int affected_rows = stmt->executeUpdate(query);

        if (affected_rows > 0)
        {
            std::cout << "User created succesfully" << std::endl;
        }

        delete stmt;

      
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

    ConnectDatabase(driver, con);
    con->setSchema(DATABASE);
    //GetAllUsers(con);
    //UpdatePassword(con,"Radev","suspenderAlumnos");
    //DeleteByUser();
    CreateUsers(con,"Radev", "suspenderAlumnos");
    DisconnectDatabase(con);
   
    system("pause");

}
