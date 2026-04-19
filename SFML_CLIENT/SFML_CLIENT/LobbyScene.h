#pragma once
#include "Scene.h"
#include "NetworkManager.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "SceneManager.h"
#include "Constants.h"

class LobbyScene : public Scene
{
private:
    sf::Font font;

public:
    LobbyScene();
	
    void OnEnter() override;

    void HandleEvent(const sf::Event& event) override;

    void Update(float dt) override;

    void Render(sf::RenderWindow& window) override;

    void OnExit() override;


    void AskCreateRoom();

    void AskDeleteRoom();

    void GetRanking();
};
