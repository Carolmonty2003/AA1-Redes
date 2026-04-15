#include <iostream>
#include <string>
#include "NetworkManager.h"

constexpr unsigned short SERVER_PORT = 55000;
const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);

int main()
{
    NetworkManager networkManager;

    if (!networkManager.Connect(SERVER_IP, SERVER_PORT))
    {
        return -1;
    }

    std::string command;
    std::string roomId;
    std::string nickname = "Edgar";
    unsigned short gamePort = 56000;

    networkManager.GetClientState().nickname = nickname;
    networkManager.GetClientState().playerId = 1;

    while (true)
    {
        networkManager.ReceiveData();

        std::cout << "\nComando (create / join / state / exit): ";
        std::cin >> command;

        if (command == "create")
        {
            std::cout << "RoomId: ";
            std::cin >> roomId;
            networkManager.SendCreateRoomRequest(roomId, nickname, gamePort);
        }
        else if (command == "join")
        {
            std::cout << "RoomId: ";
            std::cin >> roomId;
            networkManager.SendJoinRoomRequest(roomId, nickname, gamePort);
        }
        else if (command == "state")
        {
            const ClientState& state = networkManager.GetClientState();

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

        networkManager.ReceiveData();
    }

    networkManager.CloseConnection();
    return 0;
}