#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
class Button
{
public:
	Button(float xPos, float yPos, float width, float height);
	void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window);
	std::function <void()> onClick;
private:
	sf::RectangleShape rect;
};

