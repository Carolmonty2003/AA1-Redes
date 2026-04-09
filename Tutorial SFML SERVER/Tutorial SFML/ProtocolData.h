#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <vector>
#include "PacketType.h"

inline sf::Packet& operator>>(sf::Packet& packet, PacketType& type)
{
    int rawType = 0;
    packet >> rawType;
    type = static_cast<PacketType>(rawType);
    return packet;
}

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

struct LobbyPlayerInfo
{
    int playerId = -1;
    std::string username;
    std::string ip;
    unsigned short gamePort = 0;
    bool isHost = false;
};

struct RoomStatusUpdateData
{
    std::string roomId;
    int currentPlayers = 0;
    int maxPlayers = 0;
    std::vector<LobbyPlayerInfo> players;
};

struct StartGameData
{
    std::string roomId;
    int playerCount = 0;
    std::vector<LobbyPlayerInfo> players;
};

struct ErrorMessageData
{
    std::string message;
};

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