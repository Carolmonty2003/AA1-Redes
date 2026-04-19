#pragma once

#include <string>
#include <vector>
#include "ProtocolData.h"

struct ClientState
{
    // Identidad del jugador
    int playerId = -1;
    std::string nickname = "";
    unsigned short myGamePort = 0;

    // Estado de la sala
    std::string currentRoomId = "";
    bool isHost = false;
    bool isWaitingInRoom = false;

    // Estado de partida
    bool hasGameStarted = false;

    // Jugadores actuales de la sala
    std::vector<LobbyPlayerInfo> roomPlayers;

    void ResetRoomState();

    void ResetAll();

    inline bool IsLoggedIn() const {return playerId != -1 && !nickname.empty();}

    inline bool IsInRoom() const { return !currentRoomId.empty();}
};