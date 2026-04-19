#include "DatabaseConnector.h"
#include<iostream>

DatabaseConnector::DatabaseConnector()
{

}

void DatabaseConnector::ConnectDatabase()
{
	try {
		driver = get_driver_instance();
		con = driver->connect(SERVER, USERNAME, PASSWORD);
		con->setSchema(DATABASE);
		std::cout << "Connection done" << std::endl;
	}
	catch (sql::SQLException e) {
		std::cout << "Could not connect. Error message: " << e.what() << std::endl;
	}
}

void DatabaseConnector::DisconnectDatabase()
{
	con->close();
	if (con->isClosed()) {
		std::cout << "Connection closed" << std::endl;
		delete con;
	}
}

void DatabaseConnector::GetAllPlayers()
{
	sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM players");
	sql::ResultSet* res = pstmt->executeQuery();
	while (res->next())
		std::cout << "Id: " << res->getInt("Id") << " | User: " << res->getString("Username") << " | Score: " << res->getInt("Score") << std::endl;
	delete res;
	delete pstmt;
}

bool DatabaseConnector::LoginPlayer(LoginRequestData lrd)
{
	try{
		sql::PreparedStatement* pstmt = con->prepareStatement("CALL LoginPlayer( ?, ? )");
		pstmt->setString(1, lrd.username);
		pstmt->setString(2, lrd.password);
		sql::ResultSet* res = pstmt->executeQuery();
		bool savedResult = res->next();
		if(savedResult)
			std::cout << "[SERVER] " << lrd.username  << " login succeed" << std::endl;
		else
			std::cout << "[SERVER] " << lrd.username << " login failed" << std::endl;
		delete res;
		
		// Consumir resultados las stored procedure
		while(pstmt->getMoreResults()) {
			sql::ResultSet* extraRes = pstmt->getResultSet();
			if(extraRes) delete extraRes;
		}
		delete pstmt;
		
		return savedResult;
	}
	catch (sql::SQLException& e) {
		std::cout << "AddPlayer error: " << e.what() << std::endl;
		return false;
	}
}

void  DatabaseConnector::AddPlayer(RegisterRequestData rrd)
{
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("CALL AddPlayer( ?, ? )");
		pstmt->setString(1, rrd.username);
		pstmt->setString(2, rrd.password);
		pstmt->execute();
		
		while(pstmt->getMoreResults()) {
			sql::ResultSet* extraRes = pstmt->getResultSet();
			if(extraRes) delete extraRes;
		}
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		std::cout << "AddPlayer error: " << e.what() << std::endl;
	}
}

void DatabaseConnector::UpdateScore(Result r)
{
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("CALL UpdateScore( ?, ? )");
		pstmt->setString(1, r.username);
		pstmt->setInt(2, r.scoredPoints);
		pstmt->execute();
		while (pstmt->getMoreResults()) {
			sql::ResultSet* extraRes = pstmt->getResultSet();
			if (extraRes)
				delete extraRes;
		}
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		std::cout << "AddPlayer error: " << e.what() << std::endl;
	}
}

void DatabaseConnector::DeletePlayer()
{
	sql::PreparedStatement* pstmt = con->prepareStatement("CALL DeletePlayer(?)");
	pstmt->setInt(1, 5);
	pstmt->execute();
	
	while(pstmt->getMoreResults()) {
		sql::ResultSet* extraRes = pstmt->getResultSet();
		if(extraRes) delete extraRes;
	}
	delete pstmt;
}

void DatabaseConnector::PrintRanking()
{
	sql::PreparedStatement* pstmt = con->prepareStatement("CALL GetRanking(?)");
	pstmt->setString(1, "Richard");
	sql::ResultSet* res = pstmt->executeQuery();
	while (res->next())
		std::cout << res->getString("Username") << " - Score: " << res->getInt("Score") << std::endl;
	delete res;
	
	while(pstmt->getMoreResults()) {
		sql::ResultSet* extraRes = pstmt->getResultSet();
		if(extraRes) delete extraRes;
	}
	delete pstmt;
}

void DatabaseConnector::UpdatePlayerScore(int playerId, int scoreDiff)
{
	try {
        // En lugar de usar la stored procedure UpdateScore (que tiene el bug de Id = Id), 
        // hacemos la query pura para asegurarnos del correcto update en DB.
		sql::PreparedStatement* pstmt = con->prepareStatement(
			"UPDATE players SET Score = GREATEST(0, CAST(Score AS SIGNED) + ?) WHERE Id = ?"
		);
		pstmt->setInt(1, scoreDiff);
		pstmt->setInt(2, playerId);
		pstmt->execute();
		
		std::cout << "[SERVER] BD: Player " << playerId << " score actualizado con " << scoreDiff << " puntos." << std::endl;
        delete pstmt;
	}
	catch (sql::SQLException& e) {
		std::cout << "[SERVER] UpdatePlayerScore error: " << e.what() << std::endl;
	}
}
