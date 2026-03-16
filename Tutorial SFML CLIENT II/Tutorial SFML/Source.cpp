#include <SFML/Network.hpp>
#include <iostream>

#define SERVER_PORT 55000
//IMPORTANTE CERRAR LA CONEXION DEL CLIENTE
const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);


void main()
{
	sf::TcpSocket socket;
	bool gameLoop = true;

	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al conectar co el servidor" << std::endl;
	}
	
	std::cout << "Conectado con el servidor " << std::endl;

	socket.setBlocking(false);

	while (gameLoop) {
		sf::sleep(sf::seconds(1));
	}

	socket.disconnect();
	std::cout << "Desconectado del servidor" << std::endl;
}