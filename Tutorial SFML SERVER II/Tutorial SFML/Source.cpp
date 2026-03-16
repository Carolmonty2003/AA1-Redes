#include <SFML/Network.hpp>
#include <iostream>

#define LISTENER_PORT 55000


void main()
{
	bool closeServer = false;

	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector <sf::TcpSocket*> clients;
	sf::TcpSocket* newClient;

	if (listener.listen(LISTENER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al intentar escuchar en el puerto " << LISTENER_PORT << std::endl;
		closeServer = true;
	}
	
	selector.add(listener);

	while (!closeServer)
	{
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
				newClient = new sf::TcpSocket();

				if (listener.accept(*newClient) == sf::Socket::Status::Done)
				{
					//configurar antes de guardar!!
					newClient->setBlocking(false);
					selector.add(*newClient);
					//aqui va el costuctor del cliente. Ahora lo haremos simple
					clients.push_back(newClient);
					std::cout << "Nueva conexion establecida" << std::endl;
				}
			}
			else
			{
				for (int i = 0; i < clients.size(); i++)
				{
					if (selector.isReady(*clients[i]))
					{
						sf::Packet packet;
						if (clients[i]->receive(packet) == sf::Socket::Status::Done) 
						{
							std::string message;
							packet >> message;

							std::cout << "Mensaje: " << message << std::endl;
						}

						if (clients[i]->receive(packet) == sf::Socket::Status::Disconnected) 
						{
							selector.remove(*clients[i]);
							delete clients[i];
							clients.erase(clients.begin() + i);
							i--;
						}
					}
				}
			}
		}
	}

}