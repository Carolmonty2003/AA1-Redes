#pragma once
#include "Scene.h"
#include "GameManager.h"
#include <iostream>

class GameScene : public Scene
{
private:
	GameManager gameManager;

public:
	GameScene() = default;
	
	void OnEnter() override
	{
		std::cout << "Entrando a GameScene..." << std::endl;
        gameManager.InitGame();
	}

	void HandleEvent(const sf::Event& event) override
	{
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            const sf::Event::MouseButtonPressed* mbInfo = event.getIf<sf::Event::MouseButtonPressed>();
            if (mbInfo && mbInfo->button == sf::Mouse::Button::Left)
            {
                gameManager.TryPlacePieceScreen((float)mbInfo->position.x, (float)mbInfo->position.y);
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
