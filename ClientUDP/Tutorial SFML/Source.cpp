#include <SFML/Network.hpp>
#include <iostream>
#include<cstring>

#define SERVER_PORT 55000
const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);
//const sf::IpAddress SERVER_IP = sf::IpAddress(10, 40, 2, 51);

//CLIENT
void main()
{
	sf::UdpSocket serverSocket;

	char buffer[1024];
	std::size_t bufferDataSize = 0;
	int data = 0;
	std::string message = "Hola shulo";
	

	for (int i = 0; i < 100001; i++)
	{
		bufferDataSize = 0;
		//Control de la informacion ya que si envias varias informaciones
		//Se puede borrar la anterior escrita

		
		int sizeMessage = message.size();

		std::memcpy(buffer + bufferDataSize, &sizeMessage, sizeof(sizeMessage));
		bufferDataSize = sizeof(sizeMessage);

		std::memcpy(buffer + bufferDataSize, message.c_str(), sizeMessage);
		bufferDataSize += sizeMessage;


		std::memcpy(buffer + bufferDataSize, &i, sizeof(i));
		bufferDataSize += sizeof(i);

		if (serverSocket.send(buffer, bufferDataSize, SERVER_IP, SERVER_PORT) == sf::Socket::Status::Done)
		{
			std::cout << "Mensaje entregado al sistema operativo" << std::endl;
		}
		
	}

}