#pragma once

#include <string>
#include <vector>
#include "ProtocolData.h"

struct ClientState
{
    // Identidad del jugador
    int playerId = -1;
    std::string nickname = "";

    // Estado de la sala
    std::string currentRoomId = "";
    bool isHost = false;
    bool isWaitingInRoom = false;

    // Estado de partida
    bool hasGameStarted = false;

    // Jugadores actuales de la sala
    std::vector<LobbyPlayerInfo> roomPlayers;

    void ResetRoomState()
    {
        currentRoomId.clear();
        isHost = false;
        isWaitingInRoom = false;
        hasGameStarted = false;
        roomPlayers.clear();
    }

    void ResetAll()
    {
        playerId = -1;
        nickname.clear();
        ResetRoomState();
    }

    bool IsLoggedIn() const
    {
        return playerId != -1 && !nickname.empty();
    }

    bool IsInRoom() const
    {
        return !currentRoomId.empty();
    }
};