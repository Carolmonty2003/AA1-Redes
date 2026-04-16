#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "NetworkManager.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Conecta3 Cliente");
    window.setFramerateLimit(60);

    if (!NM.ConnectToServer())
    {
        return -1;
    }

    std::string command;
    std::string roomId;
    std::string nickname = "Edgar";
    unsigned short gamePort = 56000;

    NM.GetClientState().nickname = nickname;
    NM.GetClientState().playerId = 1;

    while (true)
    {
        NM.ReceiveData();

        std::cout << "\nComando (create / join / state / exit): ";
        std::cin >> command;

        if (command == "create")
        {
            std::cout << "RoomId: ";
            std::cin >> roomId;
            NM.SendCreateRoomRequest(roomId, nickname, gamePort);
        }
        else if (command == "join")
        {
            std::cout << "RoomId: ";
            std::cin >> roomId;
            NM.SendJoinRoomRequest(roomId, nickname, gamePort);
        }
        else if (command == "state")
        {
            const ClientState& state = NM.GetClientState();

            std::cout << "\n----- CLIENT STATE -----" << std::endl;
            std::cout << "playerId: " << state.playerId << std::endl;
            std::cout << "nickname: " << state.nickname << std::endl;
            std::cout << "currentRoomId: " << state.currentRoomId << std::endl;
            std::cout << "isHost: " << state.isHost << std::endl;
            std::cout << "isWaitingInRoom: " << state.isWaitingInRoom << std::endl;
            std::cout << "hasGameStarted: " << state.hasGameStarted << std::endl;
            std::cout << "roomPlayers: " << state.roomPlayers.size() << std::endl;
        }
        else if (command == "exit")
        {
            break;
        }

        NM.ReceiveData();
    }

    NM.CloseConnection();
    return 0;
}