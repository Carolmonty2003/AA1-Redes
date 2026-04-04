#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <string>
#include "Renderer.h"

class SpriteRenderer : public Renderer
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

public:
	SpriteRenderer(Transform* t) : Renderer(t) {}

	void SetTexture(const std::string& path)
	{
		if (!texture.loadFromFile(path)) {
			std::cerr << "Error cargando textura: " << path << std::endl;
		}
		sprite.setTexture(texture);
	}

	virtual void render(sf::RenderWindow& window) override
	{
		if (transform != nullptr) {
			sprite.setPosition(transform->position);
			sprite.setRotation(transform->rotation);
			sprite.setScale(transform->scale);
		}
		window.draw(sprite);
	}

	virtual void Update(float dt) override
	{
	}
};
