#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#define LISTENER_PORT 55000

void main()
{
	sf::TcpListener listener;

	sf::TcpSocket client;

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

			packet << message;

			if (client.send(packet) == sf::Socket::Status::Done)
			{
				std::cout << "Mensaje enviado " << message << std::endl;
				packet.clear();
			}
			else
			{
				std::cerr << "Error al enviar el mensaje al cliente" << std::endl;
			}


		}
		else
		{
			std::cerr << "Error al aceptar la conexion" << std::endl;
		}
	}
	


}