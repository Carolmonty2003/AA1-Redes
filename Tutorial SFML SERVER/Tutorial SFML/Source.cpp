#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <time.h>

#define LISTENER_PORT 55000

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

void main()
{
	srand(time(NULL));
	enum tipoPaquete {HANDSHAKE, LOGIN, MOVIMINETO};

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

}