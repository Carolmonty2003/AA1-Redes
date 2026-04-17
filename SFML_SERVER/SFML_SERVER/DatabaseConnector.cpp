#include "DatabaseConnector.h"

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
	sql::PreparedStatement* pstmt = con->prepareStatement("CALL LoginPlayer( ?, ? )");
	pstmt->setString(1, lrd.username);
	pstmt->setString(2, lrd.password);
	sql::ResultSet* res = pstmt->executeQuery();
	bool savedResult = res->next();
	delete res;
	delete pstmt;
	return savedResult;
}

void  DatabaseConnector::AddPlayer(RegisterRequestData rrd)
{
	sql::PreparedStatement* pstmt = con->prepareStatement("CALL AddPlayer( ?, ? )");
	pstmt->setString(1, rrd.username);
	pstmt->setString(2, rrd.password);
	pstmt->execute();
	delete pstmt;
}

void DatabaseConnector::DeletePlayer()
{
	sql::PreparedStatement* pstmt = con->prepareStatement("CALL DeletePlayer(?)");
	pstmt->setInt(1, 5);
	pstmt->execute();
}

void DatabaseConnector::PrintRanking()
{
	sql::PreparedStatement* pstmt = con->prepareStatement("CALL GetRanking(?)");
	pstmt->setString(1, "Richard");
	sql::ResultSet* res = pstmt->executeQuery();
	while (res->next())
		std::cout << res->getString("Username") << " - Score: " << res->getInt("Score") << std::endl;
	delete res;
	delete pstmt;
}
