#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "UIElement.h"
class Button: public UIElement
{
public:
	Button(float xPos, float yPos, float width, float height);
	void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
	void Draw(sf::RenderWindow& window) override;
	std::function <void()> onClick;
private:
	sf::RectangleShape rect;
};

