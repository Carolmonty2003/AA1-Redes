#pragma once

#include <SFML/Network.hpp>
#include <string>
#include "PacketType.h"
#include "ProtocolData.h"
#include "ClientState.h"

class NetworkManager
{
public:
    NetworkManager();

    bool Connect(const sf::IpAddress& serverIp, unsigned short serverPort);
    void CloseConnection();

    void SendCreateRoomRequest(const std::string& roomId, const std::string& nickname, unsigned short gamePort);
    void SendJoinRoomRequest(const std::string& roomId, const std::string& nickname, unsigned short gamePort);

    void ReceiveData();

    bool IsConnected() const;
    const ClientState& GetClientState() const;
    ClientState& GetClientState();

private:
    void ProcessPacket(sf::Packet& packet);

    void HandleCreateRoomResponse(sf::Packet& packet);
    void HandleJoinRoomResponse(sf::Packet& packet);
    void HandleRoomStatusUpdate(sf::Packet& packet);
    void HandleStartGame(sf::Packet& packet);
    void HandleErrorMessage(sf::Packet& packet);

private:
    sf::TcpSocket m_socket;
    bool m_isConnected;
    ClientState m_clientState;
};