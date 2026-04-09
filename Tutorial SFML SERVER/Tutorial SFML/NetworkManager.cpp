#include "NetworkManager.h"
#include <iostream>

NetworkManager::NetworkManager()
    : m_isRunning(false)
    , m_nextPlayerId(1)
{
    m_listener.setBlocking(false);
}

bool NetworkManager::Start(unsigned short listenPort)
{
    if (m_listener.listen(listenPort) != sf::Socket::Status::Done)
    {
        std::cerr << "[SERVER] Error al escuchar en puerto " << listenPort << std::endl;
        return false;
    }

    m_listener.setBlocking(false);
    m_isRunning = true;

    std::cout << "[SERVER] Escuchando en puerto " << listenPort << std::endl;
    return true;
}

void NetworkManager::Update()
{
    if (!m_isRunning)
    {
        return;
    }

    AcceptNewClients();
    ReceiveClientData();
}

void NetworkManager::AcceptNewClients()
{
    std::unique_ptr<sf::TcpSocket> newSocket = std::make_unique<sf::TcpSocket>();
    newSocket->setBlocking(false);

    if (m_listener.accept(*newSocket) == sf::Socket::Status::Done)
    {
        ConnectedClient newClient;
        newClient.playerId = m_nextPlayerId++;
        newClient.socket = newSocket.get();
        newClient.ip = newSocket->getRemoteAddress().value_or(sf::IpAddress::Any);

        std::cout << "[SERVER] Nuevo cliente conectado. playerId: "
            << newClient.playerId
            << " | IP: "
            << newClient.ip.toString()
            << std::endl;

        m_sockets.push_back(std::move(newSocket));
        m_clients.push_back(newClient);
    }
}

void NetworkManager::ReceiveClientData()
{
    for (int i = 0; i < static_cast<int>(m_clients.size()); ++i)
    {
        ConnectedClient& client = m_clients[i];

        if (client.socket == nullptr)
        {
            continue;
        }

        sf::Packet packet;
        sf::Socket::Status status = client.socket->receive(packet);

        while (status == sf::Socket::Status::Done)
        {
            ProcessPacket(client, packet);
            packet.clear();
            status = client.socket->receive(packet);
        }

        if (status == sf::Socket::Status::Disconnected)
        {
            std::cout << "[SERVER] Cliente desconectado. playerId: "
                << client.playerId
                << std::endl;

            RemoveDisconnectedClient(i);
            --i;
        }
    }
}

void NetworkManager::ProcessPacket(ConnectedClient& client, sf::Packet& packet)
{
    PacketType packetType = PacketType::NONE;
    packet >> packetType;

    switch (packetType)
    {
    case PacketType::CREATE_ROOM_REQUEST:
        HandleCreateRoomRequest(client, packet);
        break;

    case PacketType::JOIN_ROOM_REQUEST:
        HandleJoinRoomRequest(client, packet);
        break;

    default:
        std::cout << "[SERVER] Paquete no gestionado recibido de playerId "
            << client.playerId
            << std::endl;
        break;
    }
}

void NetworkManager::HandleCreateRoomRequest(ConnectedClient& client, sf::Packet& packet)
{
    CreateRoomRequestData requestData;
    packet >> requestData;

    client.username = requestData.username;
    client.gamePort = requestData.gamePort;

    bool success = m_roomManager.CreateRoom(requestData.roomId, client.playerId);

    if (!success)
    {
        SendCreateRoomResponse(client, false, requestData.roomId, "La sala ya existe.");
        return;
    }

    client.currentRoomId = requestData.roomId;

    SendCreateRoomResponse(client, true, requestData.roomId, "Sala creada correctamente.");
    BroadcastRoomStatus(requestData.roomId);
    TryStartGame(requestData.roomId);
}

void NetworkManager::HandleJoinRoomRequest(ConnectedClient& client, sf::Packet& packet)
{
    JoinRoomRequestData requestData;
    packet >> requestData;

    client.username = requestData.username;
    client.gamePort = requestData.gamePort;

    Room* room = m_roomManager.GetRoom(requestData.roomId);

    if (room == nullptr)
    {
        SendJoinRoomResponse(client, false, requestData.roomId, "La sala no existe.");
        return;
    }

    if (room->inGame)
    {
        SendJoinRoomResponse(client, false, requestData.roomId, "La sala ya esta en partida.");
        return;
    }

    if (static_cast<int>(room->playerIds.size()) >= room->maxPlayers)
    {
        SendJoinRoomResponse(client, false, requestData.roomId, "La sala esta llena.");
        return;
    }

    bool success = m_roomManager.JoinRoom(requestData.roomId, client.playerId);

    if (!success)
    {
        SendJoinRoomResponse(client, false, requestData.roomId, "No se ha podido unir a la sala.");
        return;
    }

    client.currentRoomId = requestData.roomId;

    SendJoinRoomResponse(client, true, requestData.roomId, "Te has unido a la sala correctamente.");
    BroadcastRoomStatus(requestData.roomId);
    TryStartGame(requestData.roomId);
}

void NetworkManager::SendCreateRoomResponse(ConnectedClient& client, bool success, const std::string& roomId, const std::string& message)
{
    if (client.socket == nullptr)
    {
        return;
    }

    sf::Packet packet;
    CreateRoomResponseData responseData;
    responseData.success = success;
    responseData.roomId = roomId;
    responseData.message = message;

    packet << static_cast<int>(PacketType::CREATE_ROOM_RESPONSE);
    packet << responseData;

    client.socket->send(packet);
}

void NetworkManager::SendJoinRoomResponse(ConnectedClient& client, bool success, const std::string& roomId, const std::string& message)
{
    if (client.socket == nullptr)
    {
        return;
    }

    sf::Packet packet;
    JoinRoomResponseData responseData;
    responseData.success = success;
    responseData.roomId = roomId;
    responseData.message = message;

    packet << static_cast<int>(PacketType::JOIN_ROOM_RESPONSE);
    packet << responseData;

    client.socket->send(packet);
}

void NetworkManager::SendErrorMessage(ConnectedClient& client, const std::string& message)
{
    if (client.socket == nullptr)
    {
        return;
    }

    sf::Packet packet;
    ErrorMessageData errorData;
    errorData.message = message;

    packet << static_cast<int>(PacketType::ERROR_MESSAGE);
    packet << errorData;

    client.socket->send(packet);
}

void NetworkManager::BroadcastRoomStatus(const std::string& roomId)
{
    Room* room = m_roomManager.GetRoom(roomId);

    if (room == nullptr)
    {
        return;
    }

    RoomStatusUpdateData roomData;
    roomData.roomId = room->roomId;
    roomData.currentPlayers = static_cast<int>(room->playerIds.size());
    roomData.maxPlayers = room->maxPlayers;

    roomData.players.clear();

    for (int playerId : room->playerIds)
    {
        ConnectedClient* roomClient = GetClientById(playerId);
        if (roomClient == nullptr)
        {
            continue;
        }

        LobbyPlayerInfo playerInfo;
        playerInfo.playerId = roomClient->playerId;
        playerInfo.username = roomClient->username;
        playerInfo.ip = roomClient->ip.toString();
        playerInfo.gamePort = roomClient->gamePort;
        playerInfo.isHost = (playerId == room->playerIds.front());

        roomData.players.push_back(playerInfo);
    }

    for (int playerId : room->playerIds)
    {
        ConnectedClient* roomClient = GetClientById(playerId);
        if (roomClient == nullptr || roomClient->socket == nullptr)
        {
            continue;
        }

        sf::Packet packet;
        packet << static_cast<int>(PacketType::ROOM_STATUS_UPDATE);
        packet << roomData;
        roomClient->socket->send(packet);
    }
}

void NetworkManager::TryStartGame(const std::string& roomId)
{
    Room* room = m_roomManager.GetRoom(roomId);

    if (room == nullptr)
    {
        return;
    }

    if (static_cast<int>(room->playerIds.size()) < room->maxPlayers)
    {
        return;
    }

    room->inGame = true;

    StartGameData startData;
    startData.roomId = room->roomId;
    startData.playerCount = static_cast<int>(room->playerIds.size());

    for (int playerId : room->playerIds)
    {
        ConnectedClient* roomClient = GetClientById(playerId);
        if (roomClient == nullptr)
        {
            continue;
        }

        LobbyPlayerInfo playerInfo;
        playerInfo.playerId = roomClient->playerId;
        playerInfo.username = roomClient->username;
        playerInfo.ip = roomClient->ip.toString();
        playerInfo.gamePort = roomClient->gamePort;
        playerInfo.isHost = (playerId == room->playerIds.front());

        startData.players.push_back(playerInfo);
    }

    for (int playerId : room->playerIds)
    {
        ConnectedClient* roomClient = GetClientById(playerId);
        if (roomClient == nullptr || roomClient->socket == nullptr)
        {
            continue;
        }

        sf::Packet packet;
        packet << static_cast<int>(PacketType::START_GAME);
        packet << startData;
        roomClient->socket->send(packet);
    }

    std::cout << "[SERVER] START_GAME enviado para sala " << roomId << std::endl;

    for (int playerId : room->playerIds)
    {
        ConnectedClient* roomClient = GetClientById(playerId);
        if (roomClient != nullptr)
        {
            roomClient->currentRoomId.clear();
        }
    }

    m_roomManager.DeleteRoom(roomId);
}

ConnectedClient* NetworkManager::GetClientById(int playerId)
{
    for (ConnectedClient& client : m_clients)
    {
        if (client.playerId == playerId)
        {
            return &client;
        }
    }

    return nullptr;
}

ConnectedClient* NetworkManager::GetClientBySocket(sf::TcpSocket* socket)
{
    for (ConnectedClient& client : m_clients)
    {
        if (client.socket == socket)
        {
            return &client;
        }
    }

    return nullptr;
}

void NetworkManager::RemoveDisconnectedClient(int index)
{
    if (index < 0 || index >= static_cast<int>(m_clients.size()))
    {
        return;
    }

    int playerId = m_clients[index].playerId;

    m_roomManager.RemovePlayerFromRoom(playerId);

    if (index < static_cast<int>(m_sockets.size()))
    {
        if (m_sockets[index] != nullptr)
        {
            m_sockets[index]->disconnect();
        }

        m_sockets.erase(m_sockets.begin() + index);
    }

    m_clients.erase(m_clients.begin() + index);
}