#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#define LISTENER_PORT 55000
#define SERVER "127.0.0.1::3306"
#define USERNAME "root"
#define PASSWORD ""
#define DATABASE "videogame"




void ConnectDatabase(sql::Driver*& driver, sql::Connection*& con) {
	try {
		driver = get_driver_instance();
		con = driver->connect(SERVER, USERNAME, PASSWORD);
		con->setSchema(DATABASE);
		std::cout << "Connection done" << std::endl;
	}
	catch (sql::SQLException e) {
		std::cout << "Could nor connect. Error message: " << e.what() << std::endl;
	}
}
void DisconnectDatabase(sql::Connection* con) {
	con->close();
	if (con->isClosed()) {
		std::cout << "Connection closed" << std::endl;
		delete con;
	}
}

//IMPORTANTE CERRAR LA CONEXION
void SendData(sf::TcpSocket& client, sf::Packet& packet)
{
	if (client.send(packet) == sf::Socket::Status::Done)
	{
		std::cout << "Mensaje enviado en el puert " << LISTENER_PORT << std::endl;
		
	}
	else
	{ 
		std::cout << "Error al enviar el mensaje" << std::endl;
	}
}
void GetAllPlayers(sql::Connection*& con) {
	sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM players");
	sql::ResultSet* res = pstmt->executeQuery();
	while (res->next())
		std::cout << "Id: " << res->getInt("Id") << " | User: " << res->getString("Username") << " | Score: " << res->getInt("Score") << std::endl;
	delete res;
	delete pstmt;
}
bool LoginPlayer(sql::Connection*& con) {
	sql::PreparedStatement* pstmt = con->prepareStatement("CALL LoginPlayer( ?, ? )");
	pstmt->setString(1, "Radev");
	pstmt->setString(2, "RichardPringado");
	sql::ResultSet* res = pstmt->executeQuery();
	bool savedResult = res->next();
	delete res;
	delete pstmt;
	return savedResult;
}
void AddPlayer(sql::Connection*& con){
	sql::PreparedStatement* pstmt = con->prepareStatement("CALL AddPlayer( ?, ? )");
	pstmt->setString(1, "JuanCuesta");
	pstmt->setString(2, "JuntaUrgente");
	pstmt->execute();
	delete pstmt;
}
void DeletePlayer(sql::Connection*& con) {
	sql::PreparedStatement* pstmt = con->prepareStatement("CALL DeletePlayer(?)");
	pstmt->setInt(1, 5); 
	pstmt->execute();
}
void PrintRanking(sql::Connection*& con) {
	sql::PreparedStatement* pstmt = con->prepareStatement("CALL GetRanking(?)");
	pstmt->setString(1, "Richard"); 
	sql::ResultSet* res = pstmt->executeQuery();
	while (res->next())
		std::cout << res->getString("Username") << " - Score: " << res->getInt("Score") << std::endl;
	delete res;
	delete pstmt;
}
void main()
{
	srand(time(NULL));
	enum tipoPaquete {HANDSHAKE, LOGIN, SIGNUP, MOVIMINETO};

	sf::TcpListener listener;

	sf::TcpSocket client;

   //https://github.com/anhstudios/mysql-connector-cpp/blob/master/examples/standalone_example.cpp
	//https://www.reddit.com/r/cpp_questions/comments/1i1snro/coding_with_c_and_mysql_how_do_i_print_out_select/
	//https://stackoverflow.com/questions/18364463/sql-using-a-prepared-statement-for-the-from-clause
	sql::Connection* conn;
	sql::Driver* driver;
	ConnectDatabase(driver,conn);
	GetAllPlayers(conn);
	//LoginPlayer();
	//AddPlayer();
	//DeletePlayer();
	//PrintRanking();
	bool closeServer = false;

	//listener.setBlocking(false);

	if (listener.listen(LISTENER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al intentar escuchar en el puerto " << LISTENER_PORT << std::endl;
	}
	
	while (!closeServer)
	{
		std::cout << "Esperando Conexion..." << std::endl;
		if (listener.accept(client) == sf::Socket::Status::Done)
		{
			std::cout << "Cliente conectado desde " << client.getRemoteAddress().value() << std::endl;
			sf::Packet packet; //Mejorar memoria solo tener un packet o limpiar la informacion con el clear
			std::string message = "Hola cliente";
			int messageInt = 2;
			int tempRandom = rand() % 2;
			packet.clear();
			switch (tempRandom)
			{
				case 0:
					message = "HOLA MI CLIENTE";
					packet << tipoPaquete::HANDSHAKE << message;
					SendData(client,packet);
					break;
				case 1:
					packet << tipoPaquete::LOGIN << messageInt << message;
					SendData(client, packet);
					break;
				case 2:
					packet << tipoPaquete::SIGNUP << messageInt << message;
				default:
					break;
			}

		}
		else
		{
			std::cerr << "Error al aceptar la conexion" << std::endl;
		}

	}
	
	client.disconnect();
	closeServer = true;
	conn->close();
}