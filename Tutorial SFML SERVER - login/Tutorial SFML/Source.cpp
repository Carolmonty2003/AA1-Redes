#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/prepared_statement.h>

#define LISTENER_PORT 55000

sql::Connection* conn; 

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
void GetAllPlayers() {
	sql::PreparedStatement* pstmt = conn->prepareStatement("SELECT * FROM players");
	sql::ResultSet* res = pstmt->executeQuery();
	while (res->next())
		std::cout << "Id: " << res->getInt("Id") << " | User: " << res->getString("Username") << " | Score: " << res->getInt("Score") << std::endl;
	delete res;
	delete pstmt;
}
bool LoginPlayer() {
	sql::PreparedStatement* pstmt = conn->prepareStatement("CALL LoginPlayer( ?, ? )");
	pstmt->setString(1, "Radev");
	pstmt->setString(2, "RichardPringado");
	sql::ResultSet* res = pstmt->executeQuery();
	bool savedResult = res->next();
	delete res;
	delete pstmt;
	return savedResult;
}
void AddPlayer(){
	sql::PreparedStatement* pstmt = conn->prepareStatement("CALL AddPlayer( ?, ? )");
	pstmt->setString(1, "JuanCuesta");
	pstmt->setString(2, "JuntaUrgente");
	pstmt->execute();
	delete pstmt;
}
void DeletePlayer() {
	sql::PreparedStatement* pstmt = conn->prepareStatement("CALL DeletePlayer(?)");
	pstmt->setInt(1, 5); 
	pstmt->execute();
}
void PrintRanking() {
	sql::PreparedStatement* pstmt = conn->prepareStatement("CALL GetRanking(?)");
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
	conn = sql::mysql::get_mysql_driver_instance()->connect("tcp://localhost:3306", "root", "");
	conn->setSchema("videogame");
	//GetAllPlayers();
	//LoginPlayer();
	//AddPlayer();
	//DeletePlayer();
	PrintRanking();
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