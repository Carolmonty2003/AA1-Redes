#pragma once

#include <SFML/Network.hpp>
#include <string>
#include "PacketType.h"

class LobbyScene
{
public:
    void AskCreateRoom(const std::string& roomId);
    void AskJoinRoom(const std::string& roomId);
    void Update();
    void Draw();

private:
    std::string m_inputRoomId;
};

