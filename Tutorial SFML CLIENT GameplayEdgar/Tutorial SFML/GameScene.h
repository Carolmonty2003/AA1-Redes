#pragma once
#include "Scene.h"
#include "GameManager.h"
#include <iostream>
#include <vector>
#include <SFML/Network.hpp>

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

    void OnEnter() override
    {
        std::cout << "Entrando a GameScene..." << std::endl;
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
