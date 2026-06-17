#include "RoomManager.h"
#include <iostream>

bool RoomManager::CreateRoom(const std::string& roomId, int creatorPlayerId)
{
    //Si existe, no hace nada y retorna false
    if (RoomExists(roomId))
        return false;

    Room newRoom;
    newRoom.roomId = roomId;
    newRoom.playerIds.push_back(creatorPlayerId);
    newRoom.inGame = false;
    newRoom.maxPlayers = MAX_PLAYERS;

    //Si no existe la cra
    m_rooms.push_back(newRoom);

    std::cout << "[SERVER][RoomManager] Sala creada: " << roomId
        << " | creatorPlayerId: " << creatorPlayerId
        << std::endl;

    return true;

}

bool RoomManager::JoinRoom(const std::string& roomId, int playerId)
{
    //Revisa si la sala existe 
    Room* room = GetRoom(roomId);

    if (room == nullptr)
    {
        return false;
    }

    //Si ya se está jugando pues retorna false como si no existe
    if (room->inGame)
    {
        return false;
    }


    //En caso de que no se esté o ha superado el num de jugadores pues retorna tambien false para no unirse 

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
    

    //Si puede, une al jugador y devuelve true
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
    //revisa los jugadores y salas para birrarlo
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
    //Revisa las salas y borra la que toque 
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
    //Busca y devuelve la sala en base al nombre
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
    //Busca y devuelve la sala en base al id del jugador
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
{ //Revisa si la sala existe con el nombre
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
{//{ //Revisa si la sala está llena con el nombre
    for (const Room& room : m_rooms)
    {
        if (room.roomId == roomId)
        {
            return static_cast<int>(room.playerIds.size()) >= room.maxPlayers;
        }
    }

    return false;
}

void RoomManager::PrintRooms() const
{
    //Imprime las salas
    std::cout << "\n[SERVER][RoomManager] Estado actual de salas:\n";

    if (m_rooms.empty())
    {
        std::cout << "  No hay salas activas.\n";
        return;
    }

    for (const Room& room : m_rooms)
    {
        std::cout << "  Sala: " << room.roomId
            << " | players: " << room.playerIds.size()
            << "/" << room.maxPlayers
            << " | inGame: " << room.inGame
            << "\n";

        for (int playerId : room.playerIds)
        {
            std::cout << "    - playerId: " << playerId << "\n";
        }
    }
}