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
		std::cerr << "Error al conectar con el servidor" << std::endl;
	}
	
	std::cout << "Conectado con el servidor " << std::endl;

	socket.setBlocking(false);

	while (gameLoop) 
	{
		std::string message;
		std::cout << "Inserta mensaje para el servidor, -1 para salir" << std::endl;
		std::cin >> message;

		if (message == "-1") 
		{
			std::cout << "Desconectado..." << std::endl;
			gameLoop = false;
		}
		else
		{
			sf::Packet packet;
			packet >> message;
			if (socket.send(packet) != sf::Socket::Status::Done)
			{
				std::cerr << "Error al enviar el paquete al servidor";
			}
			else
			{
				std::cout << "Mensaje enviado: " << message << std::endl;
			}
		}
	}

	socket.disconnect();
	std::cout << "Desconectado del servidor" << std::endl;
}