#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
class Renderer : public sf::Drawable, public sf::Transformable
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		states.transform *= getTransform();
		target.draw(sprite, states);
	}

	virtual void Update(float dt) = 0;

	void SetTexture(const std::string& path)
	{
		if (!texture.loadFromFile(path)) // <-- FALTABA cargar el archivo
			std::cerr << "Error cargando textura: " << path << std::endl;
		sprite.setTexture(texture);
	}



};

