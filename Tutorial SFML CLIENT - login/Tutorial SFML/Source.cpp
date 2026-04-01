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
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My window");
	Button button(100,100,100,100);
	button.onClick = [&window]() {
		window.close();
	};
	sf::Font font;
	font.openFromFile("C:/Windows/Fonts/arial.ttf");
	InputField inputfield(200, 200, 200, 200,font);
	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent()) https://www.sfml-dev.org/tutorials/3.0/window/events/#sfwindowbasehandleevents
		{
			button.handleEvent(*event, window);
			inputfield.handleEvent(*event, window);
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		window.clear();
		button.Draw(window);
		inputfield.Draw(window);
		window.display();
	}
	/*sf::TcpSocket socket;
	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cerr << "Error al conectar co el servidor" << std::endl;
	}
	else
	{
		std::cout << "Conectado con el servidor " << std::endl;

		socket.setBlocking(false);

		sf::Packet packet; //Un paquete para enviar y otro para recibir es decir uno para el server y otro para el cliente

		bool gameOver = false;

		while (!gameOver)
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
		}
		
	}*/
}