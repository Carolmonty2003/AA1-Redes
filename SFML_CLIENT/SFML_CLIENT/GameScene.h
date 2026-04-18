#pragma once
#include "Scene.h"
#include "GameManager.h"
#include <iostream>
#include <vector>
#include <SFML/Network.hpp>
#include "NetworkManager.h"

class GameScene : public Scene
{
private:
    GameManager gameManager;

public:
    GameScene() = default;

    void SetupGame(const std::vector<Player>& players, int localID)
    {
        gameManager.InitGame(players, localID);
    }

    void SyncNextTurn(int playerID)
    {
        gameManager.SyncNextTurn(playerID);
    }

    void OnEnter() override
    {
        std::cout << "Entrando a GameScene..." << std::endl;
        auto& state = NM.GetClientState();
        std::vector<Player> gamePlayers;
        bool amIHost = false;
        
        for (const auto& lobbyPlayer : state.roomPlayers)
        {
            Player p;
            p.id = lobbyPlayer.playerId;
            p.nickName = lobbyPlayer.username;
            gamePlayers.push_back(p);

            if (lobbyPlayer.playerId == state.playerId) {
                amIHost = lobbyPlayer.isHost;
                if (amIHost) {
                    NM.StartP2PListener(lobbyPlayer.gamePort);
                }
            }
        }
        
        SetupGame(gamePlayers, state.playerId);

        if (!amIHost) {
            for (const auto& lobbyPlayer : state.roomPlayers) {
                if (lobbyPlayer.isHost) {
                    NM.AddConnection(lobbyPlayer.ip, lobbyPlayer.gamePort);
                }
            }
        }
    }

    void HandleEvent(const sf::Event& event) override
    {
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            const sf::Event::MouseButtonPressed* mbInfo =
                event.getIf<sf::Event::MouseButtonPressed>();

            if (mbInfo && mbInfo->button == sf::Mouse::Button::Left)
            {
                gameManager.TryPlacePieceScreen(static_cast<float>(mbInfo->position.x),
                                                static_cast<float>(mbInfo->position.y));
            }
        }
    }

    void Update(float dt) override
    {
        gameManager.ReceiveNetworkMoves();
        gameManager.Update(dt);
    }

    void Render(sf::RenderWindow& window) override
    {
        gameManager.DrawGrid(window);
        gameManager.DrawHUD(window);
    }

    void OnExit() override
    {
        std::cout << "Saliendo de GameScene..." << std::endl;
    }
};
