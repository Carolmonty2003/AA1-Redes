#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include "Button.h"
#include "InputField.h"

#define SERVER_PORT 55000
const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);
enum tipoPaquete { HANDSHAKE, LOGIN, SIGNUP, MOVIMINETO };

sf::Packet& operator >>(sf::Packet& packet, tipoPaquete& tipo)
{
	int temp;
	packet >> temp;
	tipo = static_cast<tipoPaquete>(temp);

	return packet;
}

void HandShake(sf::Packet data)
{
	std::string receivedMessage;
	data >> receivedMessage;
	std::cout << "Mensaje enviado del servidor " << receivedMessage << std::endl;
}

void Login(sf::Packet data)
{
	int receivedMessage;
	std::string receivedGreeting;
	data >> receivedMessage;
	data >> receivedGreeting;
	std::cout << "Mensaje enviado del servidor int " << receivedMessage << receivedGreeting << std::endl;
}

int main()
{
	sf::TcpSocket socket;
	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al conectar co el servidor" << std::endl;
	}
	else
	{
		std::cout << "Conectado con el servidor " << std::endl;

		socket.setBlocking(false);

		sf::Packet packet; //Un paquete para enviar y otro para recibir es decir uno para el server y otro para el cliente

		sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My window");
		Button closeButton(800 - 64, 0, 64, 64);
		closeButton.onClick = [&window]() {
			window.close();
			};
		sf::Font font;
		font.openFromFile("C:/Windows/Fonts/arial.ttf");
		InputField usernameInputfield(250, 220, 300, 25, font);
		InputField passwordInputfield(250, 250, 300, 25, font);
		Button loginButton(350, 280, 100, 25);
		Button signinButton(350, 310, 100, 25);

		bool gameOver = false;

		while (!gameOver && window.isOpen())
		{
			
			if (socket.receive(packet) == sf::Socket::Status::Done)
			{
				tipoPaquete tipo;

				packet >> tipo;

				switch (tipo)
				{
				case HANDSHAKE:
					HandShake(packet);
					break;
				case LOGIN:
					Login(packet);
					break;
				case MOVIMINETO:

					break;

				case SIGNUP:

					break;
				default:
					break;
				}

				packet.clear();

			}
			if (socket.receive(packet) == sf::Socket::Status::Disconnected)
			{
				gameOver = true;
			}
			while (const std::optional event = window.pollEvent()) https://www.sfml-dev.org/tutorials/3.0/window/events/#sfwindowbasehandleevents
			{
				closeButton.handleEvent(*event, window);
				usernameInputfield.handleEvent(*event, window);
				passwordInputfield.handleEvent(*event, window);
				loginButton.handleEvent(*event, window);
				signinButton.handleEvent(*event, window);
				if (event->is<sf::Event::Closed>())
					window.close();
			}

			window.clear();
			closeButton.Draw(window);
			usernameInputfield.Draw(window);
			passwordInputfield.Draw(window);
			loginButton.Draw(window);
			signinButton.Draw(window);
			window.display();
		}
	}
}