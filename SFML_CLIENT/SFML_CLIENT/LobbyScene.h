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
	LobbyScene()
    {
        if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            std::cerr << "Warning: Failed to open arial.ttf font in Lobby" << std::endl;
        }
    }
	
	void OnEnter() override
	{
		std::cout << "Entrando al LobbyScene..." << std::endl;

        unsigned short myPort = static_cast<unsigned short>(56000 + NM.GetClientState().playerId);
        NM.GetClientState().myGamePort = myPort;
        std::cout << "[CLIENT] P2P port asignado: " << myPort << std::endl;

        // Auto Matchmaking: primero intentamos unirnos, si falla la creamos
        NM.SendJoinRoomRequest("test_room", NM.GetClientState().nickname, myPort);
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
        if (NM.GetClientState().hasGameStarted)
        {
            SM.SetNextScene("GameScene");
        }
	}

	void Render(sf::RenderWindow& window) override
	{
        sf::Text titleText(font);
        titleText.setCharacterSize(Config::UI::FONT_SIZE_TITLE);
        titleText.setPosition({ Config::Lobby::TITLE_X, Config::Lobby::TITLE_Y });
        titleText.setString("--- LOBBY SERVER ---");
        titleText.setFillColor(sf::Color::White);

        sf::Text pressSpace(font);
        pressSpace.setCharacterSize(Config::UI::FONT_SIZE_MEDIUM);
        pressSpace.setPosition({ Config::Lobby::SUBTITLE_X, Config::Lobby::SUBTITLE_Y });
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
