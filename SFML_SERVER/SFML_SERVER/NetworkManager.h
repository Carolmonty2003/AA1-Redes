#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <memory>
#include <string>
#include "ConnectedClient.h"
#include "RoomManager.h"
#include "PacketTypes.h"
#include "ProtocolData.h"

class NetworkManager
{
public:
    NetworkManager();

    bool Start(unsigned short listenPort);
    void Update();

private:
    void AcceptNewClients();
    void ReceiveClientData();

    void ProcessPacket(ConnectedClient& client, sf::Packet& packet);

    void HandleCreateRoomRequest(ConnectedClient& client, sf::Packet& packet);
    void HandleJoinRoomRequest(ConnectedClient& client, sf::Packet& packet);

    void SendCreateRoomResponse(ConnectedClient& client, bool success, const std::string& roomId, const std::string& message);
    void SendJoinRoomResponse(ConnectedClient& client, bool success, const std::string& roomId, const std::string& message);
    void SendErrorMessage(ConnectedClient& client, const std::string& message);

    void BroadcastRoomStatus(const std::string& roomId);
    void TryStartGame(const std::string& roomId);

    ConnectedClient* GetClientById(int playerId);
    ConnectedClient* GetClientBySocket(sf::TcpSocket* socket);

    void RemoveDisconnectedClient(int index);

    void PrintConnectedClients() const;

private:
    sf::TcpListener m_listener;
    bool m_isRunning;
    int m_nextPlayerId;

    std::vector<std::unique_ptr<sf::TcpSocket>> m_sockets;
    std::vector<ConnectedClient> m_clients;
    RoomManager m_roomManager;
};