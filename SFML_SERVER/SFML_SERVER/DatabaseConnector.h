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
#include "PacketTypes.h"
#include "ProtocolData.h"
#include <cppconn/resultset.h>

#define DC DatabaseConnector::Instance()

class DatabaseConnector
{
private:
	sql::Connection* con;
	sql::Driver* driver;

public:
	DatabaseConnector();
	DatabaseConnector(const DatabaseConnector&) = delete;
	DatabaseConnector& operator=(const DatabaseConnector&) = delete;
	inline static DatabaseConnector& Instance()
	{
		static DatabaseConnector nm;
		return nm;
	}
	void ConnectDatabase();
	void DisconnectDatabase();

	//Get database data funcs
	void GetAllPlayers();
	bool LoginPlayer(LoginRequestData lrd);
	void AddPlayer(RegisterRequestData rrd);
	void UpdateScore(Result r);

	void DeletePlayer();
	void UpdatePlayerScore(int playerId, int scoreDiff);
	void PrintRanking();
	std::vector<RankingData> GetRanking(std::string playerName);
};

