#include "NetworkManager.h"
#include <iostream>

NetworkManager::NetworkManager()
    : m_isConnected(false)
{
    m_socket.setBlocking(false);
}

bool NetworkManager::Connect(const sf::IpAddress& serverIp, unsigned short serverPort)
{
    if (m_socket.connect(serverIp, serverPort) != sf::Socket::Status::Done)
    {
        std::cerr << "[CLIENT] Error al conectar con el servidor." << std::endl;
        m_isConnected = false;
        return false;
    }

    m_socket.setBlocking(false);
    m_isConnected = true;

    std::cout << "[CLIENT] Conectado al servidor "
        << serverIp.toString()
        << ":"
        << serverPort
        << std::endl;

    return true;
}

void NetworkManager::CloseConnection()
{
    if (!m_isConnected)
    {
        return;
    }

    m_socket.disconnect();
    m_isConnected = false;

    std::cout << "[CLIENT] Conexion cerrada correctamente." << std::endl;
}

void NetworkManager::SendCreateRoomRequest(const std::string& roomId, const std::string& nickname, unsigned short gamePort)
{
    if (!m_isConnected)
    {
        std::cerr << "[CLIENT] No se puede enviar CREATE_ROOM_REQUEST: no hay conexion." << std::endl;
        return;
    }

    sf::Packet packet;

    CreateRoomRequestData requestData;
    requestData.roomId = roomId;
    requestData.username = nickname;
    requestData.gamePort = gamePort;

    packet << static_cast<int>(PacketType::CREATE_ROOM_REQUEST);
    packet << requestData;

    if (m_socket.send(packet) == sf::Socket::Status::Done)
    {
        std::cout << "[CLIENT] CREATE_ROOM_REQUEST enviada. RoomId: " << roomId << std::endl;
    }
    else
    {
        std::cerr << "[CLIENT] Error al enviar CREATE_ROOM_REQUEST." << std::endl;
    }
}

void NetworkManager::SendJoinRoomRequest(const std::string& roomId, const std::string& nickname, unsigned short gamePort)
{
    if (!m_isConnected)
    {
        std::cerr << "[CLIENT] No se puede enviar JOIN_ROOM_REQUEST: no hay conexion." << std::endl;
        return;
    }

    sf::Packet packet;

    JoinRoomRequestData requestData;
    requestData.roomId = roomId;
    requestData.username = nickname;
    requestData.gamePort = gamePort;

    packet << static_cast<int>(PacketType::JOIN_ROOM_REQUEST);
    packet << requestData;

    if (m_socket.send(packet) == sf::Socket::Status::Done)
    {
        std::cout << "[CLIENT] JOIN_ROOM_REQUEST enviada. RoomId: " << roomId << std::endl;
    }
    else
    {
        std::cerr << "[CLIENT] Error al enviar JOIN_ROOM_REQUEST." << std::endl;
    }
}

void NetworkManager::ReceiveData()
{
    if (!m_isConnected)
    {
        return;
    }

    sf::Packet packet;
    sf::Socket::Status status = m_socket.receive(packet);

    while (status == sf::Socket::Status::Done)
    {
        ProcessPacket(packet);

        packet.clear();
        status = m_socket.receive(packet);
    }

    if (status == sf::Socket::Status::Disconnected)
    {
        std::cout << "[CLIENT] El servidor ha cerrado la conexion." << std::endl;
        m_isConnected = false;
    }
}

bool NetworkManager::IsConnected() const
{
    return m_isConnected;
}

const ClientState& NetworkManager::GetClientState() const
{
    return m_clientState;
}

ClientState& NetworkManager::GetClientState()
{
    return m_clientState;
}

void NetworkManager::ProcessPacket(sf::Packet& packet)
{
    PacketType packetType = PacketType::NONE;
    packet >> packetType;

    switch (packetType)
    {
    case PacketType::CREATE_ROOM_RESPONSE:
        HandleCreateRoomResponse(packet);
        break;

    case PacketType::JOIN_ROOM_RESPONSE:
        HandleJoinRoomResponse(packet);
        break;

    case PacketType::ROOM_STATUS_UPDATE:
        HandleRoomStatusUpdate(packet);
        break;

    case PacketType::START_GAME:
        HandleStartGame(packet);
        break;

    case PacketType::ERROR_MESSAGE:
        HandleErrorMessage(packet);
        break;

    default:
        std::cout << "[CLIENT] Paquete recibido no gestionado." << std::endl;
        break;
    }
}

void NetworkManager::HandleCreateRoomResponse(sf::Packet& packet)
{
    CreateRoomResponseData responseData;
    packet >> responseData;

    std::cout << "[CLIENT] CREATE_ROOM_RESPONSE -> "
        << responseData.message
        << std::endl;

    if (responseData.success)
    {
        m_clientState.currentRoomId = responseData.roomId;
        m_clientState.isHost = true;
        m_clientState.isWaitingInRoom = true;
        m_clientState.hasGameStarted = false;
    }
}

void NetworkManager::HandleJoinRoomResponse(sf::Packet& packet)
{
    JoinRoomResponseData responseData;
    packet >> responseData;

    std::cout << "[CLIENT] JOIN_ROOM_RESPONSE -> "
        << responseData.message
        << std::endl;

    if (responseData.success)
    {
        m_clientState.currentRoomId = responseData.roomId;
        m_clientState.isHost = false;
        m_clientState.isWaitingInRoom = true;
        m_clientState.hasGameStarted = false;
    }
}

void NetworkManager::HandleRoomStatusUpdate(sf::Packet& packet)
{
    RoomStatusUpdateData roomData;
    packet >> roomData;

    std::cout << "[CLIENT] ROOM_STATUS_UPDATE -> Sala "
        << roomData.roomId
        << " | Jugadores: "
        << roomData.currentPlayers
        << "/"
        << roomData.maxPlayers
        << std::endl;

    m_clientState.currentRoomId = roomData.roomId;
    m_clientState.roomPlayers = roomData.players;
    m_clientState.isWaitingInRoom = true;

    for (const LobbyPlayerInfo& player : roomData.players)
    {
        std::cout << "  - " << player.username
            << " | host: " << player.isHost
            << " | ip: " << player.ip
            << " | port: " << player.gamePort
            << std::endl;
    }
}

void NetworkManager::HandleStartGame(sf::Packet& packet)
{
    StartGameData startData;
    packet >> startData;

    std::cout << "[CLIENT] START_GAME -> Sala "
        << startData.roomId
        << " | Players: "
        << startData.playerCount
        << std::endl;

    m_clientState.currentRoomId = startData.roomId;
    m_clientState.roomPlayers = startData.players;
    m_clientState.hasGameStarted = true;
    m_clientState.isWaitingInRoom = false;

    for (const LobbyPlayerInfo& player : startData.players)
    {
        std::cout << "  - " << player.username
            << " | host: " << player.isHost
            << " | ip: " << player.ip
            << " | port: " << player.gamePort
            << std::endl;
    }
}

void NetworkManager::HandleErrorMessage(sf::Packet& packet)
{
    ErrorMessageData errorData;
    packet >> errorData;

    std::cout << "[CLIENT] ERROR_MESSAGE -> "
        << errorData.message
        << std::endl;
}