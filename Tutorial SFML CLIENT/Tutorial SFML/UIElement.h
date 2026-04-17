#pragma once
#include <SFML/Graphics.hpp>
class UIElement
{
public :
	virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window) = 0;
	virtual void Draw(sf::RenderWindow& window) =  0;
};

