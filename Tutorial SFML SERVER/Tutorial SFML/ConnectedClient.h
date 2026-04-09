#pragma once

#include "RoomManager.h"
#include <iostream>
#include <SFML/Network.hpp>
#include <string>

struct ConnectedClient
{
    int playerId = -1;
    std::string username = "";
    sf::TcpSocket* socket = nullptr;
    sf::IpAddress ip;
    unsigned short gamePort = 0;
    std::string currentRoomId = "";
};

bool RoomManager::CreateRoom(const std::string& roomId, int creatorPlayerId)
{
    if (RoomExists(roomId))
    {
        return false;
    }

    Room newRoom;
    newRoom.roomId = roomId;
    newRoom.playerIds.push_back(creatorPlayerId);
    newRoom.inGame = false;
    newRoom.maxPlayers = 4;

    m_rooms.push_back(newRoom);

    std::cout << "[SERVER][RoomManager] Sala creada: " << roomId
        << " | creatorPlayerId: " << creatorPlayerId
        << std::endl;

    return true;
}

bool RoomManager::JoinRoom(const std::string& roomId, int playerId)
{
    Room* room = GetRoom(roomId);

    if (room == nullptr)
    {
        return false;
    }

    if (room->inGame)
    {
        return false;
    }

    if (static_cast<int>(room->playerIds.size()) >= room->maxPlayers)
    {
        return false;
    }

    for (int existingPlayerId : room->playerIds)
    {
        if (existingPlayerId == playerId)
        {
            return false;
        }
    }

    room->playerIds.push_back(playerId);

    std::cout << "[SERVER][RoomManager] Player " << playerId
        << " unido a sala " << roomId
        << " | " << room->playerIds.size()
        << "/" << room->maxPlayers
        << std::endl;

    return true;
}

void RoomManager::RemovePlayerFromRoom(int playerId)
{
    for (int roomIndex = 0; roomIndex < static_cast<int>(m_rooms.size()); ++roomIndex)
    {
        Room& room = m_rooms[roomIndex];

        for (int playerIndex = 0; playerIndex < static_cast<int>(room.playerIds.size()); ++playerIndex)
        {
            if (room.playerIds[playerIndex] == playerId)
            {
                room.playerIds.erase(room.playerIds.begin() + playerIndex);

                std::cout << "[SERVER][RoomManager] Player " << playerId
                    << " eliminado de sala " << room.roomId
                    << std::endl;

                if (room.playerIds.empty())
                {
                    std::cout << "[SERVER][RoomManager] Sala vacia eliminada: "
                        << room.roomId
                        << std::endl;

                    m_rooms.erase(m_rooms.begin() + roomIndex);
                }

                return;
            }
        }
    }
}

void RoomManager::DeleteRoom(const std::string& roomId)
{
    for (int i = 0; i < static_cast<int>(m_rooms.size()); ++i)
    {
        if (m_rooms[i].roomId == roomId)
        {
            std::cout << "[SERVER][RoomManager] Sala eliminada: " << roomId << std::endl;
            m_rooms.erase(m_rooms.begin() + i);
            return;
        }
    }
}

Room* RoomManager::GetRoom(const std::string& roomId)
{
    for (Room& room : m_rooms)
    {
        if (room.roomId == roomId)
        {
            return &room;
        }
    }

    return nullptr;
}

Room* RoomManager::GetRoomByPlayerId(int playerId)
{
    for (Room& room : m_rooms)
    {
        for (int existingPlayerId : room.playerIds)
        {
            if (existingPlayerId == playerId)
            {
                return &room;
            }
        }
    }

    return nullptr;
}

bool RoomManager::RoomExists(const std::string& roomId) const
{
    for (const Room& room : m_rooms)
    {
        if (room.roomId == roomId)
        {
            return true;
        }
    }

    return false;
}

bool RoomManager::IsRoomFull(const std::string& roomId) const
{
    for (const Room& room : m_rooms)
    {
        if (room.roomId == roomId)
        {
            return static_cast<int>(room.playerIds.size()) >= room.maxPlayers;
        }
    }

    return false;
}