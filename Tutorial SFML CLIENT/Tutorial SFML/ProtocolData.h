#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <vector>
#include "PacketType.h"


// Lectura del tipo de paquete
inline sf::Packet& operator>>(sf::Packet& packet, PacketType& type)
{
    int rawType = 0;
    packet >> rawType;
    type = static_cast<PacketType>(rawType);
    return packet;
}

// Datos de Login
struct LoginRequestData
{
    std::string username;
    std::string password;
};

struct LoginResponseData
{
    bool success = false;
    int playerId = -1;
    std::string username;
    int score = 0;
    std::string message;
};

// Datos de Registro
struct RegisterRequestData
{
    std::string username;
    std::string password;
};

struct RegisterResponseData
{
    bool success = false;
    std::string message;
};

// Datos de Crear Sala
struct CreateRoomRequestData
{
    std::string roomId;
    std::string username;
    unsigned short gamePort = 0;
};

struct CreateRoomResponseData
{
    bool success = false;
    std::string roomId;
    std::string message;
};

// Datos de Unirse a Sala
struct JoinRoomRequestData
{
    std::string roomId;
    std::string username;
    unsigned short gamePort = 0;
};

struct JoinRoomResponseData
{
    bool success = false;
    std::string roomId;
    std::string message;
};

// Datos de Jugador dentro del Lobby
struct LobbyPlayerInfo
{
    int playerId = -1;
    std::string username;
    std::string ip;
    unsigned short gamePort = 0;
    bool isHost = false;
};

// Estado de la Sala
struct RoomStatusUpdateData
{
    std::string roomId;
    int currentPlayers = 0;
    int maxPlayers = 0;
    std::vector<LobbyPlayerInfo> players;
};

// Inicio de partida
struct StartGameData
{
    std::string roomId;
    int playerCount = 0;
    std::vector<LobbyPlayerInfo> players;
};

// Error genérico
struct ErrorMessageData
{
    std::string message;
};

// Serializacion de LobbyPlayerInfo
inline sf::Packet& operator<<(sf::Packet& packet, const LobbyPlayerInfo& data)
{
    packet << data.playerId
        << data.username
        << data.ip
        << data.gamePort
        << data.isHost;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, LobbyPlayerInfo& data)
{
    packet >> data.playerId
        >> data.username
        >> data.ip
        >> data.gamePort
        >> data.isHost;
    return packet;
}

// Serializacion LoginRequestData
inline sf::Packet& operator<<(sf::Packet& packet, const LoginRequestData& data)
{
    packet << data.username
        << data.password;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, LoginRequestData& data)
{
    packet >> data.username
        >> data.password;
    return packet;
}

// Serializacion LoginResponseData
inline sf::Packet& operator<<(sf::Packet& packet, const LoginResponseData& data)
{
    packet << data.success
        << data.playerId
        << data.username
        << data.score
        << data.message;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, LoginResponseData& data)
{
    packet >> data.success
        >> data.playerId
        >> data.username
        >> data.score
        >> data.message;
    return packet;
}

// Serializacion RegisterRequestData
inline sf::Packet& operator<<(sf::Packet& packet, const RegisterRequestData& data)
{
    packet << data.username
        << data.password;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, RegisterRequestData& data)
{
    packet >> data.username
        >> data.password;
    return packet;
}

// Serializacion RegisterResponseData
inline sf::Packet& operator<<(sf::Packet& packet, const RegisterResponseData& data)
{
    packet << data.success
        << data.message;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, RegisterResponseData& data)
{
    packet >> data.success
        >> data.message;
    return packet;
}

// Serializacion CreateRoomRequestData
inline sf::Packet& operator<<(sf::Packet& packet, const CreateRoomRequestData& data)
{
    packet << data.roomId
        << data.username
        << data.gamePort;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, CreateRoomRequestData& data)
{
    packet >> data.roomId
        >> data.username
        >> data.gamePort;
    return packet;
}

// Serializacion CreateRoomResponseData
inline sf::Packet& operator<<(sf::Packet& packet, const CreateRoomResponseData& data)
{
    packet << data.success
        << data.roomId
        << data.message;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, CreateRoomResponseData& data)
{
    packet >> data.success
        >> data.roomId
        >> data.message;
    return packet;
}

// Serializacion JoinRoomRequestData
inline sf::Packet& operator<<(sf::Packet& packet, const JoinRoomRequestData& data)
{
    packet << data.roomId
        << data.username
        << data.gamePort;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, JoinRoomRequestData& data)
{
    packet >> data.roomId
        >> data.username
        >> data.gamePort;
    return packet;
}

// Serializacion JoinRoomResponseData
inline sf::Packet& operator<<(sf::Packet& packet, const JoinRoomResponseData& data)
{
    packet << data.success
        << data.roomId
        << data.message;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, JoinRoomResponseData& data)
{
    packet >> data.success
        >> data.roomId
        >> data.message;
    return packet;
}

// Serializacion RoomStatusUpdateData
inline sf::Packet& operator<<(sf::Packet& packet, const RoomStatusUpdateData& data)
{
    packet << data.roomId
        << data.currentPlayers
        << data.maxPlayers;

    packet << static_cast<int>(data.players.size());
    for (const LobbyPlayerInfo& player : data.players)
    {
        packet << player;
    }

    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, RoomStatusUpdateData& data)
{
    int playerCount = 0;

    packet >> data.roomId
        >> data.currentPlayers
        >> data.maxPlayers
        >> playerCount;

    data.players.clear();
    data.players.resize(playerCount);

    for (int i = 0; i < playerCount; ++i)
    {
        packet >> data.players[i];
    }

    return packet;
}

// Serializacion StartGameData
inline sf::Packet& operator<<(sf::Packet& packet, const StartGameData& data)
{
    packet << data.roomId
        << data.playerCount;

    packet << static_cast<int>(data.players.size());
    for (const LobbyPlayerInfo& player : data.players)
    {
        packet << player;
    }

    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, StartGameData& data)
{
    int vectorSize = 0;

    packet >> data.roomId
        >> data.playerCount
        >> vectorSize;

    data.players.clear();
    data.players.resize(vectorSize);

    for (int i = 0; i < vectorSize; ++i)
    {
        packet >> data.players[i];
    }

    return packet;
}

// Serializacion ErrorMessageData
inline sf::Packet& operator<<(sf::Packet& packet, const ErrorMessageData& data)
{
    packet << data.message;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, ErrorMessageData& data)
{
    packet >> data.message;
    return packet;
}