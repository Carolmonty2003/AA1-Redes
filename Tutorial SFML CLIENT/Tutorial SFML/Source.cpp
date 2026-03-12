#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#define SERVER_PORT 55000
//IMPORTANTE CERRAR LA CONEXION DEL CLIENTE
const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);

//ESTO EN EL NETWORKMANAGER
enum tipoPaquete { HANDSHAKE, LOGIN, MOVIMINETO };

sf::Packet& operator >>(sf::Packet& packet, tipoPaquete& tipo)
{
	int temp;
	packet >> temp;
	tipo = static_cast<tipoPaquete>(temp);

	return packet;
}

void HandShake(sf::Packet data)
{
	std::string receivedMessage;
	data >> receivedMessage;
	std::cout << "Mensaje enviado del servidor " << receivedMessage << std::endl;
}

void Login(sf::Packet data)
{
	int receivedMessage;
	std::string receivedGreeting;
	data >> receivedMessage;
	data >> receivedGreeting;
	std::cout << "Mensaje enviado del servidor int " << receivedMessage << receivedGreeting << std::endl;
}

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

		socket.setBlocking(false);

		sf::Packet packet; //Un paquete para enviar y otro para recibir es decir uno para el server y otro para el cliente

		bool gameOver = false;

		while (!gameOver)
		{

			if (socket.receive(packet) == sf::Socket::Status::Done)
			{
				tipoPaquete tipo;

				packet >> tipo;

				switch (tipo)
				{
				case HANDSHAKE:
					HandShake(packet);
					break;
				case LOGIN:
					Login(packet);
					break;
				case MOVIMINETO:

					break;
				default:
					break;
				}

				packet.clear();
		
			}
			if (socket.receive(packet) == sf::Socket::Status::Disconnected)
			{
				gameOver = true;
			}
		}
		
	}
}