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
	}

	void HandleEvent(const sf::Event& event) override
	{
	}

	void Update(float dt) override
	{
	}

	void Render(sf::RenderWindow& window) override
	{
		gameManager.DrawGrid(window);
	}

	void OnExit() override 
	{
		std::cout << "Saliendo de GameScene..." << std::endl;
	}
};
