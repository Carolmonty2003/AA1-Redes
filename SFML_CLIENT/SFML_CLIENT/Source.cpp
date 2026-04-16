#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "NetworkManager.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "LobbyScene.h"
#include "LoginScene.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Conecta3 Cliente");
    window.setFramerateLimit(60);

    LoginScene* loginScene = new LoginScene();
    GameScene* gameScene = new GameScene();
    LobbyScene* lobbyScene = new LobbyScene();


    SM.AddScene("LoginScene", loginScene);
    SM.AddScene("GameScene", gameScene);
    SM.AddScene("LobbyScene", lobbyScene);
    SM.InitFirstScene("LoginScene");

    if (!NM.ConnectToServer())
    {
        //return -1;
    }

    std::string command;
    std::string roomId;
    std::string nickname = "Edgar";
    unsigned short gamePort = 56000;

    NM.GetClientState().nickname = nickname;
    NM.GetClientState().playerId = 1;

    sf::Clock dtClock;
    while (window.isOpen())
    {
        /*
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
        */
        //NM.ReceiveData();
        

        float dt = dtClock.restart().asSeconds();

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();


            if (SM.GetCurrentScene())
                SM.GetCurrentScene()->HandleEvent(*event);
        }

        SM.UpdateCurrentScene(dt);
        window.clear(sf::Color(30, 30, 30));

        if (SM.GetCurrentScene())
            SM.GetCurrentScene()->Render(window);

        window.display();
    }

    //NM.CloseConnection();
    return 0;
}