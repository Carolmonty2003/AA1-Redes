#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class InputField
{
public:
    InputField(float x, float y, float w, float h, sf::Font& font);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
    std::string getText() const;
private:
    sf::RectangleShape rect;
    sf::Text text;
    std::string input;
};

