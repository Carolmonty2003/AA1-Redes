#pragma once

#define LISTENER_PORT 55000
#define SERVER "127.0.0.1::3306"
#define USERNAME "root"
#define PASSWORD ""
#define DATABASE "videogame"


#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>


class DatabaseConnector
{
public:
	DatabaseConnector();
	sql::Connection* con;
	sql::Driver* driver;
	void ConnectDatabase();
	void DisconnectDatabase();

	//Get database data funcs
	void GetAllPlayers();
	bool LoginPlayer();
	void AddPlayer();
	void DeletePlayer();
	void PrintRanking();
};

