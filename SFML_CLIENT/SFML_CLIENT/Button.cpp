#include "Button.h"


Button::Button(float xPos, float yPos, float width, float height)
{
    rect.setPosition(sf::Vector2f(xPos, yPos));
    rect.setSize(sf::Vector2f(width, height));
}

Button::Button(float xPos, float yPos, float width, float height, sf::Font& font)
{
    rect.setPosition({ xPos, yPos });
    rect.setSize({ width, height });
    text.emplace(font);
    text->setPosition({ xPos + 5, yPos + 5 });
    text->setCharacterSize(18);
    text->setFillColor(sf::Color::Black);
}

void Button::SetText(std::string newText)
{
    text->setString(newText);
}

void Button::Draw(sf::RenderWindow& window)
{
    window.draw(rect);
    if (text.has_value())
        window.draw(*text);
}

void Button::handleEvent(const sf::Event& event)
{
	if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) //https://www.sfml-dev.org/tutorials/3.0/window/events/#sfeventgetift
		if (rect.getGlobalBounds().contains(sf::Vector2f(static_cast<float>(mouseEvent->position.x),static_cast<float>(mouseEvent->position.y))))//https://en.sfml-dev.org/forums/index.php?topic=18574.0
			if (onClick)
				onClick();
}

