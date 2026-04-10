#pragma once
#include "Scene.h"
#include "NetworkManager.h"
#include <iostream>
#include <SFML/Graphics.hpp>


class LobbyScene : public Scene
{
private:
    sf::Font font;
   

public:
	LobbyScene()
    {
        if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            std::cerr << "Warning: Failed to open arial.ttf font in Lobby" << std::endl;
        }
    }
	
	void OnEnter() override
	{
		std::cout << "Entrando al Lobby (Bootstrap Server)..." << std::endl;

        NM.ConnectToServer();

	}

	void HandleEvent(const sf::Event& event) override
	{
        if (event.is<sf::Event::KeyPressed>())
        {
            const sf::Event::KeyPressed* kpInfo = event.getIf<sf::Event::KeyPressed>();
            if (kpInfo && kpInfo->code == sf::Keyboard::Key::Space)
            {
                // Go back to the game scene to start another match
                SM.SetNextScene("GameScene");
            }

            if (kpInfo && kpInfo->code == sf::Keyboard::Key::M)
            {
                NM.DisconnectFromServer();
            }

            if (kpInfo && kpInfo->code == sf::Keyboard::Key::N)
            {
                NM.ConnectToServer();
            }
        }
	}

	void Update(float dt) override
	{
        NM.NetworkFetch();
	}

	void Render(sf::RenderWindow& window) override
	{
        sf::Text titleText(font);
        titleText.setCharacterSize(40);
        titleText.setPosition({ 200.f, 200.f });
        titleText.setString("--- LOBBY SERVER ---");
        titleText.setFillColor(sf::Color::White);

        sf::Text pressSpace(font);
        pressSpace.setCharacterSize(20);
        pressSpace.setPosition({ 200.f, 300.f });
        pressSpace.setString("Presiona ESPACIO para iniciar nueva partida");
        pressSpace.setFillColor(sf::Color::Cyan);

        window.draw(titleText);
        window.draw(pressSpace);
	}

	void OnExit() override 
	{
		std::cout << "Saliendo del Lobby..." << std::endl;
	}


    void AskCreateRoom();

    void AskDeleteRoom();

    void GetRanking();
};
