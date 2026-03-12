#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#define SERVER_PORT 55000

const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);

void main()
{
	sf::TcpSocket socket;
	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al conectar co el servidor" << std::endl;
	}
	else
	{
		std::cout << "Conectado con el servidor " << std::endl;

		sf::Packet packet; //Un paquete para enviar y otro para recibir es decir uno para el server y otro para el cliente

		if (socket.receive(packet) == sf::Socket::Status::Done)
		{
			std::string receivedMessage;
			packet >> receivedMessage;

			std::cout << "Mensaje recibido del servidor: " << receivedMessage << std::endl;
		}
		else
		{
			std::cerr << "Error al recibir el mensaje del servidor" << std::endl;
		}
	}
}