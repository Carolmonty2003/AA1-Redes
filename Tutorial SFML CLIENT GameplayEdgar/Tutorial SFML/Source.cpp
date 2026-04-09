#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

#include "SceneManager.h"
#include "GameScene.h"
#include "LobbyScene.h"

#define SERVER_PORT 55000
const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);


int main()
{
	
	sf::RenderWindow window(sf::VideoMode({800, 600}), "Conecta3 Cliente");
	window.setFramerateLimit(60);

	// Escenas
	GameScene* gameScene = new GameScene();
    LobbyScene* lobbyScene = new LobbyScene();

	SM.AddScene("GameScene", gameScene);
    SM.AddScene("LobbyScene", lobbyScene);
	SM.InitFirstScene("LobbyScene");

	sf::Clock dtClock;

	
	while (window.isOpen())
	{
		
		float dt = dtClock.restart().asSeconds();
		// Eventos
		while (const std::optional<sf::Event> event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();

			
			if (SM.GetCurrentScene())
				SM.GetCurrentScene()->HandleEvent(*event);
		}

		
		SM.UpdateCurrentScene(dt);

		// Renderizado
		window.clear(sf::Color(30, 30, 30)); // Gris oscuro

		if (SM.GetCurrentScene())
		{
			SM.GetCurrentScene()->Render(window);
		}

		window.display();
	}

	return 0;
}


/*

// CODIGO RED

void FuncionalidadConsolaNetwork()
{
	sf::TcpSocket socket;
	bool gameLoop = true;

	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al conectar con el servidor" << std::endl;
	}

	std::cout << "Conectado con el servidor " << std::endl;

	socket.setBlocking(false);

	while (gameLoop)
	{
		std::string message;
		std::cout << "Inserta mensaje para el servidor, -1 para salir" << std::endl;
		std::cin >> message;

		if (message == "-1")
		{
			std::cout << "Desconectado..." << std::endl;
			gameLoop = false;
		}
		else
		{
			sf::Packet packet;
			packet << message; // NOTA: Arreglado el operador >> a << para añadir info al paquete
			if (socket.send(packet) != sf::Socket::Status::Done)
			{
				std::cerr << "Error al enviar el paquete al servidor";
			}
			else
			{
				std::cout << "Mensaje enviado: " << message << std::endl;
			}
		}
	}

	socket.disconnect();
	std::cout << "Desconectado del servidor" << std::endl;
}
// =========================================================
*/
