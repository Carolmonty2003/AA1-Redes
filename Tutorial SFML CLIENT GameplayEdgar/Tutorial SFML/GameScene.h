#pragma once
#include "Scene.h"
#include <iostream>

class GameScene : public Scene
{
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

	void Render() override
	{
	}

	void OnExit() override 
	{
		std::cout << "Saliendo de GameScene..." << std::endl;
	}
};
