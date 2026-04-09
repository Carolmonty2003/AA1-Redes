#pragma once
#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include "PacketTypes.h"

#define NM NetworkManager::Instance()

sf::Packet& operator <<(sf::Packet& packet, PacketTypes& tipo)
{
	std::string packet;
	packet = static_cast<std::string>(tipo);
	packet << packet;
}

class NetworkManager
{
#define SERVER_PORT 55000
private:
	sf::TcpSocket socket;
	const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);

public:

	inline static NetworkManager& Instance() {
		static NetworkManager nm;
		return nm;
	}

	bool ConnectToServer() {


		if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
		{
			std::cerr << "Error al conectar con el servidor" << std::endl;
			return false;
		}
		else {
			std::cout << "Conectado con el servidor " << std::endl;
			socket.setBlocking(false);
			return true;
		}
	}

	void DisconectFromServer() {
		socket.disconnect();
		std::cout << "Desconectado del servidor" << std::endl;
	}


	void SendLoginToServer(std::string user, std::string password )
	{
		;
		//ADD TO MESSAGE THE LOGIN
		std::cin >> user;
		sf::Packet packet;
		packet << user +  " " + password; // NOTA: Arreglado el operador >> a << para añadir info al paquete
		if (socket.send(packet) != sf::Socket::Status::Done)
		{
			std::cerr << "Error al enviar el paquete al servidor";
		}
		else
		{
			std::cout << "Mensaje enviado: " << packet << std::endl; 
		}
	}

	

};

