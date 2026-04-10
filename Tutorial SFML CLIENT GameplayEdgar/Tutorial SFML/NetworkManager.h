#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <SFML/Network.hpp>
#include "PacketTypes.h"

#define NM NetworkManager::Instance()

class NetworkManager
{
#define SERVER_PORT 55000
private:
	
	sf::TcpSocket serverSocket;
	const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);
	
	
	std::vector<std::unique_ptr<sf::TcpSocket>> gameConnections;

	NetworkManager() = default;
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator=(const NetworkManager&) = delete;

public:
	inline static NetworkManager& Instance() {
		static NetworkManager nm;
		return nm;
	}

	

	bool ConnectToServer() {
		if (serverSocket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
		{
			std::cerr << "Error connecting to server" << std::endl;
			return false;
		}
		std::cout << "Connected to server" << std::endl;
		serverSocket.setBlocking(false);
		return true;
	}

	void DisconnectFromServer() {
		serverSocket.disconnect();
		std::cout << "Disconnected from server" << std::endl;
	}

	void SendToServer(sf::Packet& packet) {
		if (serverSocket.send(packet) != sf::Socket::Status::Done) {
			std::cerr << "Error sending packet to server" << std::endl;
		}
	}

	void SendLoginToServer(std::string user, std::string password) {
		sf::Packet packet;
		packet << PacketTypes::LOGIN << user << password;
		SendToServer(packet);
	}

	// Search for new packets
	void NetworkFetch();

	
	void AddConnection(const std::string& ip, unsigned short port) {
		auto newSocket = std::make_unique<sf::TcpSocket>();
		if (newSocket->connect(sf::IpAddress(ip), port) == sf::Socket::Status::Done) {
			std::cout << "Connected to rival at " << ip << ":" << port << std::endl;
			newSocket->setBlocking(false);
			gameConnections.push_back(std::move(newSocket));
		} else {
			std::cerr << "Failed to connect to rival at " << ip << ":" << port << std::endl;
		}
	}

	void SendToAllConnections(sf::Packet& packet) {
		for (auto& sock : gameConnections) {
			if (sock->send(packet) != sf::Socket::Status::Done) {
				std::cerr << "Error sending packet to a connection" << std::endl;
			}
		}
	}

	
	const std::vector<std::unique_ptr<sf::TcpSocket>>& GetConnections() const {
		return gameConnections;
	}

	void ClearConnections() {
		for (auto& sock : gameConnections) {
			sock->disconnect();
		}
		gameConnections.clear();
	}
};
